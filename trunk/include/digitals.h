/*
 * buzzer.h
 *
 * Created: 02.08.2012 12:29:29
 *  Author: OliverS
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_


void digitalsInit();
void digitalsBuzz(uint16_t ms);
void digitalsBuzzBlocking(uint16_t ms);
void digitalsLoop();
void digitalsBuzzInterval(uint16_t ms);
void digitalsLed(uint16_t ms);
void digitalsLedInterval(uint16_t ms);


#endif /* BUZZER_H_ */