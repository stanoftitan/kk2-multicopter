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

uint16_t GYRO_raw[3], ACC_raw[3];
int16_t GYRO[3], ACC[3];
uint8_t BATT;

void sensorsRead()
{
	GYRO_raw[XAXIS] = adcGet(ADC_GYR_X);
	GYRO[XAXIS] = -(int16_t)(GYRO_raw[XAXIS] - Config.GYRO_zero[XAXIS]);
	GYRO_raw[YAXIS] = adcGet(ADC_GYR_Y);
	GYRO[YAXIS] = -(int16_t)(GYRO_raw[YAXIS] - Config.GYRO_zero[YAXIS]);
	GYRO_raw[ZAXIS] = adcGet(ADC_GYR_Z);
	GYRO[ZAXIS] = (int16_t)(GYRO_raw[ZAXIS] - Config.GYRO_zero[ZAXIS]);
	
	ACC_raw[XAXIS] = adcGet(ADC_ACC_X);
	ACC[XAXIS] = (int16_t)(ACC_raw[XAXIS] - Config.ACC_zero[XAXIS]);
	ACC_raw[YAXIS] = adcGet(ADC_ACC_Y);
	ACC[YAXIS] = (int16_t)(ACC_raw[YAXIS] - Config.ACC_zero[YAXIS]);
	ACC_raw[ZAXIS] = adcGet(ADC_ACC_Z);
	ACC[ZAXIS] = (int16_t)(ACC_raw[ZAXIS] - Config.ACC_zero[ZAXIS]);

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
	Config.ACC_zero[ZAXIS] -= 124;
	Config.CalibrateFlags = 1;
}
