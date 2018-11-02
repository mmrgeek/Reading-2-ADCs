#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
int right = 0;
int left = 0;
int main(void){
	DDRB |=1<<PINB0;
	DDRB |=1<<PINB1;
	//Enable a prescaler and internal or external clock
	/*
		*1,000,000/50,000 = 20
		*1,000,000/200,000 = 5
		*the prescaler factor is 16
	*/
	ADCSRA |= 1<<ADPS2;
	//use the external analog reference on AVCC pin
	ADMUX |= 1<<REFS0;
	//Enable the interrupts function in ADC
	ADCSRA |= 1<<ADIE;
	//turn on the ADC
	ADCSRA |=1<<ADEN;
	//enable global interrupts
	sei();
	//start the conversion
	ADCSRA |= 1<<ADSC;
    while (1) {
		
    }
	return 0;
}
//the interrupt function
ISR(ADC_vect){
	uint8_t low = ADCL;
	uint16_t full = ADCH<<8 | low;
	switch(ADMUX){
		case 0x40:
			right = full;
			ADMUX |= 1<<MUX0;
			break;
		
		case 0x41:
			left = full;
			ADMUX &= ~(1<<MUX0);
			break;
	}
	if(right > left){
		PORTB |=1<<PINB0;
		PORTB &=~(1<<PINB1);
	}else if(left > right){
		PORTB |=1<<PINB1;
		PORTB &=~(1<<PINB0);
	}else{
		PORTB |=1<<PINB0;
		PORTB |=1<<PINB1;
	}
	ADCSRA |= 1<<ADSC;
}