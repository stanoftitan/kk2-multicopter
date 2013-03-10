/*
* menu.c
*
* Created: 30.07.2012 18:52:51
*  Author: OliverS
*
* $Id$
*/

#ifndef NO_LCD

#include "global.h"
#include "lcd.h"
#include "keyboard.h"
#include "sensors.h"
#include "rx.h"
#include "mixer.h"
#include "digitals.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <string.h>
#include "controller.h"
#include "imu.h"

static uint8_t keys;
#define KEY_INIT	1
#define KEY_REFRESH	2
#define KEY_MENU	4
#define KEYINIT		(keys & KEY_INIT)
#define KEYREFRESH	(keys & KEY_REFRESH)
#define KEYMENU		(keys & KEY_MENU)
#define KEY1		(keys & KEY_1)
#define KEY2		(keys & KEY_2)
#define KEY3		(keys & KEY_3)
#define KEY4		(keys & KEY_4)
#define ANYKEY		(keys)
#define KEYPRESS	(keys & (KEY_1|KEY_2|KEY_3|KEY_4))
#define NOKEYRETURN {if (!keys) return;}

typedef struct
{
	uint8_t X, Y;
	void *valuePtr;
	int16_t loLimit, hiLimit;
	uint8_t len;
} edit_element_t;

typedef void (pageHandler)(void);

typedef struct
{
	const char *softkeys;
	pageHandler *handler;
	const char *screen;
} page_t;

typedef struct
{
	uint8_t len;
	PGM_P (*textSelector)(uint8_t);
	uint8_t top;
	uint8_t marked;
} menu_t;


//////////////////////////////////////////////////////////////////////////
#define P_STR static const char PROGMEM
#include "menu_text.h"
#include "menu_screen.h"

//////////////////////////////////////////////////////////////////////////
static void _hStart();
static void _hMenu();
static void _hReceiverTest();
static void _hSensorTest();
static void _hSensorCalibration();
static void _hESCCalibration();
static void _hStickCentering();
static void _hStickScaling();
static void _hShowModelLayout();
static void _hLoadModelLayout();
static void _hFactoryReset();
static void _hPIEditor();
static void _hMiscSettings();
static void _hSelflevelSettings();
static void _hCameraStabSettings();
static void _hCPPMSettings();
static void _hModeSettings();
static void _hMixerEditor();

#ifdef DEBUG
static void _hDebug();
#endif

//////////////////////////////////////////////////////////////////////////
// softkeys
P_STR _skSTART[]     = "                 MENU";
P_STR _skMENU[]      = "BACK  UP   DOWN ENTER";
P_STR _skBACK[]      = "BACK";
P_STR _skCONTINUE[]  = "BACK         CONTINUE";
P_STR _skCANCELYES[] = "CANCEL            YES";
P_STR _skPAGE[]      = "BACK PREV NEXT CHANGE";
P_STR _skEDIT[]      = "CLR  DOWN   UP   DONE";
P_STR _skBACKNEXT[]  = "BACK  NEXT";
P_STR _skCANCEL[]    = "CANCEL";

//////////////////////////////////////////////////////////////////////////
static const page_t pages[] PROGMEM = {
/*  0 */	{ _skSTART, _hStart },
/*  1 */	{ _skMENU, _hMenu },
/*  2 */	{ _skPAGE, _hPIEditor, scrPIEditor},
/*  3 */	{ _skBACK, _hReceiverTest, scrReceiverTest},
/*  4 */	{ _skPAGE, _hModeSettings, scrModeSettings},
/*  5 */	{ _skPAGE, _hStickScaling, scrStickScaling},
/*  6 */	{ _skPAGE, _hMiscSettings, scrMiscSettings},
/*  7 */	{ _skPAGE, _hSelflevelSettings, scrSelflevelSettings},
/*  8 */    { _skPAGE, _hCameraStabSettings, scrCameraStabSettings},	
/*  9 */	{ _skBACK, _hSensorTest, scrSensorTest},
/* 10 */	{ _skCONTINUE, _hSensorCalibration, scrSensorCal0},
/* 11 */	{ _skCONTINUE, _hESCCalibration, scrESCCal0},
/* 12 */	{ _skPAGE, _hCPPMSettings },
/* 13 */	{ _skCONTINUE, _hStickCentering},
/* 14 */	{ _skPAGE, _hMixerEditor, scrMixerEditor},
/* 15 */	{ _skBACKNEXT, _hShowModelLayout},
/* 16 */	{ _skMENU, _hLoadModelLayout },
/* 17 */	{ _skCANCELYES, _hFactoryReset },
#ifdef DEBUG
/* 18 */	{ _skBACK, _hDebug, scrDebug },
#endif
};

