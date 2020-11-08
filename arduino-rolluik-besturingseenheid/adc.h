/*
 * adc.h
 *
 * Initialiseer en lees van analoge poorten
 *
 *  Author: school
 */ 
void init_adc();
uint8_t get_adc_value();

void init_adc() {
    // ref=Vcc, left adjust the result (8-bit resolution),
    // select channel 0 (PC0 = input)
    ADMUX = (1<<REFS0)|(1<<ADLAR);
    // enable the ADC & prescale= 128
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint8_t get_adc_value() {
    ADCSRA|=(1<<ADSC); // start conversion
    loop_until_bit_is_clear(ADCSRA,ADSC);
    return ADCH; // 8-bit resolution, left adjusted
}