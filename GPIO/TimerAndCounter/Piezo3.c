/*
 * Piezo3.c
 *
 * Created: 2022-06-23 오전 10:49:34
 *  Author: PKNU
 */ 
//UART로 PC에서 입력받으면 Piezo 울림

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
	
volatile unsigned char TX_flag = 0;
volatile char TX_data = 0;	// 스위치 전송 값 저장 변수

unsigned char getch(void)
{
	unsigned char data;
	while(!(UCSR0A & (1 << RXC0)));
	data = UDR0;
	UCSR0A |= (1 << RXC0);
	return data;
}

void putch(unsigned char data)
{
	// UCSR0A 0x20 = 0010 0000  = 송신 데이터 레지스터 준비완료 인터럽트(Data Register Empty) Enable상태
	
	while(!(UCSR0A & (1 << UDRE0))); // 둘(UCSR0A,0x20)의 비트값중 하나라도 0이면 0 출력
	UDR0 = data;
	UCSR0A |= 0x20;
}

int main()
{
	// 연결확인용 문구 출력
	unsigned char text[] = "Hello! World! \r\n";
	
	// 키도그 입력데이터 임시 저장변수
	unsigned char RX_data = 0;
	
	// 문자열 출력시 문자의 순서기록
	unsigned char i = 0;
	
	// -------------- UARP 통신설정 --------------
	// Rx(입력,0) / TX(출력,1)
	DDRE = 0xfe;
	
	// 플래그 초기화
	UCSR0A = 0x00;
	
	// RX,TX enable
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// 비동기 방식, no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// 115200 bps
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	// -------------- Piezo 설정 --------------
	
	// PWM 출력, OCR1C(PB 8번 출력)
	DDRB = 0x80;
	
	// COM1C(1:0)=“10”, OC1C핀사용, WGM3(1:0)=“10”
	TCCR1A |= (1 << COM1C1) | (0 << COM1C0) | (1 << WGM31) | (0 << WGM30);
	
	// WGM3(3:2)=“11”, CS3(2:0)=“001” 1분주 사용
	TCCR1B |= (1 << WGM33) | (1 << WGM32) | (0 << CS32)| (0 << CS31)| (1 << CS30);
	
	TCCR1C = 0x00;
	
	// 타이머1 카운터 초기화
	TCNT1 =  0x0000;
	
	while(text[i] != '\0')
	{
		putch(text[i++]);
	}
	i = 0;
	ICR1 = 7372800 / DoReMi[0];
	OCR1C =  ICR1 / 1000;
	while(1)
	{
		// PC로 부터 입력받은 데이터를 변수 RX_data에 저장
		RX_data = getch();
		
		// RX_data 가 '0' ~ '9' 인 경우를 의미
		if((RX_data >= 0x30) && (RX_data <= 0x39))
		{
			// 누른 숫자를 음계로 사용
			ICR1 = (7372800 / DoReMi[RX_data - 0x30]);
			
			//듀티비 설정
			OCR1C = ICR1 / 4.5;
			TCCR3A = 0x40; // PE4로 출력
			
			_delay_ms(100);
			OCR1C = 0;
		}
		
		
	}
	
}