/*
 * Stepmotor1.c
 *
 * Created: 2022-06-23 오후 3:08:57
 *  Author: PKNU
 */ 
//1상 여자 방식으로 스텝 모터 돌리기
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	
	// Motor1_En을 출력으로 설정
	DDRB = 0x20;
	
	// STEP 0 ~ STEP 3 을 출력포트로 설정
	DDRD = 0xF0;
	
	// M1 Diable, DC 모터장치
	PORTB &= ~0x20;
	
	while(1)
	{
		// 1step
		PORTD = 0x10;
		_delay_ms(10);
		
		// 2step
		PORTD = 0x20;
		_delay_ms(10);
		
		// 3step
		PORTD = 0x40;
		_delay_ms(10);
		
		// 4step
		PORTD = 0x80;
		_delay_ms(10);
		
		
	}
}