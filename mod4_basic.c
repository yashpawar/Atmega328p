
#include <avr/io.h> // Standard include for AVR
#include <util/delay.h>

#define sbi(x, y) x |= _BV(y)                  // set bit
#define cbi(x, y) x &= ~(_BV(y))               // clear bit
#define tbi(x, y) x ^= _BV(y)                  // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y)) //(for input) checks if the input is high (Logic 1)

int main()
{
    DDRB = 0x30; // PORTB as OUTPUT
    PORTB = 0x08; // To enable internal pullups

    while(1) // Infinite loop
    {
        if(!is_high(PINB, PB3))
        {
            tbi(PORTB, PB4);
	    if(is_high(PINB, PB4))
            tbi(PORTB, PB5);
            
            
        }
        _delay_ms(500);
    }
    return 0;
}
