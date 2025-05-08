/*
 * FLASH_NAND_MEMORY.c
 *
 * Created: 8/6/2024 11:32:51 AM
 *  Author: agarciasampedroperez
 */ 

#include "../include/Atmega_2560_Definitions_Includes.h"
#include "../include/FLASH_NAND_MEMORY.h"
#include "../include/SPI_NAND_Commands.h"
#include "../include/AWS_Board_Operations.h"
#include "../include/Command_Line_Functions.h"

uint8_t status; //read Data register to clear SPIF flag (uselessbyte only)

char write_test[] = "TX Received \n";
char write_test1[] = "TX 1 \n";
char write_test2[] = "TX 2 \n";
char write_test3[] = "TX 3 \n";
char write_test4[] = "TX 4 \n";
char write_test5[] = "TX 5 \n";
char write_test6[] = "TX 6 \n";

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
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = WRITE_ENABLED; //Flash NAND's Write Enable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete; waits for SPIF to be logic high
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	_delay_us(5); //change delay if it doesn't work....
	
	FLASH_Status();

	//sprintf(buffer, "Write Enable: Status (0x%02X) \n", HEX_ID[0]); //hex data to string
	USART_Data("Write Enable \n");
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Write_Disable(void) //de-select Slave device and disable Write operations onto Flash NAND
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = WRITE_DISABLED; //Flash NAND's Write Disable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	_delay_us(5); //change delay if it doesn't work....
	
	FLASH_Status();
	
	//sprintf(status_feature, "Write Disable: Status (0x%02X) \n", HEX_ID[0]); //hex data to string
	USART_Data("Write Disable \n");
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Write_Data() //const char *str) //writes String data via the addressed individual characters into Flash_NAND (SPI protocol)
{
	FLASH_Write_Enable();
	
	//code below sends Program_Load command with the starting address of the cache register
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = PROGRAM_LOAD; //Sending Program_Load Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Cache_Address(s); //determines where to write data to in Cache
	
	for (int j = 0; j < strlen(CommandBuffer); j++) //each loop, the cache address pointer is incremented after each byte is shifted out...
	{
		SPDR = CommandBuffer[j]; //write char data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	/*		
	for (int i = 0; i < strlen(write_test1); i++) //each loop, the cache address pointer is incremented (I think...)
	{
		SPDR = write_test1[i]; //write char data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		//HEX_ID[i] = SPDR; //read Data Register to clear the SPIF flag
		
		//USART_TX_Data(write_test[i]);
	}
	*/
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	//sprintf(status_feature, "Program Load: Status (0x%02X) High-Byte (0x%02X) Low-Byte (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2]); //hex data to string
	USART_Data("Program Load \n");
	
	_delay_us(5); //change delay if it doesn't work....

	FLASH_Transfer_Cache();	
}

void FLASH_Transfer_Cache(void) //new command to transfer data from cache to main array
{	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	//code below sends Program_Execute command with the starting address of the main array
	SPDR = PROGRAM_EXECUTE; //Sending Program Execute Command into SPI Data Register (SPDR)
	while (!(SPSR & (1 << SPIF))); 
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s); //determines where to write data to in MainArray
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	//sprintf(status_feature, "Program Execute: Status (0x%02X) High-Byte (0x%02X) Mid-Byte (0x%02X) Low-Byte (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2],  HEX_ID[3]); //hex data to string
	USART_Data("Program Execute \n");
	
	_delay_us(5); //change delay if it doesn't work....
	
	FLASH_Status(); //checks for the status of data transfer from cache to main array from program_execute command

	FLASH_Write_Disable();
}

