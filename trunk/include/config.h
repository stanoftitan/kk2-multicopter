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
#include "pid.h"

typedef struct  
{
	uint8_t signature;
	uint8_t RX_mode;
	uint16_t RX_zero[RX_CHANNELS];
	uint16_t GYRO_zero[3];
	uint16_t ACC_zero[3];
	pid_param_t PID[3];
	uint8_t SelfLevelMode;
	uint8_t IPartMode;
	uint8_t ArmingMode;
	uint8_t LinkRollPitch;
	uint8_t MixerIndex;
	uint8_t StickScaling[4];
	uint8_t MinThrottle;
	uint8_t LCDContrast;
	uint8_t HeightDampening;
	uint8_t HeightDampeningLimit;
	uint8_t LVA;
	pid_param_t PID_SelfLevel;
	model_t Mixer;
} config_t;

extern config_t Config;

void configSave();
void configInit();


#endif /* CONFIG_H_ */