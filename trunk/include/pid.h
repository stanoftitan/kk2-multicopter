/*
 * pid.h
 *
 * Created: 02.08.2012 14:59:28
 *  Author: OliverS
 */ 


#ifndef PID_H_
#define PID_H_

typedef union 
{
	struct 
	{
		uint8_t PGain;
		uint8_t PLimit;
		uint8_t IGain;
		uint8_t ILimit;
	};		
	uint8_t UI8[4];
} pid_param_t;

typedef struct  
{
	int16_t Integral;
	int16_t Error;
} pid_state_t;

#endif /* PID_H_ */