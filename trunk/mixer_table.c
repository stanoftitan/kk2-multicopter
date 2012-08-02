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

P_STR strModel00[] = "SingleCopter 2M 2S";
P_STR strModel01[] = "SingleCopter 1M 4S";
P_STR strModel02[] = "DualCopter";
P_STR strModel03[] = "TriCopter";
P_STR strModel04[] = "Y6";
P_STR strModel05[] = "QuadroCopter +";
P_STR strModel06[] = "QuadroCopter x";
P_STR strModel07[] = "X8 +";
P_STR strModel08[] = "X8 x";
P_STR strModel09[] = "HexaCopter +";
P_STR strModel10[] = "HexaCopter x";
P_STR strModel11[] = "OctoCopter +";
P_STR strModel12[] = "OctoCopter x";
P_STR strModel13[] = "H6";
P_STR strModel14[] = "H8";
P_STR strModel15[] = "V6";
P_STR strModel16[] = "V8";
P_STR strModel17[] = "Airplane 1S Ail";
P_STR strModel18[] = "Airplane 2S Ail";
P_STR strModel19[] = "Flying Wing";

PROGMEM model_t mixerTable[MIXER_TABLE_LEN] = {
	// SingleCopter 2M 2S
	{
		strModel00,
		{
			
		},		
	},
	
	// SingleCopter 1M 4S
	{
		strModel01,
		{
			
		},		
	},
	
	// DualCopter
	{
		strModel02,
		{
			
		},		
	},
	
	// TriCopter
	{
		strModel03,
		{
			
		},		
	},
	
	// Y6
	{
		strModel04,
		{
			
		},		
	},
	

	// QuadroCopter +
	{
		strModel05,
		{
			{FLAG_ESC, 100, 0, 100, 100, 0},
			{FLAG_ESC, 100, 100, 0, -100, 0},			
			{FLAG_ESC, 100, 0, -100, 100, 0},			
			{FLAG_ESC, 100, -100, 0, -100, 0},			
		},
	},	
	
	// QuadroCopter x
	{
		strModel06,
		{
			{FLAG_ESC, 100, -71, 71, 100, 0},
			{FLAG_ESC, 100, 71, 71, -100, 0},			
			{FLAG_ESC, 100, 71, -71, 100, 0},			
			{FLAG_ESC, 100, -71, -71, -100, 0},			
		},
	},	

	// X8 +
	{
		strModel07,
		{
			
		},		
	},
	
	// X8 x
	{
		strModel08,
		{
			
		},		
	},
	
	// HexaCopter +
	{
		strModel09,
		{
			
		},		
	},
	
	// HexaCopter x
	{
		strModel10,
		{
			
		},		
	},
	
	// OctoCopter +
	{
		strModel11,
		{
			
		},		
	},
	
	// OctoCopter x
	{
		strModel12,
		{
			
		},		
	},
	
	// H6
	{
		strModel13,
		{
			
		},		
	},
	
	// H8
	{
		strModel14,
		{
			
		},		
	},
	
	// V6
	{
		strModel15,
		{
			
		},		
	},
	
	// V8
	{
		strModel16,
		{
			
		},		
	},
	
	// Airplane 1S Ail
	{
		strModel17,
		{
			
		},		
	},
	
	// Airplane 2S Ail
	{
		strModel18,
		{
			
		},		
	},
	
	// Flying Wing
	{
		strModel19,
		{
			
		},		
	},
	
};
