/*
 * mode.h
 *
 * Author: Marijn Kok, Haylee Drenth
 */

// Variabelen
int8_t mode_index = -1;

void set_temperature_mode(){
    // Verwijder andere add- en send-functies
    if (mode_index > -1) {
	    SCH_Delete_Task(mode_index); // Stop met meten van temperatuur (voor gemiddelden, niet autonomie)
	}
	if (send_index > -1) {
		SCH_Delete_Task(send_index); // Stop met sturen van temperatuurdata
	}
    mode = 1;

    mode_index = SCH_Add_Task(add_temperature,0,10000); // 10000 * 4ms = 40s
    send_index = SCH_Add_Task(send_temperature_info,0,15000); // 15000 * 4ms = 60s
}

void set_light_mode() {
    if (mode_index > -1) {
        SCH_Delete_Task(mode_index); // Stop met meten van temperatuur (voor gemiddelden, niet autonomie)
    }
    if (send_index > -1) {
        SCH_Delete_Task(send_index); // Stop met sturen van temperatuurdata
    }
    mode = 0;
    mode_index = SCH_Add_Task(add_light,0,7500); // 7500 * 4ms = 30s
    send_index = SCH_Add_Task(send_light_info,0,15000); // 15000 * 4ms = 60s
}

// Temperatuurmodus
void send_temperature_info() {
    transmit_data(calculate_float_average(temperaturevalues,5,temperature_count));
}

// Lichtmodus
void send_light_info() {
    transmit_data(calculate_uint8_average(lightvalues,5,light_count));
}