static const char* const lstMenu[] PROGMEM = {
	strPIEditor,
	strReceiverTest,
	strModeSettings,
	strStickScaling,
	strMiscSettings,
	strSelflevelSettings,
	strCameraStabSerrings,
	strSensorTest,
	strSensorCalibration,
	strESCCalibration,
	strCPPMSettings,
	strRadioCalibration,
	strMixerEditor,
	strShowMotorLayout,
	strLoadMotorLayout,
	strFactoryReset,
#ifdef DEBUG
	strDebug,
#endif
};

#define PAGE_START			0
#define PAGE_MENU			1
#define PAGE_SHOW_LAYOUT	15

static PGM_P tsmMain(uint8_t);
static PGM_P tsmLoadModelLayout(uint8_t);

uint8_t menuPage;
static uint8_t elementIndex, subpage;
static uint16_t _tStart;
static page_t currentPage;
static menu_t mnuMain = {length(lstMenu), tsmMain};
static menu_t mnuMLayout = {MODEL_TABLE_LEN, tsmLoadModelLayout};
static int16_t editValue, editLoLimit, editHiLimit, editInitialValue;
static uint8_t editMode, editValueType;
static void* editValuePtr;

#define TYPE_UINT8		0
#define TYPE_INT8		1
#define TYPE_INT16		3

static void writeSpace(int8_t len)
{
	for (int8_t i = 0; i < len; i++)
		lcdWriteChar(32);
}

static void writePadded(char *s, uint8_t len)
{
	lcdWriteString(s);
	writeSpace(len - strlen(s));
}

static void writePadded_P(const char *s, uint8_t len)
{
	lcdWriteString_P(s);
	writeSpace(len - strlen_P(s));
}

static void writeSoftkeys(const char* sk)
{
	if (!sk)
		sk = currentPage.softkeys;
	if (sk)
	{
		lcdSetPos(7, 0);
		writePadded_P(sk, 21);
	}
}

static void writeString_P(uint8_t x, uint8_t y, PGM_P str, uint8_t len, uint8_t index)
{
	lcdReverse(index == elementIndex);
	lcdSetPos(x, y);
	writePadded_P(str, len);
	lcdReverse(0);
}

static void writeValue(uint8_t x, uint8_t y, int16_t value, uint8_t len, uint8_t index)
{
	char s[7];
	itoa(value, s, 10);
	lcdReverse(index == elementIndex);
	lcdSetPos(x, y);
	writePadded(s, len);
	lcdReverse(0);
}

static void loadPage(uint8_t pageIndex)
{
	memcpy_P(&currentPage, &pages[pageIndex], sizeof(currentPage));
	menuPage = pageIndex;
	keys = KEY_MENU;
	elementIndex = 0;
	subpage = 0;
}

static void elementKey(uint8_t num)
{
	if (KEY2)	// PREV
		elementIndex = elementIndex == 0 ? num - 1 : elementIndex - 1;
	else if (KEY3) // NEXT
		elementIndex = (elementIndex + 1) % num;
}

static void editModeHandler();
static void defaultHandler()
{
	if (editMode)		// edit mode?
		editModeHandler();
	else
	{
		if (KEYINIT | KEYREFRESH)
		{
			lcdClear();
			if (currentPage.screen)
				lcdWriteString_P(currentPage.screen);
			writeSoftkeys(NULL);
			lcdSetPos(0, 0);
		}
		
		if (currentPage.handler)
			currentPage.handler();
		
	}
}

