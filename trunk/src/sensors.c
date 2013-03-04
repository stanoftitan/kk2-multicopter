/*
 * sensors.c
 *
 * Created: 27.07.2012 20:11:35
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "adc.h"
#include <string.h>
#include <stdlib.h>

uint16_t GYRO_raw[3], ACC_raw[3];
int16_t GYRO[3], ACC[3];
uint8_t BATT;

#define GYRO_DEADBAND	2

void sensorsRead()
{
	GYRO_raw[PIT] = adcGet(ADC_GYR_X);
	GYRO[PIT] = -(int16_t)(GYRO_raw[PIT] - Config.GYRO_zero[PIT]);
#if GYRO_DEADBAND > 0
	if (abs(GYRO[PIT]) <= GYRO_DEADBAND) GYRO[PIT] = 0;
#endif

	GYRO_raw[ROL] = adcGet(ADC_GYR_Y);
	GYRO[ROL] = -(int16_t)(GYRO_raw[ROL] - Config.GYRO_zero[ROL]);
#if GYRO_DEADBAND > 0
	if (abs(GYRO[ROL]) <= GYRO_DEADBAND) GYRO[ROL] = 0;
#endif

	GYRO_raw[YAW] = adcGet(ADC_GYR_Z);
	GYRO[YAW] = (int16_t)(GYRO_raw[YAW] - Config.GYRO_zero[YAW]);
#if GYRO_DEADBAND > 0
	if (abs(GYRO[YAW]) <= GYRO_DEADBAND) GYRO[YAW] = 0;
#endif
	
	ACC_raw[PIT] = adcGet(ADC_ACC_X);
	ACC[PIT] = (int16_t)(ACC_raw[PIT] - Config.ACC_zero[PIT]);
	ACC_raw[ROL] = adcGet(ADC_ACC_Y);
	ACC[ROL] = (int16_t)(ACC_raw[ROL] - Config.ACC_zero[ROL]);
	ACC_raw[YAW] = adcGet(ADC_ACC_Z);
	ACC[YAW] = (int16_t)(ACC_raw[YAW] - Config.ACC_zero[YAW]);

	BATT = adcGet(ADC_VBAT) * 100 / 376;
	
#ifdef SIMULATOR
	GYRO[0] = 0;
	GYRO[1] = 0;
	GYRO[2] = 0;
	ACC[0] = 0;
	ACC[1] = 0;
	ACC[2] = 0;
#endif
}

void sensorsCalibrate()
{
	sensorsRead();
	memcpy(&Config.GYRO_zero, &GYRO_raw, sizeof(Config.GYRO_zero));
	memcpy(&Config.ACC_zero, &ACC_raw, sizeof(Config.ACC_zero));
	//Config.ACC_zero[ZAXIS] -= 124;
	Config.CalibrateFlags = 1;
}
