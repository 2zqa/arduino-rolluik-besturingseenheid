#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#define F_CPU 16E6
#include <util/delay.h>

// Variabelen
int8_t previous_byte = 0;
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

// Vestuur licht- of temperatuurdata
void transmit_data(int8_t data) {
    transmit(30);
    transmit(rolluik_status);

    transmit(31);
    transmit(data);
}

void handle_single_command(uint8_t received_byte) {
    switch (received_byte) {
        case 0x01: // oprollen
            oprollen();
            break;
        case 0x02: // uitrollen
            uitrollen();
            break;
        case 0x03: // Stop met rollen
            stop_rollen();
            break;
        case 0x04:
            set_temperature_mode();
            break;
        case 0x05:
            set_light_mode();
            break;
//         case 0x06:
//             code hier
    }
}

// Voor wanneer er een variabele moet worden ingesteld
void handle_double_command(uint8_t previous_byte, uint8_t received_byte) {
    switch (previous_byte) {
        case 0x07: // maximumtemperatuur
            maximum_temperature = received_byte;
            break;
        case 0x08: // maximumlichteenheid
            maximum_light_intensity = received_byte;
            break;
        case 0x09: // maximumuitrolafstand
            maximum_distance = received_byte;
            break;
        case 0x0A: // minimumtemperatuur
            minimum_temperature = received_byte;
            break;
        case 0x0B: // minimumlichteenheid
            minimum_light_intensity = received_byte;
            break;
        case 0x0C:
            minimum_distance = received_byte;
            break;
    }
}

// Handel commando's af
void process_serial() {
    if(UCSR0A & (1 << RXC0)) { // Byte is ontvangen
        uint8_t received_byte = UDR0;
        if(previous_byte >= 0x07 && previous_byte <= 0x0C) {
            handle_double_command(previous_byte, received_byte);
            previous_byte = 0;
        } else {
            handle_single_command(received_byte);
            previous_byte = UDR0;
        }
    }
}