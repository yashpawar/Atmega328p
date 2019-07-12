#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function
#include <avr/interrupt.h>

void int_pins(void);
void int_interrupt(void);
void event_interrupt(void);
int main(void) {
	int_pins();
	int_interrupt();
	sei();				// enable interrupts
	
	while (1)
	{					
	}
	return(0);					
}
ISR(PCINT0_vect)
{
	event_interrupt();
}
void int_pins(void)
{
	DDRB=(1<<PB5);//I/O
	PORTB=(1<<PB4);//Enable Pull up
}
void int_interrupt(void)
{
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT4);
}
void event_interrupt(void)
{
	if(!(PINB&(1<<PB4)))
	{
		PORTB |= (1<< PB5);			
	} 		
	else 
	{
		PORTB &= ~(1<< PB5);
	}
}



