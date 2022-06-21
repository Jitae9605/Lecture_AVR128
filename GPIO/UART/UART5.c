/*
 * UART5.c
 *
 * Created: 2022-06-21 오후 2:23:45
 *  Author: PKNU
 */ 
// 누른 스위치에 반응해 구구단 출력

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void putch(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))); // 전송준비상태 까지 대기
	UDR0 = data;					 // 데이터를 UDR0 에 쓰면 전송
	UCSR0A |= (1 << UDRE0);
}

int main()
{
	unsigned char i = 0;
	
	// Rx(입력,0) / TX(출력,1)
	DDRE = 0xfe;
	
	
	DDRA = 0x00;	
	
	// 플래그 초기화
	UCSR0A = 0x00;
	
	// RX,TX enable
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// 비동기 방식, no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// 115200 bps
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	// 스위치 입력
	int Arr_switch[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	
	while(1)
	{
		for(int j = 1; j < 9; j++)
		{
			if(PINA == Arr_switch[j-1])
			{
				unsigned char arr[20];
				sprintf(arr, "\r\nLet's Start %d Dan\r\n",j);
				
				while(arr[i] != '\0')
				{
					// 문자열의 끝인 '\0'가 나올때까지 한 문자씩 출력
					putch(arr[i++]);
				}
				i = 0;
				
				unsigned char arr2[20] = "";
				for(int k = 1; k < 10; k++)
				{
					sprintf(arr2,"%d * %d = %d\r\n",j,k,j*k);
					
					while(arr2[i] != '\0')
					{
						// 문자열의 끝인 '\0'가 나올때까지 한 문자씩 출력
						putch(arr2[i++]);
					}
					i = 0;
				}
				_delay_ms(300);
			}
		}
	}
}