static void editModeHandler()
{
	if (KEY4)	// DONE;
	{
		editMode = OFF;
		
		if (editValueType == TYPE_UINT8)
			*(uint8_t*)editValuePtr = editValue;
		else if (editValueType == TYPE_INT8)
			*(int8_t*)editValuePtr = editValue;
		
		//configSave();
		lcdSelectFont(NULL);
		keys = KEY_REFRESH;
		// call defaultHandler to refresh the screen
		defaultHandler();
		return;
	}
	if (ANYKEY)
	{
		if (KEY2)	// DOWN?
		{
			if (editValue > editLoLimit)
				editValue--;
		}
		else if (KEY3)	// UP?
		{
			if (editValue < editHiLimit)
				editValue++;
		}
		else if (KEY1)	// CLR?
			editValue = editInitialValue;
			
		writeValue(2, 34, editValue, 5, -1);
	}
}

static void startEditMode(void* valuePtr, int16_t loLimit, int16_t hiLimit, uint8_t valueType)
{
	editMode = ON;
	keys = KEY_INIT;
	editValuePtr = valuePtr;
	editValueType = valueType;
	
	if (valueType == TYPE_UINT8)
		editValue = *(uint8_t*)valuePtr;
	else if (valueType == TYPE_INT8)
		editValue = *(int8_t*)valuePtr;
// 	else if (valueType == TYPE_INT16)
// 		editValue = *(int16_t*)valuePtr;
	
	editLoLimit = loLimit;
	editHiLimit = hiLimit;
	editInitialValue = editValue;
	
	lcdFillRectangle(30, 11, 98, 34, 0);
	lcdRectangle(30, 11, 98, 34, 1);
	lcdRectangle(31, 12, 97, 33, 1);
	writeSoftkeys(_skEDIT);
	lcdSelectFont(&font12x16);
	editModeHandler();
}

static uint8_t doMenu(menu_t *menu)
{
	if (!keys) return 0;
	
	// key handling
	if (KEY2)		// UP
	{
		if (menu->marked > 0) 
			menu->marked--;
	}
	else if (KEY3)		// DOWN
	{
		if (menu->marked < menu->len - 1) 
			menu->marked++;
	}
	else if (KEY4)		// ENTER
		return 1;

	if (menu->marked < menu->top)
		menu->top = menu->marked;
	else if (menu->marked - menu->top >= 5)
		menu->top = menu->marked - 4;

	// upper arrow
	lcdSetPos(0, 58);
	lcdReverse(menu->top == 0);
	lcdWriteGlyph_P(&glyArrowUp, ROP_PAINT);
		
	// text output
	for (uint8_t i = 0; i < 5 && i < menu->len; i++)
	{
		lcdSetPos(i + 1, 0);
		PGM_P item = menu->textSelector(menu->top + i);
		lcdReverse(menu->top + i == menu->marked);
		writePadded_P(item, 21);
	}

	// lower arrow
	lcdSetPos(6, 58);
	lcdReverse(menu->top >= menu->len - 5);
	lcdWriteGlyph_P(&glyArrowDown, ROP_PAINT);
	
	return 0;
}

void _hMenu()
{
	if (doMenu(&mnuMain))
		loadPage(mnuMain.marked + 2);
}

#define CENTER_X	96
#define CENTER_Y	32

static void showMotor(uint8_t motor, uint8_t withDir)
{
	uint8_t x, y;
	mixer_channel_t *channel = &Config.Mixer[motor];
	
	if (channel->IsMotor)
	{
		x = CENTER_X + (channel->Aileron / 4);
		y = CENTER_Y - (channel->Elevator / 4);
	
		lcdLine(x, y, CENTER_X, CENTER_Y);
		lcdXY(CENTER_X - 2, CENTER_Y - 2);
		lcdWriteGlyph_P(&glyFC, ROP_COPY);
		lcdXY(x - 4, y - 4);
		lcdWriteGlyph_P(&glyBall, ROP_PAINT);
		lcdXY(x - 4, y - 7);
		if (channel->Rudder >= 0)
			lcdWriteGlyph_P(&glyDirCW, ROP_PAINT);
		else
			lcdWriteGlyph_P(&glyDirCCW, ROP_PAINT);
		
		lcdXY(x - 2, y - 2);
		lcdReverse(1);
		lcdSelectFont(&font4x6);
		lcdWriteChar(motor + '1');
		lcdSelectFont(NULL);
		lcdReverse(0);
		
		if (withDir)
		{
			lcdSetPos(2, 0);
			lcdWriteString_P(strDirSeen);
			lcdSetPos(5, 0);
			if (channel->Rudder >= 0)
				lcdWriteString_P(strCW);
			else
				lcdWriteString_P(strCCW);
		}
	}
	else if (withDir)
	{
		lcdSetPos(3, 64);
		if (channel->IsServo)
			lcdWriteString_P(strServo);
		else
			lcdWriteString_P(strUnused);
	}
}

