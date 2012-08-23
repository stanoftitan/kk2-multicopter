/*
 * global.h
 *
 * Created: 27.07.2012 09:05:10
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

#define F_CPU	20000000

#define ON		1
#define OFF		0

#define noinit __attribute__((section(".noinit")))
#define length(array) (sizeof(array)/sizeof(*array))

#include "system.h"
#include <util/atomic.h>
#include "hardware.h"
#include "config.h"

#define THROTTLE_OFF	5

typedef struct  
{
	uint8_t Armed;
	uint8_t ThrottleOff;
	uint8_t SelfLevel;
	uint8_t Aux;
	
	#define ERR_NOT_CALIBRATED	0x80
	#define ERR_NO_ROLL			0x01
	#define ERR_NO_PITCH		0x02
	#define ERR_NO_YAW			0x04
	#define ERR_NO_THR			0x08
	uint8_t Error;
	
	#define MODE_NORMAL			0
	#define MODE_ESC_CAL		1
	uint8_t Mode;
} state_t;

extern state_t State;

#endif /* GLOBAL_H_ */