/*
 * main.h
 *
 * Created: 6/19/2025 11:33:17 AM
 *  Author: agarciasampedroperez
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/*Include Standard C and AVR Libraries*/
#include <xc.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/*Include All Header Files*/
#include "include/AWS_Board_Operations.h" //Our boards' operations
#include "include/Atmega_2560_Definitions_Includes.h" //these are board required parameters
#include "include/Command_Line_Functions.h" //UI operations
//#include "include/MEGA_1284P_SPI.h" //MEGA_1284P SPI Testing code...not needed at the moment
#include "include/FLASH_NAND_MEMORY.h" //FLASH_NAND Driver methods
#include "include/SPI_NAND_Commands.h" //Flash_NAND Commands
#include "include/NAND_Flash_Formatter.h" //FLASH_NAND Memory Tester/Formatter

/*Define ALL Source Files*/
#define Board_V2_2 //tracks the boards operations with corresponding ports
//#define Board_V2_1 //only one board version at a time
#define Command_Line_Functions //UI methods
#define NAND_Flash_Formatter //Flash Tester/Formatter
#define FLASH_NAND_MEMORY //FLASH_NAND Driver methods

#endif /* MAIN_H_ */