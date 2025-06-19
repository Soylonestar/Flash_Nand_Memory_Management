/*
 * NAND_Flash_Formatter.h
 *
 * Created: 5/28/2025 3:53:37 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef NAND_FLASH_FORMATTER_H_
#define NAND_FLASH_FORMATTER_H_

/*Here are test methods for the NAND Flash*/

void Page_Tester(); //test method: page valid tester, iterates through each page per block

void Write_Page_Test(); //test method: writes a small message in every page...then reads every page in the same block


#endif /* NAND_FLASH_FORMATTER_H_ */