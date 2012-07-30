/*
 * menu.c
 *
 * Created: 30.07.2012 18:52:51
 *  Author: OliverS
 */ 

#include "global.h"
#include "lcd.h"
#include "keyboard.h"
#include <avr/pgmspace.h>

static const prog_char strMENU[] = "MENU";
static const prog_char strBACK[] = "BACK";
static const prog_char strUP[] = "UP";
static const prog_char strDOWN[] = "DOWN";
static const prog_char strENTER[] = "ENTER";
static const prog_char strSAFE[] = "SAFE";
static const prog_char strOFF[] = "OFF";
static const prog_char strON[] = "ON";

static const prog_char strPIEditor[] = "PI Editor";
static const prog_char strReceiverTest[] = "Receiver Test";
static const prog_char strModeSettings[] = "Mode Settings";
static const prog_char strStickScaling[] = "Stick Scaling";
static const prog_char strMiscSettings[] = "Misc. Settings";
static const prog_char strSelflevelSettings[] = "Self-level Settings";
static const prog_char strSensorTest[] = "Sensor Test";
static const prog_char strSensorCalibration[] = "Sensor Calibration";
static const prog_char strESCCalibration[] = "ESC Calibration";
static const prog_char strMixerEditor[] = "Mixer Editor";
static const prog_char strShowMotorLayout[] = "Show Motor Layout";
static const prog_char strLoadMotorLayout[] = "Load Motor Layout";
static const prog_char strDebug[] = "Debug";

static const prog_char strSelflevel[] = "Self-level";
static const prog_char strIofPI[] = "I of PI";
static const prog_char strSpIsSp[] = " is ";

static PGM_P theMenu[] PROGMEM = {
	strPIEditor,
	strReceiverTest,
	strModeSettings,
	strStickScaling,
	strMiscSettings,
	strSelflevelSettings,
	strSensorTest,
	strSensorCalibration,
	strESCCalibration,
	strMixerEditor,
	strShowMotorLayout,
	strLoadMotorLayout,
	strDebug
};

#define PAGE_START	-1
#define PAGE_MENU	-2

static uint8_t itemTop, itemMarked, refresh;
static int8_t page;

void menuInit()
{
	page = PAGE_START;
	refresh = 1;
	lcdClear();
}

void displayMenu()
{
	lcdSetPos(0, 58);
	if (itemTop > 0)
		lcdWriteImage_P(lcdArrowUp, sizeof(lcdArrowUp));
	else
		lcdFill(0, sizeof(lcdArrowUp));
		
	for (uint8_t i = 0; i < 5; i++)
	{
		lcdSetPos(i + 1, 0);
		PGM_P item = (PGM_P)pgm_read_word(&theMenu[itemTop + i]);
		if (itemTop + i == itemMarked)
			lcdReverse(1);
		else
			lcdReverse(0);
		lcdWriteString_P(item);
		lcdFill(0, (21 - strlen_P(item)) * 6);
	}
	
	lcdReverse(0);
	lcdSetPos(6, 58);
	if (itemTop < length(theMenu) - 5)
		lcdWriteImage_P(lcdArrowDown, sizeof(lcdArrowDown));
	else
		lcdFill(0, sizeof(lcdArrowDown));
		
	lcdSetPos(7, 0);
	lcdWriteString_P(strBACK);
	lcdSetPos(7, 36);
	lcdWriteString_P(strUP);
	lcdSetPos(7, 66);
	lcdWriteString_P(strDOWN);
	lcdSetPos(7, 96);
	lcdWriteString_P(strENTER);
}

void displayStart()
{
	lcdSetPos(0, 36);
	lcdWriteString_P(strSAFE);
	lcdSetPos(3, 0);
	lcdWriteString_P(strSelflevel);
	lcdWriteString_P(strSpIsSp);
	lcdWriteString_P(strOFF);
	lcdSetPos(4, 0);
	lcdWriteString_P(strIofPI);
	lcdWriteString_P(strSpIsSp);
	lcdWriteString_P(strON);
	lcdSetPos(7, 102);
	lcdWriteString_P(strMENU);
}

void menuShow()
{
	uint8_t oldPage = page;
	uint8_t key = keyboardRead();
	if (page == PAGE_MENU)
	{
		if (key == KEY_2)		// UP
			if (itemMarked > 0) 
			{
				itemMarked--;
				if (itemMarked < itemTop)
					itemTop = itemMarked;
				refresh = 1;
			}
					
		if (key == KEY_3)		// DOWN
			if (itemMarked < length(theMenu)-1) 
			{
				itemMarked++;
				if (itemMarked - itemTop >= 5)
					itemTop = itemMarked - 4;
				refresh = 1;
			}
		if (key == KEY_1)		// BACK
		{
			page = PAGE_START;
			refresh = 1;
		}
	}
	else if (page == PAGE_START)
	{
		if (key == KEY_4)
		{
			page = PAGE_MENU;
			refresh = 1;
		}
	}		
		
	if (refresh)
	{
		if (oldPage != page)
			lcdClear();
			
		switch (page)
		{
			case PAGE_MENU: displayMenu(); break;
			case PAGE_START: displayStart(); break;
		}			
		refresh = 0;
	}		
}

