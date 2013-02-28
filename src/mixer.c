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
#include "mixer.h"
#include <string.h>
#include <avr/pgmspace.h>


uint16_t MIXER[8];
extern int16_t CHANNELS[4];

void mixerInit()
{
	
}

void mixerLoadModel(uint8_t index)
{
	uint8_t channels;
	model_t* pmodel;
	pmodel = (void*)pgm_read_word(&modelTable[index]);
	channels = (uint8_t)pgm_read_byte(&pmodel->Channels);
	memset(&Config.Mixer, 0, sizeof(Config.Mixer));
	memcpy_P(&Config.Mixer, &pmodel->Channel, (uint8_t)(channels * sizeof(*pmodel->Channel)));
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
			r += (int32_t) CONTROL[AIL] * Config.Mixer[i].Aileron;
			r += (int32_t) CONTROL[ELE] * Config.Mixer[i].Elevator;
			r += (int32_t) CONTROL[RUD] * Config.Mixer[i].Rudder;
			r += (int32_t) CONTROL[THR] * Config.Mixer[i].Throttle;
		
			MIXER[i] = r >> 7;
		}
	}
}

