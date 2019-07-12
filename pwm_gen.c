#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dc=10;//duty cycle
int main(void) {

	
	DDRB = (1 << PB5);
	PORTB = (1 << PB5);
	
	OCR0A=(dc/100)*255;
	OCR0B=255;
	
	TIMSK0 = (1 << OCIE0A)|(1 << OCIE0B);//when to generate interrupt
	sei();

	//prescaler 1024
	TCCR0B = (1 << CS00)|(1<<CS01);

	while (1){
			
		_delay_ms(500);
		dc +=20;
		if(dc > 100) dc = 0;
		OCR0A=(dc/100)*255;
		
	}

	return 0;
}



ISR(TIMER0_COMPA_vect)//ISR when Timer0 value equals that of OCR0A
{
	PORTB &= ~(1 << PB5);
}
ISR(TIMER0_COMPB_vect)//ISR when Timer0 value equals that of OCR0B
{
	PORTB |= (1 << PB5);
}
