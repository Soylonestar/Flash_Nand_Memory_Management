/*
 * FLASH_NAND_MEMORY.c
 *
 * Created: 8/6/2024 11:32:51 AM
 * Author: agarciasampedroperez
 * Description: Flash NAND Driver 
 *				Has most of the needed operations to operate a Flash NAND
 *				Write, Read, Initialize, etc.
 */ 

#include "../main.h"

#ifdef FLASH_NAND_MEMORY

uint8_t status; //read Data register to clear SPIF flag (uselessbyte only)

uint8_t write_test[] = "TX Received \nWind Speed: 14.3KPH \n\r";

/*
Order on how the write/read the Flash NAND Memory chip
-Command
-Address
-Data
*/

void CLEAR_ARR(void) //to clear all relevant arrays with null
{
	for(int i = 0; i < sizeof(Device_ID); i++)
	{
		Device_ID[i] = '\0';
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
	FLASH_NAND_CS_ENABLE();
	
	SPDR = WRITE_ENABLED; //Flash NAND's Write Enable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete; waits for SPIF to be logic high
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_NAND_CS_DISABLE();
	
	//USART_Data("Write Enable \n");
}

void FLASH_Write_Disable(void) //de-select Slave device and disable Write operations onto Flash NAND
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = WRITE_DISABLED; //Flash NAND's Write Disable command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_NAND_CS_DISABLE();
	
	//USART_Data("Write Disable \n");
}

void FLASH_Page_Program() //Write into addressed pages in Flash NAND
{
	//FLASH_Block_Erase(); //apparently I need to do a block erase of the block I'm writing to...
	
	FLASH_Write_Enable();
	
	//code below sends Program_Load command with the starting address of the cache register
	FLASH_NAND_CS_ENABLE();
	
	SPDR = PROGRAM_LOAD; //Sending Program_Load Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	_delay_us(5);
	
	FLASH_Column_Address(s, Byte_Address); //determines where to write data to in Cache
	
	for (int j = 0; j < strlen(CommandBuffer); j++) //each loop, the cache address pointer is incremented after each byte is shifted out...
	{
		SPDR = CommandBuffer[j]; //write uint8_t data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	//forcibly adding \r into that data, which sucks but will fix later on....
	SPDR = '\r'; //write uint8_t data type (byte sized) data onto cache register's address
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	/*for (int j = 0; j < strlen(write_test); j++)
	{
		SPDR = write_test[j];
		while(!(SPSR & (1 << SPIF)));
		status = SPDR;
	}*/
		
	FLASH_NAND_CS_DISABLE();
	
	//USART_Data("Page Program Load \n");

	FLASH_Program_Execute();
}

void FLASH_Random_Data_Program_x1() //can change parts of the cache buffer, based on the specified location from user
{
	FLASH_Write_Enable();
	
	FLASH_NAND_CS_ENABLE();
	
	SPDR = PROGRAM_LOAD_RANDOM_DATA; //Sending PROGRAM_LOAD_RANDOM_DATA Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF)));
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	_delay_us(5);
	
	FLASH_Column_Address(s, Byte_Address); //determines where to write data to in Cache
	
	for (int j = 0; j < strlen(CommandBuffer); j++) //each loop, the cache address pointer is incremented after each byte is shifted out...
	{
		SPDR = CommandBuffer[j]; //write uint8_t data type (byte sized) data onto cache register's address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	}
	
	FLASH_NAND_CS_DISABLE();

	//USART_Data("Random Program Load \n");

	FLASH_Program_Execute();
}

void FLASH_Program_Execute() //new command to transfer data from cache to main array
{	
	FLASH_NAND_CS_ENABLE();
	
	//code below sends Program_Execute command with the starting address of the main array
	SPDR = PROGRAM_EXECUTE; //Sending Program Execute Command into SPI Data Register (SPDR)
	while (!(SPSR & (1 << SPIF))); 
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	//_delay_us(5);
	
	FLASH_MainArray_Address(s, Byte_Address); //determines where to write data to in MainArray
	
	FLASH_NAND_CS_DISABLE();
	
	//USART_Data("Program Execute \n");
	
	FLASH_Status(); //checks for the status of data transfer from cache to main array from program_execute command
}

void FLASH_Page_Read() //reads a single page data from Block/Page array to transfer to Cache register
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = PAGE_READ; //Sending Page_Read Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	//_delay_us(5);
	
	FLASH_MainArray_Address(s, Byte_Address); //determines where to read data from in MainArray
	
	_delay_us(0.200);
	
	FLASH_NAND_CS_DISABLE();

	FLASH_Status(); //checks for the status of data transfer from main array to cache from page_read command
	
	//USART_Data("Page Read \n");
	
	FLASH_Read_From_Cache_x1();
}

void FLASH_Read_From_Cache_x1() //read data out of the cache register
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = READ_FROM_CACHE; //Sending Read_From_Cache Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	_delay_us(5);
	
	FLASH_Column_Address(s, Byte_Address); //determines where to read data from the Cache Register
	
	SPDR = 0x00; //a dummy byte sent
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_Data_Storage(s); //determines how to store data in an array
	
	FLASH_NAND_CS_DISABLE();
	
	//USART_Data("Read from Cache: \n");
}

