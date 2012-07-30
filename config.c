/*
 * config.c
 *
 * Created: 30.07.2012 11:44:47
 *  Author: OliverS
 */ 

#include "global.h"
#include "rx.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

noinit Config_t Config;
EEMEM Config_t EEConfig;

static Config_t defaultConfig PROGMEM = 
{
	.signature = 0x56,
	.RX_zero[AIL] = PWM_MID,
	.RX_zero[ELE] = PWM_MID,
	.RX_zero[THR] = PWM_LOW,
	.RX_zero[RUD] = PWM_MID,
	.RX_zero[AUX] = PWM_MID,
};

static void _initConfig()
{
	memcpy_P(&Config, &defaultConfig, sizeof(Config));
	mixerLoadTable(0);
}

static void _loadConfig()
{
	eeprom_read_block(&Config, &EEConfig, sizeof(Config));
	if (Config.signature != 0x56)
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

