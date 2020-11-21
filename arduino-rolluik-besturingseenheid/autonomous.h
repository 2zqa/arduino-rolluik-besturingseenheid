// Variabelen
float maximum_distance_cm = 100;
float minimum_distance_cm = 0.5;

float maximum_light_intensity = 200;
float minimum_light_intensity = 100;

float maximum_temperature = 25;
float minimum_temperature = 15;


// Stop automatisch rollen gebaseerd op afstand van rolluik
void check_distance() {
    uint8_t meetwaarde = get_distance();
    
    if (meetwaarde > maximum_distance_cm && status_index == -1)
    {
        stop_rollen();
        rolluik_status = 2;
        } else if (meetwaarde < minimum_distance_cm && status_index == -1){
        stop_rollen();
        rolluik_status = 1;
    }
}