void FLASH_Block_Erase() //erases data from Flash Nand Memory at the block level
{
	FLASH_Write_Enable();
	
	FLASH_NAND_CS_ENABLE();
	
	SPDR = BLOCK_ERASE; //Sending Block_Erase Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	FLASH_MainArray_Address(s, Byte_Address); //determines where to write/read data to in MainArray

	FLASH_NAND_CS_DISABLE();
	
	FLASH_Status(); //checks for the status of data deletion on the block level from Block_Erase command
	
	//USART_Data("Block Erase \n");
}

void FLASH_ID() //read device ID
{	
	FLASH_NAND_CS_ENABLE();
	
	SPDR = READ_ID; //Sending READ_ID Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	Device_ID [0] = SPDR; //clears the SPIF flag; returns Micron ID
	
	SPDR = 0x00; //send dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	Device_ID [1] = SPDR; //clears the SPIF flag; returns 2Gb 3.3V Device ID
	
	FLASH_NAND_CS_DISABLE();

	sprintf(status_feature, "Read Device ID: Micron ID (0x%02X) Device ID (0x%02X) \n", Device_ID[0], Device_ID[1]); //hex data to string
	USART_Data(status_feature);
}

void FLASH_Die_Selection() //determines what die is selected; at the moment I just want to know which die am I write/reading from
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = GET_FEATURES; //sending Get Features to read data
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = DIE_SELECT_REGISTER_ADDRESS; //sends Die Selection Register Address
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //sends dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, obtains data from register
		
	FLASH_NAND_CS_DISABLE();
	
	Print_To_User(1, 0, "Die Selection Register: 0x%02X \n", &status, status_feature);
}

void FLASH_Configuration() //gets the data from Configuration Register (at the moment just read)
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = GET_FEATURES; //sending Get Features to read data
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = CONFIGURATION_REGISTER_ADDRESS; //sends Configuration Register Address
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
	SPDR = 0x00; //sends dummy byte
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //makes sure to clear the SPIF flag in the 2560, obtains data from register
	
	FLASH_NAND_CS_DISABLE();
	
	Print_To_User(1, 0, "Configuration Register: 0x%02X \n", &status, status_feature);
}

