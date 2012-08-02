/*
 * lcd.c
 *
 * Created: 27.07.2012 10:06:52
 *  Author: OliverS
 *
 * $Id$
*/ 

#include "global.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "lcd.h"

#define BUFFERED

#define REVERSED	1
#define BIGFONT		2
static uint8_t _flags = 0;

#ifdef BUFFERED
static uint8_t _screen[128 * 8];
static uint8_t* _write_ptr;
#endif

static void sendByte(uint8_t byte)
{
	for (uint8_t i = 8; i; i--)
	{
		LCD_SCL = 0;
		if (byte & 0x80)
			LCD_SDA = 1;
		else
			LCD_SDA = 0;
		LCD_SCL = 1;
		byte <<= 1;
	}
}

static void sendCommand(uint8_t command)
{
	LCD_CS = 0;
	LCD_A0 = 0;
	sendByte(command);
	LCD_CS = 1;
}

static void sendData(uint8_t data)
{
	LCD_CS = 0;
	LCD_A0 = 1;
	sendByte(data);
	LCD_CS = 1;
}

static void incWritePtr(uint8_t val)
{
	_write_ptr += val;
	if (_write_ptr >= _screen + sizeof(_screen))
		_write_ptr -= _screen;
}

static uint8_t curLine()
{
	return (_write_ptr - _screen) / 128;
}

static uint8_t curColumn()
{
	return (_write_ptr - _screen) % 128;
}

static void writeData(uint8_t data)
{
	if (_flags & REVERSED) data ^= 0xFF;
#ifdef BUFFERED
	*_write_ptr = data;
	incWritePtr(1);
#else
	sendData(data);
#endif
}

void _lcdSetPos(uint8_t line, uint8_t column)
{
	sendCommand(0xB0 | (line & 0x07));
	sendCommand(0x10 | (column >> 4));
	sendCommand(column & 0x0f);
}

void lcdSetPos(uint8_t line, uint8_t column)
{
#ifdef BUFFERED
	line = line % 8;
	column = column % 128;
	_write_ptr = _screen + (line * 128 + column);
#else
	_lcdSetPos(line, column);
#endif
}

void lcdClear()
{
#ifdef BUFFERED
	memset(_screen, 0, sizeof(_screen));
#else
	for (uint8_t i = 0; i < 8; i++)	
	{
		lcdSetPos(i, 0);
		for (uint8_t j = 0; j < 128; j++)
			sendData(0);
	}			
#endif
	lcdSetPos(0, 0);
	_flags = 0;
}

void lcdWriteChar(char c)
{
	uint8_t b;
#ifdef BUFFERED
	if (_flags & BIGFONT)
	{
		uint8_t* w = _write_ptr;
		for (uint8_t i = 0; i < 12; i++)
		{
			b = pgm_read_byte(&lcdFontBig[c-32][i*2]);
			writeData(b);
		}
		_write_ptr = w + 128;
		for (uint8_t i = 0; i < 12; i++)
		{
			b = pgm_read_byte(&lcdFontBig[c-32][i*2+1]);
			writeData(b);
		}
		_write_ptr = w + 12;
	}		
	else
	{
#endif
		if (c == '\n')
		{
			lcdSetPos(curLine() + 1, 0);
		}			
		else
			for (uint8_t i = 0; i < 6; i++)
			{
				b = pgm_read_byte(&lcdFontSmall[c-32][i]);
				writeData(b);
			}
#ifdef BUFFERED
	}		
#endif
}

void lcdWriteString(char *s)
{
	char c;
	while ((c = *s++))
		lcdWriteChar(c);
}

void lcdWriteString_P(PGM_P s)
{
	char c;
	while ((c = pgm_read_byte(s++)))
		lcdWriteChar(c);
}

void lcdReverse(uint8_t reversed)
{
	if (reversed)
		_flags |= REVERSED;
	else
		_flags &= ~REVERSED;
}

void lcdBigFont(uint8_t bigfont)
{
	if (bigfont)
		_flags |= BIGFONT;
	else
		_flags &= ~BIGFONT;
}

void lcdWriteImage_P(PGM_P image, uint8_t width)
{
	for (uint8_t i = 0; i < width; i++)	
		writeData(pgm_read_byte(image + i));
}

void lcdSetContrast(uint8_t contrast)
{
	sendCommand(0x81);
	sendCommand(contrast & 0x3F); 
}

void lcdFill(uint8_t c, uint8_t width)
{
	for (uint8_t i = 0; i < width; i++)
		writeData(c);
}

void lcdInit()
{
#ifdef BUFFERED
	_write_ptr = _screen;
#endif

	// pins
	LCD_CS_DIR = OUTPUT;
	LCD_RST_DIR = OUTPUT;
	LCD_A0_DIR = OUTPUT;
	LCD_SCL_DIR = OUTPUT;
	LCD_SDA_DIR = OUTPUT;

	LCD_RST = 0;
	_delay_ms(1);
	LCD_RST = 1;
	_delay_ms(1);
	
	sendCommand(0xA2); // set bias to 1/9
	sendCommand(0xA0); // SEG output direction = normal
	sendCommand(0xC8); // COM output direction = reversed
	sendCommand(0x40); // start line = 0
	
	sendCommand(0xA6); // Normal Display
	sendCommand(0xA4); // display all points = OFF
	sendCommand(0x2F); // Power Control Set
	sendCommand(0x24); // set ra/rb
	lcdSetContrast(0x20);
	sendCommand(0xAF);	// Display on
}

void lcdOutput()
{
#ifdef BUFFERED 
	#define NUMCHAR	32
	static uint8_t pos;
	uint8_t* ptr = _screen + pos * NUMCHAR;
	_lcdSetPos((pos * NUMCHAR) / 128 , (pos * NUMCHAR) % 128);
	for (uint8_t i = 0; i < NUMCHAR; i++)
	{
		uint8_t b = *(ptr + i);
		sendData(b);
	}
	pos = (pos + 1) % (sizeof(_screen) / NUMCHAR);
#endif
}

