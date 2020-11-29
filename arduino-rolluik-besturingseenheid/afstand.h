/*
 * afstand.h
 *
 * Author: jacob, KEHT, Marijn Kok, Haylee Drenth
 */

/* 
 * HC-SR04
 * trigger to sensor : uno 0 (PD0) output
 * echo from sensor  : uno 3 (PD3 = INT1) input
 * 
 * DIO : uno 8  (PB0) data
 * CLK : uno 9  (PB1) clock
 * STB : uno 10 (PB2) strobe
 *
 */

#define F_CPU 16E6
#include <util/delay.h>

#define BEGIN 0x1
#define END 0x0

void init_ports(void);
void init_timer(void);
void init_ext_int(void);
uint16_t calc_cm(uint32_t counter);

volatile uint16_t gv_counter; // 16 bit counter value
volatile uint8_t gv_echo; // a flag

void init_ports(void)
{
    DDRD= (1 << 4); // set port D4 as output, D3 (and rest) as input
    PORTD = 0x00; // clear bit D0
}

void init_timer(void)
// prescaling : max time = 2^16/16E6 = 4.1 ms, 4.1 >> 2.3, so no prescaling required
// normal mode, no prescale, stop timer
{
    TCCR1A = 0;
    TCCR1B = 0;
}

void init_ext_int(void)
{
    // any change triggers ext interrupt 1
    EICRA = (1 << ISC10);
    EIMSK = (1 << INT1);
}

uint16_t calc_cm(uint32_t counter)
{
    // counter 0 ... 65535, f = 16 MHz
	// prescaler = 1024 so multiply with 1024
	// divide by 16 because FCPU = 16MHz, builtin * 1000 * 1000 for uS
	// divide uS by 58 to convert to cm
	
	return ((uint16_t) (counter*1024/16)/58);
}

uint16_t get_distance() {
    uint16_t cm = 0;
    gv_echo = BEGIN; // set flag for ISR
    // start trigger pulse lo -> hi (D4)
    PORTD |= _BV(4);
    _delay_us(12); // micro sec
    // stop trigger pulse hi -> lo (D4)
    PORTD = 0x00;
    // wait 30 milli sec, gv_counter == timer1 (read in ISR)
    _delay_ms(30);
    cm = calc_cm(gv_counter);

    return cm;
}

ISR (INT1_vect)
{
    if (gv_echo == BEGIN) {
        // set timer1 value to zero and start counting
        // C Compiler automatically handles 16-bit I/O read/write operations in the correct order
        TCNT1 = 0;
        TCCR1B |= (1<<CS10) | (0<<CS11) | (1<<CS12); // set prescaler to 1024
        // clear flag
        gv_echo = END;
    } else {
        // stop counting and read value timer1
        TCCR1B = 0;
        gv_counter = TCNT1;
    }
}