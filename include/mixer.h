/*
 * mixer.h
 *
 * Created: 30.07.2012 11:07:47
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef MIXER_H_
#define MIXER_H_

#define FLAG_SERVO		1
#define FLAG_ESC		2
#define FLAG_NONE		0

typedef struct 
{
	uint8_t flags;
	int8_t Throttle;
	int8_t Aileron;
	int8_t Elevator;
	int8_t Rudder;
	int8_t Offset;		
} MixerOutput;

typedef struct  
{
	MixerOutput Channel[8];
	char Name[20];
} MixerModel;


void mixerInit();
void mixerLoadTable(uint8_t index);


#endif /* MIXER_H_ */