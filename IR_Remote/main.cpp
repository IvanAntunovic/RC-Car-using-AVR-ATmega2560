/*
 * IR_Remote.cpp
 *
 * Created: 1/4/2017 11:50:35 AM
 * Author : Ivan Antunovic
 */ 

#include <avr/io.h>
#include "IRRemote.h"
#include "USART.h"


int main(void)
{
    IR_Remote ir;
	USART_init();
	char buffer[32];
    while (1) 
    {
		int keyPressed = ir.getKeyPressed();
		itoa(keyPressed, buffer, 10);
		
		if (ir.available() == true)
		{
			USART_puts(buffer);
			USART_puts("\n\r");
		}
    }
}

