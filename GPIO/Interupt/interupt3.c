/*
 * interupt3.c
 *
 * Created: 2022-06-20 오전 11:33:34
 *  Author: PKNU
 */ 

// 5번, 7번 인터럽트를 각각 하강, 상승에지로 설정해 LED점등의 방향을 바꾸는 실습
 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char Shift_flag = 1;

SIGNAL(INT5_vect)
{
	cli();	// 전체인터럽트를 금지
	Shift_flag = 1;
	sei();	// 전체인터럽트를 허용
}

SIGNAL(INT7_vect)
{
	cli();	// 전체인터럽트를 금지
	Shift_flag = 2;
	sei();	// 전체인터럽트를 허용
}

int main()
{
	unsigned char LED_Data = 0x01;
	
	// 포트 입출력 설정
	DDRE = 0x00;
	DDRA = 0xff;
	
	// 인터럽트 5활성화(하강에지), 7활성화(상승에지)
	EIMSK =   (1 << INT5 ) 
			| (1 << INT7 );
	
	EICRB =	  (1 << ISC51) | (0 << ISC50) 
			| (1 << ISC71) | (1 << ISC70);
	
	EIFR  =   (1 << INTF5) 
			| (1 << INTF7);
	
	sei();
	
	
	while(1)
	{
		PORTA = LED_Data;
		// LED0 -> LED3
		if(Shift_flag == 1) 
		{
			
			if(LED_Data == 0x80) LED_Data = 0x01;
			
			// LED_Data 값을 왼쪽으로 쉬프트
			else LED_Data <<= 1;
		}
		
		// LED3 -> LED0
		else if(Shift_flag == 2)
		{
			if(LED_Data == 0x01) LED_Data = 0x80;
			
			// LED_Data 값을 왼쪽으로 쉬프트
			else LED_Data >>= 1;
			
		}
		_delay_ms(100);
		
		
		
	}
	
	return 0;
}