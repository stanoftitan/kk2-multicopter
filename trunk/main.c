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
		//pwmWrite();
		lcdOutput();
		menuShow();
		buzzerLoop();
    }
}