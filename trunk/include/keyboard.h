/*
 * keyboard.h
 *
 * Created: 30.07.2012 18:29:26
 *  Author: OliverS
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define KEY_1	_BV(KEY_S1_BIT)
#define KEY_2	_BV(KEY_S2_BIT)
#define KEY_3	_BV(KEY_S3_BIT)
#define KEY_4	_BV(KEY_S4_BIT)

void keyboardInit();
uint8_t keyboardRead();
uint8_t keyboardState();

#endif /* KEYBOARD_H_ */