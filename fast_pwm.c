#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dc=10;//duty cycle
int main(void) {

	// Set Data Direction for PD6 to output for the LED
	DDRD = (1 << PD6);
	
	OCR0A=(dc/100)*255;
	
	TIMSK0 = (1 << TOIE0);//when to generate interrupt
	
	//OC0A is PD6 pin state
	// Clear OC0A on Compare Match, set OC0A at BOTTOM(0x00) 
	TCCR0A = (1 << COM0A1)| (1 << WGM01) | (1 << WGM00);// Fast PWM mode(by datasheet)
	sei();

	//prescaler 1024
	TCCR0B = (1 << CS00)|(1<<CS02);

	while (1) {
		
		_delay_ms(500);
		// Increasing the LED brightness
		dc +=20;
		// If the 8-bit register OCR0A overflows, reset it
		if(dc > 100) {
			dc = 0;
		}
	}

	return 0;
}

ISR(TIMER0_OVF_vect)
{
	OCR0A=(dc/100)*255;
}
