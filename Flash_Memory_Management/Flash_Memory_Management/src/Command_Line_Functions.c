/*
 * Command_Line_Functions.c
 *
 * Created: 8/6/2024 12:18:52 PM
 * Author: agarciasampedroperez
 * Description: This is where all the command from user go to execute corresponding commands
 */ 

#include "../include/Atmega_2560_Definitions_Includes.h"
#include "../include/Command_Line_Functions.h"
#include "../include/FLASH_NAND_MEMORY.h"
#include "../include/AWS_Board_Operations.h"

int arr_address = 0; //for CommandBuffer array

void CommandLine()
{	
	USART_Data("User Input:\n"); //asking for User Input
	
	c = USART_RX_Data(); //receive user char data type input
	while ((c != '\r') && (c!= '\n'))
	{
		CommandBuffer[arr_address] = c; //adding character into array at index, creating a string
		if (arr_address >= sizeof(CommandBuffer)) //making this an endless array for command lines
		{
			arr_address = 0; //resets array at address 0
		}
		arr_address++;
		c = USART_RX_Data(); //receive user char data type input
	}
	CommandBuffer[arr_address] = '\0'; //adding NULL '\0' to mark end of Command String...; adding carriage return '\r' until I can get NULL to work...
	arr_address = 0; //resets array at address 0
	
	USART_Data(CommandBuffer); //shows what the user wrote on terminal
	USART_TX_Data('\n');
		
	ExecuteCommand(CommandBuffer); //puts CommandLine into Execute Function...
}

void UserInput() //asking for user input
{
	USART_Data("User Input:\n"); //asking for User Input
	
	c = USART_RX_Data(); //receive user char data type input
	while ((c != '\r')) //(c != '\n') &&
	{
		CommandBuffer[arr_address] = c; //adding character into array at index, creating a string
		if (arr_address >= sizeof(CommandBuffer)) //making this an endless array for command lines
		{
			arr_address = 0; //resets array at address 0
		}
		arr_address++;
		c = USART_RX_Data(); //receive user char data type input
	}
	CommandBuffer[arr_address] = '\r'; //adding NULL '\0' to mark end of Command String...; adding carriage return '\r' until I can get NULL to work...
	arr_address = 0; //resets array at address 0
}

void ExecuteCommand(const char *str) //Execute Command Line function
{
	if (strcmp(str, "Flash ID") == 0) //Flash ID Command
	{
		FLASH_ID(); //reads Device ID
		CLEAR_ARR();
	}
	
	else if (strcmp(str, "Erase Memory") == 0) //Erase Flash Memory Command
	{
		s = 0; //sets main array address to normal operation
		CLEAR_ARR();
		FLASH_Erase(); //Erases Flash Block
	}
	
	else if (strcmp(str, "Write Test") == 0) //Basic Flash Test Command
	{
		s = 0;
		CLEAR_ARR();
		
		UserInput(); //User Input is added into an array, which will be written to memory
		USART_Data(CommandBuffer); //will show what is in the array in question...
		USART_TX_Data('\n');
		
		FLASH_Write_Data(); //adds user input into Flash Write Function, to memory...
		
		//CLEAR_ARR();
		//FLASH_Read();
		
		//USART_Data(data);
	}
	else if (strcmp(str, "Read Memory") == 0) //Basic Read Test Command
	{
		s = 0; //sets main array to normal operations
		CLEAR_ARR();
		FLASH_Read();

		USART_Data(data);
		USART_TX_Data('\n');
	}
	else if (strcmp(str, "Parameter Page") == 0) //Basic Read Test Command
	{
		s = 1; //sets main array address to the correct place
		CLEAR_ARR();
		FLASH_Para_Pg();
		
		//reading data from Data array
		for (int i = 0; i < sizeof(data); i++) //address is incremented automatically after each byte is shifted out
		{
			sprintf(status_feature, "Data Received: (0x%02X) \n", data[i]); //hex data to string
			USART_Data(status_feature);
		}	
	}
	else if (strcmp(str, "Reset") == 0) //Basic Read Test Command
	{
		CLEAR_ARR();
		FLASH_Reset();
	}
	else
	{
		USART_Data("1) Flash ID \n");
		USART_Data("2) Erase Memory \n");
		USART_Data("3) Write Test \n");
		USART_Data("4) Read Memory \n");
		USART_Data("5) Parameter Page \n");
		USART_Data("6) Reset \n");
	}
}
