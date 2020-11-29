/*
 * autonomy.h
 *
 * Author: Marijn Kok, Haylee Drenth
 */

void check_data() {
    switch (mode)
    {
        case 0: // lichtmodus
            check_light_intensity();
            break;
        case 1: // temperatuurmodus
            check_temperature();
    	    break;
    }
    check_distance();
}

// Stop automatisch rollen gebaseerd op afstand van rolluik
void check_distance() {
    uint8_t meetwaarde = get_distance();
    
    if (meetwaarde > maximum_distance && rolluik_status == BEZIG_MET_UITROLLEN) { // Wanneer rolluik ver genoeg is uitgerold (en bezig was met uitrollen) (afstand HOOG)
        stop_rollen();
    } else if (meetwaarde < minimum_distance && rolluik_status == BEZIG_MET_OPROLLEN) { // Wanneer rolluik ver genoeg is opgerold (en bezig was met oprollen) (afstand LAAG)
        stop_rollen();
    }
}

// Start automatisch rollen gebaseerd op temperatuur
void check_temperature() {
	uint8_t temperatuur = get_temperatuur();
	if (temperatuur > maximum_temperature) {
		uitrollen();
    } else if (temperatuur < minimum_temperature) {
		oprollen();
	}
}

// Rolt automatisch rolluik in of uit gebaseerd op licht
void check_light_intensity(){
	uint8_t lichtmeetwaarde = get_light();
	if (lichtmeetwaarde > maximum_light_intensity && status_index == -1) { // wanneer het te licht is, maak het donker. Kan niet wanneer het uitrolt/is uitgerold
		uitrollen();
    } else if (lichtmeetwaarde < minimum_light_intensity && status_index == -1) { // wanneer het te donker is, maak het lichter. Kan niet wanneer het oprolt/is opgerold
		oprollen();
	}
}

void set_autonomously() {
    if(check_data_index == -1) {
        check_data_index = SCH_Add_Task(check_data,0,1); // controleer variabelen en stuur autonoom rolluik aan
    }
}

void unset_autonomously() {
    if (check_data_index != -1) {
        SCH_Delete_Task(check_data_index);
        check_data_index = -1;
    }
}