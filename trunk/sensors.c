/*
 * sensors.c
 *
 * Created: 27.07.2012 20:11:35
 *  Author: OliverS
 */ 

#include "global.h"
#include "adc.h"

uint16_t GYRO[3];
uint16_t ACC[3];

void sensorsReadGyro()
{
	GYRO[0] = adcGet(ADC_GYR_X);
	GYRO[1] = adcGet(ADC_GYR_Y);
	GYRO[2] = adcGet(ADC_GYR_Z);
}

void sensorsReadAcc()
{
	ACC[0] = adcGet(ADC_ACC_X);
	ACC[1] = adcGet(ADC_ACC_Y);
	ACC[2] = adcGet(ADC_ACC_Z);
}

