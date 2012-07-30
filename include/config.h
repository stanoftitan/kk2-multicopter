/*
 * config.h
 *
 * Created: 30.07.2012 11:42:33
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "global.h"
#include "mixer.h"

typedef struct  
{
	uint8_t signature;
	uint8_t RX_mode;
	uint16_t RX_zero[RX_CHANNELS];
	uint16_t GYRO_zero[3];
	uint16_t ACC_zero[3];
	MixerModel Mixer;
} Config_t;

extern Config_t Config;

void configSave();
void configInit();


#endif /* CONFIG_H_ */