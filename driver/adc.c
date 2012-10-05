/*
 * adc.c
 *
 * Created: 27.07.2012 17:42:02
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

static uint16_t _adc[8];

__attribute__ ((section(".lowtext")))
ISR(ADC_vect, ISR_NOBLOCK)
{
	static uint8_t _index;
	_adc[_index] = ADC;
	_index = (_index + 1) % 8U;
	ADMUX = _index;
	ADCSRA |= _BV(ADSC);
}

void adcInit()
{
	DIDR0 = 0xFF;	// disable all digital inputs on Port A
	ADMUX = 0;
	ADCSRA = 0b11011111;	// ADEN, ADSC, ADIE, 
					// clk/128 = 156.250 kHz -> 13*128 = 1664 cycles per conversion i.e. 83.2us @20MHz
}

uint16_t adcRead(uint8_t channel)
{
	// disable interrupt and wait for end of conversion
	ADCSRA = (0xFF & ~_BV(ADIE));
	loop_until_bit_is_clear(ADCSRA, ADSC);
	
	ADMUX = channel;		// set channel to read from
	ADCSRA |= _BV(ADSC);	// start conversion
	loop_until_bit_is_clear(ADCSRA, ADSC);	// wait to complete
	ADCSRA = 0b11011111;	// reenable interrupt
	return ADC;
}

uint16_t adcGet(uint8_t channel)
{
	uint16_t _t;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
		_t = _adc[channel];
	return _t;
}