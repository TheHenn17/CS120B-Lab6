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

enum States{Start, Game, Wait, Stop} state;
unsigned char sequence[] = {1, 2, 4, 2};
unsigned char i;
unsigned char bl;
void Tick() {
	switch(state) {
		case Start:
			state = Game;
			i = 0;
			bl = 0x00;
			break;
		case Game:
			if((PINA & 0x01) == 0x01 && bl == 0x01) { bl = 0x00; }
			if((PINA & 0x01) == 0x00 && bl == 0x01) { state = Game; }
			else if((PINA & 0x01) == 0x01 && bl == 0x00) { state = Game; }
			else {
				state = Wait;
				i = 0;
			}
			if(i < 3) {i++;}
			else {i = 0;}
			break;
		case Wait:
			if((PINA & 0x01) == 0x01) { state = Stop; }
                        else { state = Wait; }
			break;
		case Stop:
			if((PINA & 0x01) == 0x01) { state = Stop; }
                        else { 
				state = Game;
				bl = 0x01;
				i = 0;
			}
			break;
		default:
			state = Start;
			break;
	}
	switch(state) {
		case Start:
			PORTB = 0x00;
			break;
		case Game:
			PORTB = sequence[i];
			break;
		case Wait:
		case Stop:
			break;
		default:
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(300);
	TimerOn();
	state = Start;
	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