void FLASH_Block_Lock_Setter(bool lock) //this method unlocks(false) or locks(true) the device
{
	FLASH_NAND_CS_ENABLE();
	
	if (lock) //if true, then lock the device
	{
		SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
		SPDR = BLOCK_LOCK_REGISTER_ADDRESS; //send Block Lock address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
		SPDR = 0x7C; //locked and set to its default value
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //clears the SPIF flag; returns garbage	
	}
	else //false, so device will be fully unlocked
	{
		SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
	
		SPDR = BLOCK_LOCK_REGISTER_ADDRESS; //send Block Lock address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x00; //THIS WILL FINALLY UNLOCK ALL BLOCKS IN THE DEVICE!!!!!
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //clears the SPIF flag; returns garbage
	}
	
	FLASH_NAND_CS_DISABLE();
	
	//Print_To_User(1, 0, "Block Lock Register: 0x%02X \n", status, status_feature);
}

void FLASH_Block_Lock(bool feature) //reads(false) / writes(true) block lock feature
{
	FLASH_NAND_CS_ENABLE();
	
	if (feature) //if true the write to register
	{
		SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = BLOCK_LOCK_REGISTER_ADDRESS; //send Block Lock address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x00; //TO BE DETERMINED WHAT DATA OR CONFIGURATION ILL WRITE LATER ON....
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //clears the SPIF flag; returns Block Lock register data
	}
	else //if false the just read the register
	{
		SPDR = GET_FEATURES; //Sending Get_Features Command into SPI Data Register (SPDR)
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = BLOCK_LOCK_REGISTER_ADDRESS; //send Block Lock address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
		
		SPDR = 0x00; //send dummy byte
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //clears the SPIF flag; returns Block Lock register data
	}

	FLASH_NAND_CS_DISABLE();
	
	Print_To_User(1, 0, "Block Lock Register: 0x%02X \n", &status, status_feature);
}

void FLASH_Reset() //reset memory device
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = RESET; //Sending READ_ID Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	FLASH_NAND_CS_DISABLE();

	USART_Data("Device Reseted \n");
}

void FLASH_Para_Pg() //reads from the FLASH NAND parameter page
{
	FLASH_NAND_CS_ENABLE();
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = PARAMETER_PAGE_INIT; //Sending data byte, setting CFG1 = 1 (access Parameter page), into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	FLASH_NAND_CS_DISABLE();
	
	//THIS is a PAGE READ FUNCTION....
	FLASH_Page_Read(); //change plane bit and low byte from 24-bit address

	FLASH_NAND_CS_ENABLE();
	
	SPDR = SET_FEATURES; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = CONFIGURATION_REGISTER_ADDRESS; //Sending Set_Features Command into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	SPDR = PARAMETER_PAGE_EXIT; //Sending data byte to exit Parameter page into SPI Data Register (SPDR)
	while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	
	FLASH_NAND_CS_DISABLE();
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
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte

		SPDR = 0x00; //the low-byte of the 16-bit address
		while(!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
		status = SPDR; //makes sure to clear the SPIF flag in the 2560, useless byte
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
		
			if ((SPDR == '\r') )//|| (SPDR == 0xFF)) //ÿ is 0xFF in uint8_t data type; adding carriage return '\r' until I can get NULL to work... 
			{
				Print_To_User(1, 0, "Data Received: index %i \n", i, status_feature);
				
				//sprintf(status_feature, "Data Received: index %i \n", i);
				//USART_Data(status_feature);
				
				//USART_Data("Data Received \n");
				
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
	FLASH_NAND_CS_ENABLE();
	
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
		status = SPDR; //read incoming status data and puts it into an array
		
		Print_To_User(1, 0, "Status (0x%02X)\n", &status, status_feature);
		
		//sprintf(status_feature, "Get Features: Status (0x%02X)\n", HEX_ID[0]); //hex data to string
		//USART_Data(status_feature);
		
		//_delay_us(5);
	} while (status & 0x01);
	
	FLASH_NAND_CS_DISABLE();

	//sprintf(status_feature, "Get Features: Status (0x%02X)\n", HEX_ID[0]); //hex data to string
	//USART_Data(status_feature);
	
	//CLEAR_ARR();
}

#endif