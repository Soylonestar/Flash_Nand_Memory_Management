/*
 * AWS_Board_Operations.h
 *
 * Created: 8/6/2024 3:40:15 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef AWS_BOARD_OPERATIONS_H_
#define AWS_BOARD_OPERATIONS_H_

void USART_Init(); //initialize USART for TX and RX
void USART_Data(const char *str); //addresses String data by the individual character addresses
void USART_TX_Data(char Data); //transmit char type data to serial RS232 USART0
char USART_RX_Data(); //receives char type data from serial RS232 USART0

void LED_Init(void); //initialize LEDs on Board
void LED_Toggle(); //toggle LEDs on Board
void LED_Red_Toggle(); //toggle Red Led on Board
void LED_Green_Toggle(); //toggle Green Led on Board

void VOLTAGE_Translator_Init (void); //initializes relevant voltage translators

void SLAVE_CS_Deselect(); //Deselects all Chip Selects for SPI protocol

#endif /* AWS_BOARD_OPERATIONS_H_ */