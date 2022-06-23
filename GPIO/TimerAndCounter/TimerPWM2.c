/*
 * TimerPWM2.c
 *
 * Created: 2022-06-22 오후 5:18:24
 *  Author: PKNU
 */ 
//스위치 입력 값에 따라 LED 밝기 조절 하기
#define F_CPU 7372800UL
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int Light = 0;
volatile unsigned char Light_flag = 1;

int main()
{
	DDRB = 0xff;
	DDRE = 0x00;
	
	//PC PWM 모드, 256 분주 = 28800, up카운팅일때 OC0 비교출력, down카운팅일때 Oc0 값 초기화
	TCCR2 = (1 << WGM00) | (0 << WGM01) | (1 << CS22) | (0 << CS21) | (0 << CS20) | (1 << COM01) | (0 << COM00);
	
	// 타이머0 카운터를 초기화
	TCNT0 = 0x00;
	
	// 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정
	EICRB = 0xFF; 
	
	// 인터럽트 4, 5, 6, 7을 허용
	EIMSK = 0xF0; 
	
	// 인터럽트 4, 5, 6, 7 플래그를 클리어
	EIFR = 0xF0; 
	
	sei();
	
	
	// OCR0의 크기를 만져 전원공급(파형)을 손본다 => LED밝기의 변화발생
	while (1)
	{
		// 밝음 -> 어두움 (OC0의 신호가 많다가 적어지므로)
		if(Light_flag)
		{
			// Light 값으로 밝기 제어
			OCR2 = Light;
			Light_flag = 0;
		}
		
	}
}

// LED OFF
SIGNAL(INT4_vect)
{
	cli();
	Light = 0;
	Light_flag = 1;
	sei();	
}

// LED 밝기 감소
SIGNAL(INT5_vect)
{
	cli();
	Light -= 51;
	if(Light < 0)
		Light = 0;
	Light_flag = 1;
	sei();
}

// LED 밝기 증가;
SIGNAL(INT6_vect)
{
	cli();
	Light += 51;
	if(Light > 255)
		Light = 255;
	Light_flag = 1;
	sei();
}

// LED ON;
SIGNAL(INT7_vect)
{
	cli();
	Light = 255;
	Light_flag = 1;
	sei();
}