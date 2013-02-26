/*
 * mixer.c
 *
 * Created: 30.07.2012 11:25:15
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "mixer.h"
#include "rx.h"
#include "controller.h"
#include <string.h>
#include <avr/pgmspace.h>

extern model_t const mixerTable[];

uint16_t MIXER[8];
extern int16_t CHANNELS[4];

void mixerInit()
{
	
}

void mixerLoadTable(uint8_t index)
{
	memcpy_P(&Config.Mixer, &mixerTable[index], sizeof(Config.Mixer));
	Config.MixerIndex = index;
}

void mixerMixing()
{
	int32_t r;
	
	for (uint8_t i = 0; i < 8; i++)
	{
// 		if (Config.Mixer.Channel[i].flags == FLAG_NONE)
// 			MIXER[i] = 0;
// 		else
		{
			r = 0;
			r += (int32_t) CONTROL[AIL] * Config.Mixer.Channel[i].Aileron;
			r += (int32_t) CONTROL[ELE] * Config.Mixer.Channel[i].Elevator;
			r += (int32_t) CONTROL[RUD] * Config.Mixer.Channel[i].Rudder;
			r += (int32_t) CONTROL[THR] * Config.Mixer.Channel[i].Throttle;
		
			MIXER[i] = r >> 7;
		}
	}
}

