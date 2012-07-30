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

uint16_t GYRO[3], GYRO_raw[3];
uint16_t ACC[3], ACC_raw[3];

void sensorsReadGyro()
{
	GYRO_raw[0] = adcGet(ADC_GYR_X);
	GYRO[0] = GYRO_raw[0] - Config.GYRO_zero[0];
	GYRO_raw[1] = adcGet(ADC_GYR_Y);
	GYRO[1] = GYRO_raw[1] - Config.GYRO_zero[1];
	GYRO_raw[2] = adcGet(ADC_GYR_Z);
	GYRO[2] = GYRO_raw[2] - Config.GYRO_zero[2];
}

void sensorsReadAcc()
{
	ACC_raw[0] = adcGet(ADC_ACC_X);
	ACC[0] = ACC_raw[0] - Config.ACC_zero[0];
	ACC_raw[1] = adcGet(ADC_ACC_Y);
	ACC[1] = ACC_raw[1] - Config.ACC_zero[1];
	ACC_raw[2] = adcGet(ADC_ACC_Z);
	ACC[2] = ACC_raw[2] - Config.ACC_zero[2];
}

void sensorsCalibateGyro()
{
	sensorsReadGyro();
	memcpy(&Config.GYRO_zero, &GYRO_raw, sizeof(Config.GYRO_zero));
	configSave();
}

void sensorsCalibateAcc()
{
	sensorsReadAcc();
	memcpy(&Config.ACC_zero, &ACC_raw, sizeof(Config.ACC_zero));
	configSave();
}
