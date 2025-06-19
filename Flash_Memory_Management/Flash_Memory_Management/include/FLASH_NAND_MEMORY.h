/*
 * FLASH_NAND_MEMORY.h
 *
 * Created: 7/23/2024 4:08:27 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef FLASH_NAND_MEMORY_H_
#define FLASH_NAND_MEMORY_H_

/*The following are parameters of the FLASH NAND*/

#define FLASH_NAND_ADDRESS_MAX 5 //includes # of bytes for Column Address and Block/Page Address
#define COLUMN_ADDRESS 2 //# of bytes
#define BLOCK_PAGE_ADDRESS 3 //# of bytes

#define PARAMETER_PAGE_SIZE 256 //this is just an example there is more...

#define DATA_BYTES_PER_PAGE 2048 //# of data bytes per page
#define SPARE_BYTES_PER_PAGE 128 //# of spare bytes per page
#define DATA_BYTES_PER_PARTIAL_PAGE 512 //# of data bytes per partial page
#define SPARE_BYTES_PER_PARTIAL_PAGE 32 //# of spare bytes per partial page
#define PAGES_PER_BLOCK 64 //# of pages per block
#define BLOCKS_PER_DIE 2048 //# of blocks per die
#define DIE_PER_MODULE 1 //# of dies
#define MAXIMUM_BAD_BLOCKS_PER_DIE 40 //# of bad blocks per die

/*Here is where Flash NAND parameters ends*/

uint8_t data[256]; //actual data from FLASH_Nand Memory
uint8_t status_feature[256];
uint8_t Device_ID[2];
uint8_t Byte_Address[FLASH_NAND_ADDRESS_MAX]; //unsigned 8-bits for hex values

/*The following is the functions to talk/read from the FLASH NAND*/

void SPI_MASTER_Init(); //initializes Serial Peripheral Interference (SPI) operations in Master Mode

void FLASH_Write_Enable(); //select Slave device and initializes Write operations onto Flash NAND
void FLASH_Write_Disable(); //de-select Slave device and disable Write operations onto Flash NAND

void FLASH_Page_Program(); //Write into addressed pages in Flash NAND
void FLASH_Program_Execute(); //command to transfer data from cache register to block/page array
void FLASH_Random_Data_Program_x1(); //writes or replaces data in a page with existing data 
 
void FLASH_Page_Read(); //transfers data from block/page array to Cache Register
void FLASH_Read_From_Cache_x1(); //read data out of the cache register

void FLASH_Block_Erase(); //erases data from Flash NAND Memory at the block level
void FLASH_ID(); //read device ID
void FLASH_Reset(); //reset memory device

void FLASH_Para_Pg(); //reads from the FLASH NAND parameter page

void CLEAR_ARR();//clears Arrays
void FLASH_Status(); //checks status till the data completes transferring
void FLASH_Die_Selection(); //determines which die is being used (default on power on/reset is die 0)
void FLASH_Configuration(); //at the moment, only obtains data from the Configuration register
void FLASH_Block_Lock_Setter(bool lock); //locks(true) / unlocks(false) device
void FLASH_Block_Lock(bool feature); // reads(false) / writes(true) block lock feature

void FLASH_MainArray_Address(int s, const uint8_t *array); //determines what 24-bit address (17-bits [block/page], rest is dummy bits) we write/read in the block/page array
void FLASH_Column_Address(int s, const uint8_t *array); //determines what 16-bit address (12-bits actually, plane bit dependent) we write/read from in the cache register

void FLASH_Data_Storage(int s); //determines how to store FLASH NAND data in an array (Parameter page dependent)

/*Here is where the FLASH functions ends*/

#endif /* FLASH_NAND_MEMORY_H_ */