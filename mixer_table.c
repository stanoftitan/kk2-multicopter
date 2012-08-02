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

#define P_STR static const prog_char

P_STR str00[] = "SingleCopter 2M 2S";
P_STR str01[] = "SingleCopter 1M 4S";
P_STR str02[] = "DualCopter";
P_STR str03[] = "TriCopter";
P_STR str04[] = "Y6";
P_STR str05[] = "QuadroCopter +";
P_STR str06[] = "QuadroCopter x";
P_STR str07[] = "X8 +";
P_STR str08[] = "X8 x";
P_STR str09[] = "HexaCopter +";
P_STR str10[] = "HexaCopter x";
P_STR str11[] = "OctoCopter +";
P_STR str12[] = "OctoCopter x";
P_STR str13[] = "H6";
P_STR str14[] = "H8";
P_STR str15[] = "V6";
P_STR str16[] = "V8";
P_STR str17[] = "Airplane 1S Ail";
P_STR str18[] = "Airplane 2S Ail";
P_STR str19[] = "Flying Wing";

PROGMEM mixer_model_t mixerTable[MIXER_TABLE_LEN] = {
	// SingleCopter 2M 2S
	{
		str00,
		{
			
		},		
	},
	
	// SingleCopter 1M 4S
	{
		str01,
		{
			
		},		
	},
	
	// DualCopter
	{
		str02,
		{
			
		},		
	},
	
	// TriCopter
	{
		str03,
		{
			
		},		
	},
	
	// Y6
	{
		str04,
		{
			
		},		
	},
	

	// QuadroCopter +
	{
		str05,
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
		str06,
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

	// X8 +
	{
		str07,
		{
			
		},		
	},
	
	// X8 x
	{
		str08,
		{
			
		},		
	},
	
	// HexaCopter +
	{
		str09,
		{
			
		},		
	},
	
	// HexaCopter x
	{
		str10,
		{
			
		},		
	},
	
	// OctoCopter +
	{
		str11,
		{
			
		},		
	},
	
	// OctoCopter x
	{
		str12,
		{
			
		},		
	},
	
	// H6
	{
		str13,
		{
			
		},		
	},
	
	// H8
	{
		str14,
		{
			
		},		
	},
	
	// V6
	{
		str15,
		{
			
		},		
	},
	
	// V8
	{
		str16,
		{
			
		},		
	},
	
	// Airplane 1S Ail
	{
		str17,
		{
			
		},		
	},
	
	// Airplane 2S Ail
	{
		str18,
		{
			
		},		
	},
	
	// Flying Wing
	{
		str19,
		{
			
		},		
	},
	
};
