/*
 * Board_V2_2.c
 *
 * Created: 8/6/2024 12:21:22 PM
 *  Author: agarciasampedroperez
 * Description: This is for our V2.2 board function
 */ 

#include "../main.h"

#ifdef Board_V2_2

void USART_Init(void) //TXD0(PE1) setting USART0 on V2.2 Boards
{
	//Set Internal Clock Generator or Baud Rate
	UBRR0 = UBRR_CALC; //setting Baud Rate for USART
	
	//Set pin and transmitter for USART
	DDRE |= (1 << PE1); //enabling TX pin as output
	UCSR0B |= (1 << TXEN0); //enabling Transmitter for USART overriding pin (PE1)
	
	//Set pin and receiver for USART
	DDRE &= ~(1 << PE0); //enabling RX pin as input
	UCSR0B |= (1 << RXEN0); //enabling Receiver for USART overriding pin (PE0)
	
	//Turns on the RS232_01 so that TX goes to serial port; we do this because this is an active low shutdown;
	DDRA |= (1 << DDA2); //setting PA2 for RS232 USART0 as output
	PORTA |= (1 << PA2); //"enabling" RS232 USART0 (turning on)
}

void USART_Data (const uint8_t *str) //addresses String Data by the individual uint8_t addresses
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		USART_TX_Data(str[i]); //sends uint8_t data to Transmit function
	}
}

void USART_TX_Data(uint8_t Data) //transmit uint8_t data type to serial (Transmit Function)
{
	while (!(UCSR0A & (1 << UDRE0))) //wait for empty transmit buffer...
	{
		;
	}
	UDR0 = Data; //Puts data into the buffer, sends the data through TX
}

uint8_t USART_RX_Data() //receive uint8_t data type from serial (Receive Function)
{
	while (!(UCSR0A & (1 << RXC0))) //wait for buffer to receive data...
	{
		;
	}
	return UDR0; //returns uint8_t data type in buffer
}

void LED_Init()
{
	DDRH |= (1 << DDH5) | (1 << PH6); //setting GREEN_LED to output (PH5); setting RED_LED to output (PH6)
	
	PORTH |= (1 << PH5) | (1 << PH6); //initializing GREEN_LED off (PH5); initializing RED_LED off (PH6)
}

void LED_Toggle()
{
	PORTH ^= (1 << PH5) | (1 << PH6); //toggle GREEN_LED (PH5); toggle RED_LED (PH6)
}

void LED_Red_Toggle()
{
	PORTH ^= (1 << PH6); //toggle RED_LED (PH6)
}

void LED_Green_Toggle()
{
	PORTH ^= (1 << PH5); //toggle GREEN_LED (PH5)
}

void VOLTAGE_Translator_Init()
{
	//For ~EN_VTRAN_01 (SPI, data lines) must be set to logic-high for three-state output mode for normal operations
	DDRC |= (1 << PC6); //setting pin as output
	PORTC |= (1 << PC6); //setting pin to logic high
	
	//For EN_VTRAN_03 (SPI, ~CS_FLASH-0#) must be set logic-high for normal operations
	DDRG |= (1 << PG0); //setting pin as output
	PORTG |= (1 << PG0); //setting pin to logic high
		
	//For EN_VTRAN_02 (SPI, ~CS_SD and ~CS_ADS_0#) must be set logic-high for normal operations
	DDRG |= (1 << PG1); //setting pin as output
	PORTG |= (1 << PG1); //setting pin to logic high
}

void SLAVE_CS_Deselect() //PULL ALL ~CS TO HIGH because SPI bus is shared
{
	//~CS, Chip Select (~CS_FLASH-01), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PA3); //setting ~CS pin (PA3) as output;
	PORTA |= (1 << PA3); //setting ~CS pin (PA3) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_FLASH-02), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PA4); //setting ~CS pin (PA4) as output;
	PORTA |= (1 << PA4); //setting ~CS pin (PA4) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_FLASH-03), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PA5); //setting ~CS pin (PA5) as output;
	PORTA |= (1 << PA5); //setting ~CS pin (PA5) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_FLASH-04), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PA6); //setting ~CS pin (PA6) as output;
	PORTA |= (1 << PA6); //setting ~CS pin (PA6) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_SD), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PA7); //setting ~CS pin (PA7) as output;
	PORTA |= (1 << PA7); //setting ~CS pin (PA7) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_ADS_01), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PF1); //setting ~CS pin (PF1) as output;
	PORTA |= (1 << PF1); //setting ~CS pin (PF1) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_ADS_02), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PF2); //setting ~CS pin (PF2) as output;
	PORTA |= (1 << PF2); //setting ~CS pin (PF2) high to de-select the slave device (we are not working on it yet)
	
	//~CS, Chip Select (~CS_ADS_03), must be driven low (and settled as output) to set selected slave device to active power mode
	DDRA |= (1 << PF3); //setting ~CS pin (PF3) as output;
	PORTA |= (1 << PF3); //setting ~CS pin (PF3) high to de-select the slave device (we are not working on it yet)
}

void FLASH_NAND_CS_ENABLE() //enable the Chip Select of the FLASH NAND
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
}

void FLASH_NAND_CS_DISABLE() //disable the Chip Select of the FLASH NAND
{
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
}

#endif