/*
 * FLASH_NAND_MEMORY.h
 *
 * Created: 7/23/2024 4:08:27 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef FLASH_NAND_MEMORY_H_
#define FLASH_NAND_MEMORY_H_

#define FLASH_NAND_ADDRESS_MAX 10 //includes # of bytes for Column Address and Block/Page Address
//#define COLUMN_ADDRESS 4 //# of bytes
//#define BLOCK_PAGE_ADDRESS 6 //# of bytes

uint8_t data[254]; //actual data from FLASH_Nand Memory
uint8_t status_feature[30];
uint8_t HEX_ID[5]; //unsigned 8-bits for hex values
uint8_t Byte_Address[FLASH_NAND_ADDRESS_MAX];

void SPI_MASTER_Init(); //initializes Serial Peripheral Interference (SPI) operations in Master Mode

void FLASH_Write_Enable(); //select Slave device and initializes Write operations onto Flash NAND
void FLASH_Write_Disable(); //de-select Slave device and disable Write operations onto Flash NAND

void FLASH_Page_Program(int s, const uint8_t *array); //Write into addressed pages in Flash NAND
void FLASH_Transfer_Cache(int s, const uint8_t *array); //new command to transfer data from cache to main array
void FLASH_Transfer_MainArray(); //new command to transfer data from main array to cache
void FLASH_Read(); //reads data from FLASH_NAND Memory chip (SPI protocol)

void FLASH_Block_Erase(); //erases data from Flash NAND Memory at the block level
void FLASH_ID(); //read device ID
void FLASH_Reset(); //reset memory device

void FLASH_Para_Pg(); //reads from the FLASH NAND parameter page

void CLEAR_ARR();//clears Arrays
void FLASH_Status(); //checks status till the data completes transferring

void FLASH_MainArray_Address(int s, const uint8_t *array); //determines what 24-bit address (17-bits [block/page], rest is dummy bits) we write/read from
void FLASH_Column_Address(int s, const uint8_t *array); //determines what 16-bit address (12-bits actually) we write/read from (plane bit dependent)

void FLASH_Data_Storage(int s); //determines how to store FLASH NAND data in an array (Parameter page dependent)

#endif /* FLASH_NAND_MEMORY_H_ */