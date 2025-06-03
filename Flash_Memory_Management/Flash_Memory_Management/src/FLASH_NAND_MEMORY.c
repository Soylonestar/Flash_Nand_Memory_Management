/*
 * FLASH_NAND_MEMORY.c
 *
 * Created: 8/6/2024 11:32:51 AM
 * Author: agarciasampedroperez
 * Description: Flash NAND Driver 
 *				Has most of the needed operations to operate a Flash NAND
 *				Write, Read, Initialize, etc.
 */ 

#include "../include/Atmega_2560_Definitions_Includes.h"
#include "../include/FLASH_NAND_MEMORY.h"
#include "../include/SPI_NAND_Commands.h"
#include "../include/AWS_Board_Operations.h"
#include "../include/Command_Line_Functions.h"
#include "../include/NAND_Flash_Formatter.h"

uint8_t status; //read Data register to clear SPIF flag (uselessbyte only)

uint8_t write_test[] = "TX Received \n";
uint8_t write_test1[] = "TX 1 \n";
uint8_t write_test2[] = "TX 2 \n";
uint8_t write_test3[] = "TX 3 \n";
uint8_t write_test4[] = "TX 4 \n";
uint8_t write_test5[] = "TX 5 \n";
uint8_t write_test6[] = "TX 6 \n";

/*
Order on how the write/read the Flash NAND Memory chip
-Command
-Address
-Data
*/

void CLEAR_ARR(void) //to clear all relevant arrays with null
{
	for(int i = 0; i < sizeof(HEX_ID); i++)
	{
		HEX_ID[i] = '\0';
	}
	
	for(int i = 0; i < sizeof(data); i++)
	{
		data[i] = '\0';
	}
	
	for (int i = 0; i < sizeof(CommandBuffer); i++)
	{
		CommandBuffer[i] = '\0';
	}
	
	/*for (int i = 0; i < sizeof(Byte_Address); i++)
	{
		Byte_Address[i] = '\0';
	}*/
	
	for (int i = 0; i < sizeof(status_feature); i++)
	{
		status_feature[i] = '\0';
	}
}

//Master Mode (for ATmega 2560) allows full duplex communication between the ATmega 2560 to Flash NAND Memory chip
void SPI_MASTER_Init() //initializes Serial Peripheral Interference (SPI) operations in Master Mode
{
	//DDR pin assignment, ~CS, MOSI, SCK are outputs (1), and MISO is input (0) (in Master Mode)
	DDRB |= (1 << PB2) | (1 << PB1); //setting MASTER_MOSI (PB2) as output; setting MASTER_SCK (PB1) as output;
	DDRB &= ~(1 << PB3); //setting MASTER_MISO (PB3) as input;
	
	//MUST set ~SS on the ATmega 2560 in order to get SPI operations working...
	DDRB |= (1 << PB0); //setting ~SS pin (PB0) as output;
	PORTB |= (1 << PB0); //setting ~SS pin (PB0) high to "de-select" slave device (not connected to the Flash NAND anyway)
	//PORTB &= ~(1 << PB0); //setting ~SS pin (PB0) low to "select" slave device (not connected to the Flash NAND anyway)
	
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0); // SPE: enables the SPI; MSTR: sets SPI to Master SPI mode; SPR1 and SPR0: sets the SCK Frequency (divisor of 16);
	SPCR &= ~(1 << DORD) | ~(1 << CPOL) | ~(1 << CPHA) | ~(1 << SPR1);//DORD, Data order set to MSB transmitted first; SPI Timing Mode is 0, where CPOL and CPHA are both 0
}

void FLASH_Write_Enable (void) //select Slave device and initializes Write operations onto Flash NAND
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
		
	SPDR = WRITE_ENABLED; //Flash NAND's Write Enable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete; waits for SPIF to be logic high
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	USART_Data("Write Enable \n");
}

