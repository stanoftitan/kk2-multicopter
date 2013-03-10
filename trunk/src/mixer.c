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
#include "pwm.h"
#include <string.h>
#include <avr/pgmspace.h>

#define max(a,b) ((a)>(b)?(a):(b))


uint16_t MIXER[OUTPUTS];
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

void mixerCalculate()
{
	int32_t r;
	
	for (uint8_t i = 0; i < OUTPUTS; i++)
	{
 		if (Config.Mixer[i].Flags == FLAG_NONE)
 			MIXER[i] = 0;
 		else
		{
			if (Config.Mixer[i].IsServo)
				r = PWM_MID - PWM_LOW;
			else
				r = 0;
				
			r += (int32_t) CONTROL[ROL] * Config.Mixer[i].Aileron;
			r += (int32_t) CONTROL[PIT] * Config.Mixer[i].Elevator;
			r += (int32_t) CONTROL[YAW] * Config.Mixer[i].Rudder;
			r += (int32_t) CONTROL[THR] * Config.Mixer[i].Throttle;
			r += (int32_t) ((PWM_MID - PWM_LOW) * 32 / 50) * Config.Mixer[i].Offset;
			MIXER[i] = limit(PWM_LOW + (r >> 5), PWM_MIN, PWM_MAX);
		}
	}
}

void mixerOutput()
{
	for (uint8_t i = 0; i < OUTPUTS; i++)
	{
		if (Config.Mixer[i].IsServo)
			pwmWrite(i, MIXER[i]);
		else if (Config.Mixer[i].IsMotor)
		{
			if (State.Armed && !State.ThrottleOff)
				pwmWrite(i, max(MIXER[i], PWM_LOW + Config.MinThrottle * 10));
			else
				pwmWrite(i, PWM_LOW);
		}
		else
			pwmWrite(i, 0);
	}			
}
