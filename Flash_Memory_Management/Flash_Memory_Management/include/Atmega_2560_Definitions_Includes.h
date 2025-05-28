/*
 * Atmega_2560_Definitions_Includes.h
 *
 * Created: 8/6/2024 12:34:40 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef ATMEGA_2560_DEFINITIONS_INCLUDES_H_
#define ATMEGA_2560_DEFINITIONS_INCLUDES_H_

/*The following is basics libraries and our microcontroller parameters*/

#define F_CPU 16000000UL //system clock 16MHz
#define USART_BAUD 9600 //baud rate of USART


#include <xc.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#endif /* ATMEGA_2560_DEFINITIONS_INCLUDES_H_ */