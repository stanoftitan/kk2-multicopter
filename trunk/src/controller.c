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
#include "imu.h"
#include <string.h>

int16_t CONTROL[4];

static pid_state_t PID[3];

int16_t limit(int16_t value, int16_t low, int16_t high)
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
	
	if (index == YAW)
		err += GYRO_RATE[YAW];
	else
		err += ANGLE[index];
		
	r = (int32_t)err * Config.PID[index].PGain;
	
	PID[index].Integral += err >> 8;
	//r += PID[index].Integral * Config.PID[index].IGain;
	
	//r += (PID[index].Error - err) * Config.PID[index].DGain;
	//PID[index].Error = err;
	
	r >>= 12;
	if (r < 0) r++;
	return r;
}

uint16_t calcThrottle()
{
	int32_t r;
	r = RX[THR] * Config.StickScaling[THR];
	return r >> 5;
}

void controllerCalculate()
{
	CONTROL[ROL] = -calcChannel(ROL);
	CONTROL[PIT] = calcChannel(PIT);
	CONTROL[YAW] = -calcChannel(YAW);
	CONTROL[THR] = calcThrottle();
}

void controllerReset()
{
	// reset PIDs
	memset (&PID, 0, sizeof(PID));
}