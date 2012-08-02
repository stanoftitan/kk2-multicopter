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

// for debugging
#include <stdlib.h>
#include <string.h>

state_t State;

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
	
	if (Config.IPartMode)	// AUX
		State.IofPI = State.Aux;
	else
		State.IofPI = ON;

	if (Config.SelfLevelMode)	// AUX
		State.SelfLevel = State.Aux;
}

int main(void)
{
	// setup hardware w/o interrupts
	setup();
	
	// enable interrupts
	sei();
	
	// init hardware
	init();

	buzzerBuzz(77);
	lcdClear();
		
    while(1)
    {
		LED_TOGGLE;
		
		rxRead();
		sensorsReadGyro();
		sensorsReadAcc();
		CheckState();
		
		//pwmWrite();
		lcdOutput();
		menuShow();
		buzzerLoop();
    }
}