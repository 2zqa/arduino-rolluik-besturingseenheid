#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#define F_CPU 16E6
#include <util/delay.h>

// output on USB = PD1 = board pin 1
// datasheet p.190; F_OSC = 16 MHz & baud rate = 19.200
#define UBBRVAL 51

void uart_init() {
	// set the baud rate
	UBRR0H = 0;
	UBRR0L = UBBRVAL;
	// disable U2X mode
	UCSR0A = 0;
	// enable transmitter and receiver
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// set frame format : asynchronous, 8 data bits, 1 stop bit, no parity
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

void transmit(uint8_t data) {
	// wait for an empty transmit buffer
	// UDRE is set when the transmit buffer is empty
	loop_until_bit_is_set(UCSR0A,UDRE0);
	// send the data
	UDR0 = data;
}

uint8_t receive() {
	loop_until_bit_is_set(UCSR0A,RXC0); // dit kan een tijdje duren, of zelfs oneindig!
	return UDR0;
}

int main(void) {
	uart_init();
	DDRD = 0xff; // stel op poort D alles in op "output"
	PORTD = 0xff; // zet lampjes aan aan
	while(1) {
		uint8_t received_bit = receive();
		//transmit(received_bit); // stuur terug wat je ontvangt, handig voor debugging
		if (received_bit == 0x01) {
			PORTD = 0xff;
		} else {
			PORTD = 0x00;
		}
	}
}