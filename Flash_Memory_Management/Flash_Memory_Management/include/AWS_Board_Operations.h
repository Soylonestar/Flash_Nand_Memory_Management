/*
 * AWS_Board_Operations.h
 *
 * Created: 8/6/2024 3:40:15 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef AWS_BOARD_OPERATIONS_H_
#define AWS_BOARD_OPERATIONS_H_

/*This our basic board operations, unique for our board*/

void USART_Init(); //initialize USART for TX and RX
void USART_Data(const uint8_t *str); //addresses String data by the individual uint8_t addresses
void USART_TX_Data(uint8_t Data); //transmit uint8_t type data to serial RS232 USART0
uint8_t USART_RX_Data(); //receives uint8_t type data from serial RS232 USART0

void LED_Init(void); //initialize LEDs on Board
void LED_Toggle(); //toggle LEDs on Board
void LED_Red_Toggle(); //toggle Red Led on Board
void LED_Green_Toggle(); //toggle Green Led on Board

void VOLTAGE_Translator_Init (void); //initializes relevant voltage translators

void SLAVE_CS_Deselect(); //Deselects all Chip Selects for SPI protocol

void FLASH_NAND_CS_ENABLE(); //enable the Chip Select of the FLASH NAND
void FLASH_NAND_CS_DISABLE(); //disable the Chip Select of the FLASH NAND

#endif /* AWS_BOARD_OPERATIONS_H_ */