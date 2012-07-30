/*
 * mixer.c
 *
 * Created: 30.07.2012 11:25:15
 *  Author: OliverS
 */ 

#include "global.h"
#include "mixer.h"
#include <string.h>
#include <avr/pgmspace.h>

extern MixerModel mixerTable[] PROGMEM;

uint16_t MIXER[8];

void mixerInit()
{
	
}

void mixerLoadTable(uint8_t index)
{
	memcpy_P(&Config.Mixer, &mixerTable[index], sizeof(Config.Mixer));
}

void mixerDoMixing()
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (Config.Mixer.Channel[i].flags == 0)
			MIXER[i] = 0;
		else
		{
			// @@@
		}
	}
}

