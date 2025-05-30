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
	 //first obtain Columns and Block/Page addresses
	USART_Data("---Column Address--- 2 bytes \n");
	COLUMN_BLOCK_PAGE_ADDRESSER(false);

	USART_Data("---Block/Page Address--- 3 bytes \n");
	COLUMN_BLOCK_PAGE_ADDRESSER(true);
	
	for (int i = 0; i < DATA_BYTES_PER_PAGE; i++)
	{
		;
	}
}
