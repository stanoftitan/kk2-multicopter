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
#include "buzzer.h"
#include <util/delay.h>
#include <avr/wdt.h>

// for debugging
#include <stdlib.h>
#include <string.h>

state_t State;
static const prog_char versionNum[] = "Version 0.2a";
static const prog_char versionAuthor[] = "By Oliver Schulz";


__attribute__((naked))
__attribute__((section(".init3")))
void stop_wdt()
{
	// clear watchdog reset flag!!
	MCUSR = 0;
	// and stop watchdog
	wdt_disable();
}

void init()
{
	configInit();
	adcInit();
	rxInit(Config.RX_mode);
	mixerInit();
	pwmInit();
	lcdInit();
	keyboardInit();
	menuInit();
}

void CheckState()
{
	State.ThrottleOff = RX[THR] <= 5;
	State.Aux = RX[AUX] > 10;
	
	if (Config.SelfLevelMode)	// AUX
		State.SelfLevel = State.Aux;
	
	uint8_t e = 0;
	if (!Config.CalibrateFlags)
		e |= ERR_NOT_CALIBRATED;
	e |= (~RX_good) & (ERR_NO_PITCH|ERR_NO_ROLL|ERR_NO_THR|ERR_NO_YAW);
	
	State.Error = e;
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
	//lcdWriteGlyph_P(&glyLogo, ROP_COPY);
	lcdSetPos(1, 0);
	lcdWriteString_P(versionNum);
	lcdSetPos(2, 0);
	lcdWriteString_P(versionAuthor);
	buzzerBuzzWait(500);
	WAITMS(700);

	while(1)
	{
 		LED_TOGGLE;
		rxRead();
		
		CheckState();
		sensorsRead();

		for (uint8_t i = 0; i < 5; i++)
			pwmWrite(i+1, RX_raw[i]);

		EVERYMS(20)
			menuShow();

 		buzzerLoop();
		 
	}
}