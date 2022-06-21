/*
 * UART4.c
 *
 * Created: 2022-06-21 오후 12:02:05
 *  Author: PKNU
 */ 
//UART로 PC에서 FND 점멸시키기
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0;	// 스위치 전송 값 저장 변수

// 7-segment에 표시할 글자의 입력 데이터를 저장
unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};
	
void putch(unsigned char data)
{
	
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
	UCSR0A |= (1 << UDRE0);
}

unsigned char getch(void)
{
	
	unsigned char data;
	while(!(UCSR0A & (1 << RXC0)));
	data = UDR0;
	UCSR0A |= (1 << RXC0);
	return data;
}

int main()
{
	unsigned char RX_data = 0;
	
	// RX (0, 입력) / TX (1, 출력)
	DDRE = 0x0E;
	
	// 포트A를 출력으로 설정	
	DDRA = 0xFF; 
	
	// 플래그 초기화
	UCSR0A = 0x00;
	
	// RX,TX enable
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// 비동기 방식, no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// 115200 bps
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	while(1)
	{
		// PC로 부터 입력받은 데이터를 변수 RX_data에 저장
		RX_data = getch();
		
		// RX_data 가 '0' ~ '9' 인 경우를 의미
		if((RX_data >= 0x30) && (RX_data <= 0x39))
		{
			// 아스키 코드값의 '0'은 0x30이다.
			PORTA = FND_DATA_TBL[RX_data - 0x30];
		}
		
	}
	
	
}