/*
 * lcd.h
 *
 * Created: 27.07.2012 13:13:11
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "fonts.h"
#include "glyphs.h"
#include <avr/pgmspace.h>

void lcdInit();
void lcdClear();
void lcdSetPos(uint8_t line, uint8_t column);
void lcdWriteChar(char c);
void lcdWriteString(char *s);
void lcdWriteString_P(PGM_P s);
void lcdReverse(uint8_t reversed);
void lcdSetContrast(uint8_t contrast);
void lcdSetPixel(uint8_t x, uint8_t y, uint8_t on);
void lcdLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void lcdEnable();
void lcdDisable();
void lcdSelectFont(const fontdescriptor_t *font);
void lcdXY(uint8_t x, uint8_t y);
void lcdWriteGlyph_P(const glyph_t *glyph, uint8_t mode);
void lcdFillRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void lcdRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

#define ROP_COPY	0
#define ROP_PAINT	1
#define ROP_INVERT	2

#endif /* LCD_H_ */