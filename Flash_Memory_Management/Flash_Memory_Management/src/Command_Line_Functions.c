/*
 * Command_Line_Functions.c
 *
 * Created: 8/6/2024 12:18:52 PM
 * Author: agarciasampedroperez
 * Description: This is where all the command from user go to execute corresponding commands
 */ 

#include "../main.h"

#ifdef Command_Line_Functions

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
	
	//CommandBuffer[arr_address + 1] = '\r';
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

void COLUMN_BLOCK_PAGE_ADDRESSER(bool addr_type) //gets hex number for Column and Block/Page address
{
	UserInput(false); //get the hex numbers, false because not command
	
	if (addr_type == false) //false, so address type is Column Address
	{
		if (input_counter != 4) //has to be 4 inputs
		{
			USART_Data("Make sure only 4 inputs. \n");
			UserInput(false);
		}
		
		if (HEX_Verification()) //if true (valid)
		{
			for (int i = 0; i < COLUMN_ADDRESS + 1; i+=2)
			{
				Byte_Address[i / 2] = (CommandBuffer[i] << 4) | CommandBuffer[i + 1];
			}
			
			Print_To_User(COLUMN_ADDRESS, 0, "Column Address -> 0x%02X \n", Byte_Address, status_feature);	
		}
		else
		{
			COLUMN_BLOCK_PAGE_ADDRESSER(addr_type); //call again
		}
	}
	else //true, so address type is Block/Page Address
	{
		if (input_counter != 6) //has to be 6 inputs
		{
			USART_Data("Make sure only 6 inputs. \n");
			UserInput(false);
		}
		
		if(HEX_Verification()) //if true (valid)
		{
			for (int i = 0; i < BLOCK_PAGE_ADDRESS + 2; i+=2)
			{
				Byte_Address[COLUMN_ADDRESS + (i / 2)] = (CommandBuffer[i] << 4) | CommandBuffer[i + 1];
			}
			
			Print_To_User(BLOCK_PAGE_ADDRESS, COLUMN_ADDRESS, "Block/Page Address -> 0x%02X \n", Byte_Address, status_feature);
		}
		else
		{
			COLUMN_BLOCK_PAGE_ADDRESSER(addr_type); //call again
		}
	}
}

bool HEX_Verification() //verifies uint8_t (ASCII) to Hex validity and allocates in appropriated location
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
			return false; //return false, indicating failed test
		}
	}
	return true; //return true, indicating passed test
	//Print_To_User(6, 0, "Parsed nibble: 0x%02X\n", CommandBuffer, status_feature); //troubleshooting, verifies if ASCII is converted to hex values
}

void NAND_Address_Checker(bool choice) //returns user the Column and Block/Page Addresses
{
	if(choice) //true so write new address
	{	
		USART_Data("---Column Address--- 2 bytes \n");
		COLUMN_BLOCK_PAGE_ADDRESSER(false);

		USART_Data("---Block/Page Address--- 3 bytes \n");
		COLUMN_BLOCK_PAGE_ADDRESSER(true);
	}
	else //false so read current address
	{
		USART_Data("---Column Address--- 2 bytes \n");
		Print_To_User(COLUMN_ADDRESS, 0, "0x%02X \n", Byte_Address, status_feature);

		USART_Data("---Block/Page Address--- 3 bytes \n");
		Print_To_User(BLOCK_PAGE_ADDRESS, COLUMN_ADDRESS, "0x%02X \n", Byte_Address, status_feature);	
	}
}

