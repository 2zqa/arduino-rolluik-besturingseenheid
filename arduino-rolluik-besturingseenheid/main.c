#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "mijn_serial.h"
#include "adc.h"
#include "afstand.h"

// The array of tasks
sTask SCH_tasks_G[SCH_MAX_TASKS];

// index van status task
int8_t status_index = -1;
int8_t previous_byte = -1;

float maximum_distance_cm = 100;
float minimum_distance_cm = 0.5;

float maximum_light_intensity = 100;
float minimum_light_intensity = 20;

float maximum_temperature = 25;
float minimum_temperature = 15;


/*------------------------------------------------------------------*-

SCH_Dispatch_Tasks()

This is the 'dispatcher' function.  When a task (function)
is due to run, SCH_Dispatch_Tasks() will run it.
This function must be called (repeatedly) from the main loop.

-*------------------------------------------------------------------*/

void SCH_Dispatch_Tasks(void)
{
    unsigned char Index;

    // Dispatches (runs) the next task (if one is ready)
    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if((SCH_tasks_G[Index].RunMe > 0) && (SCH_tasks_G[Index].pTask != 0))
        {
            (*SCH_tasks_G[Index].pTask)();  // Run the task
            SCH_tasks_G[Index].RunMe -= 1;   // Reset / reduce RunMe flag

            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if(SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
}

/*------------------------------------------------------------------*-

SCH_Add_Task()

Causes a task (function) to be executed at regular intervals
or after a user-defined delay

pFunction - The name of the function which is to be scheduled.
            NOTE: All scheduled functions must be 'void, void' -
            that is, they must take no parameters, and have
            a void return type.

DELAY     - The interval (TICKS) before the task is first executed

PERIOD    - If 'PERIOD' is 0, the function is only called once,
            at the time determined by 'DELAY'.  If PERIOD is non-zero,
            then the function is called repeatedly at an interval
            determined by the value of PERIOD (see below for examples
            which should help clarify this).


RETURN VALUE:

Returns the position in the task array at which the task has been
added.  If the return value is SCH_MAX_TASKS then the task could
not be added to the array (there was insufficient space).  If the
return value is < SCH_MAX_TASKS, then the task was added
successfully.

Note: this return value may be required, if a task is
to be subsequently deleted - see SCH_Delete_Task().

EXAMPLES:

Task_ID = SCH_Add_Task(Do_X,1000,0);
Causes the function Do_X() to be executed once after 1000 sch ticks.

Task_ID = SCH_Add_Task(Do_X,0,1000);
Causes the function Do_X() to be executed regularly, every 1000 sch ticks.

Task_ID = SCH_Add_Task(Do_X,300,1000);
Causes the function Do_X() to be executed regularly, every 1000 ticks.
Task will be first executed at T = 300 ticks, then 1300, 2300, etc.

-*------------------------------------------------------------------*/

unsigned char SCH_Add_Task(void (*pFunction)(), const unsigned int DELAY, const unsigned int PERIOD)
{
    unsigned char Index = 0;

    // First find a gap in the array (if there is one)
    while((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }

    // Have we reached the end of the list?
    if(Index == SCH_MAX_TASKS)
    {
        // Task list is full, return an error code
        return SCH_MAX_TASKS;
    }

    // If we're here, there is a space in the task array
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay =DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;

    // return position of task (to allow later deletion)
    return Index;
}

/*------------------------------------------------------------------*-

SCH_Delete_Task()

Removes a task from the scheduler.  Note that this does
*not* delete the associated function from memory:
it simply means that it is no longer called by the scheduler.

TASK_INDEX - The task index.  Provided by SCH_Add_Task().

RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL

-*------------------------------------------------------------------*/

unsigned char SCH_Delete_Task(const unsigned char TASK_INDEX)
{
    // Return_code can be used for error reporting, NOT USED HERE THOUGH!
    unsigned char Return_code = 0;

    SCH_tasks_G[TASK_INDEX].pTask = 0;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;

    return Return_code;
}

/*------------------------------------------------------------------*-

SCH_Init_T1()

Scheduler initialisation function.  Prepares scheduler
data structures and sets up timer interrupts at required rate.
You must call this function before using the scheduler.

-*------------------------------------------------------------------*/

void SCH_Init_T0(void)
{
    unsigned char i;

    for(i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_Delete_Task(i);
    }

    // Set up Timer 0
    // Values for 1ms and 10ms ticks are provided for various crystals

    // Hier moet de timer periode worden aangepast ....!
    TCCR0A |= (1 << WGM01);
    OCR0A = (uint8_t)250; // OCRn = 16000000/prescale/1000 * <gewenste tijd in ms>, waar OCRn een integer is en < 256
    TIMSK0 |= (1 << OCIE0A); // enable 
    TCCR0B |= (1 << CS02);
}

/*------------------------------------------------------------------*-

SCH_Start()

Starts the scheduler, by enabling interrupts.

NOTE: Usually called after all regular tasks are added,
to keep the tasks synchronised.

NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! ?????

-*------------------------------------------------------------------*/

void SCH_Start(void)
{
    sei();
}

/*------------------------------------------------------------------*-

SCH_Update

This is the scheduler ISR.  It is called at a rate
determined by the timer settings in SCH_Init_T1().

-*------------------------------------------------------------------*/

ISR(TIMER0_COMPA_vect)
{
    unsigned char Index;
    for(Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        // Check if there is a task at this location
        if(SCH_tasks_G[Index].pTask)
        {
            if(SCH_tasks_G[Index].Delay == 0)
            {
                // The task is due to run, Inc. the 'RunMe' flag
                SCH_tasks_G[Index].RunMe += 1;

                if(SCH_tasks_G[Index].Period)
                {
                    // Schedule periodic tasks to run again
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                    SCH_tasks_G[Index].Delay -= 1;
                }
            }
            else
            {
                // Not yet ready to run: just decrement the delay
                SCH_tasks_G[Index].Delay -= 1;
            }
        }
    }
}

// ------------------------------------------------------------------

void init_rolluik() {
    DDRB = 0xff;
}

/*
Stelt het lampje op groen in
*/
void oprollen() {
    PORTB = 0x01;
    transmit(10);
    if (status_index == -1) // als het lampje niet al knippert, laat het knipperen
    {
        transmit(20);
        status_index = SCH_Add_Task(status,0,10);
    }
}

void uitrollen() {
    PORTB = 0x02;
    if (status_index == -1) // als het lampje niet al knippert, laat het knipperen
    {
        status_index = SCH_Add_Task(status,0,10);
    }
    
}

/*
Schakel gele lampje
*/
void status() {
    PORTB ^= 0x04;
}

void stop_rollen() {
    PORTB = 0x00;
    if (status_index != -1) // als het lampje knippert, stop het
    {
        SCH_Delete_Task(status_index);
        status_index = -1;
    }
}

void process_serial() {
    if(UCSR0A & (1 << RXC0)) {
        uint8_t received_byte = UDR0;
        switch (received_byte) {
            case 0x01: // Uitrollen
                oprollen();
                break;
            case 0x02: // Inrollen
                uitrollen();
                break;
            case 0x03: // Stop met rollen
                stop_rollen();
                break;
        }
    }
    
}

float get_temperatuur() {
    return 0.48828125*get_adc_value(0)-50;
}

uint8_t get_light() {
    return get_adc_value(1);
}

   // wordt 1x per seconde aangeroepen
void send_temperature_info() {
    transmit((int8_t)get_temperatuur());
}

void send_distance_info() {
    transmit((int8_t)get_distance());
}

void send_light_info() {
    transmit(get_light());
}

void check_temperature() {
	//uint8_t tijdelijke_temperatuur = 30; // later: afstand = get_distance();
	
	if (get_temperatuur() > maximum_temperature)
	{
		uitrollen();
	} else if (get_temperatuur() < minimum_temperature) {
		oprollen();
	}
}

// Stop automatisch rollen gebaseerd op afstand van rolluik
void check_distance() {
    uint8_t huidige_tijdelijke_meetwaarde = 30; // later: afstand = get_distance();
	
	if (huidige_tijdelijke_meetwaarde > maximum_distance_cm && status_index != -1)
	{
		stop_rollen();
	} else if (huidige_tijdelijke_meetwaarde < minimum_distance_cm && status_index != -1){
		stop_rollen();
	}
}

//float get_light_intensity(){}
	
void check_light_intensity(){
	uint8_t tijdelijke_lichtmeetwaarde = 30; // later: lichtintensiteit = get_lightintensity();
	if (tijdelijke_lichtmeetwaarde > maximum_light_intensity) {
		uitrollen();
	} else if (tijdelijke_lichtmeetwaarde < minimum_light_intensity) {
		oprollen();
	}
}

//void send_light_intensity_info(){}

int main()
{
    // Inits
    init_adc();
    init_rolluik();
    uart_init();
    SCH_Init_T0();
    init_ports();
    init_timer();
    init_ext_int();
    
    // schakel scheduler in (interrupts)
    SCH_Start();
    
    // Taken
    SCH_Add_Task(process_serial,0,10); // commando's uitvoeren: oprollen, etc
    //SCH_Add_Task(send_temperature_info,0,100); // stuur temp in graden celcius
    SCH_Add_Task(check_distance,0,1); // WIP: stuur automatisch stop-commando's gebaseerd op afstand
	SCH_Add_Task(check_light_intensity,0,300);
    
    // Debug
    //SCH_Add_Task(send_distance_info,0,100);
    SCH_Add_Task(send_light_info,0,100);

    // Handel taken af
    while (1) {
        SCH_Dispatch_Tasks();
    }
    return 0;
}
