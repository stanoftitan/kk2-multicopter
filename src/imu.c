/*
 * imu.c
 *
 * Created: 23.02.2013 15:07:21
 *  Author: OliverS
 *
 * $Id$
*/ 

#include "global.h"
#include "sensors.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

int16_t ANGLE[3];
int16_t ACC_ANGLE[3];
int16_t GYRO_RATE[3];

static const uint8_t convtab[] PROGMEM =
{
	0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
	16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 
	29, 31, 32, 33, 34, 35, 36, 37, 38, 39, 41, 42, 43, 44, 
	46, 47, 48, 50, 51, 53, 54, 56, 57, 59, 61, 63, 65, 67, 
	69, 72, 76, 80, 90, 
};

static int16_t getAccAngle(int16_t input)
{
	uint8_t index = abs(input) >> 1;
	if (index >= length(convtab))
		index = length(convtab) - 1;
	int16_t conv = pgm_read_byte(&convtab[index]) * 2;
	if (input & 1) conv++;
	if (input < 0) conv = -conv;
	return conv << 7;
}

static int16_t calcGyroRate(uint8_t axis, uint16_t dt)
{
	int32_t r = (int32_t)GYRO[axis] * dt >> 16;
	if (r < 0) r++;	
	return (int16_t)r;
}

#define ALPHA				1
#define MAXALPHA			256
static int16_t calcComplementaryAngle(uint8_t axis)
{
	int32_t r;
	r = (int32_t)(ANGLE[axis] + GYRO_RATE[axis]) * (MAXALPHA - ALPHA);
	r += (int32_t)ACC_ANGLE[axis] * ALPHA;
	r >>= 8;
	if (r < 0) r++;
	return r;
}

static int16_t calcYawAngle()
{
	//int32_t r;
	return ANGLE[YAW] + GYRO_RATE[YAW];
}

static void imuComplementary()
{
	static uint32_t lastCall;
	uint16_t dt = ticks() - lastCall;
		
	ACC_ANGLE[ROL] = getAccAngle(ACC[ROL]) + (Config.AccTrim[ROL] << 8);
	ACC_ANGLE[PIT] = getAccAngle(ACC[PIT]) + (Config.AccTrim[PIT] << 8);
	ACC_ANGLE[YAW] = getAccAngle(ACC[YAW]) + (90 << 8);

	GYRO_RATE[ROL] = calcGyroRate(ROL, dt);
	GYRO_RATE[PIT] = calcGyroRate(PIT, dt);
	GYRO_RATE[YAW] = calcGyroRate(YAW, dt);
	
	ANGLE[ROL] = calcComplementaryAngle(ROL);
	ANGLE[PIT] = calcComplementaryAngle(PIT);
	ANGLE[YAW] = calcYawAngle();
	
	lastCall += dt;
}

void imuCalculate()
{
	imuComplementary();
}