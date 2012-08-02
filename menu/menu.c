/*
 * menu.c
 *
 * Created: 30.07.2012 18:52:51
 *  Author: OliverS
 */ 

#include "global.h"
#include "lcd.h"
#include "keyboard.h"
#include "sensors.h"
#include "rx.h"
#include "mixer.h"
#include <avr/pgmspace.h>
#include <stdlib.h>

uint8_t _mykey;
#define KEY_INIT	1
#define ISINIT		(_mykey == KEY_INIT)
#define KEY1		(_mykey == KEY_1)
#define KEY2		(_mykey == KEY_2)
#define KEY3		(_mykey == KEY_3)
#define KEY4		(_mykey == KEY_4)
#define ANYKEY		(_mykey)
#define NOKEYRETURN {if (!_mykey) return;}

#define P_STR static const prog_char

typedef const prog_char screen[7][22];
typedef struct  
{
	uint8_t posX, posY;
	const prog_char *text;
} PROGMEM element;

#include "menu_text.h"
#include "menu_screen.h"

typedef void (pageHandler)(void);
typedef struct  
{
	element *softkeys;
	uint8_t numSoftkeys;	
	pageHandler *handler;
	void *staticElements;
	uint8_t numStaticElements;
} page_t;


//////////////////////////////////////////////////////////////////////////
void _hStart();
void _hMenu();
static element _sePIEditor[] = {
	{ 0,  0, strAxis},
	{ 1,  0, strPGain},
	{ 2,  0, strPLimit},
	{ 3,  0, strIGain},
	{ 4,  0, strILimit},
};

void _hReceiverTest();
static element _seReceiverTest[] = {
	{ 0,  0, strAileron },
	{ 1,  0, strElevator },
	{ 2,  0, strThrottle },
	{ 3,  0, strRudder },
	{ 4,  0, strAuxiliary },
};

void _hSensorTest();
static element _seSensorTest[] = {
	{ 0,  0, strGyro },
	{ 0, 30, strX },
	{ 1,  0, strGyro },
	{ 1, 30, strY },
	{ 2,  0, strGyro },
	{ 2, 30, strZ },
	{ 3,  0, strAcc },
	{ 3, 30, strX },
	{ 4,  0, strAcc },
	{ 4, 30, strY },
	{ 5,  0, strAcc },
	{ 5, 30, strZ }
};

void _hSensorCalibration();
void _hESCCalibration();
void _hRadioCalibration();
void _hLoadMotorLayout();

//////////////////////////////////////////////////////////////////////////
// softkey lines
static element _skSTART[] = { { 7, 102, strMENU } };
static element _skMENU[] = { 
	{ 7, 0, strBACK },
	{ 7, 36, strUP },
	{ 7, 66, strDOWN },
	{ 7, 96, strENTER },
};
static element _skBACK[] = { { 7, 0, strBACK} };
static element _skCONTINUE[] = { { 7, 78, strCONTINUE} };
static element _skPAGE[] = {
	{ 7, 0, strBACK },
	{ 7, 30, strPREV },
	{ 7, 60, strNEXT },
	{ 7, 90, strCHANGE },
};

//////////////////////////////////////////////////////////////////////////
page_t pages[] PROGMEM = {
/*  0 */	{ _skSTART, length(_skSTART), _hStart },
/*  1 */	{ _skMENU, length(_skMENU), _hMenu },
/*  2 */	{ _skPAGE, length(_skPAGE), NULL, _sePIEditor, length(_sePIEditor)},
/*  3 */	{ _skBACK, 1, _hReceiverTest, _seReceiverTest, length(_seReceiverTest)},
/*  4 */	{  },
/*  5 */	{  },
/*  6 */	{  },
/*  7 */	{  },
/*  8 */	{ _skBACK, 1, _hSensorTest, _seSensorTest, length(_seSensorTest)},
/*  9 */	{ _skCONTINUE, 1, _hSensorCalibration, scrSensorCal[0]},
/* 10 */	{ _skCONTINUE, 1, _hESCCalibration},
/* 11 */	{ _skCONTINUE, 1, _hRadioCalibration, scrRadioCal[0]},
/* 12 */	{  },
/* 13 */	{  },
/* 14 */	{ _skMENU, length(_skMENU), _hLoadMotorLayout },
/* 15 */	{  },
};

#define NO_PAGE 0xFF

typedef struct  
{
	uint8_t len;
	PGM_P (*textSelector)(uint8_t);
	uint8_t top;
	uint8_t marked;
} menu_t;

const prog_char *lstMenu[] PROGMEM = {
	strPIEditor,
	strReceiverTest,
	strModeSettings,
	strStickScaling,
	strMiscSettings,
	strSelflevelSettings,
	strSensorTest,
	strSensorCalibration,
	strESCCalibration,
	strRadioCalibration,
	strMixerEditor,
	strShowMotorLayout,
	strLoadMotorLayout,
	strDebug,
};

#define PAGE_START	0
#define PAGE_MENU	1

PGM_P tsmMain(uint8_t);
PGM_P tsmLoadMotorLayout(uint8_t);

static uint8_t page, subpage;
static uint16_t _tStart;
static page_t currentPage;
static menu_t mnuMain = {length(lstMenu), tsmMain};
static menu_t mnuMLayout = {MIXER_TABLE_LEN, tsmLoadMotorLayout};

void writeScreen(screen theScreen)
{
	for (uint8_t i = 0; i < 7; i++)
	{
		lcdSetPos(i, 0);
		lcdWriteString_P(theScreen[i]);
	}
}

