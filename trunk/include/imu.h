/*
 * imu.h
 *
 * Created: 23.02.2013 15:09:25
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef IMU_H_
#define IMU_H_

extern int16_t ANGLE[3];
extern int16_t GYRO_RATE[3];

void imuCalculate();


#endif /* IMU_H_ */