void FLASH_Write_Disable(void) //de-select Slave device and disable Write operations onto Flash NAND
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = WRITE_DISABLED; //Flash NAND's Write Disable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	USART_Data("Write Disable \n");
}

void FLASH_Page_Program() //Write into addressed pages in Flash NAND
{
	//FLASH_Block_Erase(); //apparently I need to do a block erase of the block I'm writing to...
	
	FLASH_Write_Enable();
	
	//code below sends Program_Load command with the starting address of the cache register
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = PROGRAM_LOAD; //Sending Program_Load Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Column_Address(s, Byte_Address); //determines where to write data to in Cache
	
	for (int j = 0; j < strlen(CommandBuffer); j++) //each loop, the cache address pointer is incremented after each byte is shifted out...
	{
		SPDR = CommandBuffer[j]; //write uint8_t data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
		
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	USART_Data("Program Load \n");

	FLASH_Program_Execute();	
}

void FLASH_Random_Data_Program_x1() //can change parts of the cache buffer, based on the specified location from user
{
	FLASH_Write_Enable();
	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = PROGRAM_LOAD_RANDOM_DATA; //Sending PROGRAM_LOAD_RANDOM_DATA Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Column_Address(s, Byte_Address); //determines where to write data to in Cache
	
	for (int j = 0; j < strlen(CommandBuffer); j++) //each loop, the cache address pointer is incremented after each byte is shifted out...
	{
		SPDR = CommandBuffer[j]; //write uint8_t data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	USART_Data("Program Load \n");

	FLASH_Program_Execute();
}

void FLASH_Program_Execute() //new command to transfer data from cache to main array
{	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	//code below sends Program_Execute command with the starting address of the main array
	SPDR = PROGRAM_EXECUTE; //Sending Program Execute Command into SPI Data Register (SPDR)
	while (!(SPSR & (1 << SPIF))); 
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s, Byte_Address); //determines where to write data to in MainArray
	
	_delay_us(5); //change delay if it doesn't work....
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	USART_Data("Program Execute \n");
	
	FLASH_Status(); //checks for the status of data transfer from cache to main array from program_execute command

	FLASH_Write_Disable();
}

void FLASH_Page_Read() //reads data from Block/Page array to transfer to Cache register
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = PAGE_READ; //Sending Page_Read Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s, Byte_Address); //determines where to read data from in MainArray
	
	_delay_us(5);
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	FLASH_Status(); //checks for the status of data transfer from main array to cache from page_read command
	
	USART_Data("Page Read \n");
	
	FLASH_Read_From_Cache_x1();
}

void FLASH_Read_From_Cache_x1() //read data out of the cache register
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = READ_FROM_CACHE; //Sending Read_From_Cache Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Column_Address(s, Byte_Address); //determines where to read data from the Cache Register
	
	SPDR = 0x00; //a dummy byte sent
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Data_Storage(s); //determines how to store data in an array
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	USART_Data("Read from Cache: \n");
}

void FLASH_Block_Erase() //erases data from Flash Nand Memory at the block level
{
	FLASH_Write_Enable();
	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = BLOCK_ERASE; //Sending Block_Erase Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	//FLASH_MainArray_Address(s, array); //determines where to write/read data to in MainArray

	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	USART_Data("Block Erase \n");
	
	FLASH_Status(); //checks for the status of data deletion on the block level from Block_Erase command
	
	FLASH_Write_Disable();
}

void FLASH_ID() //read device ID
{	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = READ_ID; //Sending READ_ID Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	HEX_ID [0] = SPDR; //clears the SPIF flag; returns Micron ID
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	HEX_ID [1] = SPDR; //clears the SPIF flag; returns 2Gb 3.3V Device ID
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	sprintf(status_feature, "Read Device ID: Micron ID (0x%02X) Device ID (0x%02X) \n", HEX_ID[0], HEX_ID[1]); //hex data to string
	USART_Data(status_feature);
}

void FLASH_Reset() //reset memory device
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = RESET; //Sending READ_ID Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	USART_Data("Device Reseted \n");
}