static void writeList(element list[], uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		element e;
		memcpy_P(&e, &list[i], sizeof(e));
		lcdSetPos(e.posX, e.posY);
		lcdWriteString_P(e.text);
	}
}

static void writeSoftkeys()
{
	if (currentPage.numSoftkeys)
		writeList(currentPage.softkeys, currentPage.numSoftkeys);	
}

void loadPage(uint8_t pageIndex)
{
	memcpy_P(&currentPage, &pages[pageIndex], sizeof(currentPage));
	page = pageIndex;
}

void defaultHandler()
{
	if (currentPage.softkeys == NULL)
	{
		if (ISINIT)
		{
			lcdWriteString_P(PSTR("Under construction."));
			writeList(_skBACK, 1);
		}
		else if (KEY1)
			loadPage(PAGE_MENU);
			
		return;
	}

	if (ISINIT)
	{
		if (currentPage.numStaticElements)
			writeList(currentPage.staticElements, currentPage.numStaticElements);
		else if (currentPage.staticElements)
			writeScreen(currentPage.staticElements);
		writeSoftkeys();
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
		{
			menu->marked--;
			if (menu->marked < menu->top)
				menu->top = menu->marked;
		}
	}					
	else if (KEY3)		// DOWN
	{
		if (menu->marked < menu->len - 1) 
		{
			menu->marked++;
			if (menu->marked - menu->top >= 5)
				menu->top = menu->marked - 4;
		}
	}
	else if (KEY4)		// ENTER
		return 1;
	
	// text output
	lcdSetPos(0, 58);
	if (menu->top > 0)
		lcdWriteImage_P(lcdArrowUp, sizeof(lcdArrowUp));
	else
		lcdFill(0, sizeof(lcdArrowUp));
		
	for (uint8_t i = 0; i < 5 && i < menu->len; i++)
	{
		lcdSetPos(i + 1, 0);
		PGM_P item = menu->textSelector(menu->top + i);
		if (menu->top + i == menu->marked)
			lcdReverse(1);
		else
			lcdReverse(0);
		lcdWriteString_P(item);
		lcdFill(0, (21 - strlen_P(item)) * 6);
	}
	
	lcdReverse(0);
	lcdSetPos(6, 58);
	if (menu->top < menu->len - 5)
		lcdWriteImage_P(lcdArrowDown, sizeof(lcdArrowDown));
	else
		lcdFill(0, sizeof(lcdArrowDown));	
	
	return 0;
}

void _hMenu()
{
	if (doMenu(&mnuMain))
		loadPage(mnuMain.marked + 2);
}

void _hLoadMotorLayout()
{
	doMenu(&mnuMLayout);
}

void _hStart()
{
	NOKEYRETURN;
	if (KEY4)	// MENU
	{
		loadPage(PAGE_MENU);
		return;
	}
	
	if (ISINIT)
	{
		lcdSetPos(0, 36);
		lcdBigFont(1);
		lcdWriteString_P(strSAFE);
		lcdBigFont(0);
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
}

void _hSensorTest()
{
	char s[7];
	utoa(GYRO_raw[0], s, 10);
	lcdSetPos(0, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
	utoa(GYRO_raw[1], s, 10);
	lcdSetPos(1, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
	utoa(GYRO_raw[2], s, 10);
	lcdSetPos(2, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
	
	utoa(ACC_raw[0], s, 10);
	lcdSetPos(3, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
	utoa(ACC_raw[1], s, 10);
	lcdSetPos(4, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
	utoa(ACC_raw[2], s, 10);
	lcdSetPos(5, 48);
	lcdWriteString(s);
	lcdFill(0, 6);
}

void _hReceiverTest()
{
	char s[7];
	for (uint8_t i = 0; i < RX_CHANNELS; i++)
	{
		lcdSetPos(i, 66);
		if (RX_raw[i])
		{
			itoa(RX[i], s, 10);
			lcdWriteString(s);
			lcdFill(0, 30);
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
			_tStart = millis();
		}			
	}
	else if (subpage == 1)
	{
		lcdSetPos(3, 18);
		lcdWriteString_P(strWait);
		lcdSetPos(3, 66);
		uint8_t sec = (millis() - _tStart) / 1000;
		lcdWriteChar((5-sec) + 48);
		lcdWriteString_P(strSec);
		if (sec >= 5)
		{
			sensorsCalibateGyro();
			sensorsCalibateAcc();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys();
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
			writeScreen(scrESCCal[subpage]);
			writeSoftkeys();
			subpage++;
		}		
	}
}

void _hRadioCalibration()
{
	if (subpage == 0)
	{
		if (KEY4)
		{
			rxCalibrate();
			lcdClear();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys();
			subpage = 1;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

void menuShow()
{
	static uint8_t oldPage = NO_PAGE;
	_mykey = keyboardRead();
	if (KEY1)	// BACK
	{
		if (page > PAGE_MENU)
			loadPage(PAGE_MENU);
		else if (page == PAGE_MENU)
			loadPage(PAGE_START);
	}
		
	if (oldPage != page)
	{
		_mykey = KEY_INIT;
		subpage = 0;
		lcdClear();
		oldPage = page;
	}			
	defaultHandler();
}

void menuInit()
{
	loadPage(PAGE_START);
	//loadPage(14);
}

PGM_P tsmMain(uint8_t index)
{
	return (PGM_P)pgm_read_word(&lstMenu[index]);
}

PGM_P tsmLoadMotorLayout(uint8_t index)
{
	return mixerTable[index].Name;
}
