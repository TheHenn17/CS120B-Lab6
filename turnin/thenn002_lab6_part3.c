/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//Demo: https://drive.google.com/open?id=1DyCzBmeebcWmwSnKnrr1XvNpMnL0cd5C

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States{Start, Wait, Inc, Dec, Rst} state;
unsigned char i;
unsigned char cnt;
void Tick() {
	switch(state) {
		case Start:
			state = Wait;
			break;
		case Wait:
			if((PINA & 0x03) == 0x00) { state = Rst; }
			else if((PINA & 0x03) == 0x02) {
				state = Inc;
				i = 0;
				if(cnt < 9) { cnt++; }
			}
			else if((PINA & 0x03) == 0x01) {
				state = Dec;
				i = 0;
				if(cnt > 0) { cnt--; }
			}
			else { state = Wait; }
			break;
		case Inc:
			if((PINA & 0x03) == 0x00) { state = Rst; }
                        else if((PINA & 0x03) == 0x02) {
                                state = Inc;
                                if(i > 10) {
                                	if(cnt < 9) { cnt++; }
					i = 0;
				}
                        }
                        else if((PINA & 0x03) == 0x01) {
                                state = Dec;
                                i = 0;
                                if(cnt > 0) { cnt--; }
                        }
                        else { state = Wait; }
			break;
		case Dec:
			if((PINA & 0x03) == 0x00) { state = Rst; }
                        else if((PINA & 0x03) == 0x02) {
                                state = Inc;
                        	i = 0;
				if(cnt < 9) { cnt++; }
                        }
                        else if((PINA & 0x03) == 0x01) {
                                state = Dec;
                                if(i > 10) {
                                	if(cnt > 0) { cnt--; }
					i = 0;
				}
                        }
                        else { state = Wait; }
			break;
		case Rst:
			if((PINA & 0x03) == 0x00) { state = Rst; }
                        else if((PINA & 0x03) == 0x02) {
                                state = Inc;
                                i = 0;
                                if(cnt < 9) { cnt++; }
                        }
                        else if((PINA & 0x03) == 0x01) {
                                state = Dec;
                                i = 0;
				if(cnt > 0) { cnt--; }
                        }
                        else { state = Wait; }
			break;
		default:
			state = Start;
			break;
	}
	switch(state) {
		case Start:
			PORTB = cnt;
			break;
		case Wait:
			PORTB = cnt;
			break;
		case Inc:
		case Dec:
			PORTB = cnt;
			i++;
			break;
		case Rst:
			cnt = 0;
			PORTB = cnt;
			break;
		default:
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(100);
	TimerOn();
	state = Start;
	cnt = 7;
	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
