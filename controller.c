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
	
	err = RX[index] * Config.StickScaling[index];
	err -= GYRO[index] * 100;
	err >>= 7;
	
	r = err * Config.PID[index].PGain;
	
	IntegralSum[index] += err;
	r += IntegralSum[index] * Config.PID[index].IGain;
	
	return r >> 6;
}

uint16_t calcThrottle()
{
	int32_t r;
	r = RX[THR] * Config.StickScaling[THR];
	return r;
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