void FLASH_Read() //reads data from FLASH_NAND Memory chip (SPI protocol)
{
	FLASH_Transfer_MainArray();
	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = READ_FROM_CACHE; //Sending Read_From_Cache Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Cache_Address(s); //determines where to read data from in Cache
	
	SPDR = 0x00; //a dummy byte sent
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Data_Storage(s); //determines how to store data in an array
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	//sprintf(status_feature, "Read From Cache: Status (0x%02X) High-Byte (0x%02X) Low-Byte (0x%02X) Dummy (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2], HEX_ID[3]); //hex data to string
	USART_Data("Read from Cache: \n");
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Transfer_MainArray(void) //new command to transfer data from main array to cache
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = PAGE_READ; //Sending Page_Read Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s); //determines where to read data to in MainArray
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	_delay_us(5); //change delay if it doesn't work....
	
	FLASH_Status(); //checks for the status of data transfer from main array to cache from page_read command
	
	//sprintf(status_feature, "Page Read: Status (0x%02X) High-Byte (0x%02X) Mid-Byte (0x%02X) Low-Byte (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2],  HEX_ID[3]); //hex data to string
	USART_Data("Page Read \n");
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Erase() //erases data from Flash Nand Memory at the block level
{
	FLASH_Write_Enable();
	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....	
	
	SPDR = BLOCK_ERASE; //Sending Block_Erase Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s); //determines where to write/read data to in MainArray

	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	//sprintf(status_feature, "Block Erase: Status (0x%02X) High-Byte (0x%02X) Mid-Byte (0x%02X) Low-Byte (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2], HEX_ID[3]); //hex data to string
	USART_Data("Block Erase \n");
	
	_delay_us(5); //change delay if it doesn't work....
	
	FLASH_Status(); //checks for the status of data deletion on the block level from Block_Erase command
	
	FLASH_Write_Disable();
}

void FLASH_ID() //read device ID
{	
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
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
	HEX_ID [1] = SPDR; //clears the SPIF flag; returns 4Gb 3.3V Device ID
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	sprintf(status_feature, "Read Device ID: Micron ID (0x%02X) Device ID (0x%02X) \n", HEX_ID[0], HEX_ID[1]); //hex data to string
	USART_Data(status_feature);
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Reset() //reset memory device
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = RESET; //Sending READ_ID Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [0] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence

	_delay_us(5); //change delay if it doesn't work....
	
	USART_Data("Device Reseted \n");
}

void FLASH_Para_Pg() //reads from the FLASH NAND parameter page
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [0] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte

	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [1] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte

	SPDR = PARAMETER_PAGE_INIT; //Sending data byte, setting CFG1 = 1 (access Parameter page), into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [1] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	_delay_us(5); //change delay if it doesn't work....
	
	//THIS is a PAGE READ FUNCTION....
	FLASH_Read(); //changed plane bit and low byte from 24-bit address

	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [0] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte

	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [1] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte

	SPDR = PARAMETER_PAGE_EXIT; //Sending data byte to exit Parameter page into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	//HEX_ID [1] = SPDR; //no need to read because the while loop's purpose is to make sure the SPIF flag clears; useless byte
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_MainArray_Address(int s) //24-bit address
{
	if (s == 0) //normal memory operation
	{
		SPDR = 0x00; //the high-byte of the 24-bit address
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
				
		SPDR = 0x00; //the mid-byte of the 24-bit address
		while (!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x00; //the low-byte of the 24-bit address; block/page address is 0x00
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

void FLASH_Cache_Address(int s) //16-bit address (actual address size is 12-bits)
{
	if (s == 0) //normal  memory operations (sets plane bit to 1)
	{
		SPDR = 0x10; //the high-byte of the 16-bit address; 0x10 sets the plane select bit to 1
		while(!(SPSR & (1 << SPIF)));
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte

		SPDR = 0x00; //the low-byte of the 16-bit address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	if (s == 1) //parameter page requires this for cache (may change)
	{
		SPDR = 0x00; //the high-byte of the 16-bit address; setting plane bit to 0x00 because of parameter pg
		while(!(SPSR & (1 << SPIF)));
		HEX_ID [0]= SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte

		SPDR = 0x00; //the low-byte of the 16-bit address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		HEX_ID [0]= SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
}

void FLASH_Status() //this makes sure that data finishes transferring
{
	PORTA &= ~(1 << PA3); //~CS pin set low for selecting slave device
	
	_delay_us(5); //change delay if it doesn't work....
	
	SPDR = GET_FEATURES; //sending Get Features command into the SPI Data Register (SPDR)
	while (!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = STATUS_REGISTER_ADDRESS; //status register address for the feature operation
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	for (int i = 0; i < 2; i++ )
	{
		SPDR = 0x00; //sending dummy byte to continue to get status
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		HEX_ID [i] = SPDR; //read incoming status data and puts it into an array
	}
	
	PORTA |= (1 << PA3); //~CS pin set high for de-selecting slave device; to end the command sequence
	
	//sprintf(status_feature, "Get Features: Status (0x%02X) Status Register (0x%02X) Status Cleared (0x%02X) \n", HEX_ID[0], HEX_ID[1], HEX_ID[2]); //hex data to string
	sprintf(status_feature, "Get Features: Status Cleared (0x%02X), (0x%02X)\n", HEX_ID[0], HEX_ID[1]); //hex data to string
	USART_Data(status_feature);
	
	_delay_us(5); //change delay if it doesn't work....
}

void FLASH_Data_Storage(int s) //determines how to store FLASH NAND data in an array
{
	if (s == 0) //normal method for strings
	{
		//reading and writing char data type into data[]
		for (int i = 0; i < sizeof(data); i++) //address is incremented automatically after each byte is shifted out
		{
			SPDR = 0x00; //a dummy byte, ignored by the Flash NAND...to get data back
			while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
			data[i] = SPDR; //read Data Register and adds it to data array
		
			if (SPDR == '\r') //|| SPDR == 'ÿ') //ÿ is 0xFF in character data type; adding carriage return '\r' until I can get NULL to work... 
			{
				USART_Data("Data Received \n");
				break;
			}
		}
	}
	
	if (s == 1) //parameter page requires this
	{
		//reading and writing char data type into data[]
		for (int i = 0; i < sizeof(data); i++) //address is incremented automatically after each byte is shifted out
		{
			SPDR = 0x00; //a dummy byte, ignored by the Flash NAND...to get data back
			while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
			data[i] = SPDR; //read Data Register and adds it to data array
		}
	}
}