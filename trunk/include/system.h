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

#define TICKSPERMICRO		(20UL)
#define MICROTOTICKS(us)	(us * TICKSPERMICRO)
#define TICKSTOMICRO(t)		(t / TICKSPERMICRO)

void setup();
uint32_t ticks();
//uint16_t micros();
uint16_t millis();

#ifdef SIMULATOR
#define EVERYMS(ms)
#define WAITMS(ms)
#define FIXEDMS(ms)

#define EVERYUS(ms)
#define WAITUS(ms)
#define FIXEDUS(ms) 
#else /* SIMULATOR */

static __inline__ void __iWaitForMS(uint16_t __m, uint16_t ms)
{
	while (millis() - __m < ms);
}

static __inline__ void __iWaitForTicks(uint32_t __m, uint32_t t)
{
	while (ticks() - __m < t);
}

#define EVERYMS(ms) static uint16_t __CONCAT(_t,__LINE__); for(uint16_t _m = millis(); _m - __CONCAT(_t,__LINE__) >= ms; __CONCAT(_t,__LINE__) = _m)
#define WAITMS(ms) for(uint16_t _m = millis(); millis() - _m < ms;)
#define FIXEDMS(ms) for(uint16_t __m = millis(); millis() - __m < ms; __iWaitForMS(__m, ms))

#define EVERYUS(us) static uint32_t __CONCAT(_t,__LINE__); for(uint32_t _m = ticks(); _m - __CONCAT(_t,__LINE__) >= MICROTOTICKS(us); __CONCAT(_t,__LINE__) = _m)
#define WAITUS(us) for(uint32_t _m = ticks(); ticks() - _m < MICROTOTICKS(us);)
#define FIXEDUS(us) for(uint32_t __m = ticks(); ticks() - __m < MICROTOTICKS(us); __iWaitForTicks(__m, MICROTOTICKS(us)))

#endif /* SIMULATOR */

#endif /* SYSTEM_H_ */