/*
 * buzzer.c
 *
 * Created: 02.08.2012 12:24:36
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

typedef struct  
{
	uint16_t end;
//	uint16_t endInterval;
	uint16_t interval;
} digital_t;

static digital_t digLED, digBUZZER;

void digitalsInit()
{
	BUZZER_DIR = OUTPUT;
	LED_DIR = OUTPUT;
}

void digitalsBuzzBlocking(uint16_t ms)
{
	BUZZER = ON;
	WAITMS(ms);
	BUZZER = OFF;
}

#define _digDev(a, state, toggle)	\
	if (a.interval)					\
	{								\
		if (m >= a.end)				\
		{							\
			toggle;					\
			a.end = m + a.interval;	\
		}							\
	}								\
	else if (state)					\
		if (m >= a.end)				\
			toggle;

void digitalsLoop()
{
	uint16_t m = millis();
	_digDev(digBUZZER, BUZZER, BUZZER_TOGGLE)
	_digDev(digLED, LED, LED_TOGGLE);
}

void digitalsBuzz(uint16_t ms)
{
	digBUZZER.end = millis() + ms;
	digBUZZER.interval = 0;
	BUZZER = ON;
}

void digitalsBuzzInterval(uint16_t ms)
{
	if (!digBUZZER.interval)
		digBUZZER.end = millis();
	digBUZZER.interval = ms;
	if (!ms)
		BUZZER = OFF;
}

void digitalsLed(uint16_t ms)
{
	digLED.end = millis() + ms;
	digLED.interval = 0;
	LED = ON;
}

void digitalsLedInterval(uint16_t ms)
{
	if (!digLED.interval)
		digLED.end = millis();
	digLED.interval = ms;
	if (!ms)
		LED = OFF;
}