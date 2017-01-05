#ifndef _IRREMOTE_H_
#define _IRREMOTE_H_

/*
 * IR_Remote.h
 *
 * Created: 1/4/2017 11:50:35 AM
 * Author : Ivan Antunovic
 * Code can be freely used and shared.
 */ 


/*
################################################################################################################
################################################################################################################
##                         TSOP 4838 KHz
##                      1 = OUT, 2 = GND, 3 = VS
##            PD0 INTERRUPT 0  , ARDUINO PIN = 21
##
##
##
################################################################################################################
################################################################################################################
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

class IR_Remote
{
	private:
		volatile uint32_t bitPattern, newKey;
		uint8_t timerValue;
		uint8_t msCount;
		char pulseCount;	
		bool isReady;
		
	public:
		IR_Remote();
		static IR_Remote* IR_Remote_pointer;
		void interrupt_0_init();
		void timer_1_init();
		uint32_t getKeyPressed();
		bool available();
		
	public:
		inline void handle_interruptCOMPA(void);
		inline void handle_interruptINT0(void);
};

#endif
