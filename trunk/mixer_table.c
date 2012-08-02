/*
 * mixer_table.c
 *
 * Created: 30.07.2012 12:39:29
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "mixer.h"
#include <avr/pgmspace.h>

PROGMEM mixer_model_t mixerTable[MIXER_TABLE_LEN] = {
	// QuadroCopter +
	{
		"QuadroCopter +",
		{
			{FLAG_ESC, 100, 0, 100, 100, 0},
			{FLAG_ESC, 100, 100, 0, -100, 0},			
			{FLAG_ESC, 100, 0, -100, 100, 0},			
			{FLAG_ESC, 100, -100, 0, -100, 0},			
			{FLAG_NONE},			
			{FLAG_NONE},			
			{FLAG_NONE},			
			{FLAG_NONE},			
		},
	},	
	
	// QuadroCopter x
	{
		"QuadroCopter x",
		{
			{FLAG_ESC, 100, -71, 71, 100, 0},
			{FLAG_ESC, 100, 71, 71, -100, 0},			
			{FLAG_ESC, 100, 71, -71, 100, 0},			
			{FLAG_ESC, 100, -71, -71, -100, 0},			
			{FLAG_NONE},			
			{FLAG_NONE},			
			{FLAG_NONE},			
			{FLAG_NONE},			
		},
	},	
};
