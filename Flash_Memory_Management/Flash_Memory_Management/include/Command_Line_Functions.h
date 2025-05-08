/*
 * Command_Line_Functions.h
 *
 * Created: 8/6/2024 11:05:10 AM
 *  Author: agarciasampedroperez
 */ 


#ifndef COMMAND_LINE_FUNCTIONS_H_
#define COMMAND_LINE_FUNCTIONS_H_

void CommandLine(); //function that ask for user' command line
void UserInput();
void ExecuteCommand(const char *str); //here is where all the command lines are going to be in...might be its own header file

char CommandBuffer[254]; //where the CommandLine will be added to by individual characters
char c; //for receiving USART_RX_Data

#endif /* COMMAND_LINE_FUNCTIONS_H_ */