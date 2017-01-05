/*
 * HCSR04.cpp
 *
 * Created: 1/3/2017 7:25:34 PM
 * Author : fairenough
 */ 

#include "USART.h"
#include "HCSR04.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


int main(void)
{
	
	HCSR04 hcsr04(F_CPU);
	//Serial.begin(9600);
	//uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	USART_init();
	char buffer[7];
	
	sei();
	
	while (1)
	{
		
		//itoa((int)hcsr04.getDistance(),(char*) buffer, 10);
		int distance = hcsr04.getDistance();
		itoa(distance,(char*) buffer, 10);
		USART_puts(buffer);
		USART_puts("\n\r");
		_delay_ms(200);
		itoa(distance, buffer, 10);
		
	}
}

