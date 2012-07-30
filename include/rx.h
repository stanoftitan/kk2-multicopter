/*
 * rx.h
 *
 * Created: 27.07.2012 18:48:51
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef RX_H_
#define RX_H_

extern int16_t RX[RX_CHANNELS];

#define RX_MODE_CPPM	0
#define RX_MODE_PWM		1

#define AIL			0
#define ELE			1
#define THR			2
#define RUD			3
#define AUX			4

void rxInit(uint8_t mode);
void rxRead();
void rxCalibrate();



#endif /* RX_H_ */