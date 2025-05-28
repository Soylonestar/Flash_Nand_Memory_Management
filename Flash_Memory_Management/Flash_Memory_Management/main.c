/*
 * main.c
 *
 * Created: 7/8/2024 9:42:56 AM
 *  Author: agarciasampedroperez
 */ 

////This is for V2.2 Boards/////

//Testing the push method of github

////But for testing purpose, USART will be set for V2.1 boards///////////

#include "include/Atmega_2560_Definitions_Includes.h"
#include "include/SPI_NAND_Commands.h" //adding Flash_NAND commands
#include "include/AWS_Board_Operations.h" //adding V2.2 operations
#include "include/FLASH_NAND_MEMORY.h" //adding FLASH_NAND memory operations
#include "include/Command_Line_Functions.h" //adding Command Lines Header File...

//#include "MEGA_1284P_SPI.h" //adding MEGA_1284P SPI Testing code

int main(void)
{
	LED_Init();
	VOLTAGE_Translator_Init();
	USART_Init();
	SLAVE_CS_Deselect();
	SPI_MASTER_Init();
	LED_Green_Toggle();
	//PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
    while(1)
    {		
		CLEAR_ARR();
		USART_Data("->Arrays Cleared \n");
		
		UserInput(true); //true therefore command
		
		LED_Red_Toggle();
		_delay_ms(100);
		
	}
}
