float get_temperatuur() {
    return 0.48828125*get_adc_value(0)-50;
}

uint8_t get_light() {
    return get_adc_value(1);
}

void add_temperature(){
    add_float_to_array(get_temperatuur(),temperaturevalues,5);
}

void add_light(){
    add_uint8_to_array(get_light(),lightvalues,5);
}

void add_uint8_to_array(uint8_t waarde, uint8_t array[], uint8_t len) {
    // Verplaats alle indexen in de array eentje naar rechts, overschrijf de oudste
    // Voeg waarde toe aan array op index 0
    for(uint8_t i = len-1; i > 0; i--){
        array[i] = array[i-1];
    }
    array[0] = waarde;
}

void add_float_to_array(float waarde, float array[], uint8_t len) {
    // Verplaats alle indexen in de array eentje naar rechts, overschrijf de oudste
    // Voeg waarde toe aan array op index 0
    for(uint8_t i = len-1; i > 0; i--){
        array[i] = array[i-1];
    }
    array[0] = waarde;
}

float calculate_uint8_average(uint8_t array[], uint8_t len) {
    uint8_t sum = 0;
    for (int i = 0; i < len; i++){
        sum += array[i];
    }
    return sum/len;
}

float calculate_float_average(float array[], uint8_t len) {
    uint8_t sum = 0;
    for (int i = 0; i < len; i++) {
        // tel -255 niet mee in gemiddelden
        if(array[i] != -255) {
            sum += array[i];
            } else {
            len--;
        }
    }
    return sum/len;
}