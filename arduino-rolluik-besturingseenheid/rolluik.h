// Variabelen
int8_t status_index = -1;


//Zorgt dat poorten 8,9 en 10 output zijn
void init_rolluik() {
    DDRB |= (1 << 0) | (1 << 1) | (1 << 2);
}

// Schakelt groene lampje in
void oprollen() {
	rolluik_status = 3;
    PORTB = (1 << 0);
	start_rollen();
}

// Schakelt rode lampje in
void uitrollen() {
	rolluik_status = 3;
    PORTB = (1 << 1);
	start_rollen();
}

// Laat gele lampje knipperen
void start_rollen() {
	if (status_index == -1) // als het lampje niet al knippert, laat het knipperen
	{
		status_index = SCH_Add_Task(toggle_status_led,0,25);
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
    }
}
