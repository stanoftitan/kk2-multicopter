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
#define SIGNATURE	0x58

static config_t defaultConfig PROGMEM = 
{
	.signature = SIGNATURE,
	.MixerIndex = 0,
	.RX_mode = RX_MODE_CPPM_FUTABA,
	.RX_zero[AIL] = PWM_MID,
	.RX_zero[ELE] = PWM_MID,
	.RX_zero[RUD] = PWM_MID,
	.RX_zero[THR] = PWM_LOW,
	.RX_zero[AUX] = PWM_MID,
	.GYRO_zero[AIL] = 536,
	.GYRO_zero[ELE] = 548,
	.GYRO_zero[RUD] = 565,
	.ACC_zero[AIL]  = 621,
	.ACC_zero[ELE]  = 611,
	.ACC_zero[RUD]  = 766,
	.PID[AIL] = { 150, 100, 50, 20},
	.PID[ELE] = { 150, 100, 50, 20},
	.PID[RUD] = { 150,  20, 50, 10},
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

