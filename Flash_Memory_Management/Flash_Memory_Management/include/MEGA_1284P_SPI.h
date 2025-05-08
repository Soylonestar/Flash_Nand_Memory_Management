/*
 * MEGA_1284P_SPI.h
 *
 * Created: 7/23/2024 3:28:58 PM
 *  Author: agarciasampedroperez
 */ 


#ifndef MEGA_1284P_SPI_H_
#define MEGA_1284P_SPI_H_

//ATMEGA 2560 is Master
//MEGA 1284P is Slave

		//THIS IS FOR MAIN.C While(1) loop.....
		/*
		//This is for MEGA_1284P_SPI header file...
		
		//RECEIVE_DATA(); //In order to get this to work, then I need to change MEGA 1284P as Master
		// Then change ATMEGA 2560 as SLAVE
		
		TRANSMIT_DATA();
		*/

void SPI_Write(char Data); 
char SPI_Read();

void TRANSMIT_DATA();
void RECEIVE_DATA();

uint8_t status;

char write_test[] = "TX received \n";

//char char_data = '!';
char buffer[20];
char data;

/*
DOESN'T LIKE BIT BANGING
Meaning that setting ~SS on/off manually won't work, may be different for ~CS...
*/

void SPI_Write (char Data) //write to Mega 1284P
{	
	SPDR = Data; //add Data to SPI Data register
	while (!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	status = SPDR; //read Data register to clear SPIF flag
}

char SPI_Read (void) //receive data from Mega 1284P
{
	SPDR = 0xFF; //shifting dummy byte to get data from Slave
	while (!(SPSR & (1 << SPIF))); //waiting until serial transfer is complete
	return SPDR; //return data received from data register
}

void TRANSMIT_DATA ()
{
	status = SPDR; //read Data register to clear SPIF flag
	
	/*
		//The working code that allow individual char data type to be transmitted...
		
		for (int i = 33; i < 126; i++)
		{
			USART_Data("Transmitting: ");
			//sprintf(buffer, "%d \n", count);
			//USART_Data(buffer);
			USART_TX_Data(char_data);
			USART_TX_Data('\n');
			
			LED_Green_Toggle();
			_delay_ms(100);
			LED_Green_Toggle();
			
			SPI_Write(char_data);
			
			LED_Red_Toggle();
			_delay_ms(100);
			LED_Red_Toggle();
			
			char_data++;
		}
		
		char_data = '!';
		*/
		
		USART_Data("Transmitting: ");
		for (size_t i = 0; i < strlen(write_test); i++)
		{
			SPI_Write(write_test[i]); //sends char data to Slave
			USART_TX_Data(write_test[i]);	
		}
		
		LED_Green_Toggle();
		_delay_ms(950);
}

//IN order to receive data, ATMEGA 2560 must be slave, and MEGA 1284P must be set as Master....

void RECEIVE_DATA ()
{
	status = SPDR; //read Data register to clear SPIF flag
	
	for (int i = 0 ; i < sizeof(buffer); i++)
		{
			data = SPI_Read();
			buffer [i] = data;
			if ( data == '\n')
			{
				break;
			}
		}

		USART_Data("Receiving: ");
		USART_Data(buffer);
		
		LED_Red_Toggle();
		_delay_ms(950);
}

#endif /* MEGA_1284P_SPI_H_ */