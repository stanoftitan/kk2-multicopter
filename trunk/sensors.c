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
	GYRO_raw[0] = adcGet(ADC_GYR_X);
	GYRO[0] = (int16_t)(GYRO_raw[0] - Config.GYRO_zero[0]) >> 1;
	GYRO_raw[1] = adcGet(ADC_GYR_Y);
	GYRO[1] = (int16_t)(GYRO_raw[1] - Config.GYRO_zero[1]) >> 1;
	GYRO_raw[2] = adcGet(ADC_GYR_Z);
	GYRO[2] = (int16_t)(GYRO_raw[2] - Config.GYRO_zero[2]) >> 1;
	
	ACC_raw[0] = adcGet(ADC_ACC_X);
	ACC[0] = (int16_t)(ACC_raw[0] - Config.ACC_zero[0]) >> 1;
	ACC_raw[1] = adcGet(ADC_ACC_Y);
	ACC[1] = (int16_t)(ACC_raw[1] - Config.ACC_zero[1]) >> 1;
	ACC_raw[2] = adcGet(ADC_ACC_Z);
	ACC[2] = (int16_t)(ACC_raw[2] - Config.ACC_zero[2]) >> 1;

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

void sensorsCalibate()
{
	sensorsRead();
	memcpy(&Config.GYRO_zero, &GYRO_raw, sizeof(Config.GYRO_zero));
	memcpy(&Config.ACC_zero, &ACC_raw, sizeof(Config.ACC_zero));
	Config.CalibrateFlags = 1;
}
