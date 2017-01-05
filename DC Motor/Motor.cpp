/*************************************************************************
* Title: C++ file for controlling 4 DC Motors using 2 L293D motor drivers
* and 74HCT595 shift register
* Author: Ivan Antunovic
* Target: AVR ATmega2560
* This software is free to use and share
**************************************************************************/


#include <avr/io.h>
#include "Motor.h"

static uint8_t latch_state;


MotorController::MotorController (void)
{
	TimerInitalized = false;
};

/************************************************************************/
/* Initilaze pins as outputs needed for shift register control                                                                    */
/************************************************************************/
void MotorController::enable ()
{
	LATCH_DDR |= 1 << LATCH;
	ENABLE_DDR |= 1 << ENABLE;
	CLK_DDR |= 1 << CLK;
	DATA_DDR |= 1 << DATA;
	
	latch_state = 0;

	latch_tx();	
	
	ENABLE_PORT &= ~(1 << ENABLE);
}

/************************************************************************/
/* Depending on what motor was chosen and what direction command was chosen
/*	shift register gives output with this function                                 
/************************************************************************/
void MotorController::latch_tx()
{
	LATCH_PORT &= ~(1 << LATCH);
	DATA_PORT &= ~(1 << DATA);
	
	for (uint8_t i = 0; i < 8; i++)
	{
		CLK_PORT &= ~(1 << CLK);	//set clock LOW
		
		if (latch_state & (1 << (7 - i)) ) 
			DATA_PORT |= 1 << DATA;
		else
			DATA_PORT &= ~(1 << DATA);
		
		CLK_PORT |= 1 << CLK;		//set clock HIGH
	}
	LATCH_PORT |= 1 << LATCH;
}

static MotorController MC;

/************************************************************************/
/*                         DC MOTORS                                                                   */
/************************************************************************/


/************************************************************************/
/* initialize fast PWM for Motor 1                                      */
/************************************************************************/

inline void initPWM1(uint8_t freq)
{
	#if defined(__AVR_ATmega324p__)
		 // uses PWM timer2 pin PB3 
		TCCR2A |= 1 << COM2A1 | 1 << WGM20) | (1 << WGM21); // fast PWM, turn on oc2a
		TCCR2B = freq & 0x7;
		OCR2A = 0;
	#elif defined(__AVR_ATmega2560__)
		// Atmega2560 , pin 11 = PB5 (OC1A)
		TCCR1A |= (1 << COM1A1) | (1 << WGM10); // fast PWM, turn on
		TCCR1B = (freq & 0x7) | (1 << WGM12);
		OCR1A = 0;
		
		DDRB |= 1 << PB5;
	#else
		#error "This chip is not supported!"
	#endif
}

/************************************************************************/
/* Set PWM duty cycle for Motor 3                                       */
/************************************************************************/
inline void setPWM1 (uint8_t s)
{
	#if defined(__AVR_ATmega324p__)
		OCR2A = s;
	#elif defined(__AVR_ATmega2560__)
		OCR1A = s;
	#else
		#error "This chip is not supported!"
	#endif
}

/************************************************************************/
/* initialize fast PWM for Motor 2                                      */
/************************************************************************/
inline void initPWM2(uint8_t freq)
{
	#if defined(__AVR_ATmega324p__)
		TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); // fast PWM, turn on oc2b
		TCCR2B = freq & 0x7;
		OCR2B = 0;
	#elif defined(__AVR_ATmega2560__)
		//On ATmega2560 , pin 3 = PE5 (OC3C)
		TCCR3A |= (1 << COM1C1) | (1 << WGM10); // fast PWM, turn on oc3c
		TCCR3B = (freq & 0x7) | (1 << WGM12);
		OCR3C = 0;
		
		DDRE |= 1 << PE5;
	#else
		#error "This chip is not supported!"
	#endif
	
	//pinMode(3, OUTPUT);
}

/************************************************************************/
/* Set PWM duty cycle for Motor 2                                       */
/************************************************************************/
inline void setPWM2 (uint8_t s)
{
	#if defined(__AVR__ATmega324p)
		OCR2B = s;
	#elif defined(__AVR_ATmega2560__)
		//On ATmega2560 , pin 11 = PB5 (OC1A)
	#else
		#error "This chip is not supported!"
	#endif
}
/************************************************************************/
/* initialize fast PWM for Motor 3                                      */
/************************************************************************/
inline void initPWM3 (uint8_t freq)
{
	#if defined(__AVR_ATmega324p__)
		 // use PWM from timer0A / PD6 (pin 6)
		 TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on OC0A
		 //TCCR0B = freq & 0x7;
		 OCR0A = 0;
	#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		// On ATmega2560 , pin 6 = PH3 (OC4A)
		TCCR4A |= _BV(COM1A1) | _BV(WGM10); // fast PWM, turn on oc4a
		TCCR4B = (freq & 0x7) | _BV(WGM12);
		//TCCR4B = 1 | _BV(WGM12);
		OCR4A = 0;
	
		DDRH |= 1 << PH3; 
	#else
		#error "This chip is not supported!"
	#endif
}


/************************************************************************/
/* Set PWM duty cycle for Motor 3                                       */
/************************************************************************/
inline void setPWM3 (uint8_t s)
{
	#if defined(__AVR_ATmega324p__)
		OCR0A = s;
	#elif defined(__AVR_ATmega2560__)
		OCR4A = s;
	#else	
		#error "This chip is not supported!"
	#endif
}