void ExecuteCommand(const uint8_t *str) //Execute Command Line function
{
	if (strcmp(str, "Flash ID") == 0) //Flash ID Command
	{
		CLEAR_ARR();
		FLASH_ID(); //reads Device ID
	}
	
	else if (strcmp(str, "NAND Address") == 0) //Tells user what is Column and Block/Page Addresses
	{
		CLEAR_ARR();
		
		USART_Data("1) Read \n");
		USART_Data("2) Write \n");
		
		UserInput(false);
		
		if (strcmp(CommandBuffer, "Read") == 0)
		{
			NAND_Address_Checker(false); //read current NAND addresses
			ExecuteCommand("NAND Addresses");
		}
		else if (strcmp(CommandBuffer, "Write") == 0)
		{
			NAND_Address_Checker(true); //write new NAND addresses
			ExecuteCommand("NAND Addresses");
		}
	}
	
	else if (strcmp(str, "Parameter Page") == 0) //Basic Read Test Command
	{
		s = 1; //sets main array address to the correct place
		CLEAR_ARR();
		FLASH_Para_Pg();
		
		//reading data from Data array
		//Print_To_User(PARAMETER_PAGE_SIZE, 0, "%i->Data Received: 0x%02X \n", data, status_feature);
		
		for (int i = 0; i < sizeof(data); i++)	//PARAMETER_PAGE_SIZE; i++) //address is incremented automatically after each byte is shifted out
		{
			sprintf(status_feature, "%i->Data Received: 0x%02X \n", i, data[i]); //hex data to string
			USART_Data(status_feature);
		}	
	}
	
	else if (strcmp(str, "Feature Registers") == 0) //leads user to the Feature registers of the device
	{
		CLEAR_ARR();
		
		USART_Data("1) Block Lock \n");
		USART_Data("2) Configuration \n");
		USART_Data("3) Status \n");
		USART_Data("4) Die Select \n");
		
		UserInput(false);
		
		if (strcmp(CommandBuffer, "Block Lock") == 0) //goes to Block Lock Register
		{
			CLEAR_ARR();
			
			USART_Data("1) Lock \n");
			USART_Data("2) Unlock \n");
			USART_Data("3) Read \n");
			USART_Data("4) Configure \n");
			
			UserInput(false); //User Input is added into an array, which will be written to memory
			
			if (strcmp(CommandBuffer, "Lock") == 0) //Locks Flash Device
			{
				FLASH_Block_Lock_Setter(true);
				ExecuteCommand("Feature Registers");
			}
			else if (strcmp(CommandBuffer, "Unlock") == 0) //Unlocks Flash Device
			{
				FLASH_Block_Lock_Setter(false);
				ExecuteCommand("Feature Registers");
			}
			else if (strcmp(CommandBuffer, "Read") == 0) //Read Block Lock Register
			{
				FLASH_Block_Lock(false);
				ExecuteCommand("Feature Registers");
			}
			else if (strcmp(CommandBuffer, "Configure") == 0) //Configure Block Lock Register
			{
				FLASH_Block_Lock(true);
				ExecuteCommand("Feature Registers");
			}
			else 
			{
				ExecuteCommand("Feature Registers");
			}
		}
		
		else if (strcmp(CommandBuffer, "Configuration") == 0) //goes to the Configuration Register
		{
			CLEAR_ARR();
			FLASH_Configuration(); //get Configuration Register Data
			ExecuteCommand("Feature Registers");
		}
		
		else if (strcmp(CommandBuffer, "Status") == 0) //goes to the Status Register
		{
			CLEAR_ARR();
			FLASH_Status(); //reads status register
			
			ExecuteCommand("Feature Registers");
		}
		
		else if (strcmp(CommandBuffer, "Die Select") == 0) //goes to the Die Register
		{
			CLEAR_ARR();
			FLASH_Die_Selection(); //at the moment only reads to this
			ExecuteCommand("Feature Registers");
		}
	}
	
	else if (strcmp(str, "Test Methods") == 0) //this holds test methods of the device
	{
		CLEAR_ARR();
		
		USART_Data("1) Write Test \n");
		USART_Data("2) Read Test \n");
		USART_Data("3) Erase Test \n");
		USART_Data("4) Block Checker \n");
		USART_Data("5) Page Checker \n");
		USART_Data("6) NAND Address \n");
		
		UserInput(false);
		
		if (strcmp(CommandBuffer, "Write Test") == 0) //write to memory test
		{
			s = 0;
			CLEAR_ARR();
				
			Print_To_User(FLASH_NAND_ADDRESS_MAX, 0, "Here is our NAND Address: 0x%02X \n", Byte_Address, status_feature);
			
			UserInput(false);
			
			FLASH_Page_Program(); //adds user input into Flash Write Function, to memory...
			
			ExecuteCommand("Test Methods");
		}
		
		else if (strcmp(CommandBuffer, "Read Test") == 0) //read from memory test
		{
			s = 0; //sets main array to normal operations
			CLEAR_ARR();
			FLASH_Page_Read();

			USART_Data(data);
			USART_TX_Data('\n');
			
			ExecuteCommand("Test Methods");
		}
		
		else if (strcmp(CommandBuffer, "Erase Test") == 0) //erases by blocks
		{
			s = 0; //sets main array address to normal operation
			CLEAR_ARR();
			FLASH_Block_Erase(); //Erases Flash Block
			
			ExecuteCommand("Test Methods");
		}
		
		//this might stick as part of the main options, but its mostly meant for debugging bad blocks...
		else if (strcmp(CommandBuffer, "Block Checker") == 0) //verifies availability of the block
		{
			s = 0;
			CLEAR_ARR();
			FLASH_Page_Read();

			if (Device_ID[0] != 0xFF)
			{
				USART_Data("Block is marked bad\n");
			}
			else
			{
				USART_Data("Block appears clean\n");
			}
			
			ExecuteCommand("Test Methods");
		}
		
		//meant for debugging bad pages in block...
		else if (strcmp(str, "Page Checker") == 0) //determines if page is good or bad...
		{
			s = 0;
			Page_Tester();
			
			ExecuteCommand("Test Methods");
		}
		
		else if (strcmp(str, "NAND Address") == 0) //Tells user what is Column and Block/Page Addresses
		{
			CLEAR_ARR();
			
			USART_Data("1) Read \n");
			USART_Data("2) Write \n");
			
			UserInput(false);
			
			if (strcmp(CommandBuffer, "Read") == 0)
			{
				NAND_Address_Checker(false); //read current NAND addresses
				ExecuteCommand("Test Methods");
			}
			else if (strcmp(CommandBuffer, "Write") == 0)
			{
				NAND_Address_Checker(true); //write new NAND addresses
				ExecuteCommand("Test Methods");
			}
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
		USART_Data("2) NAND Address \n");
		USART_Data("3) Parameter Page \n");
		USART_Data("4) Feature Registers \n");
		USART_Data("5) Test Methods \n");
		USART_Data("6) Reset \n");
	}
}

#endif