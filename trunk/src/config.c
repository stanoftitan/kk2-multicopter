/*
 * config.c
 *
 * Created: 30.07.2012 11:44:47
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "rx.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

noinit config_t Config;
EEMEM config_t EEConfig;
#define SIGNATURE	0x60

static const config_t defaultConfig PROGMEM = 
{
	.signature = SIGNATURE,
	.MixerIndex = 0,
	.RX_zero[AIL] = PWM_MID,
	.RX_zero[ELE] = PWM_MID,
	.RX_zero[RUD] = PWM_MID,
	.RX_zero[THR] = PWM_LOW,
	.RX_zero[AX1] = PWM_MID,
	.RX_zero[AX2] = PWM_MID,
	.RX_zero[AX3] = PWM_MID,
	.RX_zero[AX4] = PWM_MID,
	.RX_chmap[AIL] = 1,
	.RX_chmap[ELE] = 2,
	.RX_chmap[RUD] = 4,
	.RX_chmap[THR] = 3,
	.RX_chmap[AX1] = 5,
	.RX_chmap[AX2] = 6,
	.RX_chmap[AX3] = 7,
	.RX_chmap[AX4] = 8,
	.GYRO_zero[ROL] = 536,
	.GYRO_zero[PIT] = 548,
	.GYRO_zero[YAW] = 565,
	.ACC_zero[ROL]  = 611,
	.ACC_zero[PIT]  = 620,
	.ACC_zero[YAW]  = 766,
	.PID[ROL] = {{ 50, 100, 25, 20}},
	.PID[PIT] = {{ 50, 100, 25, 20}},
	.PID[YAW] = {{ 50,  20, 50, 10}},
	.SelfLevelMode = SELFLEVEL_ON,
	.LinkRollPitch = ON,
	.AutoDisarm = ON,
	.ReceiverMode = RX_MODE_CPPM,
	.StickScaling = { 30, 30, 50, 90},
	.MinThrottle = 10,
	.LCDContrast = 32,
	.HeightDampening = 0,
	.HeightDampeningLimit = 30,
	.LVA = 0,
	.ServoFilter = 50,
	.PID_SelfLevel = {{ 100, 20, 0, 0}},
	.AccTrim = { 0, 0},
	.Camera = {0, 50, 0, 50},
	
};

static void _initConfig()
{
	memcpy_P(&Config, &defaultConfig, sizeof(Config));
	mixerLoadModel(Config.MixerIndex);
}

void configLoad()
{
	eeprom_read_block(&Config, &EEConfig, sizeof(Config));
	if (Config.signature != SIGNATURE)
		_initConfig();
}

void configInit()
{
	configLoad();
}

void configSave()
{
	eeprom_update_block(&Config, &EEConfig, sizeof(Config));
}

void configReset()
{
	_initConfig();
}

