void init_adc() {
    // Source: https://medium.com/@jrejaud/arduino-to-avr-c-reference-guide-7d113b4309f7
    // 16Mhz / 128 = 125kHz ADC reference clock
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
    
    // Voltage reference from AVcc (5V on ATMega328p)
    ADMUX |= (1<<REFS0);
    
    ADCSRA |= (1<<ADEN);    // Turn on ADC
    ADCSRA |= (1<<ADSC);    // Do a preliminary conversion
}

uint8_t get_adc_value(uint8_t pin) {
    // Source: https://medium.com/@jrejaud/arduino-to-avr-c-reference-guide-7d113b4309f7
    ADMUX &= 0xF0;    // Clear previously read channel
    ADMUX |= pin;    // Define new ADC Channel to read, analog pins 0 to 5 on ATMega328p
    
    ADCSRA |= (1<<ADSC);    // New Conversion
    ADCSRA |= (1<<ADSC);    // Do a preliminary conversion
    
    // Wait until conversion is finished
    while(ADCSRA & (1<<ADSC));
    
    // Return ADC value
    return ADCW;
}