/************************************************************************/
/* initialize fast PWM for Motor 4                                      */
/************************************************************************/
inline void initPWM4 (uint8_t freq)
{
	#if defined(__AVR_ATmega324p)
		// use PWM from timer0B / PD5 (pin 5)
		TCCR0A |= _BV(COM0B1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on oc0a
		//TCCR0B = freq & 0x7;
		OCR0B = 0;
	#elif defined(__AVR_ATmega2560__)
		 // on ATmega2560, pin 5 = PE3 (OC3A)
		 TCCR3A |= _BV(COM1A1) | _BV(WGM10); // fast PWM, turn on oc3a
		 TCCR3B = (freq & 0x7) | _BV(WGM12);
		 //TCCR4B = 1 | _BV(WGM12);
		 OCR3A = 0;
		
		 DDRE |= 1 << PE3;
	#else
		#error "This chip is not supported"!
	#endif
}

/************************************************************************/
/* Set PWM duty cycle for Motor 4                                       */
/************************************************************************/
inline void setPWM4 (uint8_t s)
{
	#if defined(__AVR_ATmega324p__)
		OCR0B = s;
	#elif defined(__AVR_ATmega2560__)
		OCR3A = s;
	#else	
		#error "This chips is not supported!"
	#endif
}

/************************************************************************/
/* Constructor receives parameter which motor should be used (1, 2, 3, 4)
/*	Optional parameterr is PWM frequency
/************************************************************************/

DCMotor::DCMotor (uint8_t motorNum, uint8_t pwmFreq)
{
	this->motorNum = motorNum;
	this->pwmFreq = pwmFreq;
	
	MC.enable();

	switch(motorNum)
	{
		case 1:
			// Set both motor pins to 0
			latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
			MC.latch_tx();
			initPWM1(pwmFreq);
			break;
		case 2:
			// Set both motor pins to 0
			latch_state &= ~(1 << MOTOR2_A) &  ~(1 << MOTOR2_B);
			MC.latch_tx();
			initPWM2(pwmFreq);
			break;
		case 3:
			// Set both motor pins to 0
			latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B);
			MC.latch_tx();
			initPWM3(pwmFreq);
			break;
		case 4:
			// Set both motor pins to 0
			latch_state &= ~(1 << MOTOR4_A) & ~(1 << MOTOR3_B);
			MC.latch_tx();
			initPWM4(pwmFreq); 	
	}
}
/************************************************************************/
/* Constructor receives parameter which motor should be used (1, 2, 3 ,4)
/* Depending on what motor is chosen, variables a and b are checked with switch
/* case statement                                                                    
/************************************************************************/

void DCMotor::run (uint8_t cmd)
{
	uint8_t a, b;

	switch (this->motorNum)
	{
		case 1:
			// MOTOR1_A 2 (Q2) i MOTOR1_B 3 (Q3)
			a = MOTOR1_A; b = MOTOR1_B; break; 
		case 2:
			// MOTOR2_A 1 (Q1) i MOTOR2_B 4 (Q4)
			a = MOTOR2_A; b = MOTOR2_B; break;
		case 3:
			// MOTOR3_A 5 ( i  MOTOR3_B 7
			a = MOTOR3_A; b = MOTOR3_B; break;
		case 4:
			// MOTOR4_A 0 i MOTOR4_B 6
			a = MOTOR4_A; b = MOTOR4_B; break;
		default:
			return;
	}

	switch (cmd)
	{
		/* Following comments are valid if motor 1 was chosen: parameter motorNum = 1 */
		// If  motorNum = 1 -> a = 2; b = 3;
		case FORWARD:
			// a = 2
			// (1 << a) = 0b0000 0100
			// latch_state = 0b0000 0100
			latch_state |= (1 << a);
			// b = 3 
			// ~(1 << b) = ~(0b0000 1000) = (0b1111 0111)
			// latch_state = (0b0000 0100) & (0b1111 0111)
			// latch_state = 0b0000 0100 = 0x04	
			latch_state &= ~(1 << b);

			// Q2 = 1 , Q3 = 0
			// 	M1A = 1 , M1B = 0
			MC.latch_tx();
			break;
		case BACKWARD:
			// a = 2
			latch_state &= ~(1 << a); // latch_state = 0

			// b = 3
			latch_state |= (1 << b); // latch_state = 8

			// Q2 = 0 , Q3 = 1
			// 	M1A = 1 , M1B = 0
			MC.latch_tx();
			break;
		case RELEASE: 
			latch_state &= ~(1 << a);
			latch_state &= ~(1 << b);
			
			// Q2 = 0 , Q3 = 0
			// 	M1A = 0 , M1B = 0
			// stop motors
			MC.latch_tx();
			break;
	}
}
/************************************************************************
Depending which motor is chosen [1:4], set PWW duty cycle for the according 
motor                                                  
************************************************************************/

void DCMotor::setSpeed (uint8_t speed)
{
	switch(this->motorNum)
	{
		case 1:
			setPWM1(speed); break;
		case 2:
			setPWM2(speed);	break;
		case 3:
			setPWM3(speed);	break;
		case 4:
			setPWM4(speed); break;
	 }
}

