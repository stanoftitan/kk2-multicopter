/*
 * pwm.c
 *
 * Created: 28.07.2012 18:08:27
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "rx.h"
#include <avr/pgmspace.h>

static uint16_t PWM[OUTPUTS];
static const uint8_t masktable[] PROGMEM = {_BV(OUT1_BIT), _BV(OUT2_BIT), _BV(OUT3_BIT), _BV(OUT4_BIT), _BV(OUT5_BIT), _BV(OUT6_BIT), _BV(OUT7_BIT), _BV(OUT8_BIT)};

#define USE_2_OC

static uint8_t checkLoRate(uint16_t* lastStart)
{
	uint16_t t = millis();
	if (t - *lastStart >= LO_RATE_CYCLE)
	{
		*lastStart = t;
		return ON;
	}
	else
		return OFF;
}

static uint16_t doOutput(uint8_t index, uint8_t loActive)
{
	if (PWM[index] && Config.Mixer[index].Flags && (Config.Mixer[index].IsMotor || loActive || Config.Mixer[index].IsHiRate))
	{
		cli();
		OUT_PORT |= pgm_read_byte(&masktable[index]);
		return (uint16_t)(TCNT1 + (PWM[index]));
	}
	else
		return (uint16_t)(TCNT1 + MICROTOTICKS(100));	
}

// for odd output numbers
__attribute__ ((section(".lowtext")))
ISR(TIMER1_COMPA_vect)
{
	static uint8_t index;
	static uint8_t loActive;
	static uint16_t lastLoStart;
	
	OUT_PORT &= ~(_BV(OUT1_BIT) | _BV(OUT3_BIT) | _BV(OUT5_BIT) | _BV(OUT7_BIT));
	sei();
	
	if (index == 0)
		loActive = checkLoRate(&lastLoStart);

	OCR1A = doOutput(index * 2, loActive);
	sei();
	index = (index + 1) % 4;
}

// for even output numbers
__attribute__ ((section(".lowtext")))
ISR(TIMER1_COMPB_vect)
{
	static uint8_t index;
	static uint8_t loActive;
	static uint16_t lastLoStart;
	
	OUT_PORT &= ~(_BV(OUT2_BIT) | _BV(OUT4_BIT) | _BV(OUT6_BIT) | _BV(OUT8_BIT));
	sei();
	
	if (index == 0)
		loActive = checkLoRate(&lastLoStart);
	
	OCR1B = doOutput(index * 2 + 1, loActive);
	sei();
	index = (index + 1) % 4;
}


void pwmInit()
{
	OUT_DDR = 0xFF;			// all pins to output mode
	OUT_PORT = 0x00;		// all OFF
}

void pwmWrite(uint8_t channel, uint16_t value)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
		PWM[channel] = (uint16_t) MICROTOTICKS(value);
}

void pwmEnable()
{
	OCR1A = TCNT1;
	OCR1B = TCNT1;
	TIFR1 |= _BV(OCF1A) | _BV(OCF1B);		// clear OCA and OCB interrupt
	TIMSK1 |= _BV(OCIE1A) | _BV(OCIE1B);	// enable OCA and OCB interrupt on timer1
}

void pwmDisable()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		TIMSK1 &= ~(_BV(OCIE1A) | _BV(OCIE1B));	// disable OCA and OCB interrupt on timer1
		TIFR1 |= _BV(OCF1A) | _BV(OCF1B);		// clear OCA and OCB interrupt
	}
	OUT_PORT = 0x00;		// all OFF
}
