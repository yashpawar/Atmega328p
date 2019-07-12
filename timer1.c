
#include <avr/io.h> // Standard include for AVR
#include <avr/interrupt.h>//to accept interrupts

#define sbi(x, y) x |= _BV(y)                  // set bit
#define cbi(x, y) x &= ~(_BV(y))               // clear bit
#define tbi(x, y) x ^= _BV(y)                  // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y)) //(for input) checks if the input is high (Logic 1)

int main()
{
    DDRB = 0x30;
    OCR1A = 15625;//value of timer when interrupt should occur
    TIMSK1 = (1 << OCIE1A);//comparison to be done with OCR1A
    sei();//enable interrupts
    TCCR1B= (1 << WGM12)|(1 << CS10)|(1 << CS12);//ctc mode(clear when interrupt) and prescaling of 1024
    
    while(1) // Infinite loop
    {
    }
    return 0;
}
ISR(TIMER1_COMPA_vect)//ISR when Timer1 value equals that of OCR1A
{
	cli();//disable taking interrupts
        tbi(PORTB, PB4);
	if(is_high(PINB, PB4))
        tbi(PORTB, PB5);
        //add delay (button and debouncing case)
        sei();//reenable inturrupts
}
