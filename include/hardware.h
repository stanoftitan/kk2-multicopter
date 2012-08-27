/*
 * hardware.h
 *
 * Created: 27.07.2012 09:07:19
 *  Author: OliverS
 *
 * $Id$
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>

typedef struct  
{
	unsigned int bit0 : 1;
	unsigned int bit1 : 1;
	unsigned int bit2 : 1;
	unsigned int bit3 : 1;
	unsigned int bit4 : 1;
	unsigned int bit5 : 1;
	unsigned int bit6 : 1;
	unsigned int bit7 : 1;
} volatile _bitreg8;

#define _REG_BIT2(r,b)	((*(_bitreg8*)&r).bit ## b)
#define _REG_BIT(r,b)	_REG_BIT2(r,b)

#define OUTPUT		1
#define INPUT		0

/// LED
#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED_PIN		PINB
#define LED_BIT		3
#define LED			_REG_BIT(LED_PORT, LED_BIT)
#define LED_DIR		_REG_BIT(LED_DDR, LED_BIT)
#define LED_TOGGLE	(_REG_BIT(LED_PIN, LED_BIT) = 1)

/// LCD
#define LCD_PORT	PORTD
#define LCD_DDR		DDRD
#define LCD_PIN		PIND
#define LCD_CS_BIT	5
#define LCD_RST_BIT	6
#define LCD_A0_BIT	7
#define LCD_SCL_BIT	4
#define LCD_SDA_BIT	1
#define LCD_CS		_REG_BIT(LCD_PORT, LCD_CS_BIT)
#define LCD_RST		_REG_BIT(LCD_PORT, LCD_RST_BIT)
#define LCD_A0		_REG_BIT(LCD_PORT, LCD_A0_BIT)
#define LCD_SCL		_REG_BIT(LCD_PORT, LCD_SCL_BIT)
#define LCD_SDA		_REG_BIT(LCD_PORT, LCD_SDA_BIT)
#define LCD_CS_DIR	_REG_BIT(LCD_DDR, LCD_CS_BIT)
#define LCD_RST_DIR	_REG_BIT(LCD_DDR, LCD_RST_BIT)
#define LCD_A0_DIR	_REG_BIT(LCD_DDR, LCD_A0_BIT)
#define LCD_SCL_DIR	_REG_BIT(LCD_DDR, LCD_SCL_BIT)
#define LCD_SDA_DIR	_REG_BIT(LCD_DDR, LCD_SDA_BIT)

/// ADC
#define ADC_GYR_X	1
#define ADC_GYR_Y	4
#define ADC_GYR_Z	2
#define ADC_ACC_X	5
#define ADC_ACC_Y	6
#define ADC_ACC_Z	7
#define ADC_VBAT	3

/// RX
// 1. AIL
#define RX_AIL_DDR	DDRD
#define RX_AIL_PIN	PIND
#define RX_AIL_BIT	3
#define RX_AIL		_REG_BIT(RX_AIL_PIN, RX_AIL_BIT)
#define RX_AIL_DIR	_REG_BIT(RX_AIL_DDR, RX_AIL_BIT)
// 2. ELE
#define RX_ELE_DDR	DDRD
#define RX_ELE_PIN	PIND
#define RX_ELE_BIT	2
#define RX_ELE		_REG_BIT(RX_ELE_PIN, RX_ELE_BIT)
#define RX_ELE_DIR	_REG_BIT(RX_ELE_DDR, RX_ELE_BIT)
// 3. THR
#define RX_THR_DDR	DDRD
#define RX_THR_PIN	PIND
#define RX_THR_BIT	0
#define RX_THR		_REG_BIT(RX_THR_PIN, RX_THR_BIT)
#define RX_THR_DIR	_REG_BIT(RX_THR_DDR, RX_THR_BIT)
// 4. RUD
#define RX_RUD_DDR	DDRB
#define RX_RUD_PIN	PINB
#define RX_RUD_BIT	2
#define RX_RUD		_REG_BIT(RX_RUD_PIN, RX_RUD_BIT)
#define RX_RUD_DIR	_REG_BIT(RX_RUD_DDR, RX_RUD_BIT)
// 5. AUX
#define RX_AUX_DDR	DDRB
#define RX_AUX_PIN	PINB
#define RX_AUX_BIT	0
#define RX_AUX		_REG_BIT(RX_AUX_PIN, RX_AUX_BIT)
#define RX_AUX_DIR	_REG_BIT(RX_AUX_DDR, RX_AUX_BIT)

#define RX_CHANNELS			5
#define PWM_MIN				800		// was 922
#define PWM_LOW				1120	// used if output = 0
#define PWM_MID				1520
#define PWM_MAX				2200	// was 2118
#define PWM_CAL_RANGE		100
#define PPM_SYNC_LENGTH		3000

/// Outputs
#define OUT_PORT	PORTC
#define OUT_DDR		DDRC
#define OUT1_BIT	6
#define OUT1		_REG_BIT(OUT_PORT, OUT1_BIT)
#define OUT2_BIT	4
#define OUT2		_REG_BIT(OUT_PORT, OUT2_BIT)
#define OUT3_BIT	2
#define OUT3		_REG_BIT(OUT_PORT, OUT3_BIT)
#define OUT4_BIT	3
#define OUT4		_REG_BIT(OUT_PORT, OUT4_BIT)
#define OUT5_BIT	1
#define OUT5		_REG_BIT(OUT_PORT, OUT5_BIT)
#define OUT6_BIT	0
#define OUT6		_REG_BIT(OUT_PORT, OUT6_BIT)
#define OUT7_BIT	5
#define OUT7		_REG_BIT(OUT_PORT, OUT7_BIT)
#define OUT8_BIT	7
#define OUT8		_REG_BIT(OUT_PORT, OUT8_BIT)


/// Keyboard
#define KEYBOARD_PORT	PORTB
#define KEYBOARD_DDR	DDRB
#define KEYBOARD_PIN	PINB
#define KEY_S1_BIT		7
#define KEY_S2_BIT		6
#define KEY_S3_BIT		5
#define KEY_S4_BIT		4

/// Buzzer
#define BUZZER_PORT		PORTB
#define BUZZER_DDR		DDRB
#define BUZZER_PIN		PINB
#define BUZZER_BIT		1
#define BUZZER_DIR		_REG_BIT(BUZZER_DDR, BUZZER_BIT)
#define BUZZER			_REG_BIT(BUZZER_PORT, BUZZER_BIT)
#define BUZZER_TOGGLE	(_REG_BIT(BUZZER_PIN, BUZZER_BIT) = 1)

#endif /* HARDWARE_H_ */