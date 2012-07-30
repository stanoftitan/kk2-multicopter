/*
 * global.h
 *
 * Created: 27.07.2012 09:05:10
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

#define F_CPU	20000000
#define MICROTOTCNT1(x) (((unsigned long)x) * 20)
#define TNCT1TOMICRO(x) (x / 20)

#define ON		1
#define OFF		0

#define noinit __attribute__((section(".noinit")))
#define length(array) (sizeof(array)/sizeof(*array))

#include "system.h"
#include <util/atomic.h>
#include "hardware.h"
#include "config.h"

#endif /* GLOBAL_H_ */