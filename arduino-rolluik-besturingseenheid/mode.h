// Variabelen
int8_t mode_index = -1;


void set_temperature_mode(){
	if (mode_index > -1)
	{
	    SCH_Delete_Task(mode_index); // Stop met sturen van temperatuurdata
	}
	if (send_index > -1)
	{
		SCH_Delete_Task(send_index);
	}
    send_index = SCH_Add_Task(add_temperature,0,1000); // TODO: 0 toevoegen aan period
    mode_index = SCH_Add_Task(send_temperature_info,0,1500); // Start met sturen van temperatuurdata // TODO: 0 toevoegen aan period
}

void set_light_mode() {
	if (mode_index > -1)
	{
		SCH_Delete_Task(mode_index); // Stop met sturen van lichtdata
	}
	if (send_index > -1)
	{
		SCH_Delete_Task(send_index);
	}
    send_index = SCH_Add_Task(add_light,0,750); // TODO: 0 toevoegen aan period
    mode_index = SCH_Add_Task(send_light_info,0,1500); // Start met sturen van lichtdata  // TODO: 0 toevoegen aan period
}

// Temperatuurmodus
void send_temperature_info() {
    transmit_data(calculate_float_average(temperaturevalues,5,temperature_count));
}

// Lichtmodus
void send_light_info() {
    transmit_data(calculate_uint8_average(lightvalues,5,light_count));
}