/*
 * Piezo2.c
 *
 * Created: 2022-06-23 오전 10:19:04
 *  Author: PKNU
 */ 
//스위치 입력 값에 따라 PIEZO 울리기
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {262, 294, 330, 349, 392, 440, 494, 523};
	
volatile unsigned char sound_flag = 1;


int main()
{
	// PE3(OCR3)만 출력으로
	DDRE = 0x08;
	
	// WGM3(1:0) = “00”
	TCCR3A = (0 << COM3C1) | (0 << COM3C0) | (0 << WGM31) | (0 << WGM30);
	
	// WGM3(3:2) = “11” , CS3(2:0) = “001” 1분주 사용, WGM3(3:0) = “1110”, Fast PWM, 모드 14
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (0 << CS32)| (0 << CS31)| (1 << CS30);
	
	TCCR3C = 0x00;
	
	// 타이머3 카운터 초기화
	TCNT3 =  0x0000;
	
	// 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정
	EICRB = 0xff;
	
	// 인터럽트 4, 5, 6, 7을 허용
	EIMSK = 0xF0; 
	
	// 인터럽트 4, 5, 6, 7 플래그를 클리어
	EIFR = 0xF0; 
	
	sei();
	
	
	
	while(1)
	{
		if(sound_flag)
		{
			_delay_ms(50);
			
			// Piezo소리끔
			TCCR3A = 0x00; 
			
			sound_flag = 0;
		}
		
	}
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
	cli(); 
	ICR3 = (7372800 / DoReMi[0]) / 20; // 도의 음향을 연주한다
	TCCR3A = 0x40; // PE4로 출력
	sound_flag = 1; // 부저 음이 발생하도록 설정
	sei(); 
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
	cli();
	ICR3 = (7372800 / DoReMi[1]) / 20; // 레의 음향을 연주한다
	TCCR3A = 0x40; // PE4로 출력
	sound_flag = 1; // 부저 음이 발생하도록 설정
	sei(); 
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
	cli();
	ICR3 = (7372800 / DoReMi[2]) / 20; // 미의 음향을 연주한다
	TCCR3A = 0x40; // PE4로 출력
	sound_flag = 1; // 부저 음이 발생하도록 설정
	sei(); 
}

SIGNAL(INT7_vect) // 인터럽트 서비스 루틴
{
	cli();
	ICR3 = (7372800 / DoReMi[3]) / 20; // 파의 음향을 연주한다
	TCCR3A = 0x40; // PE4로 출력
	sound_flag = 1; // 부저 음이 발생하도록 설정
	sei(); 
}