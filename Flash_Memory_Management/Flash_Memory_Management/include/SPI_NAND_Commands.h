/*
 * SPI_NAND_Commands.h
 *
 * Created: 7/17/2024 11:05:13 AM
 *  Author: agarciasampedroperez
 */ 


#ifndef SPI_NAND_COMMANDS_H_
#define SPI_NAND_COMMANDS_H_

/*These are the Command bytes to send to the FLASH NAND*/

#define WRITE_ENABLED 0x06 //write enable command's address; enables change onto Flash_NAND memory
#define WRITE_DISABLED 0x04 //write disabled command's address; disables change onto Flash_NAND memory
#define PROGRAM_LOAD 0x02 //program load command's address; allows data to be written onto Cache registers
#define PROGRAM_EXECUTE 0x10 //program execute command's address; transfer cache data into main array
#define PAGE_READ 0x13 //page read command's address; transfers data from main array to cache registers
#define READ_FROM_CACHE 0x0B //read from cache command's address; read data out of cache
#define GET_FEATURES 0x0F //get features command's address; can lead to the monitoring of the device status
#define SET_FEATURES 0x1F //set features command's address; can alter the device configuration
#define STATUS_REGISTER_ADDRESS 0xC0 //this is the address of the status register
#define CONFIGURATION_REGISTER_ADDRESS 0xB0 //this is the address of the Configuration register
#define PARAMETER_PAGE_INIT 0x40 //setting CFG1 = 1 to access the Parameter pg in the Flash NAND
#define PARAMETER_PAGE_EXIT 0x00 //data value that allows exiting parameter page reading
#define BLOCK_ERASE 0xD8 //block erase command's address; erases memory at block level
#define READ_ID 0x9F //Read ID command; obtains device ID and Device Configuration Data
#define RESET 0xFF //Reset Command; used to put memory device into known condition and abort the command sequence progress

#endif /* SPI_NAND_COMMANDS_H_ */