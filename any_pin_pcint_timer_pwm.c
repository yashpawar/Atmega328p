#include <avr/io.h>				// this is always included in AVR programs
#include <util/delay.h>			// add this to use the delay function
#include <avr/interrupt.h>

void int_pins(void);
void int_interrupt(void);
void int_timer(void);
void start_timer(void);
void set_dutycycle(void);
void event_interrupt1(void);
void event_interrupt2(void);

int dutycycle=0;
void main(void)
{
	int_pins();
	int_interrupt();
	int_timer();
	set_dutycycle();
	sei();// enable interrupts			
	start_timer();
	while (1);				
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
//timer for pwm
ISR(TIMER0_COMPA_vect)//ISR when Timer0 value equals that of OCR0A
{
	if(j!=100)
	PORTB &= ~(1 << PB5);
}
ISR(TIMER0_OVF_vect)//ISR when Timer0 value overflows
{
	if(j!=0)
	PORTB |= (1 << PB5);
}
//

void int_pins(void)
{
	DDRB=(1<<PB5);//I/O
	PORTB=(1<<PB5);
	PORTB|=(1<<PB4)|(1<<PB3);//Enable Pull up
}
void int_interrupt(void)
{
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT4)|(1<<PCINT3);
}
void int_timer(void)
{
	TIMSK0 = (1 << OCIE0A)|(TOIE0);//when to generate interrupt	
}
void set_dutycycle(void)
{
	OCR0A=(dutycycle/100)*255;
}
void event_interrupt1(void)
{
		j+=20;
		if (j>=100) j=100;
		set_dutycycle();
		_delay_ms(1000);//delay required for debouncing	
}
void event_interrupt2(void)
{
		j-=20;
		if (j<=0) j=0;
		set_dutycycle();
		_delay_ms(1000);//delay required for debouncing	
}
void start_timer(void)
{
	//prescaler 
	TCCR0B = (1<<CS02);
}






