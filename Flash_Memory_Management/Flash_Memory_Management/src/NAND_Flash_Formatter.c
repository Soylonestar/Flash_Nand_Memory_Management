/*
 * NAND_Flash_Formatter.c
 *
 * Created: 5/28/2025 3:54:49 PM
 * Author: agarciasampedroperez
 * Description: This is where I'll test the NAND Flash blocks/pages, test out formats, etc.
 */ 

#include "../include/Atmega_2560_Definitions_Includes.h"
#include "../include/Command_Line_Functions.h"
#include "../include/FLASH_NAND_MEMORY.h"
#include "../include/AWS_Board_Operations.h"
#include "../include/NAND_Flash_Formatter.h"

void Page_Tester() //tests each page per block
{
	
	
	 uint8_t block_page_start = 0x00; //
	 
	 for (int i = 0; i < DATA_BYTES_PER_PAGE; i++)
	 {
		 ;
	 }
}
