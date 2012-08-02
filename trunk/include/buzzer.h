/*
 * buzzer.h
 *
 * Created: 02.08.2012 12:29:29
 *  Author: OliverS
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_


void buzzerInit();
void buzzerBuzz(uint16_t ms);
void buzzerBuzzWait(uint16_t ms);
void buzzerLoop();
void buzzerInterval(uint16_t ms);

#endif /* BUZZER_H_ */