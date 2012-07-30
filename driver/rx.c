/*
 * receiver.c
 *
 * Created: 27.07.2012 18:07:00
 *  Author: OliverS
 *
 * $Id$
 */ 

/*
 * Receiver driver supports two modes:
 * 1. CPPM mode: CPPM signal on AIL pin (PB3/INT1)
 * 2. PWM mode: 5 PWM inputs (AIL, ELE, THR, RUD, AUX) on 
 *				pins PB3/INT1, PD2/INT0, PD0, PB2/INT2, PB0
 *
 */

#include "global.h"
#include "rx.h"
#include <string.h>

int16_t RX[RX_CHANNELS];
uint16_t RX_raw[RX_CHANNELS];
static int16_t RX_isr[RX_CHANNELS];
static uint8_t _mode;

// ISR for vector INT1. Handles CPPM signal in CPPM mode, or PWM signal for AIL in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT1_vect)
{
	static uint16_t _start;
	static int8_t _index;
	static uint8_t _sync;

	uint16_t t = micros();
	sei();
	
	if (_mode == RX_MODE_CPPM)
	{
		if ((t - _start) > PPM_SYNC_LENGTH)
		{
			_index = 0;
			_sync = 1;
		}			
		else if (_sync != 0 && _index >= 0 && _index < RX_CHANNELS)
		{
			if ((t - _start) < PWM_MIN)
				_sync = 0;
			else
			{
				RX_isr[_index] = t - _start;
				_index++;
			}				
		}			
		_start = t;
	}
	else
	{
		if (RX_AIL)
			_start = t;
		else
			RX_isr[AIL] = t - _start;
	}
}

// ISR for vector INT0. Handles PWM signal for ELE in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT0_vect)
{
	static uint16_t _start;
	uint16_t t = micros();
	sei();
	
	if (RX_ELE)
		_start = t;
	else
		RX_isr[ELE] = t - _start;
}

// ISR for vector INT2. Handles PWM signal for RUD in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT2_vect)
{
	static uint16_t _start;
	uint16_t t = micros();
	sei();
	
	if (RX_RUD)
		_start = t;
	else
		RX_isr[RUD] = t - _start;
}

// ISR for vector PCI3. Handles PWM signal for THR in PWM mode
__attribute__ ((section(".lowtext")))
ISR(PCINT3_vect)
{
	static uint16_t _start;
	uint16_t t = micros();
	sei();
	
	if (RX_THR)
		_start = t;
	else
		RX_isr[THR] = t - _start;	
}

// ISR for vector PCI1. Handles PWM signal for AUX in PWM mode
__attribute__ ((section(".lowtext")))
ISR(PCINT1_vect)
{
	static uint16_t _start;
	uint16_t t = micros();
	sei();
	
	if (RX_AUX)
		_start = t;
	else
		RX_isr[AUX] = t - _start;	
}

void rxInit(uint8_t mode)
{
	if (mode == RX_MODE_CPPM)
	{
		// set pin direction
		RX_AIL_DIR = INPUT;

		// enable interrupt INT1 for CPPM signal on PD3
		EICRA  = /*_BV(ISC10) |*/ _BV(ISC11);	// rising edge on PD3 (INT1) 
		EIFR   = _BV(INTF1);				// clear interrupt
		EIMSK  = _BV(INT1);					// enable interrupt for INT1
		PCICR  = 0;							// disable all PCIs
	}
	else
	{
		// set pins direction
		RX_AIL_DIR = INPUT;
		RX_ELE_DIR = INPUT;
		RX_THR_DIR = INPUT;
		RX_RUD_DIR = INPUT;
		RX_AUX_DIR = INPUT;
			
		// enable interrupts
		EICRA  = _BV(ISC00) | _BV(ISC10) | _BV(ISC20);	// any edge on INT0, INT1 and INT2
		EIFR   = _BV(INTF0) | _BV(INTF1) | _BV(INTF2);	// clear interrupts
		EIMSK  = _BV(INT0)  | _BV(INT1)  | _BV(INT2);	// enable interrupt for INT0, INT1 and INT2
		PCMSK1 = _BV(PCINT8);							// enable PCINT8 (AUX) -> PCI1
		PCMSK3 = _BV(PCINT24);							// enable PCINT24 (THR) -> PCI3
		PCIFR  = _BV(PCIF1) | _BV(PCIF3);				// clear interrupts
		PCICR |= _BV(PCIE1) | _BV(PCIE3);				// enable PCI1 and PCI3
	}	
	_mode = mode;	
}

void rxRead()
{
	uint16_t b;
	for (uint8_t i = 0; i < RX_CHANNELS; i++)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
			b = RX_isr[i];
		if (b >= PWM_MIN && b <= PWM_MAX)
		{
			RX_raw[i] = b; 
			RX[i] = (int16_t)(b - Config.RX_zero[i]) >> 2;
		}
		else
		{
			RX[i] = 0;
		}			
	}			
}

void rxCalibrate()
{
	rxRead();
	memcpy(&Config.RX_zero, &RX_raw, sizeof(Config.RX_zero));
	configSave();
}