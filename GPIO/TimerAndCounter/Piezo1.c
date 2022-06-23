/*
 * Piezo1.c
 *
 * Created: 2022-06-23 오전 9:32:57
 *  Author: PKNU
 */ 
//PWM으로 PIEZO 울리기
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

// 피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};


int main()
{
	unsigned char piano = 0;
	
	// PWM 출력, OCR1C(PB 8번 출력)
	DDRB = 0x80;
	
	// COM1C(1:0)=“10”, OC1C핀사용, WGM3(1:0)=“10”
	TCCR1A |= (1 << COM1C1) | (0 << COM1C0) | (1 << WGM31) | (0 << WGM30);
	
	// WGM3(3:2)=“11”, CS3(2:0)=“001” 1분주 사용
	TCCR1B |= (1 << WGM33) | (1 << WGM32) | (0 << CS32)| (0 << CS31)| (1 << CS30);
	
	TCCR1C = 0x00;
	
	// 타이머1 카운터 초기화
	TCNT1 =  0x0000;
	
	while(1)
	{
		// 버튼에 맞는 음향을 연주
		ICR1 = 7372800 / DoReMi[piano];
		
		// 50% 듀티비
		OCR1C = ICR1 / 5;
		
		// piano 변수 1증가
		piano++;
		
		//piano가 9가 되면 초기화
		if(8 < piano) piano = 0;
		_delay_ms(1000);
	}
}