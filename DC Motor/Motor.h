#ifndef MOTOR_H
#define MOTOR_H

/*************************************************************************
* Title: C++ file for controlling 4 DC Motors using 2 L293D motor driver 
* and 74HCT595 shift register
* Author: Ivan Antunovic
* Target: AVR ATmega2560
* This software is free to use and share
**************************************************************************/


#include <inttypes.h>
#include <avr/io.h>


#define MICROSTEPS 16                       // 8 ili 16

#define MOTOR12_64KHZ _BV(CS20)             // no prescaler
#define MOTOR12_8KHZ _BV(CS21)              // prescaler 8
#define MOTOR12_2KHZ _BV(CS21) | _BV(CS20)  // prescaler 32
#define MOTOR12_1KHZ _BV(CS22)              // prescaler 64

#define MOTOR34_64KHZ _BV(CS00)             // no prescaler
#define MOTOR34_8KHZ _BV(CS01)              // prescaler 8
#define MOTOR34_1KHZ _BV(CS01) | _BV(CS00)  // prescaler 64

#define DC_MOTOR_PWM_RATE   MOTOR34_8KHZ    // PWM rate for DC motore
#define STEPPER1_PWM_RATE   MOTOR12_64KHZ   // PWM rate for stepper 1
#define STEPPER2_PWM_RATE	MOTOR34_64KHZ	// PWM rate for stepper 2




// Bit positions for shift register 74HCT595 output
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

//Constants used for method calls used for controlling DC motors 
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

/* Pins used for shift register 74HCT595 control */

// STCP	Pin 12	Storage Register Clock Input
#define LATCH 4
#define LATCH_DDR DDRB
#define LATCH_PORT PORTB

// SHCP	Pin 11 Shift Register Clock Input
#define CLK_PORT PORTD
#define CLK_DDR DDRD
#define CLK 4

// OE	Pin 13	Output enable input (Aktivan na LOW)
#define ENABLE_PORT PORTD
#define ENABLE_DDR DDRD
#define ENABLE 7

// DS	Pin 14	Serial Data Input
#define DATA 0                 
#define DATA_DDR DDRB		
#define DATA_PORT PORTB

class MotorController
{
	public:
		MotorController();
		void enable(void);
		friend class DCMotor;
		void latch_tx(void);
		uint8_t TimerInitalized;
};

class DCMotor
{
	private:
		uint8_t motorNum, pwmFreq;
		
	public:
		DCMotor(uint8_t motorNum, uint8_t freq = DC_MOTOR_PWM_RATE);
		void run(uint8_t);
		void setSpeed(uint8_t);
		
};

#endif
