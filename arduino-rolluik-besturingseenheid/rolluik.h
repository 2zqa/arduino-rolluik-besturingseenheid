/*
 * rolluik.h
 *
 * Author: Marijn Kok, Haylee Drenth
 */

// Variabelen
int8_t status_index = -1;


//Zorgt dat poorten 8,9 en 10 output zijn
void init_rolluik() {
    DDRB |= (1 << 0) | (1 << 1) | (1 << 2);
}

// Schakelt groene lampje in
void oprollen() {
    if(rolluik_status != OPGEROLD) { // als het nog niet volledig is opgerold
	    rolluik_status = BEZIG_MET_OPROLLEN; // lampje wordt ROOD
        PORTB = (1 << 0);
	    start_rollen();
    }        
}

// Schakelt rode lampje in
void uitrollen() {
    if(rolluik_status != UITGEROLD) { // als het nog niet volledig is uitgerold
	    rolluik_status = BEZIG_MET_UITROLLEN; // bezig met uitrollen, lampje wordt GROEN
        PORTB = (1 << 1);
	    start_rollen();
    }        
}

// Laat gele lampje knipperen
void start_rollen() {
	if (status_index == -1) // als het lampje niet al knippert, laat het knipperen
	{
		status_index = SCH_Add_Task(toggle_status_led,0,125);
	}
}

// Schakelt gele lampje om
void toggle_status_led() {
    PORTB ^= (1 << 2); // XOR
}

// Stopt knipperen van gele lampje
void stop_rollen() {
    if (status_index != -1) // als het lampje knippert, stop het
    {
        SCH_Delete_Task(status_index);
        status_index = -1;
        PORTB &= ~(1 << 2);
        
        switch(rolluik_status) {
            case BEZIG_MET_UITROLLEN:
                rolluik_status = UITGEROLD;
                break;
            case BEZIG_MET_OPROLLEN:
                rolluik_status = OPGEROLD;
                break;
        }
    }
}
