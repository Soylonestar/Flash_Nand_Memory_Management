/*
 * FLASH_NAND_MEMORY.h
 *
 * Created: 7/23/2024 4:08:27 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef FLASH_NAND_MEMORY_H_
#define FLASH_NAND_MEMORY_H_

char data[254]; //actual data from FLASH_Nand Memory
char status_feature[30];
char HEX_ID[10];
int s;

void SPI_MASTER_Init(); //initializes Serial Peripheral Interference (SPI) operations in Master Mode

void FLASH_Write_Enable(); //select Slave device and initializes Write operations onto Flash NAND
void FLASH_Write_Disable(); //de-select Slave device and disable Write operations onto Flash NAND

void FLASH_Write_Data();//const char *str); //writes String data via the addressed individual characters into Flash_NAND (SPI protocol)
void FLASH_Transfer_Cache(void); //new command to transfer data from cache to main array
void FLASH_Transfer_MainArray(); //new command to transfer data from main array to cache
void FLASH_Read(); //reads data from FLASH_NAND Memory chip (SPI protocol)

void FLASH_Erase(); //erases data from Flash Nand Memory at the block level
void FLASH_ID(); //read device ID
void FLASH_Reset(); //reset memory device

void FLASH_Para_Pg(); //reads from the FLASH NAND parameter page

void CLEAR_ARR();//clears Arrays
void FLASH_Status(); //checks status till the data completes transferring

void FLASH_MainArray_Address(int s); //determines what 24-bit address we write/read from (starting address dependent)
void FLASH_Column_Address(int s); //determines what 16-bit address (12-bits actually) we write/read from (plane bit dependent)

void FLASH_Data_Storage(int s); //determines how to store FLASH NAND data in an array (Parameter page dependent)

#endif /* FLASH_NAND_MEMORY_H_ */