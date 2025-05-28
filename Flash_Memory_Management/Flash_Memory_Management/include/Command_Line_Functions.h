/*
 * Command_Line_Functions.h
 *
 * Created: 8/6/2024 11:05:10 AM
 *  Author: agarciasampedroperez
 */ 


#ifndef COMMAND_LINE_FUNCTIONS_H_
#define COMMAND_LINE_FUNCTIONS_H_

/*All this is the UI to talk to the FLASH NAND from user inputs*/

void UserInput(bool command); //true if command, false if normal input
void ExecuteCommand(const uint8_t *str); //here is where all the command lines are going to be in...might be its own header file

uint8_t CommandBuffer[254]; //where the CommandLine will be added to by individual uint8_t
uint8_t c; //for receiving USART_RX_Data

int s; //this is for preset memory allocation (parameter pg for example);

void HEX_Parser(bool addr_type); //this function puts hex values for false (Column Addr.) or true (Block/Page Addr.)
void HEX_Verification(); //this is for conversion to hex values from ASCII (uint8_t) user input

void Print_To_User(int num_elements, int offset, const char *msg, const uint8_t *arr_start, uint8_t *arr_dest); //in need to transfer data from an array to another array with a message to print
void NAND_Address_Checker(); //returns user the Column and Block/Page Addresses

#endif /* COMMAND_LINE_FUNCTIONS_H_ */