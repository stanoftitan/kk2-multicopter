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

#define P_STR static const char PROGMEM

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
P_STR strModel20[] = "Y4";
P_STR strModel21[] = "V-Tail";

model_t const mixerTable[MIXER_TABLE_LEN] PROGMEM = {
	// SingleCopter 2M 2S
	{
		strModel00,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 0  , 100, 0  , FLAG_ESC},
			{100, 0  , 0  ,-100, 0  , FLAG_ESC},
			{0  , 100, 0  , 0  , 50 , FLAG_SERVO},
			{0  , 0  , 100, 0  , 50 , FLAG_SERVO},
		},
	},
	
	// SingleCopter 1M 4S
	{
		strModel01,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 0  , 0  , 0  , FLAG_ESC},
			{0  , 100, 0  , 100, 50 , FLAG_SERVO},
			{0  , 0  , 100, 100, 50 , FLAG_SERVO},
			{0  ,-100, 0  , 100, 50 , FLAG_SERVO},
			{0  , 0  ,-100, 100, 50 , FLAG_SERVO},
		},
	},
	
	// DualCopter
	{
		strModel02,
		{
		//   thr roll pitch yaw offs  flags
			{100, 100, 0  , 0  , 0  , FLAG_ESC},
			{100,-100, 0  , 0  , 0  , FLAG_ESC},
			{0  , 0  , 100, 100, 50 , FLAG_SERVO},
			{0  , 0  ,-100, 100, 50 , FLAG_SERVO},
		},
	},
	
	// TriCopter
	{
		strModel03,
		{
		//   thr roll pitch yaw offs  flags
			{100,-87 , 50 , 0  , 0  , FLAG_ESC},
			{100, 87 , 50 ,-1  , 0  , FLAG_ESC},
			{100, 0  ,-100, 0  , 0  , FLAG_ESC},
			{0  , 0  , 0  , 100, 50 , FLAG_SERVO},
		},
	},
	
	// Y6
	{
		strModel04,
		{
		//   thr roll pitch yaw offs  flags
			{100,-87 , 50 , 100, 0  , FLAG_ESC},
			{100,-87 , 50 ,-100, 0  , FLAG_ESC},
			{100, 87 , 50 , 100, 0  , FLAG_ESC},
			{100, 87 , 50 ,-100, 0  , FLAG_ESC},
			{100, 0  ,-100, 100, 0  , FLAG_ESC},
			{100, 0  ,-100,-100, 0  , FLAG_ESC},
		},
	},
	

	// QuadroCopter +
	{
		strModel05,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 100, 100, 0  , FLAG_ESC},
			{100, 100, 0  ,-100, 0  , FLAG_ESC},
			{100, 0  ,-100, 100, 0  , FLAG_ESC},
			{100,-100, 0  ,-100, 0  , FLAG_ESC},
		},
	},	
	
	// QuadroCopter x
	{
		strModel06,
		{
		//   thr roll pitch yaw offs  flags
			{100,-71 , 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 71 ,-100, 0  , FLAG_ESC},
			{100, 71 ,-71 , 100, 0  , FLAG_ESC},
			{100,-71 ,-71 ,-100, 0  , FLAG_ESC},
		},
	},	

	// X8 +
	{
		strModel07,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 100, 100, 0  , FLAG_ESC},
			{100, 0  , 100,-100, 0  , FLAG_ESC},
			{100, 100, 0  , 100, 0  , FLAG_ESC},
			{100, 100, 0  ,-100, 0  , FLAG_ESC},
			{100, 0  ,-100, 100, 0  , FLAG_ESC},
			{100, 0  ,-100,-100, 0  , FLAG_ESC},
			{100,-100, 0  , 100, 0  , FLAG_ESC},
			{100,-100, 0  ,-100, 0  , FLAG_ESC},
		},
	},
	
	// X8 x
	{
		strModel08,
		{
		//   thr roll pitch yaw offs  flags
			{100,-71 , 71 , 100, 0  , FLAG_ESC},
			{100,-71 , 71 ,-100, 0  , FLAG_ESC},
			{100, 71 , 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 71 ,-100, 0  , FLAG_ESC},
			{100, 71 ,-71 , 100, 0  , FLAG_ESC},
			{100, 71 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-71 ,-71 , 100, 0  , FLAG_ESC},
			{100,-71 ,-71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// HexaCopter +
	{
		strModel09,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 100, 100, 0  , FLAG_ESC},
			{100, 87 , 50 ,-100, 0  , FLAG_ESC},
			{100, 87 ,-50 , 100, 0  , FLAG_ESC},
			{100, 0  ,-100,-100, 0  , FLAG_ESC},
			{100,-87 ,-50 , 100, 0  , FLAG_ESC},
			{100,-87 , 50 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// HexaCopter x
	{
		strModel10,
		{
		//   thr roll pitch yaw offs  flags
			{100, 50 , 87 , 100, 0  , FLAG_ESC},
			{100, 100, 0  ,-100, 0  , FLAG_ESC},
			{100, 50 ,-87 , 100, 0  , FLAG_ESC},
			{100,-50 ,-87 ,-100, 0  , FLAG_ESC},
			{100,-100, 0  , 100, 0  , FLAG_ESC},
			{100,-50 , 87 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// OctoCopter +
	{
		strModel11,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 100, 100, 0  , FLAG_ESC},
			{100, 71 , 71 ,-100, 0  , FLAG_ESC},
			{100, 100, 0  , 100, 0  , FLAG_ESC},
			{100, 71 ,-71 ,-100, 0  , FLAG_ESC},
			{100, 0  ,-100, 100, 0  , FLAG_ESC},
			{100,-71 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-100, 0  , 100, 0  , FLAG_ESC},
			{100,-71 , 71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// OctoCopter x
	{
		strModel12,
		{
		//   thr roll pitch yaw offs  flags
			{100, 38 , 92 , 100, 0  , FLAG_ESC},
			{100, 92 , 38 ,-100, 0  , FLAG_ESC},
			{100, 92 ,-38 , 100, 0  , FLAG_ESC},
			{100, 38 ,-92 ,-100, 0  , FLAG_ESC},
			{100,-38 ,-92 , 100, 0  , FLAG_ESC},
			{100,-92 ,-38 ,-100, 0  , FLAG_ESC},
			{100,-92 , 38 , 100, 0  , FLAG_ESC},
			{100,-38 , 92 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// H6
	{
		strModel13,
		{
		//   thr roll pitch yaw offs  flags
			{100, 71 , 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 0  ,-100, 0  , FLAG_ESC},
			{100, 71 ,-71 , 100, 0  , FLAG_ESC},
			{100,-71 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-71 , 0  , 100, 0  , FLAG_ESC},
			{100,-71 , 71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// H8
	{
		strModel14,
		{
		//   thr roll pitch yaw offs  flags
			{100, 71 , 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 24 ,-100, 0  , FLAG_ESC},
			{100, 71 ,-24 , 100, 0  , FLAG_ESC},
			{100, 71 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-71 ,-71 , 100, 0  , FLAG_ESC},
			{100,-71 ,-24 ,-100, 0  , FLAG_ESC},
			{100,-71 , 24 , 100, 0  , FLAG_ESC},
			{100,-71 , 71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// V6
	{
		strModel15,
		{
		//   thr roll pitch yaw offs  flags
			{100, 100, 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 0  ,-100, 0  , FLAG_ESC},
			{100, 42 ,-71 , 100, 0  , FLAG_ESC},
			{100,-42 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-71 , 0  , 100, 0  , FLAG_ESC},
			{100,-100, 71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// V8
	{
		strModel16,
		{
		//   thr roll pitch yaw offs  flags
			{100, 100, 71 , 100, 0  , FLAG_ESC},
			{100, 81 , 24 ,-100, 0  , FLAG_ESC},
			{100, 61 ,-24 , 100, 0  , FLAG_ESC},
			{100, 42 ,-71 ,-100, 0  , FLAG_ESC},
			{100,-42 ,-71 , 100, 0  , FLAG_ESC},
			{100,-61 ,-24 ,-100, 0  , FLAG_ESC},
			{100,-81 , 24 , 100, 0  , FLAG_ESC},
			{100,-100, 71 ,-100, 0  , FLAG_ESC},
		},
	},
	
	// Airplane 1S Ail
	{
		strModel17,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 0  , 0  , 0  , FLAG_ESC},
			{0  , 100, 0  , 0  , 50 , FLAG_SERVO},
			{0  , 0  , 100, 0  , 50 , FLAG_SERVO},
			{0  , 0  , 0  , 100, 50 , FLAG_SERVO},
		},
	},
	
	// Airplane 2S Ail
	{
		strModel18,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 0  , 0  , 0  , FLAG_ESC},
			{0  , 100, 0  , 0  , 50 , FLAG_SERVO},
			{0  , 100, 0  , 0  , 50 , FLAG_SERVO},
			{0  , 0  , 100, 0  , 50 , FLAG_SERVO},
			{0  , 0  , 0  , 100, 50 , FLAG_SERVO},
		},
	},
	
	// Flying Wing
	{
		strModel19,
		{
		//   thr roll pitch yaw offs  flags
			{100, 0  , 0  , 0  , 0  , FLAG_ESC},
			{0  , 50 , 50 , 0  , 50 , FLAG_SERVO},
			{0  , 50 ,-50 , 0  , 50 , FLAG_SERVO},
		},
	},
	
	// Y4
	{
		strModel20,
		{
		//   thr roll pitch yaw offs  flags
			{100,-71 , 71 , 100, 0  , FLAG_ESC},
			{100, 71 , 71 ,-100, 0  , FLAG_ESC},
			{100, 0  ,-100, 100, 0  , FLAG_ESC},
			{100, 0  ,-100,-100, 0  , FLAG_ESC},
		},
	},

	// V-Tail
	{
		strModel21,
		{
		//   thr roll pitch yaw offs  flags
			{100,-71 , 71 , 0  , 0  , FLAG_ESC},
			{100, 71 , 71 ,-1  , 0  , FLAG_ESC},
			{120, 0  ,-90 , 100, 0  , FLAG_ESC},
			{120, 0  ,-90 ,-100, 0  , FLAG_ESC},
		},
	},
};
