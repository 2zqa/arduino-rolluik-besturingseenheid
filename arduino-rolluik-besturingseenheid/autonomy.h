// Stop automatisch rollen gebaseerd op afstand van rolluik
void check_distance() {
    uint8_t meetwaarde = get_distance();
    
    if (meetwaarde > maximum_distance && status_index == -1)
    {
        stop_rollen();
        rolluik_status = 2;
        } else if (meetwaarde < minimum_distance && status_index == -1){
        stop_rollen();
        rolluik_status = 1;
    }
}