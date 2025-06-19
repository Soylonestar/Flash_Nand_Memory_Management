/*
 * NAND_Flash_Formatter.c
 *
 * Created: 5/28/2025 3:54:49 PM
 * Author: agarciasampedroperez
 * Description: This is where I'll test the NAND Flash blocks/pages, test out formats, etc.
 */

#include "../main.h"

#ifdef NAND_Flash_Formatter
 
void Page_Tester() //tests each page per block
{
	 //first obtain Columns and Block/Page addresses
	USART_Data("---Column Address--- 2 bytes \n");
	COLUMN_BLOCK_PAGE_ADDRESSER(false); //obtain the column address

	USART_Data("---Block/Page Address--- 3 bytes \n");
	COLUMN_BLOCK_PAGE_ADDRESSER(true); //obtain the Block/Page address
		
	for (int i = 0; i < PAGES_PER_BLOCK; i++)
	{
		CLEAR_ARR();
		FLASH_Page_Read();
		
		if (Byte_Address[4] == 0x40) //break loop if passed this point (x3F) is where all page bits are set to 1
		{
			break;
		}
		
		if (data[0] != 0xFF)
		{
			Print_To_User(1, 4, "0x%02X This Page is marked bad \n", Byte_Address, status_feature);
		}
		else
		{
			Print_To_User(1, 4, "0x%02X This Page is clean \n", Byte_Address, status_feature);
		}
		
		Byte_Address[4]++; //increment page hopefully...
	}
}

void Write_Page_Test()
{
	;
}

#endif