static void _hShowModelLayout()
{
	if (ANYKEY)
	{
		if (KEY2)	// NEXT
			elementIndex = (elementIndex + 1) % 9;
		
		lcdClear();
		writeSoftkeys(NULL);
		lcdSetPos(0, 0);
		lcdWriteString_P(strOutput);
		lcdWriteChar(32);
		if (elementIndex == 0)
		{
			lcdWriteString_P(strALL);
			for (uint8_t i = 0; i < 8; i++)
				showMotor(i, 0);
		}			
		else
		{
			lcdWriteChar(elementIndex + '0');
			showMotor(elementIndex - 1, 1);
		}
	}
}

static void _hLoadModelLayout()
{
	if (KEYINIT)
		mnuMLayout.marked = Config.MixerIndex;

	if (elementIndex == 0)
	{
		if (doMenu(&mnuMLayout))
		{
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strAreYouSure);
			writeSoftkeys(_skCANCELYES);
			elementIndex = 1;
		}
	}		
	else if (KEY4)		// YES
	{
		mixerLoadModel(mnuMLayout.marked);
		//configSave();
		loadPage(PAGE_SHOW_LAYOUT);
	}
}

static void _hStart()
{
	char s[7];
	if (KEY4)	// MENU
	{
		arm(OFF);
		loadPage(PAGE_MENU);
		return;
	}
	
	if (KEYINIT || KEYREFRESH)
	{
		if (Config.ArmingMode)
			arm(ON);
		
		if (State.Armed)
		{
			lcdSetPos(3, 36);
			lcdSelectFont(&font12x16);
			lcdWriteString_P(strARMED);
			lcdSelectFont(NULL);
			return;
		}
		else
		{
			lcdSetPos(0, 36);
			lcdSelectFont(&font12x16);
			lcdWriteString_P(strSAFE);
			lcdSelectFont(NULL);
			lcdSetPos(2, 0);
			lcdWriteString_P(scrStart);
		}
	}
	
	if (!State.Armed)
	{
		lcdSetPos(2, 84);
		if (State.SelfLevel)
			writePadded_P(strON, 3);
		else		
			writePadded_P(strOFF, 3);
	
		lcdSetPos(3, 0);
		if (State.Error)
		{
			lcdWriteString_P(strError);
			lcdWriteChar(32);
			if (State.Error & ERR_NOT_CALIBRATED)
				lcdWriteString_P(strSensorNotCal);
			else 
			{
				lcdWriteString_P(PSTR("no "));
				const char* s;
				if ((State.Error & ERR_NO_RX) == ERR_NO_RX)
					s = PSTR("RX");
				else if (State.Error & ERR_NO_ROLL)
					s = strRoll;
				else if (State.Error & ERR_NO_PITCH)
					s = strPitch;
				else if (State.Error & ERR_NO_YAW)
					s = strYaw;
				else 
					s = strThro;
				lcdWriteString_P(s);
				lcdWriteString_P(PSTR(" input"));
			}
		}	
		else
			lcdWriteString_P(PSTR("Ready for departure!"));
	
		// battery level
		lcdSetPos(4, 13*6);
		utoa(BATT / 10, s, 10);
		lcdWriteString(s);
		lcdWriteChar('.');
		utoa(BATT % 10, s, 10);
		writePadded(s, 3);
	
		// roll angle
		writeValue(5, 13*6, (int16_t)ANGLE[ROL] >> 8, 7, -1);
	
		// pitch angle
		writeValue(6, 13*6, (int16_t)ANGLE[PIT] >> 8, 7, -1);
	}	
}

