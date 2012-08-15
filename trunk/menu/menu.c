/*
 * menu.c
 *
 * Created: 30.07.2012 18:52:51
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "lcd.h"
#include "keyboard.h"
#include "sensors.h"
#include "rx.h"
#include "mixer.h"
#include "buzzer.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/wdt.h>

uint8_t _mykey;
#define KEY_INIT	1
#define ISINIT		(_mykey & KEY_INIT)
#define KEY1		(_mykey & KEY_1)
#define KEY2		(_mykey & KEY_2)
#define KEY3		(_mykey & KEY_3)
#define KEY4		(_mykey & KEY_4)
#define ANYKEY		(_mykey)
#define KEYPRESS	(_mykey & (KEY_1|KEY_2|KEY_3|KEY_4))
#define NOKEYRETURN {if (!_mykey) return;}

typedef const prog_char screen_t[7][22];
typedef struct  
{
	uint8_t posX, posY;
	const prog_char *text;
} PROGMEM element_t;

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
#define P_STR static const prog_char
#include "menu_text.h"
#include "menu_screen.h"

//////////////////////////////////////////////////////////////////////////
void _hStart();
void _hMenu();
void _hReceiverTest();
void _hSensorTest();
void _hSensorCalibration();
void _hESCCalibration();
void _hStickCentering();
void _hShowModelLayout();
void _hLoadModelLayout();
void _hDebug();
void _hFactoryReset();

//////////////////////////////////////////////////////////////////////////
// softkeys
static const prog_char _skSTART[]     = "                 MENU";
static const prog_char _skMENU[]      = "BACK  UP   DOWN ENTER";
static const prog_char _skBACK[]      = "BACK";
static const prog_char _skCONTINUE[]  = "BACK         CONTINUE";
static const prog_char _skCANCELYES[] = "CANCEL            YES";
static const prog_char _skPAGE[]      = "BACK PREV NEXT CHANGE";
static const prog_char _skBACKNEXT[]  = "BACK  NEXT";
static const prog_char _skCANCEL[]    = "CANCEL";

//////////////////////////////////////////////////////////////////////////
static const page_t pages[] PROGMEM = {
/*  0 */	{ _skSTART, _hStart },
/*  1 */	{ _skMENU, _hMenu },
/*  2 */	{ _skPAGE, NULL, scrPIEditor},
/*  3 */	{ _skBACK, _hReceiverTest, scrReceiverTest},
/*  4 */	{ _skPAGE, NULL, scrModeSettings},
/*  5 */	{ _skPAGE, NULL, scrStickScaling},
/*  6 */	{ _skPAGE, NULL, scrMiscSettings},
/*  7 */	{ _skPAGE, NULL, scrSelflevelSettings},
/*  8 */	{ _skBACK, _hSensorTest, scrSensorTest},
/*  9 */	{ _skCONTINUE, _hSensorCalibration, scrSensorCal0},
/* 10 */	{ _skCONTINUE, _hESCCalibration, scrESCCal0},
/* 11 */	{ _skPAGE, NULL, scrCPPMSettings},
/* 12 */	{ _skCONTINUE, _hStickCentering, scrRadioCal0},
/* 13 */	{ _skPAGE, NULL, scrMixerEditor},
/* 14 */	{ _skBACKNEXT, _hShowModelLayout},
/* 15 */	{ _skMENU, _hLoadModelLayout },
/* 16 */	{ _skBACK, _hDebug },
/* 16 */	{ _skCANCELYES, _hFactoryReset },
};

static const prog_char *lstMenu[] PROGMEM = {
	strPIEditor,
	strReceiverTest,
	strModeSettings,
	strStickScaling,
	strMiscSettings,
	strSelflevelSettings,
	strSensorTest,
	strSensorCalibration,
	strESCCalibration,
	strCPPMSettings,
	strRadioCalibration,
	strMixerEditor,
	strShowMotorLayout,
	strLoadMotorLayout,
	strDebug,
	strFactoryReset,
};

#define PAGE_START			0
#define PAGE_MENU			1
#define PAGE_SHOW_LAYOUT	14

PGM_P tsmMain(uint8_t);
PGM_P tsmLoadModelLayout(uint8_t);

static uint8_t page, subpage;
static uint16_t _tStart;
static page_t currentPage;
static menu_t mnuMain = {length(lstMenu), tsmMain};
static menu_t mnuMLayout = {MIXER_TABLE_LEN, tsmLoadModelLayout};

static void writeList(const element_t list[], uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		element_t e;
		memcpy_P(&e, &list[i], sizeof(e));
		lcdSetPos(e.posX, e.posY);
		lcdWriteString_P(e.text);
	}
}

