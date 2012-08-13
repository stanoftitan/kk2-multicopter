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
#include <avr/pgmspace.h>

int16_t RX[RX_CHANNELS];
uint16_t RX_raw[RX_CHANNELS];
uint8_t RX_good;
static int16_t RX_isr[RX_CHANNELS];
static uint8_t _mode;
static uint8_t _RX_good;


// ISR for vector INT1. Handles CPPM signal in CPPM mode, or PWM signal for AIL in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT1_vect)
{
	static uint32_t _start;
	static uint8_t _index;
	static uint8_t _mask;

	uint32_t t = ticks();
	sei();
	
	if (_mode != RX_MODE_PWM)
	{
		if ((t - _start) > MICROTOTICKS(PPM_SYNC_LENGTH))
		{
			_index = 0;
			_mask = 1;
		}			
		else if (_index >= 0 && _index < RX_CHANNELS)
		{
			RX_isr[_index] = t - _start;
			_index++;
			_RX_good |= _mask;
			_mask <<= 1;
		}			
		_start = t;
	}
	else
	{
		if (RX_AIL)
			_start = t;
		else
		{
			RX_isr[AIL] = t - _start;
			_RX_good |= _BV(AIL);
		}			
	}
}

// ISR for vector INT0. Handles PWM signal for ELE in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT0_vect)
{
	static uint16_t _start;
	uint16_t t = ticks();
	sei();
	
	if (RX_ELE)
		_start = t;
	else
	{
		RX_isr[ELE] = t - _start;
		_RX_good |= _BV(ELE);
	}		
}

// ISR for vector INT2. Handles PWM signal for RUD in PWM mode
__attribute__ ((section(".lowtext")))
ISR(INT2_vect)
{
	static uint16_t _start;
	uint16_t t = ticks();
	sei();
	
	if (RX_RUD)
		_start = t;
	else
	{
		RX_isr[RUD] = t - _start;
		_RX_good |= _BV(RUD);
	}		
}

// ISR for vector PCI3. Handles PWM signal for THR in PWM mode
__attribute__ ((section(".lowtext")))
ISR(PCINT3_vect)
{
	static uint16_t _start;
	uint16_t t = ticks();
	sei();
	
	if (RX_THR)
		_start = t;
	else
	{
		RX_isr[THR] = t - _start;	
		_RX_good |= _BV(THR);
	}		
}

// ISR for vector PCI1. Handles PWM signal for AUX in PWM mode
__attribute__ ((section(".lowtext")))
ISR(PCINT1_vect)
{
	static uint16_t _start;
	uint16_t t = ticks();
	sei();
	
	if (RX_AUX)
		_start = t;
	else
	{
		RX_isr[AUX] = t - _start;	
		_RX_good |= _BV(AUX);
	}		
}

void rxInit(uint8_t mode)
{
	if (mode != RX_MODE_PWM)
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
	RX_good = 0;
}

void rxRead()
{
	uint16_t b;
	uint8_t index;
	
	EVERYMS(50)
	{
		RX_good = _RX_good;
		_RX_good = 0;
	}	
	
	for (uint8_t i = 0; i < RX_CHANNELS; i++)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
			b = RX_isr[i];

		b = TICKSTOMICRO(b);
		
		if (_mode == RX_MODE_PWM)
			index = i;
		else
			index = Config.RX_chmap[i];
		
		if (b >= PWM_MIN && b <= PWM_MAX)
		{
			RX_raw[index] = b; 
			RX[index] = (int16_t)(RX_raw[index] - Config.RX_zero[index]) >> 2;
		}
		/*
		else
			RX_raw[index] = 0;
			*/
	}
	RX[THR]	>>= 1;
}

void rxCalibrate()
{
	rxRead();
	memcpy(&Config.RX_zero, &RX_raw, sizeof(Config.RX_zero));
}