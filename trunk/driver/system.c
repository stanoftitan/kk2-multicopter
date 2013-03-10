/*
 * system.c
 *
 * Created: 30.07.2012 12:46:10
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

static uint16_t t1_millis, t1_millis_fract, t1_ovf;

ISR(TIMER1_OVF_vect)
{
	t1_ovf++;

	t1_millis += 3;
	t1_millis_fract += 2768;
	if (t1_millis_fract >= 10000)
	{
		t1_millis_fract -= 10000;
		t1_millis++;
	}
}

uint32_t ticks()
{
	uint16_t t1, t2;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		t1 = t1_ovf;
		t2 = TCNT1;
		if ((TIFR1 & _BV(TOV1)) && (t2 < 65533))
			t1++;
	}
	return ((uint32_t)t1 << 16) | t2;
}


uint16_t micros()
{
/*
	uint16_t t1, t2;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		t1 = TCNT1;
		t2 = t1_ovf;
	}		
	return  (t2 * 65536 + t1 * 2) / (2 * TICKSPERMICRO);
*/
	return TICKSTOMICRO(ticks());
}

// accuracy is 3ms
uint16_t millis()
{
	uint16_t t;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		t = t1_millis;
	return t;
}

void setup()
{
	// disable JTAG port
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD);
	
	// disable all hardware but Timer1
	//PRR0 = 0xFF & ~PRTIM1;
	//PRR1 = 0xFF;
	
	LED_DIR = OUTPUT;
	
	// Timer1 @20MHz with overflow interrupt
	TCCR1B = _BV(CS10);
	TIMSK1 = _BV(TOIE1);
}