static void writeSoftkeys(const char* sk)
{
	if (!sk)
		sk = currentPage.softkeys;
	if (sk)
	{
		lcdSetPos(7, 0);
		lcdWriteString_P(sk);
	}
}

void loadPage(uint8_t pageIndex)
{
	memcpy_P(&currentPage, &pages[pageIndex], sizeof(currentPage));
	page = pageIndex;
}

void defaultHandler()
{
	if (ISINIT)
	{
		if (currentPage.screen)
			lcdWriteString_P(currentPage.screen);
		writeSoftkeys(currentPage.softkeys);
	}
		
	if (currentPage.handler)
		currentPage.handler();
}

uint8_t doMenu(menu_t *menu)
{
	if (!_mykey) return 0;
	
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
	
	// text output
	lcdSetPos(0, 58);
	if (menu->top > 0)
		lcdWriteGlyph_P(&glyArrowUp, 0);
		
	for (uint8_t i = 0; i < 5 && i < menu->len; i++)
	{
		lcdSetPos(i + 1, 0);
		PGM_P item = menu->textSelector(menu->top + i);
		if (menu->top + i == menu->marked)
			lcdReverse(1);
		else
			lcdReverse(0);
		lcdWriteString_P(item);
		for (uint8_t j = 0; j < 21 - strlen_P(item); j++)
			lcdWriteChar(32);
	}

	lcdReverse(0);
	lcdSetPos(6, 58);
	if (menu->top < menu->len - 5)
		lcdWriteGlyph_P(&glyArrowDown, 0);
	
	return 0;
}

void _hMenu()
{
	if (doMenu(&mnuMain))
		loadPage(mnuMain.marked + 2);
}

static void showMotor(uint8_t motor, uint8_t withDir)
{
	uint8_t x = 96;
	uint8_t y = 32;
	mixer_channel_t *channel = &Config.Mixer.Channel[motor];
	
	if (channel->flags & FLAG_ESC)
	{
		x += (channel->Aileron >> 2);
		y -= (channel->Elevator >> 2);
	
		lcdLine(x, y, 96, 32);
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
		if (channel->flags == FLAG_NONE)
			lcdWriteString_P(strUnused);
		else
			lcdWriteString_P(strServo);
	}
}

void _hShowModelLayout()
{
	if (ANYKEY)
	{
		if (KEY2)	// NEXT
			subpage = (subpage + 1) % 9;
		
		lcdClear();
		lcdWriteString_P(strOutput);
		lcdWriteChar(32);
		if (subpage == 0)
		{
			lcdWriteString_P(strALL);
			for (uint8_t i = 0; i < 8; i++)
				showMotor(i, 0);
		}			
		else
		{
			lcdWriteChar(subpage + '0');
			showMotor(subpage - 1, 1);
		}
		writeSoftkeys(NULL);
	}
}

void _hLoadModelLayout()
{
	if (ISINIT)
		mnuMLayout.marked = Config.MixerIndex;

	if (subpage == 0)
	{
		if (doMenu(&mnuMLayout))
		{
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strAreYouSure);
			writeSoftkeys(_skCANCELYES);
			subpage = 1;
		}
	}		
	else if (KEY4)		// YES
	{
		mixerLoadTable(mnuMLayout.marked);
		configSave();
		loadPage(PAGE_SHOW_LAYOUT);
	}
}

void _hStart()
{
	char s[7];
	if (KEY4)	// MENU
	{
		loadPage(PAGE_MENU);
		return;
	}
	
	if (ISINIT)
	{
		lcdSetPos(0, 36);
		lcdSelectFont(&font12x16);
		lcdWriteString_P(strSAFE);
		lcdSelectFont(NULL);
		lcdSetPos(3, 0);
		lcdWriteString_P(strSelflevel);
		lcdWriteString_P(strSpIsSp);
		lcdSetPos(5, 0);
		lcdWriteString_P(strBattery);
	}
	
	lcdSetPos(3, 84);
	if (State.SelfLevel)
		lcdWriteString_P(strON);
	else		
		lcdWriteString_P(strOFF);
	
	lcdSetPos(4, 0);
	if (State.Error)
	{
		lcdWriteString_P(strError);
		lcdWriteChar(32);
		if (State.Error & ERR_NOT_CALIBRATED)
			lcdWriteString_P(strSensorNotCal);
		else 
		{
			lcdWriteString_P(PSTR("no "));
			if (State.Error & ERR_NO_ROLL)
				lcdWriteString_P(strRoll);
			else if (State.Error & ERR_NO_PITCH)
				lcdWriteString_P(strPitch);
			else if (State.Error & ERR_NO_YAW)
				lcdWriteString_P(strYaw);
			else 
				lcdWriteString_P(strThro);
			lcdWriteString_P(PSTR(" input"));
		}
	}	
	else
		lcdWriteString_P(PSTR("Ready for departure!"));
	
	lcdSetPos(5, 9*6);
	utoa(BATT / 10, s, 10);
	lcdWriteString(s);
	lcdWriteChar('.');
	utoa(BATT % 10, s, 10);
	lcdWriteString(s);
	lcdWriteString_P(PSTR(" V "));
}

