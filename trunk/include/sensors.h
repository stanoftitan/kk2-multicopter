/*
 * sensors.h
 *
 * Created: 30.07.2012 12:37:13
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

extern int16_t GYRO[3];
extern uint16_t GYRO_raw[3];
extern int16_t ACC[3];
extern uint16_t ACC_raw[3];
extern uint8_t BATT;

void sensorsRead();
void sensorsCalibate();

#endif /* SENSORS_H_ */