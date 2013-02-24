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
#define SIGNATURE	0x5F

static const config_t defaultConfig PROGMEM = 
{
	.signature = SIGNATURE,
	.MixerIndex = 0,
	.RX_zero[AIL] = PWM_MID,
	.RX_zero[ELE] = PWM_MID,
	.RX_zero[RUD] = PWM_MID,
	.RX_zero[THR] = PWM_LOW,
	.RX_zero[AUX] = PWM_MID,
	.RX_chmap[AIL] = 1,
	.RX_chmap[ELE] = 2,
	.RX_chmap[RUD] = 4,
	.RX_chmap[THR] = 3,
	.RX_chmap[AUX] = 5,
	.GYRO_zero[XAXIS] = 536,
	.GYRO_zero[YAXIS] = 548,
	.GYRO_zero[ZAXIS] = 565,
	.ACC_zero[XAXIS]  = 621,
	.ACC_zero[YAXIS]  = 611,
	.ACC_zero[ZAXIS]  = 766,
	.PID[XAXIS] = { 50, 100, 25, 20},
	.PID[YAXIS] = { 50, 100, 25, 20},
	.PID[ZAXIS] = { 50,  20, 50, 10},
	.LinkRollPitch = ON,
	.AutoDisarm = ON,
	.ReceiverMode = RX_MODE_CPPM,
	//.StickScaling = { 200, 200, 200, 200},
	.StickScaling = { 30, 30, 50, 90},
	.MinThrottle = 10,
	.LCDContrast = 32,
	.HeightDampening = 0,
	.HeightDampeningLimit = 30,
	.LVA = 0,
	.ServoFilter = 50,
	.PID_SelfLevel = { 100, 20, 0, 0},
	.AccTrimPitch = 0,
	.AccTrimRoll = 0,
	.Camera = {0, 50, 0, 50},
	
};

static void _initConfig()
{
	memcpy_P(&Config, &defaultConfig, sizeof(Config));
	mixerLoadTable(Config.MixerIndex);
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

