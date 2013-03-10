/*
 * serial.c
 *
 * Created: 25.02.2013 22:14:49
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"

#define BUFFER_SIZE	32

struct FifoBuffer
{
	uint8_t Buffer[BUFFER_SIZE];
	volatile uint8_t ReadPtr;
	volatile uint8_t WritePtr;
};

static noinit struct FifoBuffer TX_Buf;
static noinit struct FifoBuffer RX_Buf;

static uint8_t IsFull(struct FifoBuffer* buf)
{
	return (uint8_t)(buf->WritePtr + 1) % BUFFER_SIZE == buf->ReadPtr;
}

static void Enqueue(struct FifoBuffer* buf, uint8_t c)
{
	if (!IsFull(buf))
	{
		buf->Buffer[buf->WritePtr] = c;
		buf->WritePtr = (uint8_t)(buf->WritePtr + 1) % BUFFER_SIZE;
	}
}

static int16_t Dequeue(struct FifoBuffer* buf)
{
	if (buf->ReadPtr != buf->WritePtr)
	{
		uint8_t c = buf->Buffer[buf->ReadPtr];
		buf->ReadPtr = (uint8_t)(buf->ReadPtr + 1) % BUFFER_SIZE;
		return c;
	}
	return -1;
}

__attribute__((section(".lowtext")))
ISR(USART1_UDRE_vect)
{
	int16_t c = Dequeue(&TX_Buf);
	if (c >= 0)
		UDR1 = c;
	else	// buffer empty -> switch off interrupt
		UCSR1B &= ~_BV(UDRIE1);
}

__attribute__((section(".lowtext")))
ISR(USART1_RX_vect)
{
	Enqueue(&RX_Buf, UDR1);
}

void serialInit()
{
	// enable hardware
	//PRR0 |= PRUSART1;
	
	// reset pointers
	TX_Buf.WritePtr = 0;
	TX_Buf.ReadPtr = 0;
	RX_Buf.ReadPtr = 0;
	RX_Buf.WritePtr = 0;
		
	// using USART1 with TXD on PD3 and RXD on PD2. Running with 115.2 kbaud
	UBRR1 = 10;
	UCSR1A = 0;
	UCSR1B = _BV(TXEN1) | _BV(RXEN1) | _BV(RXCIE1);
	UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);
}

void serialWriteChar(char c)
{
	if (UCSR1B == 0)
		return;
		
	// if data register empty put byte in that register
	if (UCSR1A & _BV(UDRE1))
	{
		UDR1 = c;
		UCSR1B |= _BV(UDRIE1);
	}
	else
	{
		// wait for free space in buffer
		while (IsFull(&TX_Buf));
		Enqueue(&TX_Buf, c);
	}
}

uint8_t serialAvail()
{
	return RX_Buf.ReadPtr != RX_Buf.WritePtr;
}

int16_t serialReadChar()
{
	return Dequeue(&RX_Buf);
}

void serialTerminate()
{
	UCSR1A = 0;
	UCSR1B = 0;	
}