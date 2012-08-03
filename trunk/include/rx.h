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
extern uint16_t RX_raw[RX_CHANNELS];

#define RX_MODE_PWM				0
#define RX_MODE_CPPM_FUTABA		1
#define RX_MODE_CPPM_GRAUPNER	2

#define AIL			0
#define ELE			1
#define RUD			2
#define THR			3
#define AUX			4

void rxInit(uint8_t mode);
void rxRead();
void rxCalibrate();



#endif /* RX_H_ */