/*
 * system.c
 *
 * Created: 30.07.2012 12:46:10
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

static uint16_t t1_millis, t1_millis_fract, t1_micros;
static uint8_t t1_micros_fract;

ISR(TIMER1_OVF_vect)
{
	t1_millis += 3;
	t1_millis_fract += 2768;
	if (t1_millis_fract >= 10000)
	{
		t1_millis++;
		t1_millis_fract -= 10000;
	}
	
	t1_micros += 3276;
	t1_micros_fract += 8;
	if (t1_micros_fract >= 10)
	{
		t1_micros++;
		t1_micros_fract -= 10;
	}
}

uint16_t micros()
{
	uint16_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		t = TCNT1;
	return (t1_micros + TCNT1 / 20);
}

uint16_t millis()
{
	uint16_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		t = TCNT1;
	return (t1_millis + TCNT1 / 20000);
}

void setup()
{
	LED_DIR = OUTPUT;
	
	// Timer1 @20MHz with overflow interrupt
	TCCR1B = _BV(CS10);
	TIMSK1 = _BV(TOIE1);
}