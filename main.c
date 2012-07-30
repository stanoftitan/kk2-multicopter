/*
 * main.c
 *
 * Created: 27.07.2012 09:03:19
 *  Author: OliverS
 */ 

#include "global.h"
#include "lcd.h"
#include "adc.h"
#include "rx.h"
#include "sensors.h"
#include "pwm.h"
#include "mixer.h"
#include <util/delay.h>

// for debugging
#include <stdlib.h>
#include <string.h>


void init()
{
	configInit();
	adcInit();
	rxInit(RX_MODE_CPPM);
	mixerInit();
	pwmInit();
	lcdInit();
}

int main(void)
{
	char s[10];
	// setup hardware w/o interrupts
	setup();
	
	// enable interrupts
	sei();
	
	// init hardware
	init();
	
	//_delay_ms(1000);
	uint8_t r = 0;
	lcdClear();	

	lcdSetPos(0, 0);
	lcdWriteString_P(PSTR("AIL:"));
	lcdSetPos(1, 0);
	lcdWriteString_P(PSTR("ELE:"));
	lcdSetPos(2, 0);
	lcdWriteString_P(PSTR("THR:"));
	lcdSetPos(3, 0);
	lcdWriteString_P(PSTR("RUD:"));
	lcdSetPos(4, 0);
	lcdWriteString_P(PSTR("AUX:"));
	lcdSetPos(0, 64);
	lcdWriteString_P(PSTR("G_X:"));
	lcdSetPos(1, 64);
	lcdWriteString_P(PSTR("G_Y:"));
	lcdSetPos(2, 64);
	lcdWriteString_P(PSTR("G_Z:"));
	lcdSetPos(3, 64);
	lcdWriteString_P(PSTR("A_X:"));
	lcdSetPos(4, 64);
	lcdWriteString_P(PSTR("A_Y:"));
	lcdSetPos(5, 64);
	lcdWriteString_P(PSTR("A_Z:"));

    while(1)
    {
		LED_TOGGLE;
		
		rxRead();
		sensorsReadGyro();
		sensorsReadAcc();
		//pwmWrite();
		lcdOutput();
		
		
		lcdSetPos(0, 30);
		itoa(RX[0], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);
		
		lcdSetPos(1, 30);
		itoa(RX[1], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(2, 30);
		itoa(RX[2], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(3, 30);
		itoa(RX[3], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(4, 30);
		itoa(RX[4], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(0, 94);
		itoa(GYRO[0], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);
		
		lcdSetPos(1, 94);
		itoa(GYRO[1], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(2, 94);
		itoa(GYRO[2], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(3, 94);
		itoa(ACC[0], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(4, 94);
		itoa(ACC[1], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

		lcdSetPos(5, 94);
		itoa(ACC[2], s, 10);
		lcdWriteString(s);
		lcdWriteChar(32);

    }
}