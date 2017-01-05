#include "IRRemote.h"


/*
 * IR_Remote.cpp
 *
 * Created: 1/4/2017 11:50:35 AM
 * Author : Ivan Antunovic
 * Code can be freely used and shared.
 */ 


IR_Remote* IR_Remote::IR_Remote_pointer;

IR_Remote::IR_Remote()
{ 
	bitPattern = 0, newKey = 0;
	msCount = 0;
	pulseCount = 0;
	isReady = false;
	
	IR_Remote_pointer = this;
	interrupt_0_init();
	timer_1_init();
}

uint32_t IR_Remote::getKeyPressed()
{
	return newKey;
}

bool IR_Remote::available()
{
	if(isReady == true)
		return true;
	else
		return false;
}

void IR_Remote::interrupt_0_init()
{
  EIMSK |= (1 << INT0); /* enable INT0 */
  EICRA |= (1 << ISC01) | 1 << ISC00; /* trigger when RISING edge */
  sei();
}

void IR_Remote::timer_1_init()
{
  TCCR1B |= 1 << WGM12 | 1 << CS10; //no prescaling , OCCR1 MODE
  
  TCNT1 = 0;
  OCR1A = 1000; // svaku 1ms TIMER1 INTERRUPT
  
  TIMSK1 |= 1 << OCIE1A;
  
  sei();
}

void IR_Remote::handle_interruptINT0()
{
	timerValue = msCount; //spremi proteklo vrijeme od zadnjeg prekida
	msCount = 0;
	isReady = false;
	TCNT1 = 0;
	
	pulseCount++; // inkrement posto je doslo do rastuceg brida
	
	if (timerValue >= 50) // ako je sirina impulsa veca od 50 ms, znaci da je
	// zapocni start of frame
	{
		pulseCount = -2; // preskoci prva dva rastuca brida
		bitPattern = 0;
	}
	else if (pulseCount >= 0 && pulseCount < 32) // ako se radi o korisnom podatku
	{
		if (timerValue >= 2){}
		else
		bitPattern |= (uint32_t)1 << (31 - pulseCount);
		
	}
	else if(pulseCount >= 32) // Ako je cijeli frame (4 bytea) primljen
	{                         // pulsecount == 32 , primljen 33 bit , odnosno bit
		newKey = bitPattern; // zapisi primljeni podatak
		pulseCount = 0;
		isReady = true;
	}
}

void IR_Remote::handle_interruptCOMPA()
{
	if(msCount < 50) // Sve dok je vrijeme manje od 50 ms
		msCount++;
}
/* #############################################################
  U ovom ISR-u se provjerava koliko je proslo sekundi pri dolasku 
  rastuceg brida te se o razlici duljina zakljuÄ?uje koji podatak je
  dosao na pin
  
  EXTERNAL INTERRUPT 0 = PIND2
  ############################################################
*/
ISR (INT0_vect)
{
	IR_Remote::IR_Remote_pointer->handle_interruptINT0();
}


ISR (TIMER1_COMPA_vect)
{
	IR_Remote::IR_Remote_pointer->handle_interruptCOMPA();
}
