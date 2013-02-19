/*
 * controller.c
 *
 * Created: 24.08.2012 17:52:49
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "rx.h"
#include "sensors.h"
#include <string.h>

int16_t CONTROL[4];

static int16_t IntegralSum[4];

static int16_t limit(int16_t value, int16_t low, int16_t high)
{
	if (value < low) return low;
	else if (value > high) return high;
	else return value;
}

int16_t calcChannel(uint8_t index)
{
	int16_t err;
	int32_t r;
	
	// RX = -140..+140
	// StickScaling = 0..+200
	// ==> -28000..+28000 -> 16bit
	err = RX[index] * Config.StickScaling[index];
	err >>= 6;
	err -= GYRO[index] * 2;
	
	r = err * Config.PID[index].PGain;
	
	//IntegralSum[index] += err;
	//r += IntegralSum[index] * Config.PID[index].IGain;
	
	return r >> 4;
}

uint16_t calcThrottle()
{
	int32_t r;
	r = RX[THR] * Config.StickScaling[THR];
	return r >> 4;
}

void controller()
{
	CONTROL[AIL] = calcChannel(AIL);
	CONTROL[ELE] = calcChannel(ELE);
	CONTROL[RUD] = calcChannel(RUD);
	CONTROL[THR] = calcThrottle();
}

void controllerReset()
{
	// reset integral sums
	memset (&IntegralSum, 0, sizeof(IntegralSum));
}