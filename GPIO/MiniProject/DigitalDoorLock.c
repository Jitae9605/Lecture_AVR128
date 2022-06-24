/*
 * DigitalDoorLock.c
 *
 * Created: 2022-06-24 오전 9:55:49
 *  Author: PKNU
 */ 
#define LCD	8
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "Lcd.h"

volatile unsigned char Pass[5];
volatile unsigned char Pass_Same_flag = 0; // 번호 일치시 1, 아니면 0;
volatile unsigned char Insert_flag = 0; // 입력있음
volatile unsigned char Intrrupt_flag = 0; // 인터럽트 4(통신입력 on = 1 /off = 0)
volatile unsigned char mode_flag = 0; // 인터럽트 5(문열기 = 1 / 비번바꾸기 = 0)

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
	
void init_keypad();
unsigned char keyscan();
void putch(unsigned char data);
unsigned char getch(void);
void IsCorrectPass(unsigned char InputPass[]);
void SetPass(unsigned char setPass[]);

int main()
{
	unsigned char Password[] = "1234";
	SetPass(Password);
	unsigned char i = 0;
	
	// -------------- 인터럽트 설정 --------------
	DDRE = 0x80;
	EIMSK = (1 << INT4) | (1 << INT5);
	EICRB = (1 << ISC41) | (0 << ISC40) | (1 << ISC51) | (0 << ISC50);
	sei();
	
	// -------------- 모터 설정 --------------
	// MOTOR1_EN 를 출력 포트로 설정
	DDRB = (1 << DDRB5);
	
	// STEP0 ~ STEP3을 출력 포트로 설정
	DDRD = 0xf0;
	
	// M1 Disable, DC 모터 정지
	PORTB &= ~(1 << DDRB5);
	
	// -------------- UARP 설정 --------------
	// RX (0, 입력) / TX (1, 출력)
	
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
	
	// PWM 출력, OCR1C(PB 8번 출력) // 
	DDRB |= (0 << DDRB7) ;
	
	// COM1C(1:0)=“10”, OC1C핀사용, WGM3(1:0)=“10”
	TCCR1A |= (1 << COM1C1) | (0 << COM1C0) | (1 << WGM31) | (0 << WGM30);
	
	// WGM3(3:2)=“11”, CS3(2:0)=“001” 1분주 사용
	TCCR1B |= (1 << WGM33) | (1 << WGM32) | (0 << CS32)| (0 << CS31)| (1 << CS30);
	
	TCCR1C = 0x00;
	
	// 타이머1 카운터 초기화
	TCNT1 =  0x0000;
	
	// 피에조 소리 줄이기
	ICR1 = 7372800 / DoReMi[0];
	OCR1C =  ICR1 / 10000;
	
	// -------------- LCD 설정 --------------
	DDRA = 0xFF;
	DDRG = 0x0F;
	
	Lcd_Init();
	Lcd_Clear();
	Lcd_Clear();
	
	init_keypad();
	unsigned char Inputkey[5] = "";
	unsigned char temp;
	unsigned char text[] = "Input Password \r\n";
	
	while(text[i] != '\0')
	{
		putch(text[i++]);
	}
	
	i = 0;

	
	while (1)
	{
		
		if(Intrrupt_flag == 0)
		{
			for(int j = 0; j < 4; j++ )
			{
				while(Insert_flag == 0)
				{
					Inputkey[j] = keyscan();
				}
				Insert_flag = 0;
				_delay_ms(200);
			}
			Inputkey[5] = '\0';
		}
		
		else
		{
			for(int j = 0; j < 4; j++ )
			{
				while(Insert_flag == 0)
				{
					temp = getch();
					if((temp >= 0x30) && (temp <= 0x39))
					{
						// 아스키 코드값의 '0'은 0x30이다.
						Inputkey[j] = temp;
					}
				}
				Insert_flag = 0;
				//_delay_ms(200);
			}
			Inputkey[5] = '\0';
		}
		
		while(Inputkey[i] != '\0')
		{
			putch(Inputkey[i++]);
		}
		
		i = 0;
		
		IsCorrectPass(Inputkey);
		
		if(Pass_Same_flag == 1) // 비번일치
		{
			// PWM 출력, OCR1C(PB 8번 출력) // 소리알람
			DDRB |= (1 << DDRB7) ;
			_delay_ms(100);
			DDRB |= (0 << DDRB7) ;
			
			for(int l = 0; l < 8; l++)
			{
				ICR1 = 7372800 / DoReMi[l];
				OCR1C =  ICR1 / 4.5;
				_delay_ms(100);
			}
			OCR1C =  ICR1 / 10000;
			
			// 스텝모터 회전
			for(i = 0; i< 12; i++)
			{
				// 1 step
				PORTD = 0x30;
				_delay_ms(10);
				
				// 2 step
				PORTD = 0x90;
				_delay_ms(10);
				
				// 3 step
				PORTD = 0xC0;
				_delay_ms(10);
				
				// 4 step
				PORTD = 0x60;
				_delay_ms(10);
			}
			
			i = 0;
			unsigned char open_text[] = "\r\nopen \r\n";
			while(open_text[i] != '\0')
			{
				putch(open_text[i++]);
			}
			i = 0;
			
			unsigned char *open_text_LCD = "Open";
			Lcd_Pos(0,0);
			Lcd_STR(open_text_LCD);
			
			Pass_Same_flag = 0; 
			Insert_flag = 0;

		}
		
		else // 불일치
		{
			// PWM 출력, OCR1C(PB 8번 출력) // 소리알람
			DDRB |= (1 << DDRB7) ;
			ICR1 = 7372800 / DoReMi[0];
			OCR1C =  ICR1 / 2;
			_delay_ms(500);
			OCR1C =  ICR1 / 10000;
			
			unsigned char *open_text_LCD = "Error Number";
			Lcd_Pos(0,0);
			Lcd_STR(open_text_LCD);
		}
		
		
	}
	
	return 0;
}

