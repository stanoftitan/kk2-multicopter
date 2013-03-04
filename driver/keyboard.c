/*
 * keyboard.c
 *
 * Created: 30.07.2012 14:41:27
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "keyboard.h"
#include "digitals.h"

#define ALLKEYS (KEY_1 | KEY_2 | KEY_3 | KEY_4)
static uint8_t lastKeys;
static uint16_t keyDown;
static uint8_t repeat;

uint8_t keyboardState()
{
	return ~KEYBOARD_PIN & ALLKEYS;
}

uint8_t keyboardRead()
{
	uint16_t m = millis();
	uint8_t keys = keyboardState();
	uint8_t newkeys = keys & ~lastKeys;
	if (keys)
	{
		if (lastKeys != keys)
		{
			keyDown = m;
			repeat = 0;
			digitalsBuzz(5);
		}			
		if (repeat > 15 && m - keyDown >= 20)
		{
			newkeys = keys;
			keyDown = m;
		}
		else if (repeat && m - keyDown >= 100)
		{
			newkeys = keys;
			keyDown = m;
			repeat++;
		}
		else if (!repeat && m - keyDown >= 500)
		{
			newkeys = keys;
			keyDown = m;
			repeat = 1;
		}
	}		
	lastKeys = keys;
	return newkeys;
}

void keyboardInit()
{
	// key pins to input
	KEYBOARD_DDR |= ALLKEYS;
	// pull up resistors on
	KEYBOARD_PORT |= ALLKEYS;
}

	