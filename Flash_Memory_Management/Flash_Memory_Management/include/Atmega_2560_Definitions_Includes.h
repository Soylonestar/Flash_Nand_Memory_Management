/*
 * Atmega_2560_Definitions_Includes.h
 *
 * Created: 8/6/2024 12:34:40 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef ATMEGA_2560_DEFINITIONS_INCLUDES_H_
#define ATMEGA_2560_DEFINITIONS_INCLUDES_H_

/*The following is our micro controller parameters*/

//#define F_CPU 16000000UL //system clock 16MHz (commented out because I made a global symbol in the C Compiler)
#define USART_BAUD 9600 //baud rate of USART
#define UBRR_CALC ((float)(F_CPU / (16 * (float)USART_BAUD)) - 1) //Equation in Pg 203 for calculating (setting Baud Rate) UBRR Value

#endif /* ATMEGA_2560_DEFINITIONS_INCLUDES_H_ */