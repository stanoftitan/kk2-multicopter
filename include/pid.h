/*
 * pid.h
 *
 * Created: 02.08.2012 14:59:28
 *  Author: OliverS
 */ 


#ifndef PID_H_
#define PID_H_

typedef struct 
{
	uint8_t PGain;
	uint8_t PLimit;
	uint8_t IGain;
	uint8_t ILimit;
} pid_param_t;


#endif /* PID_H_ */