void FLASH_Para_Pg() //reads from the FLASH NAND parameter page
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = PARAMETER_PAGE_INIT; //Sending data byte, setting CFG1 = 1 (access Parameter page), into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	//THIS is a PAGE READ FUNCTION....
	FLASH_Page_Read(); //change plane bit and low byte from 24-bit address

	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = PARAMETER_PAGE_EXIT; //Sending data byte to exit Parameter page into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
}

void FLASH_MainArray_Address(int s, const uint8_t *array) //24-bit address 4,2Gbs (7 dummy bits, [16:6] Blocks / [5:0] Pages)
{
	if (s == 0) //normal memory operation
	{
		SPDR = array[2]; //the high-byte of the 24-bit address
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
				
		SPDR = array[3]; //the mid-byte of the 24-bit address (block 15, page 0)
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = array[4]; //the low-byte of the 24-bit address; block/page address is 0x00
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	if (s == 1) //access to parameter page
	{
		SPDR = 0x00; //the high-byte of the 24-bit address
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x00; //the mid-byte of the 24-bit address
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x01; //the low-byte of the 24-bit address; block/page address is 0x01 for Parameter pg.
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
}

/*Wasn't complete correct, this is column address to show where to put the data to in memory...*/
void FLASH_Column_Address(int s, const uint8_t *array) //16-bit address (actual address size is 12-bits)
{
	/*Will change to switch-case if necessary*/
	if (s == 0) //normal memory operations (sets plane bit to 1 for that address; odd plane)
	{
		SPDR = array[0]; //the high-byte of the 16-bit address; 0x10 sets the plane select bit to 1
		while(!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte

		SPDR = array[1]; //the low-byte of the 16-bit address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	if (s == 1) //this is where the parameter page is located, plane bit 0 (even)
	{
		SPDR = 0x00; //the high-byte of the 16-bit address; setting plane bit to 0x00 because of parameter pg
		while(!(SPSR & (1 << SPIF)));
		HEX_ID [0]= SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte

		SPDR = 0x00; //the low-byte of the 16-bit address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		HEX_ID [1]= SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
}

void FLASH_Data_Storage(int s) //determines how to store FLASH NAND data in an array
{
	if (s == 0) //normal method for strings
	{
		//reading and writing uint8_t data type into data[]
		for (int i = 0; i < sizeof(data); i++) //address is incremented automatically after each byte is shifted out
		{
			SPDR = 0x00; //a dummy byte, ignored by the Flash NAND...to get data back
			while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
			data[i] = SPDR; //read Data Register and adds it to data array
		
			if (SPDR == '\r') //|| SPDR == 'ÿ') //ÿ is 0xFF in uint8_t data type; adding carriage return '\r' until I can get NULL to work... 
			{
				USART_Data("Data Received \n");
				break;
			}
		}
	}
	
	if (s == 1) //parameter page requires this
	{
		//reading and writing uint8_t data type into data[]
		for (int i = 0; i < sizeof(data); i++) //address is incremented automatically after each byte is shifted out
		{
			SPDR = 0x00; //a dummy byte, ignored by the Flash NAND...to get data back
			while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
			data[i] = SPDR; //read Data Register and adds it to data array
		}
	}
}

void FLASH_Status() //this makes sure that data finishes transferring
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	SPDR = GET_FEATURES; //sending Get Features command into the SPI Data Register (SPDR)
	while (!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = STATUS_REGISTER_ADDRESS; //status register address for the feature operation
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	//Waits for OIP bit to clear (set to 0)
	do
	{
		SPDR = 0x00; //sending dummy byte to continue to get status
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		HEX_ID [0] = SPDR; //read incoming status data and puts it into an array
		sprintf(status_feature, "Get Features: Status (0x%02X)\n", HEX_ID[0]); //hex data to string
		USART_Data(status_feature);
	} while (HEX_ID[0] & 0x01);
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
}