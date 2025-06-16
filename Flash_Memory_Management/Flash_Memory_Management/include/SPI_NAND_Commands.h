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

#define PROGRAM_LOAD 0x02 //program load command's address; allows data to be written onto Cache registers (resets cache buffer to FFh values)
#define PROGRAM_EXECUTE 0x10 //program execute command's address; transfer cache register data into block/page array
#define PROGRAM_LOAD_RANDOM_DATA 0x84 //program load random data's address; updates only the cache buffer's data bytes specified by the command input sequence

#define PAGE_READ 0x13 //page read command's address; transfers data from block/page array to cache registers
#define READ_FROM_CACHE 0x03 //read from cache command's address; read data out of cache

#define GET_FEATURES 0x0F //get features command's address; only reads the designated register address given by user
#define SET_FEATURES 0x1F //set features command's address; can alter the device configuration

#define STATUS_REGISTER_ADDRESS 0xC0 //this is the address of the status register
#define CONFIGURATION_REGISTER_ADDRESS 0xB0 //this is the address of the Configuration register
#define DIE_SELECT_REGISTER_ADDRESS 0xD0 //only available in 4Gb or greater stacked die devices
#define BLOCK_LOCK_REGISTER_ADDRESS 0xA0 //feature that protects entire or ranges of device blocks from PROGRAM and ERASE operations

#define PARAMETER_PAGE_INIT 0x40 //setting CFG1 = 1 to access the Parameter pg in the Flash NAND
#define PARAMETER_PAGE_EXIT 0x00 //data value that allows exiting parameter page reading

#define BLOCK_ERASE 0xD8 //block erase command's address; erases memory at block level

#define READ_ID 0x9F //Read ID command; obtains device ID and Device Configuration Data

#define RESET 0xFF //Reset Command; used to put memory device into known condition and abort the command sequence progress

#endif /* SPI_NAND_COMMANDS_H_ */