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
#define SIGNATURE	0x56

static config_t defaultConfig PROGMEM = 
{
	.signature = SIGNATURE,
	.MixerIndex = 0,
	.RX_mode = RX_MODE_CPPM,
	.RX_zero[AIL] = PWM_MID,
	.RX_zero[ELE] = PWM_MID,
	.RX_zero[THR] = PWM_LOW,
	.RX_zero[RUD] = PWM_MID,
	.RX_zero[AUX] = PWM_MID,
	.GYRO_zero[0] = 536,
	.GYRO_zero[1] = 548,
	.GYRO_zero[2] = 565,
	.ACC_zero[0]  = 621,
	.ACC_zero[1]  = 611,
	.ACC_zero[2]  = 766,
	.PID[0] = { 150, 100, 50, 20},
	.PID[1] = { 150, 100, 50, 20},
	.PID[2] = { 150,  20, 50, 10},
	.StickScaling = { 30, 30, 50, 90},
	.MinThrottle = 10,
	.LCDContrast = 32,
	.HeightDampening = 0,
	.HeightDampeningLimit = 30,
	.LVA = 0,
	.PID_SelfLevel = { 40, 20, 0, 0},
};

static void _initConfig()
{
	memcpy_P(&Config, &defaultConfig, sizeof(Config));
	mixerLoadTable(Config.MixerIndex);
}

static void _loadConfig()
{
	eeprom_read_block(&Config, &EEConfig, sizeof(Config));
	if (Config.signature != SIGNATURE)
		_initConfig();
}

void configInit()
{
	_loadConfig();
}

void configSave()
{
	eeprom_update_block(&Config, &EEConfig, sizeof(Config));
}

