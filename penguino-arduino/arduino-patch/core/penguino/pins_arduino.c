/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: pins_arduino.c 254 2007-04-20 23:17:38Z mellis $
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] = {
//	NOT_A_PORT,
	NOT_A_PORT,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
       	&DDRA,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#else
	NOT_A_PORT,
#endif 
	&DDRB,
	&DDRC,
	&DDRD,
};

const uint8_t PROGMEM port_to_output_PGM[] = {
//	NOT_A_PORT,
	NOT_A_PORT,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	&PORTA,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#else
	NOT_A_PORT,
#endif 
	&PORTB,
	&PORTC,
	&PORTD,
};

const uint8_t PROGMEM port_to_input_PGM[] = {
//	NOT_A_PORT,
	NOT_A_PORT,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	&PINA,
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#else
	NOT_A_PORT,
#endif 
	&PINB,
	&PINC,
	&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD, /* 1 */
	PD, /* 2 */
	PD, /* 3 */
	PD, /* 4 */
	PD, /* 5 */
	PD, /* 6 */
	PD, /* 7 */
	PB, /* 8 */
	PB, /* 9 */
	PB, /* 10 */
	PB, /* 11 */
	PB, /* 12 */
	PB, /* 13 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	PB, /* 14 */
	PB, /* 15 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#endif
	PC, /* 16 */
	PC, /* 17 */
	PC, /* 18 */
	PC, /* 19 */
	PC, /* 20 */
	PC, /* 21 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
   PC, /* 22 */
	PC, /* 23 */
	PA, /* 24 */
	PA, /* 25 */
	PA, /* 26 */
	PA, /* 27 */
	PA, /* 28 */
	PA, /* 29 */
	PA, /* 30 */
	PA, /* 31 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#endif 
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	_BV(6),
	_BV(7),
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#endif
	_BV(0), /* 16, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	_BV(6),
	_BV(7),
	_BV(0), /* 24, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#endif
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, 	/* 0 - port D */
	NOT_ON_TIMER, 	/* 1 - PD1 */
	NOT_ON_TIMER, 	/* 2- PD2 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega168__)
	// on the ATmega168, digital pin 3 has hardware pwm
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
	TIMER2B, 		/* 3 - PD3 */
	NOT_ON_TIMER, 	/* 4 - PD4 */
	TIMER0B, 		/* 5 - PD5 */
	TIMER0A, 		/* 6 - PD6 */
	NOT_ON_TIMER, 	/* 7 - PD7 */
#elif defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	NOT_ON_TIMER, 	/* 3 - PD3 */
	TIMER1B, 		/* 4 - PD4 */
	TIMER1A, 		/* 5 - PD5 */
	NOT_ON_TIMER, 	/* 6 - PD6 */
	TIMER2,       	/* 7 - PD7 */
#else
	NOT_ON_TIMER, 	/* 3 - PD3 */
	NOT_ON_TIMER, 	/* 4 - PD4 */
	NOT_ON_TIMER, 	/* 5 - PD5 */
	NOT_ON_TIMER, 	/* 6 - PD6 */
	NOT_ON_TIMER, 	/* 7 - PD7 */
#endif
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */

	NOT_ON_TIMER, 	/* 8 - port B */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	NOT_ON_TIMER, 	/* 9 - PB1 */
	NOT_ON_TIMER, 	/* 10 - PB2 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#else
	TIMER1A, 		/* 9 - PB1 */
	TIMER1B, 		/* 10 - PB2 */
#endif

#if defined(__AVR_ATmega48__) || defined( __AVR_ATmega168__ )
	TIMER2A, 		/* 11 - PB3 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#elif defined( __AVR_ATmega32__ ) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	TIMER0, 		/* 11 - PB3 --- TIMER OC0 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - END */
#else
	TIMER2, 		/* 11 - PB3 */
#endif
	NOT_ON_TIMER, /* 12 - PB4 */
	NOT_ON_TIMER, /* 13 - PB5 */
	NOT_ON_TIMER, /* 14 - PB6 */
	NOT_ON_TIMER, /* 15 - PB7 */
	NOT_ON_TIMER, /* 16 - port C */
	NOT_ON_TIMER, /* 17 - PC1 */
	NOT_ON_TIMER, /* 18 - PC2 */
	NOT_ON_TIMER, /* 19 - PC3 */
	NOT_ON_TIMER, /* 20 - PC4 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#if defined(__AVR_ATmega32__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644__)
	NOT_ON_TIMER, /* 21 - PC5 */
	NOT_ON_TIMER, /* 22 - PC6 */
	NOT_ON_TIMER, /* 23 - PC7 */
	NOT_ON_TIMER, /* 24, port A */
	NOT_ON_TIMER, /* 25 - PA1 */
	NOT_ON_TIMER, /* 26 - PA2 */
	NOT_ON_TIMER, /* 27 - PA3 */
	NOT_ON_TIMER, /* 28 - PA4 */
	NOT_ON_TIMER, /* 29 - PA5 */
	NOT_ON_TIMER, /* 30 - PA6 */
	NOT_ON_TIMER, /* 31 - PA7 */
/* ATMega32/324P/644 mod provided by AndreS at robotcraft.ca - START */
#endif
};

