/*
 * model_table.c
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

static const model_t model00 PROGMEM =
	// SingleCopter 2M 2S
	{
		strModel00, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 0  , 100, 0  , FLAG_ESC}},
			{{100, 0  , 0  ,-100, 0  , FLAG_ESC}},
			{{0  , 100, 0  , 0  , 50 , FLAG_SERVO}},
			{{0  , 0  , 100, 0  , 50 , FLAG_SERVO}},
		},
	};

static const model_t model01 PROGMEM =
	// SingleCopter 1M 4S
	{
		strModel01, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 0  , 0  , 0  , FLAG_ESC}},
			{{0  , 100, 0  , 100, 50 , FLAG_SERVO}},
			{{0  , 0  , 100, 100, 50 , FLAG_SERVO}},
			{{0  ,-100, 0  , 100, 50 , FLAG_SERVO}},
			{{0  , 0  ,-100, 100, 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model02 PROGMEM =
	// DualCopter
	{
		strModel02, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 100, 0  , 0  , 0  , FLAG_ESC}},
			{{100,-100, 0  , 0  , 0  , FLAG_ESC}},
			{{0  , 0  , 100, 100, 50 , FLAG_SERVO}},
			{{0  , 0  ,-100, 100, 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model03 PROGMEM =
	// TriCopter
	{
		strModel03, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-87 , 50 , 0  , 0  , FLAG_ESC}},
			{{100, 87 , 50 ,-1  , 0  , FLAG_ESC}},
			{{100, 0  ,-100, 0  , 0  , FLAG_ESC}},
			{{0  , 0  , 0  , 100, 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model04 PROGMEM =
	// Y6
	{
		strModel04, 6,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-87 , 50 , 100, 0  , FLAG_ESC}},
			{{100,-87 , 50 ,-100, 0  , FLAG_ESC}},
			{{100, 87 , 50 , 100, 0  , FLAG_ESC}},
			{{100, 87 , 50 ,-100, 0  , FLAG_ESC}},
			{{100, 0  ,-100, 100, 0  , FLAG_ESC}},
			{{100, 0  ,-100,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model05 PROGMEM =
	// QuadroCopter +
	{
		strModel05, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 100, 100, 0  , FLAG_ESC}},
			{{100, 100, 0  ,-100, 0  , FLAG_ESC}},
			{{100, 0  ,-100, 100, 0  , FLAG_ESC}},
			{{100,-100, 0  ,-100, 0  , FLAG_ESC}},
		},
	};	
	
static const model_t model06 PROGMEM =
	// QuadroCopter x
	{
		strModel06, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-71 , 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 71 ,-100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 ,-100, 0  , FLAG_ESC}},
		},
	};	

static const model_t model07 PROGMEM =
	// X8 +
	{
		strModel07, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 100, 100, 0  , FLAG_ESC}},
			{{100, 0  , 100,-100, 0  , FLAG_ESC}},
			{{100, 100, 0  , 100, 0  , FLAG_ESC}},
			{{100, 100, 0  ,-100, 0  , FLAG_ESC}},
			{{100, 0  ,-100, 100, 0  , FLAG_ESC}},
			{{100, 0  ,-100,-100, 0  , FLAG_ESC}},
			{{100,-100, 0  , 100, 0  , FLAG_ESC}},
			{{100,-100, 0  ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model08 PROGMEM =
	// X8 x
	{
		strModel08, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-71 , 71 , 100, 0  , FLAG_ESC}},
			{{100,-71 , 71 ,-100, 0  , FLAG_ESC}},
			{{100, 71 , 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 71 ,-100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 , 100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model09 PROGMEM =
	// HexaCopter +
	{
		strModel09, 6,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 100, 100, 0  , FLAG_ESC}},
			{{100, 87 , 50 ,-100, 0  , FLAG_ESC}},
			{{100, 87 ,-50 , 100, 0  , FLAG_ESC}},
			{{100, 0  ,-100,-100, 0  , FLAG_ESC}},
			{{100,-87 ,-50 , 100, 0  , FLAG_ESC}},
			{{100,-87 , 50 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model10 PROGMEM =
	// HexaCopter x
	{
		strModel10, 6,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 50 , 87 , 100, 0  , FLAG_ESC}},
			{{100, 100, 0  ,-100, 0  , FLAG_ESC}},
			{{100, 50 ,-87 , 100, 0  , FLAG_ESC}},
			{{100,-50 ,-87 ,-100, 0  , FLAG_ESC}},
			{{100,-100, 0  , 100, 0  , FLAG_ESC}},
			{{100,-50 , 87 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model11 PROGMEM =
	// OctoCopter +
	{
		strModel11, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 100, 100, 0  , FLAG_ESC}},
			{{100, 71 , 71 ,-100, 0  , FLAG_ESC}},
			{{100, 100, 0  , 100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100, 0  ,-100, 100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-100, 0  , 100, 0  , FLAG_ESC}},
			{{100,-71 , 71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model12 PROGMEM =
	// OctoCopter x
	{
		strModel12, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 38 , 92 , 100, 0  , FLAG_ESC}},
			{{100, 92 , 38 ,-100, 0  , FLAG_ESC}},
			{{100, 92 ,-38 , 100, 0  , FLAG_ESC}},
			{{100, 38 ,-92 ,-100, 0  , FLAG_ESC}},
			{{100,-38 ,-92 , 100, 0  , FLAG_ESC}},
			{{100,-92 ,-38 ,-100, 0  , FLAG_ESC}},
			{{100,-92 , 38 , 100, 0  , FLAG_ESC}},
			{{100,-38 , 92 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model13 PROGMEM =
	// H6
	{
		strModel13, 6,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 71 , 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 0  ,-100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-71 , 0  , 100, 0  , FLAG_ESC}},
			{{100,-71 , 71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model14 PROGMEM =
	// H8
	{
		strModel14, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 71 , 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 24 ,-100, 0  , FLAG_ESC}},
			{{100, 71 ,-24 , 100, 0  , FLAG_ESC}},
			{{100, 71 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-71 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-71 ,-24 ,-100, 0  , FLAG_ESC}},
			{{100,-71 , 24 , 100, 0  , FLAG_ESC}},
			{{100,-71 , 71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model15 PROGMEM =
	// V6
	{
		strModel15, 6,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 100, 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 0  ,-100, 0  , FLAG_ESC}},
			{{100, 42 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-42 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-71 , 0  , 100, 0  , FLAG_ESC}},
			{{100,-100, 71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model16 PROGMEM =
	// V8
	{
		strModel16, 8,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 100, 71 , 100, 0  , FLAG_ESC}},
			{{100, 81 , 24 ,-100, 0  , FLAG_ESC}},
			{{100, 61 ,-24 , 100, 0  , FLAG_ESC}},
			{{100, 42 ,-71 ,-100, 0  , FLAG_ESC}},
			{{100,-42 ,-71 , 100, 0  , FLAG_ESC}},
			{{100,-61 ,-24 ,-100, 0  , FLAG_ESC}},
			{{100,-81 , 24 , 100, 0  , FLAG_ESC}},
			{{100,-100, 71 ,-100, 0  , FLAG_ESC}},
		},
	};
	
static const model_t model17 PROGMEM =
	// Airplane 1S Ail
	{
		strModel17, 4, 
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 0  , 0  , 0  , FLAG_ESC}},
			{{0  , 100, 0  , 0  , 50 , FLAG_SERVO}},
			{{0  , 0  , 100, 0  , 50 , FLAG_SERVO}},
			{{0  , 0  , 0  , 100, 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model18 PROGMEM =
	// Airplane 2S Ail
	{
		strModel18, 5,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 0  , 0  , 0  , FLAG_ESC}},
			{{0  , 100, 0  , 0  , 50 , FLAG_SERVO}},
			{{0  , 100, 0  , 0  , 50 , FLAG_SERVO}},
			{{0  , 0  , 100, 0  , 50 , FLAG_SERVO}},
			{{0  , 0  , 0  , 100, 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model19 PROGMEM =
	// Flying Wing
	{
		strModel19, 3,
		{
		//    thr roll pitch yaw offs  flags
			{{100, 0  , 0  , 0  , 0  , FLAG_ESC}},
			{{0  , 50 , 50 , 0  , 50 , FLAG_SERVO}},
			{{0  , 50 ,-50 , 0  , 50 , FLAG_SERVO}},
		},
	};
	
static const model_t model20 PROGMEM =
	// Y4
	{
		strModel20, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-71 , 71 , 100, 0  , FLAG_ESC}},
			{{100, 71 , 71 ,-100, 0  , FLAG_ESC}},
			{{100, 0  ,-100, 100, 0  , FLAG_ESC}},
			{{100, 0  ,-100,-100, 0  , FLAG_ESC}},
		},
	};

static const model_t model21 PROGMEM =
	// V-Tail
	{
		strModel21, 4,
		{
		//    thr roll pitch yaw offs  flags
			{{100,-71 , 71 , 0  , 0  , FLAG_ESC}},
			{{100, 71 , 71 ,-1  , 0  , FLAG_ESC}},
			{{120, 0  ,-90 , 100, 0  , FLAG_ESC}},
			{{120, 0  ,-90 ,-100, 0  , FLAG_ESC}},
		},
	};



const model_t* const modelTable[] PROGMEM = {
	&model00,
	&model01,
	&model02,
	&model03,
	&model04,
	&model05,
	&model06,
	&model07,
	&model08,
	&model09,
	&model10,
	&model11,
	&model12,
	&model13,
	&model14,
	&model15,
	&model16,
	&model17,
	&model18,
	&model19,
	&model20,
	&model21,
};
