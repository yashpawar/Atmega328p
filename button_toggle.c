
#include <avr/io.h> // Standard include for AVR

#define sbi(x, y) x |= _BV(y)                  // set bit
#define cbi(x, y) x &= ~(_BV(y))               // clear bit
#define tbi(x, y) x ^= _BV(y)                  // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y)) //(for input) checks if the input is high (Logic 1)

int main()
{
    DDRB = 0x20; // PORTB as OUTPUT
    PORTB = 0x10; // To enable internal pullups

    while(1) // Infinite loop
    {
        if(is_high(PINB, PD4))
        {
            cbi(PORTB, PB5); 
        }
        else
        { 
            sbi(PORTB, PB5);
        }
    }
    return 0;
}
