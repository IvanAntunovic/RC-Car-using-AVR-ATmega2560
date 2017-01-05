/*
 * ATmega2560_DC_MOTOR.cpp
 *
 * Created: 12/18/2016 1:57:45 PM
 * Author : Ivan Antunovic
 */
  
#include <Motor.h>
#include <avr/delay.h>

DCMotor motor_1(1);
DCMotor motor_2(2);
DCMotor motor_3(3);
DCMotor motor_4(4);

int main(void)
{
	motor_1.setSpeed(120);
    motor_1.run(RELEASE);

    motor_2.setSpeed(120);
    motor_2.run(RELEASE);

    motor_3.setSpeed(120);
    motor_3.run(RELEASE);

    motor_4.setSpeed(120);
    motor_4.run(RELEASE);

    while (1) 
    {
		for (i = 0; i < 10000; i++)
		{
			if (i < 5000)
			{
			  motor_1.run(FORWARD);
		      motor_2.run(FORWARD);
		      motor_3.run(FORWARD),
		      motor_4.run(FORWARD);
			}
			if (i >= 5000 && i <= 10000)
			{
			  motor_1.run(BACKWARD);
	          motor_2.run(BACKWARD);
	          motor_3.run(BACKWARD),
	          motor_4.run(BACKWARD);
			}
		}
    }
}

