/*
 * TimerPWM1.c
 *
 * Created: 2022-06-22 오후 4:25:34
 *  Author: PKNU
 */ 
//PWM으로 LED 밝기 조절 하기
#define F_CPU 7372800UL
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	unsigned char Light = 0;
	
	DDRB = 0xff;
	
	//PC PWM 모드, 256 분주 = 28800, up카운팅일때 OC0 비교출력, down카운팅일때 Oc0 값 초기화
	TCCR0 = (1 << WGM00) | (0 << WGM01) | (1 << CS02) | (1 << CS01) | (0 << CS00) | (1 << COM01) | (1 << COM00);
	
	// 타이머0 카운터를 초기화
	TCNT0 = 0x00;
	
	
	// OCR0의 크기를 만져 전원공급(파형)을 손본다 => LED밝기의 변화발생
	while (1)
	{
		// 밝음 -> 어두움 (OC0의 신호가 많다가 적어지므로)
		for(Light = 0; Light < 255; Light++)
		{
			OCR0 = Light;
			_delay_ms(10);
		}
		
		// 어두움 -> 밝음 (OC0의 신호가 적다가 많아지므로)
		for(Light = 255; 0 < Light; Light--)
		{
			OCR0 = Light;
			_delay_ms(10);
		}
		
	}
}