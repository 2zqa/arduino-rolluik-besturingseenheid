/*
 * main.c
 *
 * Author: Onbekend (AVR_TTC_scheduler/basiscode), Marijn Kok, Haylee Drenth
 */

// Scheduler data structure for storing task data
typedef struct
{
   // Pointer to task
   void (* pTask)(void);
   // Initial delay in ticks
   unsigned int Delay;
   // Periodic interval in ticks
   unsigned int Period;
   // Runme flag (indicating when the task is due to run)
   unsigned char RunMe;
} sTask;

// Function prototypes
//-------------------------------------------------------------------

void SCH_Init_T0(void);
void SCH_Start(void);
// Core scheduler functions
void SCH_Dispatch_Tasks(void);
unsigned char SCH_Add_Task(void (*)(void), const unsigned int, const unsigned int);
unsigned char SCH_Delete_Task(const unsigned char);

// Globale variabelen
uint8_t rolluik_status = 0;
int8_t send_index = -1;
int8_t check_data_index = -1; // scheduler index voor controleren van temperatuur of lichtdata voor autonomie
int8_t check_distance_index = -1; // scheduler index voor controleren afstand
float temperaturevalues[5] = { 0 };
uint8_t temperature_count = 0;
uint8_t lightvalues[5] = { 0 };
uint8_t light_count = 0;
uint8_t mode = 1; // 0 = lichtmodus, 1 = temperatuurmodus
#define UITGEROLD 0 // afstand is HOOG
#define OPGEROLD 1 // afstand is LAAG
#define BEZIG_MET_UITROLLEN 2 // afstand wordt GROTER
#define BEZIG_MET_OPROLLEN 3 // afstand wordt KLEINER

// Autonomy
void check_temperature();
void check_light_intensity();
void check_distance();
void check_data();
void set_autonomously();
void unset_autonomously();
uint8_t maximum_distance = 20;
uint8_t minimum_distance = 8;

uint8_t maximum_light_intensity = 200;
uint8_t minimum_light_intensity = 50;

int8_t maximum_temperature = 24;
int8_t minimum_temperature = 19;

// Adc
void init_adc();
uint8_t get_adc_value(uint8_t pin);

// Rolluik
void init_rolluik();
void oprollen();
void uitrollen();
void toggle_status_led();
void stop_rollen();
void start_rollen();

// Mode
void set_light_mode();
void set_temperature_mode();
void send_light_info();
void send_temperature_info();

// Serial functies
void process_serial();
void handle_double_command(uint8_t previous_byte, uint8_t received_byte);
void handle_single_command(uint8_t received_byte);

// Temperatuur functies
void init_temperatuur();
float get_temperatuur();

// Afstand functies
void init_distance_sensor();

// Lichtintensiteit functies
uint8_t get_light();

// Overige functies
float calculate_uint8_average(uint8_t array[], uint8_t len, uint8_t counter);
float calculate_float_average(float array[], uint8_t len, uint8_t counter);
void add_uint8_to_array(uint8_t waarde, uint8_t array[], uint8_t len, uint8_t *counter);
void add_float_to_array(float waarde, float array[], uint8_t len, uint8_t *counter);

// Maximum number of tasks
#define SCH_MAX_TASKS (6) // process_serial, check_data, toggle_status_led, (add_temperature+send_temperature OF add_light+send_light_info, ofwel 2) maakt 5. +1 voor margin
