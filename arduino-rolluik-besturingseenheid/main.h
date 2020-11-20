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

// rolluik functies
void init_rolluik();
void oprollen();
void uitrollen();
void toggle_status_led();
void stop_rollen();
void start_rollen();

// Serial functies
void process_serial();

// Temperatuur functies
void init_temperatuur();
void send_temperature_info();
float get_temperatuur();
void set_temperature_mode();

// Afstand functies
void init_distance_sensor();
void check_distance();

// Lichtintensiteit functies
void check_light_intensity();
uint8_t get_light();
void set_light_mode();

// Overige functies
float calculate_uint8_average(uint8_t array[], uint8_t len);
float calculate_float_average(float array[], uint8_t len);
void add_uint8_to_array(uint8_t waarde, uint8_t array[], uint8_t len);
void add_float_to_array(float waarde, float array[], uint8_t len);

// hier het aantal taken aanpassen ....!!
// Maximum number of tasks

#define SCH_MAX_TASKS (6)
