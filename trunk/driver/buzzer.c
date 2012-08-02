/*
 * buzzer.c
 *
 * Created: 02.08.2012 12:24:36
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

static uint16_t endBuzz, endInternal, interval;

void buzzerInit()
{
	BUZZER_DIR = OUTPUT;
}

void buzzerBuzzWait(uint16_t ms)
{
	BUZZER = ON;
	WAITMS(ms);
	BUZZER = OFF;
}

void buzzerLoop()
{
	uint16_t m = millis();
	if (interval)
	{
		if (m >= endInternal)
		{
			BUZZER_TOGGLE;
			endInternal = m + interval;
		}
	}		
	else if (BUZZER)
		if (m >= endBuzz)
			BUZZER_TOGGLE;
}

void buzzerBuzz(uint16_t ms)
{
	endBuzz = millis() + ms;
	interval = 0;
	BUZZER_TOGGLE;
}

void buzzerInterval(uint16_t ms)
{
	if (!interval)
		endInternal = millis();
	interval = ms;
	if (!ms)
		BUZZER = OFF;
}