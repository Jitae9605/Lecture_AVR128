/*
 * interupt4.c
 *
 * Created: 2022-06-20 오전 11:54:13
 *  Author: PKNU
 */ 

// 5번, 7번 인터럽트를 각각 하강, 상승에지로 설정해 LED점등의 방향을 바꾸는 실습
 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char Time_Stop = 0;

SIGNAL(INT5_vect)
{
	cli();	// 전체인터럽트를 금지
	if(Time_Stop == 0)
	{
		Time_Stop = 1;
	}
	else
	{
		Time_Stop = 0;
	}
	sei();	// 전체인터럽트를 허용
}


int main()
{
	unsigned char FND_DATA_TBL[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0X7C,0X07,0X7F,0X67};
	unsigned char cnt = 0;	// FND table 카운터 변수
	
	// 포트 입출력 설정
	DDRE = 0x00;
	DDRA = 0xff;
	
	// 인터럽트 5활성화(상승에지), 7활성화(상승에지)
	EIMSK =   (1 << INT5 ) ;
	EICRB =	  (1 << ISC51) | (1 << ISC50) ;
	EIFR  =   (1 << INTF5) ;
	sei();
	
	
	while(1)
	{
		PORTA = FND_DATA_TBL[cnt];
		
		if(Time_Stop == 0)
		{
			
			cnt++;
			if(cnt > 9) cnt = 0;
		}
		_delay_ms(200);
		
		
		
	}
	
	return 0;
}