void init_keypad()
{
	DDRC = 0xF8; //0b 1111 1000
	PORTC = 0x07; //0b 0000 0111
}

void IsCorrectPass(unsigned char InputPass[])
{
	int k = 0;
	for(int i = 0; i < 5; i++)
	{
		if(InputPass[i] == Pass[i])
			k++;
		
	}
	if(k == 5)
		Pass_Same_flag = 1;
}

unsigned char keyscan()
{
	PORTC = 0x08;		// 0000 1000
	_delay_ms(1);
	if((PINC & 0x07) == 0x01) {Insert_flag = 1; return '1'; }
	else if((PINC & 0x07) == 0x02) {Insert_flag = 1; return '2'; }
	else if((PINC & 0x07) == 0x04) {Insert_flag = 1; return '3';}
	_delay_ms(10);
	
	PORTC = 0x10;		// 0001 0000
	_delay_ms(1);
	if((PINC & 0x07) == 0x01) {Insert_flag = 1; return '4'; }
	else if((PINC & 0x07) == 0x02) {Insert_flag = 1; return '5'; }
	else if((PINC & 0x07) == 0x04) {Insert_flag = 1; return '6';}
	_delay_ms(10);
	
	PORTC = 0x20;		// 0010 0000
	_delay_ms(1);
	if((PINC & 0x07) == 0x01) {Insert_flag = 1; return '7'; }
	else if((PINC & 0x07) == 0x02) {Insert_flag = 1; return '8'; }
	else if((PINC & 0x07) == 0x04) {Insert_flag = 1; return '9'; }
	_delay_ms(10);
	
	PORTC = 0x40;		// 0100 0000
	_delay_ms(1);
	if((PINC & 0x07) == 0x01) {Insert_flag = 1; return '*'; }
	else if((PINC & 0x07) == 0x02) {Insert_flag = 1; return '0'; }
	else if((PINC & 0x07) == 0x04) {Insert_flag = 1; return '#'; }
	_delay_ms(10);
	
	return 0;
}

void SetPass(unsigned char setPass[])
{
	for(int i = 0; i< 5; i++)
	{
		Pass[i] = setPass[i];
	}
	
}

void putch(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))); // 전송준비상태 까지 대기
	UDR0 = data;					 // 데이터를 UDR0 에 쓰면 전송
	UCSR0A |= (1 << UDRE0);
}

unsigned char getch(void)
{
	unsigned char data;
	while(!(UCSR0A & (1 << RXC0)));
	data = UDR0;
	UCSR0A |= (1 << RXC0);
	Insert_flag = 1;
	return data;
}

SIGNAL(INT4_vect)
{
	cli();
	if(Intrrupt_flag == 0)
	{
		Intrrupt_flag = 1;
		PORTE = ( 1 << DDRE7);
		_delay_ms(200);
	}
	
	else
	{
		Intrrupt_flag = 0;
		PORTE = ( 0 << DDRE7);
		_delay_ms(200);
	}
	sei();
}

SIGNAL(INT5_vect)
{
	cli();
	
	sei();
}