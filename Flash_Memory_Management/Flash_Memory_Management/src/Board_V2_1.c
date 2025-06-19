/*
 * Board_V2_1.c
 *
 * Created: 8/6/2024 3:30:31 PM
 *  Author: agarciasampedroperez
 */ 

#include "../main.h"

#ifdef Board_V2_1

void USART_Init() //setting USART1 on V2.1 Boards
{
	UBRR1 = UBRR_CALC; //setting Baud Rate for USART
	
	//Following is setting the USART Transmitter; TXD1(PD3)
	DDRD = (1 << DDD3); //enabling TX pin as output; DDD3 can also be PD3 b/c both is defined 3
	UCSR1B |= (1 << TXEN1); //enabling TX for USART1 overriding pin (PD3)
	
	//Turns on the RS232_01 so that TX goes to serial port; we do this because this is an active low shutdown;
	DDRA |= (1 << DDA2); //setting PA2 for RS232 USART1 as output
	PORTA |= (1 << PA2); //"enabling" RS232 USART1 (turning on)
}

void USART_Data(const char *str) //addresses String data by the individual character addresses
{
	for (size_t i = 0; i <strlen(str); i++)
	{
		USART_TX_Data(str[i]); //sends char data to Transmit function
	}
}

void USART_TX_Data(char Data) //transmit char data type to serial (Transmit Function)
{
	while (!(UCSR1A & (1 << UDRE1))) //wait to empty transmit buffer
	{
		;
	}
	UDR1 = Data; //Puts data into buffer, sends the data through serial
}

void LED_Init()
{
	DDRH |= (1 << DDH5); //setting GREEN_LED to output
	DDRB |= (1 << PB6); //setting RED_LED to output
	
	PORTH |= (1 << PH5); //initializing GREEN_LED off
	PORTB |= (1 << PB6); //initializing RED_LED off
}

void LED_Toggle()
{
	PORTH ^= (1 << PH5); //toggle GREEN_LED
	PORTB ^= (1 << PB6); //toggle RED_LED
}

void LED_Red_Toggle()
{
	PORTB ^= (1 << PB6); //toggle RED_LED
}

void LED_Green_Toggle()
{
	PORTH ^= (1 << PH5); //toggle GREEN_LED
}

void VOLTAGE_Translator_Init()
{
	//Turning on the Voltage Translators must be set to logic-high
	
	//Code below is for PICO,POCI, SCK data lines
	DDRC |= (1 << PC6); //setting pin as output
	PORTC |= (1 << PC6); //setting pin to logic high
}

#endif