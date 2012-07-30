/*
 * sensors.h
 *
 * Created: 30.07.2012 12:37:13
 *  Author: OliverS
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

extern uint16_t GYRO[3];
extern uint16_t ACC[3];

void sensorsReadGyro();
void sensorsReadAcc();


#endif /* SENSORS_H_ */