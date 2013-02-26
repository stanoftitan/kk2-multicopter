/*
 * main.c
 *
 * Created: 27.07.2012 09:03:19
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "lcd.h"
#include "adc.h"
#include "rx.h"
#include "sensors.h"
#include "pwm.h"
#include "mixer.h"
#include "keyboard.h"
#include "menu.h"
#include "digitals.h"
#include "controller.h"
#include "imu.h"
#include "serial.h"
#include <avr/wdt.h>

// for debugging
#include <stdlib.h>
#include <string.h>

state_t State;
static const char versionNum[] PROGMEM = "Version 0.2b";
static const char versionAuthor[] PROGMEM = "by Oliver Schulz";
#define CYCLE_TIME		2000	// us


__attribute__((naked))
__attribute__((section(".init3")))
__attribute__((used))
static void stop_wdt()
{
	// clear watchdog reset flag!!
	MCUSR = 0;
	// and stop watchdog
	wdt_disable();
}

static void init()
{
	configInit();
	adcInit();
	rxInit(Config.ReceiverMode);
	mixerInit();
	pwmInit();
	lcdInit();
	keyboardInit();
	menuInit();
	if (Config.ReceiverMode == RX_MODE_CPPM)
		serialInit();
}

static void CheckState()
{
	State.ThrottleOff = RX[THR] < THROTTLE_OFF;
	State.Aux = RX[AUX] > 10;
	
	if (Config.SelfLevelMode)	// AUX
		State.SelfLevel = State.Aux;
	
	uint8_t e = 0;
	if (!Config.CalibrateFlags)
		e |= ERR_NOT_CALIBRATED;
	e |= (~RX_good) & (ERR_NO_PITCH|ERR_NO_ROLL|ERR_NO_THR|ERR_NO_YAW);
	
	State.Error = e;
}

static void ESCCalibration()
{
	State.Mode = MODE_ESC_CAL;
	lcdClear();
	lcdSetPos(3, 18);
	lcdWriteString_P(PSTR("Calibrating ESCs"));
	pwmEnable();
	while(1)
	{
		rxRead();
		if (keyboardState() != (KEY_1 | KEY_4))
		{
			State.Mode = MODE_NORMAL;
			pwmDisable();
			break;
		}
			
		for (uint8_t i = 0; i < RX_CHANNELS; i++)
		pwmWrite(i, RX_raw[THR]);
	}
}

static void serialWriteInt16(int16_t value)
{
	serialWriteChar(value & 0xFF);
	serialWriteChar(value >> 8);
}

static void debug_output()
{
	serialWriteChar(0x1F);
	serialWriteChar(0x1E);
	serialWriteChar(0x1C);
	serialWriteInt16(GYRO[0]);
	serialWriteInt16(GYRO[1]);
	serialWriteInt16(GYRO[2]);
	serialWriteInt16(ACC[0]);
	serialWriteInt16(ACC[1]);
	serialWriteInt16(ACC[2]);
	serialWriteInt16(ANGLE[0]);
	serialWriteInt16(ANGLE[1]);
	serialWriteInt16(ANGLE[2]);
}

int main(void)
{
	// setup hardware w/o interrupts
	setup();
	
	// enable interrupts
	sei();
	
	// init hardware
	init();

	lcdClear();
	lcdSetPos(0, 0);
	lcdSelectFont(&font12x16);
	lcdWriteString_P(PSTR("KK2-Copter"));
	lcdSelectFont(NULL);
	lcdSetPos(3, 0);
	lcdWriteString_P(versionNum);
	lcdSetPos(4, 0);
	lcdWriteString_P(versionAuthor);
	digitalsBuzzBlocking(500);
	WAITMS(700);
	
	if (keyboardState() == (KEY_1 | KEY_4))		// enter ESC Calibration mode?
		ESCCalibration();

	LOOPUS(CYCLE_TIME)
	{
 		LED_TOGGLE;
		rxRead();			// 
		CheckState();
		sensorsRead();
		imuCalculate();
		controller();
		mixerMixing();

		for (uint8_t i = 0; i < 5; i++)
			pwmWrite(i+1, RX_raw[i]);

		EVERYMS(25)
			menuShow();
	
	 	digitalsLoop();
		
		#ifdef DEBUG
		EVERYMS(10) 
			debug_output();
		#endif
	}
}