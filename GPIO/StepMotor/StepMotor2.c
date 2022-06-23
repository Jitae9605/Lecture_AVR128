/*
 * StepMotor2.c
 *
 * Created: 2022-06-23 오후 3:30:13
 *  Author: PKNU
 */ 

//2상 여자 방식으로 스텝 모터 돌리기

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>


int main()
{
	unsigned char i;
	 
	 // MOTOR1_EN 를 출력 포트로 설정
	DDRB = 0x20;
	
	// STEP0 ~ STEP3을 출력 포트로 설정
	DDRD = 0xf0;
	
	// M1 Disable, DC 모터 정지
	PORTB &= ~0x20;
	
	while(1)
	{
		
		for(i = 0; i< 12; i++)
		{
			// 1 step
			PORTD = 0x30;
			_delay_ms(10);
			
			// 2 step
			PORTD = 0x90;
			_delay_ms(10);
			
			// 3 step
			PORTD = 0xC0;
			_delay_ms(10);
			
			// 4 step
			PORTD = 0x60;
			_delay_ms(10);
		}
		
		_delay_ms(1000);
	}
	
}