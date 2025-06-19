/*
 * main.c
 *
 * Created: 7/8/2024 9:42:56 AM
 *  Author: agarciasampedroperez
 */ 

////This is for V2.2 Boards/////

//Testing the push method of github

#include "main.h" //has everything...

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
