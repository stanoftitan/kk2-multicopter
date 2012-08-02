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
} mixer_output_t;

typedef struct  
{
	char Name[22];
	mixer_output_t Channel[8];
} mixer_model_t;

#define MIXER_TABLE_LEN		2
extern mixer_model_t mixerTable[MIXER_TABLE_LEN];


void mixerInit();
void mixerLoadTable(uint8_t index);

#endif /* MIXER_H_ */