static void _hSensorTest()
{
	for (uint8_t i = 0; i < 3; i++)
	{
		writeValue(i, 48, GYRO[i], 5, -1);
		writeValue(i + 3, 48, ACC[i], 5, -1);
	}	
	writeValue(6, 48, BATT, 5, -1);
}

static void _hReceiverTest()
{
	static const char* const info[6][2] PROGMEM = {
		{ strLeft, strRight },
		{ strForward, strBack },
		{ strRight, strLeft },
		{ strIdle, strFull },
		{ strOff, strOn },
		{ strOff, strOn },
	};
	
	for (uint8_t i = 0; i < 6; i++)
	{
		if (RX_good & _BV(i))
		{
			writeValue(i, 66, RX[i], 4, -1);
			lcdSetPos(i, 96);
			if (i == THR)
			{
				if (State.ThrottleOff)
					writePadded_P(strIdle, 5);
				else if (RX[THR] >= RX_THRESHOLD)
					writePadded_P(strFull, 5);
				else
					writeSpace(5);
			}
			else
			{
				if (abs(RX[i]) > (RX_THRESHOLD / 2))
					writePadded_P((PGM_P)pgm_read_word(&info[i][RX[i] > 0]), 5);
				else
					writeSpace(5);
			}
		}
		else
		{
			lcdSetPos(i, 66);
			lcdWriteString_P(strNoSignal);
		}
	}
}

