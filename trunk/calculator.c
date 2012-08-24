/*
 * calculator.c
 *
 * Created: 24.08.2012 17:52:49
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "rx.h"
#include "sensors.h"

int16_t CHANNELS[4];


int16_t calcChannel(uint8_t index)
{
	int32_t r;
	
	r = RX[index] * Config.StickScaling[index];
	r += GYRO[index] * 50;
	
	r *= Config.PID[index].PGain;
	
	return r >> 6;
}

uint16_t calcThrottle()
{
	int32_t r;
	r = RX[THR] * Config.StickScaling[THR];
	
	return r;
}

void calc()
{
	CHANNELS[AIL] = calcChannel(AIL);
	CHANNELS[ELE] = calcChannel(ELE);
	CHANNELS[RUD] = calcChannel(RUD);
	CHANNELS[THR] = calcThrottle();
}
