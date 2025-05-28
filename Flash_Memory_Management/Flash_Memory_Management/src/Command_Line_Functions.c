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
#include "../include/NAND_Flash_Formatter.h"

int arr_address = 0; //for CommandBuffer array
int input_counter = 0; //to count the number of input characters 

void UserInput(bool command) //asking for user input
{
	USART_Data("->User Input:\n"); //asking for User Input
	
	input_counter = 0; //reset input counter
	
	c = USART_RX_Data(); //receive user uint8_t data type input
	while ((c != '\r')) //(c != '\n') &&
	{
		input_counter++; //increment input counter
		CommandBuffer[arr_address] = c; //adding uint8_t into array at index, creating a string
		if (arr_address >= sizeof(CommandBuffer)) //making this an endless array for command lines
		{
			arr_address = 0; //resets array at address 0
		}
		arr_address++;
		c = USART_RX_Data(); //receive user uint8_t data type input
	}
	//CommandBuffer[arr_address] = '\n';
	CommandBuffer[arr_address] = '\0'; //adding NULL '\0' to mark end of Command String...; adding carriage return '\r' until I can get NULL to work...
	arr_address = 0; //resets array at address 0
	
	USART_Data(CommandBuffer); //shows what the user wrote on terminal
	USART_TX_Data('\n');
	
	if (command)
		ExecuteCommand(CommandBuffer); //if command true, pass command to execute function
}

void Print_To_User(int num_elements, int offset, const char *msg, const uint8_t *arr_start, uint8_t *arr_dest) //# of elements to print along with message, and to which array
{
	for (int i = 0; i < num_elements; i++)
	{
		sprintf(arr_dest, msg, arr_start[i + offset]);
		USART_Data(arr_dest);
	}
}

void HEX_Parser(bool addr_type) //gets hex number for Column and Block/Page address
{
	bool temp = addr_type; ///CHANGE!!!!!!!
	
	UserInput(false); //get the hex numbers
	
	if (addr_type == false) //false, so address type is Column Address
	{
		if (input_counter > 4)
		{
			USART_Data("One to many. Make sure only 4 inputs. \n");
			UserInput(false);
		}
		
		HEX_Verification();
		
		for (int i = 0; i < COLUMN_ADDRESS + 1; i+=2)
		{
			Byte_Address[i / 2] = (CommandBuffer[i] << 4) | CommandBuffer[i + 1];
		}
		
		Print_To_User(COLUMN_ADDRESS, 0, "Column Address -> 0x%02X \n", Byte_Address, status_feature);	
	}
	else //true Block/Page Address
	{
		if (input_counter > 6)
		{
			USART_Data("One to many. Make sure only 6 inputs. \n");
			UserInput(false);
		}
		
		HEX_Verification();
		
		for (int i = 0; i < BLOCK_PAGE_ADDRESS + 2; i+=2)
		{
			Byte_Address[COLUMN_ADDRESS + (i / 2)] = (CommandBuffer[i] << 4) | CommandBuffer[i + 1];
		}
		
		Print_To_User(BLOCK_PAGE_ADDRESS, COLUMN_ADDRESS, "Block/Page Address -> 0x%02X \n", Byte_Address, status_feature);
	}
}

void HEX_Verification() //verifies uint8_t (ASCII) to Hex validity and allocates in appropriated location
{
	//parse the data into usable hex values
	for (int i = 0; i <= FLASH_NAND_ADDRESS_MAX; i++)
	{
		if (CommandBuffer[i] >= '0' && CommandBuffer[i] <= '9')
		{
			CommandBuffer[i] = CommandBuffer[i] - '0';
		}
		else if (CommandBuffer[i] >= 'A' && CommandBuffer[i] <= 'F')
		{
			CommandBuffer[i] = CommandBuffer[i] - 'A' + 10;
		}
		else if (CommandBuffer[i] >= 'a' && CommandBuffer[i] <= 'f')
		{
			CommandBuffer[i] = CommandBuffer[i] - 'a' + 10;
		}
		else if (CommandBuffer[i] == '\r' || CommandBuffer[i] == '\n' || CommandBuffer[i] == '\0')
		{
			break;
		}
		else
		{
			USART_Data("Wrong Input \nMake sure input is HEX valid [0 - F] \n");
			Print_To_User(1, 0, "Here is what was inputted: 0x%02X \n", CommandBuffer, status_feature);
			CLEAR_ARR();
			HEX_Parser(false);
		}
	}
	//Print_To_User(6, 0, "Parsed nibble: 0x%02X\n", CommandBuffer, status_feature); //troubleshooting, verifies if ASCII is converted to hex values
}

void NAND_Address_Checker() //returns user the Column and Block/Page Addresses
{
	USART_Data("---Column Address--- 2 bytes \n");
	Print_To_User(COLUMN_ADDRESS, 0, "0x%02X \n", Byte_Address, status_feature);

	USART_Data("---Block/Page Address--- 3 bytes \n");	
	Print_To_User(BLOCK_PAGE_ADDRESS, COLUMN_ADDRESS, "0x%02X \n", Byte_Address, status_feature);
}

void ExecuteCommand(const uint8_t *str) //Execute Command Line function
{
	if (strcmp(str, "Flash ID") == 0) //Flash ID Command
	{
		CLEAR_ARR();
		FLASH_ID(); //reads Device ID
	}
	
	else if (strcmp(str, "Erase Memory") == 0) //Erase Flash Memory Command
	{
		s = 0; //sets main array address to normal operation
		CLEAR_ARR();
		FLASH_Block_Erase(); //Erases Flash Block
	}
	
	else if (strcmp(str, "NAND Addresses") == 0) //Tells user what is Column and Block/Page Addresses
	{
		NAND_Address_Checker();
	}
	
	else if (strcmp(str, "Write Test") == 0) //Basic Flash Test Command
	{
		s = 0;
		CLEAR_ARR();
		
		UserInput(false); //User Input is added into an array, which will be written to memory
		
		USART_Data("---Column Address--- 2 bytes \n");
		HEX_Parser(false);

		USART_Data("---Block/Page Address--- 3 bytes \n");
		HEX_Parser(true);
		
		Print_To_User(FLASH_NAND_ADDRESS_MAX, 0, "Here is what was inputted: 0x%02X \n", Byte_Address, status_feature);
		
		FLASH_Page_Program(); //adds user input into Flash Write Function, to memory...
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
		for (int i = 0; i < PARAMETER_PAGE_SIZE; i++) //address is incremented automatically after each byte is shifted out
		{
			sprintf(status_feature, "%i->Data Received: 0x%02X \n", i, data[i]); //hex data to string
			USART_Data(status_feature);
		}	
	}
	
	else if (strcmp(str, "Flash Status") == 0) //Read Flash NAND status register
	{
		CLEAR_ARR();
		FLASH_Status();
	}
	
	//this might stick as part of the main options, but its mostly meant for debugging bad blocks...
	else if (strcmp(str, "Block Checker") == 0) //Determines if block is good or bad...
	{
		s = 0;
		CLEAR_ARR();
		FLASH_Read();

		if (HEX_ID[0] != 0xFF) 
		{
			USART_Data("Block is marked bad\n");
		} 
		else 
		{
			USART_Data("Block appears clean\n");
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
		USART_Data("6) Flash Status \n");
		USART_Data("7) Block Checker \n");
		USART_Data("8) NAND Addresses \n");
		USART_Data("9) Reset \n");
	}
}