static void _hSensorCalibration()
{
	if (elementIndex == 0)
	{
		if (KEY4)
		{
			elementIndex = 1;
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strWait);
			lcdSetPos(3, 78);
			lcdWriteString_P(strSec);
			writeSoftkeys(_skCANCEL);
			_tStart = millis();
		}			
	}
	else if (elementIndex == 1)
	{
		lcdSetPos(3, 66);
		uint8_t sec = (millis() - _tStart) / 1000;
		lcdWriteChar((5-sec) + 48);
		if (sec >= 5)
		{
			sensorsCalibrate();
			configSave();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys(NULL);
			elementIndex = 2;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

static void _hESCCalibration()
{
	if (ANYKEY)
	{
		if (elementIndex >= length(scrESCCal))
			loadPage(PAGE_MENU);
		else
		{
			lcdClear();
			PGM_P s = (PGM_P)pgm_read_word(&scrESCCal[elementIndex]);
			lcdWriteString_P(s);
			writeSoftkeys(NULL);
			elementIndex++;
		}		
	}
}

static void _hStickCentering()
{
	if (KEYINIT)
	{
		if ((RX_good & 0x0F) != 0x0F)
		{
			lcdWriteString_P(scrRadioCal1);
			elementIndex = 1;
			writeSoftkeys(_skBACK);
		}
		else
			lcdWriteString_P(scrRadioCal0);
	}
	
	if (elementIndex == 0)
	{
		if (KEY4)
		{
			lcdClear();
			if (rxCalibrate())
			{
				configSave();
				lcdSetPos(3, 0);
				lcdWriteString_P(strCalSucc);
			}
			else
				lcdWriteString_P(scrRadioCal2);

			writeSoftkeys(NULL);
			elementIndex = 1;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

#ifdef DEBUG
static void _hDebug()
{
	writeValue(0, 66, Config.MixerIndex, 2, -1);
	writeValue(1, 66, CONTROL[0], 6, -1);
	writeValue(2, 66, CONTROL[1], 6, -1);
	writeValue(3, 66, CONTROL[2], 6, -1);
	writeValue(4, 66, CONTROL[3], 6, -1);
}
#endif

static void _hFactoryReset()
{
	if (KEYINIT)
	{
		lcdSetPos(3, 18);
		lcdWriteString_P(strAreYouSure);
	}
	else if (KEY4)	// Yes
	{
		configReset();
		configSave();
		
		// force reset by enabling watchdog and enter endless loop
		cli();
		wdt_enable(WDTO_15MS);
		for(;;);
	}
}

static void _hModeSettings()
{
	NOKEYRETURN;
	elementKey(5);
	if (KEY4)	// CHANGE?
	{
		if (elementIndex == 0) Config.SelfLevelMode = (Config.SelfLevelMode + 1) % 3;
		else if (elementIndex == 1) Config.ArmingMode = !Config.ArmingMode;
		else if (elementIndex == 2) Config.LinkRollPitch = !Config.LinkRollPitch;
		else if (elementIndex == 3) Config.AutoDisarm = !Config.AutoDisarm;
		else Config.ReceiverMode = !Config.ReceiverMode; rxInit(Config.ReceiverMode);
		//configSave();
	}
	
	const char* str;
	if (Config.SelfLevelMode == SELFLEVEL_ON) str = strOn;
	else if (Config.SelfLevelMode == SELFLEVEL_AUX) str = strAUX;
	else str = strStick;
	
	writeString_P(0, 84, str, 5, 0);
	writeString_P(1, 84, Config.ArmingMode ? strOn : strStick, 5, 1);
	writeString_P(2, 102, Config.LinkRollPitch ? strYes : strNo, 3, 2);
	writeString_P(3, 84, Config.AutoDisarm ? strYes : strNo, 3, 3);
	writeString_P(4, 84, Config.ReceiverMode ? strYes : strNo, 3, 4);
}

static void _hPIEditor()
{
	NOKEYRETURN;
	
	elementKey(5);

	uint8_t index = subpage;
	if (Config.LinkRollPitch) index = subpage & 0xFE; 
	
	if (KEY4) // CHANGE
	{
		if (elementIndex == 0)
		{
			subpage = (subpage + 1) % 3;
			index = subpage;
			if (Config.LinkRollPitch) index = subpage & 0xFE;
		}			
		else
		{
			startEditMode(&Config.PID[index].UI8[elementIndex - 1], 0, 200, TYPE_UINT8);
			return;
		}
	}
	
	lcdSetPos(0, 30);
	lcdReverse(elementIndex == 0);
	switch (subpage)
	{
		case 0: writePadded_P(strRollAil, 16); break;
		case 1: writePadded_P(strPitchEle, 16); break;
		default: writePadded_P(strYawRud, 16); break;
	}
	
	for (uint8_t i = 0; i < 4; i++)
		writeValue(i + 2, 60, Config.PID[index].UI8[i], 5, i + 1);
}

static void _hMixerEditor()
{
	NOKEYRETURN;
		
	if (KEY4)	// CHANGE?
	{
		if (elementIndex == 0)
			subpage = (subpage + 1) % length(Config.Mixer);
		else if (elementIndex <= 5)
		{
			startEditMode(&Config.Mixer[subpage].I8[elementIndex - 1], -127, 127, TYPE_INT8); 
			return;
		}						
		else if (elementIndex == 6)		// type
		{
			if (Config.Mixer[subpage].Flags == 0)
				Config.Mixer[subpage].Flags = FLAG_ESC | FLAG_HIGH;
			else if (Config.Mixer[subpage].IsMotor)
				Config.Mixer[subpage].Flags = FLAG_SERVO;
			else 
				Config.Mixer[subpage].Flags = FLAG_NONE;
		}		
		else
			Config.Mixer[subpage].Flags ^= FLAG_HIGH;
	}
	
	elementKey(8);
	writeValue(0, 120, subpage + 1, 1, 0);
	for (uint8_t i = 0; i < 5; i++)
		writeValue(i, 60, Config.Mixer[subpage].I8[i], 4, i + 1);
	const char *s;
	
	if (Config.Mixer[subpage].IsMotor)
		s = strESC;
	else if (Config.Mixer[subpage].IsServo)
		s = strServo;
	else
		s = strOff;
		
	writeString_P(5, 36, s, 5, 6);
	writeString_P(5, 108, Config.Mixer[subpage].IsMotor || Config.Mixer[subpage].IsHiRate ? strHigh : strLow, 3, 7);
}

static void simplePageHandler(const edit_element_t *elements, uint8_t len)
{
	NOKEYRETURN;
	edit_element_t element;
	elementKey(len);
	if (KEY4)	// CHANGE?
	{
		memcpy_P(&element, &elements[elementIndex], sizeof(element));
		startEditMode(element.valuePtr, element.loLimit, element.hiLimit, TYPE_UINT8);
		return;
	}
	for (uint8_t i = 0; i < len; i++)
	{
		memcpy_P(&element, &elements[i], sizeof(element));
		writeValue(element.X, element.Y, *(uint8_t*)element.valuePtr, element.len, i);
	}
}

static void _hStickScaling()
{
	static edit_element_t const elements[] PROGMEM = {
		{ 2, 78, &Config.StickScaling[AIL], 0, 200, 5 },
		{ 3, 78, &Config.StickScaling[ELE], 0, 200, 5 },
		{ 4, 78, &Config.StickScaling[RUD], 0, 200, 5 },
		{ 5, 78, &Config.StickScaling[THR], 0, 200, 5 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hMiscSettings()
{
	static edit_element_t const elements[] PROGMEM = {
		{ 0, 102, &Config.MinThrottle, 0, 20, 4 },
		{ 1, 102, &Config.HeightDampening, 0, 250, 4 },
		{ 2, 102, &Config.HeightDampeningLimit, 0, 30, 4 },
		{ 3, 102, &Config.LVA, 0, 250, 4 },
		{ 4, 102, &Config.ServoFilter, 0, 100, 4 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hSelflevelSettings()
{
	static edit_element_t const elements[] PROGMEM = {
		{ 0, 54, &Config.PID_SelfLevel.PGain, 0, 250, 5 },
		{ 1, 54, &Config.PID_SelfLevel.PLimit, 0, 250, 5 },
		{ 3, 96, &Config.AccTrim[ROL], -45, 45, 5 },
		{ 4, 96, &Config.AccTrim[PIT], -45, 45, 5 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hCameraStabSettings()
{
	static edit_element_t const elements[] PROGMEM = {
		{ 2, 84, &Config.Camera.RollGain, -128, 127, 5 },
		{ 3, 84, &Config.Camera.RollOffset, -128, 127, 5 },
		{ 4, 84, &Config.Camera.PitchGain, -128, 127, 5 },
		{ 5, 84, &Config.Camera.PitchOffset, -128, 127, 5 },
	};
	simplePageHandler(elements, length(elements));
}

static void writeCPPMScreen(uint8_t index)
{
	lcdClear();
	lcdWriteString_P(strCPPMSettings);
	lcdSetPos(2, 0);
	if (index == 0)
		lcdWriteString_P(scrCPPMSettings1);
	else
		lcdWriteString_P(scrCPPMSettings2);
	writeSoftkeys(NULL);
}

static void _hCPPMSettings()
{
	NOKEYRETURN;
	uint8_t oldsubpage = elementIndex / 4;
	elementKey(8);
	uint8_t subpage = elementIndex / 4;
	
	if (KEYINIT || KEYREFRESH || oldsubpage != subpage)
		writeCPPMScreen(subpage);
	
	if (KEY4)	// CHANGE?
	{
		startEditMode(&Config.RX_chmap[subpage * 4 + elementIndex - 1], 1, 8, TYPE_UINT8);
		return;
	}
	
	for (int i = 0; i < 4; i++)
		writeValue(2 + i, 13*6, Config.RX_chmap[subpage * 4 + i], 1, i + subpage * 4);
}

void menuLoop()
{
	if (keys == 0)
		keys = keyboardRead();
		
	if (KEY1 && !editMode)	// BACK
	{
		if (menuPage > PAGE_MENU)
			loadPage(PAGE_MENU);
		else if (menuPage == PAGE_MENU)
		{
			configSave();
			loadPage(PAGE_START);
		}			
	}
	
	if (KEYMENU)
		keys = KEY_INIT;
	defaultHandler();
	keys &= KEY_MENU;
}

void menuInit()
{
	loadPage(PAGE_START);
}

void menuRefresh()
{
	keys |= KEY_REFRESH;
	menuLoop();
}

static PGM_P tsmMain(uint8_t index)
{
	return (PGM_P)pgm_read_word(&lstMenu[index]);
}

static PGM_P tsmLoadModelLayout(uint8_t index)
{
	model_t model;
	model_t* pmodel = (model_t*)pgm_read_word(&modelTable[index]);
	memcpy_P(&model, pmodel, sizeof(model));
	return (PGM_P)model.Name;
}

#endif /* NO_LCD */
