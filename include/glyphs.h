/*
 * glyphs.h
 *
 * Created: 14.08.2012 19:53:06
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef GLYPHS_H_
#define GLYPHS_H_

#include <avr/pgmspace.h>

typedef struct
{
	uint8_t sizeX, sizeY;
	prog_char glyph[];
} glyph_t;

extern const glyph_t glyArrowDown;
extern const glyph_t glyArrowUp;
extern const glyph_t glyBall;
extern const glyph_t glyDirCW;
extern const glyph_t glyDirCCW;

#endif /* SYMBOLS_H_ */