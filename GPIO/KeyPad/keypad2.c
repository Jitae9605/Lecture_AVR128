/*
 * KeyPad2.c
 *
 * Created: 2022-06-24 오전 9:33:12
 *  Author: PKNU
 */ 
// 키패드를 이용해 LED 불켜기

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>


// 키패드 이용방법
// 키패드는 각각 이 핀에 연결된게 아니라 각기 다른 2개의 핀쌍에 스위치가 연결되어 있는꼴
// -> 스위치가 눌렸을때 연결된 핀의 조합으로 어느 스위치가 눌린건지 판단

// * 스위치 원리
// 끊어져있던게 연결되는 것 => A핀으로 1을 출력했을때 스위치가 눌러져있었다면 1이 입력되어 들어왔을것 임을 이용해 판단

// *현재 연결상황
// A	: 0    1    2    3    4    5    6    7
// pad  : c0   c1   c2   r0   r1   r2   r3   없음

// 포트A의 0 ~ 2을 출력(Row), 3 ~ 6을 입력(Colum)으로 설정



void init_keypad()
{
	DDRA = 0xF8; //0b 1111 1000
	PORTA = 0x07; //0b 0000 0111
}


unsigned char keyscan()
{
	PORTA = 0x08;		// 0000 1000s
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '1';
	else if((PINA & 0x07) == 0x02) return '2';
	else if((PINA & 0x07) == 0x04) return '3';
	_delay_ms(10);
	
	PORTA = 0x10;		// 0001 0000
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '4';
	else if((PINA & 0x07) == 0x02) return '5';
	else if((PINA & 0x07) == 0x04) return '6';
	_delay_ms(10);
	
	PORTA = 0x20;		// 0010 0000
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '7';
	else if((PINA & 0x07) == 0x02) return '8';
	else if((PINA & 0x07) == 0x04) return '9';
	_delay_ms(10);
	
	PORTA = 0x40;		// 0100 0000
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '*';
	else if((PINA & 0x07) == 0x02) return '0';
	else if((PINA & 0x07) == 0x04) return '#';
	_delay_ms(10);
	
	return 0;
}
int main()
{
	DDRC = 0xFf;
	init_keypad();
	
	while(1){
		unsigned char key = keyscan();
		if(key == '1') PORTC = 0x00;
		else if(key == '2') PORTC = 0x01;
		else if(key == '3') PORTC = 0x02;
		else if(key == '4') PORTC = 0x04;
		else if(key == '5') PORTC = 0x08;
		else if(key == '6') PORTC = 0x10;
		else if(key == '7') PORTC = 0x20;
		else if(key == '8') PORTC = 0x40;
		else if(key == '9') PORTC = 0x80;
		else if(key == '0') PORTC = 0xFF;
		else if(key == '*') PORTC = 0xFE;
		else if(key == '#') PORTC = 0xFD;
	}
	return 0;
}