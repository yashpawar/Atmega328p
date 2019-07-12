

#include <avr/io.h> // Standard include for AVR
#include <util/delay.h>
#include <avr/interrupt.h>
#define sbi(x, y) x |= _BV(y)    // set bit
#define cbi(x, y) x &= ~(_BV(y)) // clear bit
#define tbi(x, y) x ^= _BV(y)    // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y))

int j=0,l=0;
int main()
{
    DDRB = 0x20; // PORTB as OUTPUT
    PORTB = 0x11;
    PORTD = (1<<PORTD3);//for pull-up
    EIMSK = (1<<INT1);//accept interrupt from INT1
    
    int i;
    sei();
    
    while(1) // Infinite loop
    {
		for(i = 0; i < 100; i++)//variable j represents time out of 100 for which we want out lead to be on
		{
			if(i < j)
				sbi(PORTB, PB5);
			else
				cbi(PORTB, PB5);
		}	
    }
    return 0;
}
ISR(INT1_vect)//interrupt servic routine triggred for INT1
{
	cli();//stop taking interrupts
	if (l==0) j+=50;
	if (l==1) j-=50;
	if (j>=100) l=1;
	if (j<=0) l=0;
	_delay_ms(1000);//delay required for debouncing
	sei();//start taking interrupts
}
