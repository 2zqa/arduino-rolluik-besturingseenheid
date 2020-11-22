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
float temperaturevalues[5] = { 0 };
uint8_t temperature_count = 0;
uint8_t lightvalues[5] = { 0 };
uint8_t light_count = 0;

// Autonomy
uint8_t maximum_distance = 100;
uint8_t minimum_distance = 2;

uint8_t maximum_light_intensity = 200;
uint8_t minimum_light_intensity = 10;

int8_t maximum_temperature = 25;
int8_t minimum_temperature = 15;

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
void check_distance();

// Lichtintensiteit functies
void check_light_intensity();
uint8_t get_light();

// Overige functies
float calculate_uint8_average(uint8_t array[], uint8_t len, uint8_t counter);
float calculate_float_average(float array[], uint8_t len, uint8_t counter);
void add_uint8_to_array(uint8_t waarde, uint8_t array[], uint8_t len, uint8_t *counter);
void add_float_to_array(float waarde, float array[], uint8_t len, uint8_t *counter);

// hier het aantal taken aanpassen ....!!
// Maximum number of tasks

#define SCH_MAX_TASKS (6)
