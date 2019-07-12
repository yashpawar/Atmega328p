#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function
#include <avr/interrupt.h>

void int_pins(void);
void int_interrupt(void);
void event_interrupt1(void);
void event_interrupt2(void);
void pwm_loop(void);

int j=0;
int main(void) {
	int_pins();
	int_interrupt();
	sei();				// enable interrupts
	while (1)
	{
		pwm_loop();			
	}
	return(0);					
}
ISR(PCINT0_vect)
{
	cli();//stop taking interrupts
	if(!(PINB&(1<<PB4)))
	{
		event_interrupt1();
	} 		
	
	if(!(PINB&(1<<PB3)))
	{
		event_interrupt2();		
	} 
	sei();//start taking interrupts		
}
void int_pins(void)
{
	DDRB=(1<<PB5);//I/O
	PORTB=(1<<PB4)|(1<<PB3);//Enable Pull up
}
void int_interrupt(void)
{
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT4)|(1<<PCINT3);
}
void pwm_loop(void)
{
		for(int i = 0; i < 100; i++)//variable j represents time out of 100 for which we want out lead to be on
		{
			if(i < j)
				PORTB |= (1<< PB5);
			else
				PORTB &= ~(1<< PB5);
		}
}
void event_interrupt1(void)
{
		j+=20;
		if (j>=100) j=100;
		_delay_ms(1000);//delay required for debouncing	
}
void event_interrupt2(void)
{
		j-=20;
		if (j<=0) j=0;
		_delay_ms(1000);//delay required for debouncing	
}