void _hSensorTest()
{
	char s[7];
	utoa(GYRO_raw[0], s, 10);
	lcdSetPos(0, 48);
	lcdWriteString(s);
	utoa(GYRO_raw[1], s, 10);
	lcdSetPos(1, 48);
	lcdWriteString(s);
	utoa(GYRO_raw[2], s, 10);
	lcdSetPos(2, 48);
	lcdWriteString(s);
	
	utoa(ACC_raw[0], s, 10);
	lcdSetPos(3, 48);
	lcdWriteString(s);
	utoa(ACC_raw[1], s, 10);
	lcdSetPos(4, 48);
	lcdWriteString(s);
	utoa(ACC_raw[2], s, 10);
	lcdSetPos(5, 48);
	lcdWriteString(s);
	
	utoa(BATT, s, 10);
	lcdSetPos(6, 48);
	lcdWriteString(s);
}

void _hReceiverTest()
{
	char s[7];
	for (uint8_t i = 0; i < RX_CHANNELS; i++)
	{
		lcdSetPos(i, 66);
		if (RX_good & _BV(i))
		{
			itoa(RX[i], s, 10);
			lcdWriteString(s);
		}
		else
			lcdWriteString_P(strNoSignal);
	}			
}

void _hSensorCalibration()
{
	if (subpage == 0)
	{
		if (KEY4)
		{
			subpage = 1;
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strWait);
			lcdSetPos(3, 78);
			lcdWriteString_P(strSec);
			writeSoftkeys(_skCANCEL);
			_tStart = millis();
		}			
	}
	else if (subpage == 1)
	{
		lcdSetPos(3, 66);
		uint8_t sec = (millis() - _tStart) / 1000;
		lcdWriteChar((5-sec) + 48);
		if (sec >= 5)
		{
			sensorsCalibate();
			configSave();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys(NULL);
			subpage = 2;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

void _hESCCalibration()
{
	if (ANYKEY)
	{
		if (subpage >= length(scrESCCal))
			loadPage(PAGE_MENU);
		else
		{
			lcdClear();
			PGM_P s = (PGM_P)pgm_read_word(&scrESCCal[subpage]);
			lcdWriteString_P(s);
			writeSoftkeys(NULL);
			subpage++;
		}		
	}
}

void _hStickCentering()
{
	if (subpage == 0)
	{
		if (KEY4)
		{
			rxCalibrate();
			configSave();
			lcdClear();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys(NULL);
			subpage = 1;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

void _hDebug()
{
	lcdSetPos(0, 0);
	lcdWriteString_P(PSTR("MixerIndex: "));
	char s[7];
	utoa(Config.MixerIndex, s, 10);
	lcdWriteString(s);
}

void _hFactoryReset()
{
	if (ISINIT)
	{
		lcdSetPos(3, 18);
		lcdWriteString_P(strAreYouSure);
	}
	else if (KEY4)	// Yes
	{
		configReset();
		configSave();
		cli();
		wdt_enable(WDTO_15MS);
		for(;;);
	}
}

void menuShow()
{
	static uint8_t oldPage = 0xFF;
	
	_mykey = keyboardRead();
		
	if (KEY1)	// BACK
	{
		if (page > PAGE_MENU)
			loadPage(PAGE_MENU);
		else if (page == PAGE_MENU)
			loadPage(PAGE_START);
	}
	
	lcdDisable();
	if (oldPage != page)
	{
		_mykey |= KEY_INIT;
		subpage = 0;
		lcdClear();
		oldPage = page;
	}
	defaultHandler();
	lcdEnable();

	if (KEYPRESS)
		buzzerBuzz(5);
}

void menuInit()
{
	loadPage(PAGE_START);
}

PGM_P tsmMain(uint8_t index)
{
	return (PGM_P)pgm_read_word(&lstMenu[index]);
}

PGM_P tsmLoadModelLayout(uint8_t index)
{
	return (PGM_P)pgm_read_word(&mixerTable[index].Name);
}
