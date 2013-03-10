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

#define FLAG_NONE		0
#define FLAG_SERVO		2
#define FLAG_ESC		1
#define FLAG_HIGH		4

typedef union
{
	int8_t I8[6];
	struct
	{
		int8_t Throttle;
		int8_t Aileron;
		int8_t Elevator;
		int8_t Rudder;
		int8_t Offset;
		union
		{
			uint8_t Flags;
			struct 
			{
				uint8_t IsMotor : 1;
				uint8_t IsServo : 1;
				uint8_t IsHiRate : 1;
			};			
		};		
	}; 
} mixer_channel_t;

// type for storing the model parameters
typedef struct  
{
	const char *Name;
	uint8_t Channels;
	mixer_channel_t Channel[];
} model_t;

// type for storing just mixer values
typedef mixer_channel_t mixer_t[OUTPUTS];  

#define MODEL_TABLE_LEN		22
extern const model_t* const modelTable[MODEL_TABLE_LEN];
extern uint16_t MIXER[OUTPUTS];

void mixerInit();
void mixerLoadModel(uint8_t index);
void mixerCalculate();
void mixerOutput();

#endif /* MIXER_H_ */