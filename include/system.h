/*
 * system.h
 *
 * Created: 28.07.2012 14:51:38
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stdint.h"

void setup();
uint16_t micros();
uint16_t millis();

#define EVERYMS(ms) static uint16_t __CONCAT(_t,__LINE__); for(uint16_t _m = millis(); _m - __CONCAT(_t,__LINE__) >= ms; __CONCAT(_t,__LINE__) += ms)
#define WAITMS(ms) for(uint16_t _m = millis(); millis() - _m < ms;);

#endif /* SYSTEM_H_ */