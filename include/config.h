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
	int8_t RollGain;
	int8_t RollOffset;
	int8_t PitchGain;
	int8_t PitchOffset;
} camera_t;

typedef struct  
{
	uint8_t signature;
	uint16_t RX_zero[RX_CHANNELS];
	uint8_t RX_chmap[RX_CHANNELS];
	uint16_t GYRO_zero[3];
	uint16_t ACC_zero[3];
	uint8_t CalibrateFlags;
	pid_param_t PID[3];
	
	#define SELFLEVEL_STICK		0
	#define SELFLEVEL_AUX		1
	#define SELFLEVEL_ON		2
	uint8_t SelfLevelMode;
	
	uint8_t ArmingMode;
	uint8_t AutoDisarm;
	uint8_t LinkRollPitch;
	uint8_t ReceiverMode;
	uint8_t MixerIndex;
	uint8_t StickScaling[4];
	uint8_t MinThrottle;
	uint8_t LCDContrast;
	uint8_t HeightDampening;
	uint8_t HeightDampeningLimit;
	uint8_t LVA;
	uint8_t ServoFilter;
	pid_param_t PID_SelfLevel;
	int8_t AccTrim[2];
	mixer_t Mixer;
	camera_t Camera;
} config_t;

extern config_t Config;

void configInit();
void configLoad();
void configSave();
void configReset();


#endif /* CONFIG_H_ */