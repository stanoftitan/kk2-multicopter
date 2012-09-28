/*
 * fonts.h
 *
 * Created: 14.08.2012 14:07:51
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef FONTS_H_
#define FONTS_H_

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct  
{
	uint8_t sizeX, sizeY;
	PGM_P (*selector)(unsigned char);
} fontdescriptor_t;


extern const fontdescriptor_t font4x6;
extern const fontdescriptor_t font6x8;
extern const fontdescriptor_t font12x16;

extern const fontdescriptor_t *fonts[];

#endif /* FONT_H_ */