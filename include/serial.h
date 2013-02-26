/*
 * serial.h
 *
 * Created: 25.02.2013 23:20:28
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

void serialInit();
void serialTerminate();
void serialWriteChar(char c);
int16_t serialReadChar();
uint8_t serialAvail();


#endif /* SERIAL_H_ */