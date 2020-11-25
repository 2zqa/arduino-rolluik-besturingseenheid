void check_data() {
    check_distance();
    switch (mode)
    {
        case 0: // lichtmodus
            check_light_intensity();
            break;
        case 1: // temperatuurmodus
            check_temperature();
    	    break;
    }
}

// Stop automatisch rollen gebaseerd op afstand van rolluik
void check_distance() {
    uint8_t meetwaarde = get_distance();
    
    if (meetwaarde > maximum_distance && status_index != -1 && rolluik_status == 3) // Wanneer maximumafstand overschreden is en het lampje knippert
    {
        stop_rollen();
        rolluik_status = 1; // uitgerold (blokkeert zon)
    } else if (meetwaarde < minimum_distance && status_index != -1 && rolluik_status == 2) { // Wanneer minimumafstand overschreden is en het lampje knippert
        stop_rollen();
        rolluik_status = 0; // opgerold (zon komt binnen)
    }
}

// Start automatisch rollen gebaseerd op temperatuur
void check_temperature() {
	uint8_t temperatuur = get_temperatuur();
	if (temperatuur > maximum_temperature && status_index == -1) {
		uitrollen();
    } else if (temperatuur < minimum_temperature && status_index == -1) {
		oprollen();
	}
}

// Rolt automatisch rolluik in of uit gebaseerd op licht
void check_light_intensity(){
	uint8_t lichtmeetwaarde = get_light();
	if (lichtmeetwaarde > maximum_light_intensity && status_index == -1 && rolluik_status != 4 && rolluik_status != 2) { // wanneer het te licht is, maak het donker. Kan niet wanneer het uitrolt/is uitgerold
		uitrollen();
    } else if (lichtmeetwaarde < minimum_light_intensity && status_index == -1 && rolluik_status != 3 && rolluik_status != 1) { // wanneer het te donker is, maak het lichter. Kan niet wanneer het oprolt/is opgerold
		oprollen();
	}
}