
#include <stdbool.h>
#include <stdint.h>
#include "mongoose/mongoose.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/lwiplib.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "ethernet/pinout.h"
#include "driverlib/timer.h"
#include "display/CFAF128128B0145T.h"
#include "sensorControl/ambientLight/Opt3001.h"
#include "sensorControl/internalTemperature/internalTemperature.h"
#include "sensorControl/buttons/buttons.h"
#include "display/boosterpackUI.h"

// MACROS
// system
#define SYSTICKHZ 100   // 100 Hz
#define SYSTICKMS (1000 / SYSTICKHZ) // 10 ms

// button
#define APP_BUTTON_POLL_DIVIDER 60

// Settings for MQTT Broker
#define MQTT_BROKER_ADDRESS "192.168.100.100:1883"
#define MQTT_USER_NAME NULL
#define MQTT_USER_PWD NULL

// MQTT State
#define receive 0
#define sub 1
#define unsub 2
#define pub 3

// Special IP address values that lwiplib uses.
#define IP_LINK_DOWN (0xffffffffU)
#define IP_LINK_UP (0)


// Device name for MQTT connection
//TODO: set client name (max. 19 char)
#define mqtt_client_name ""

// enumeration of states for lwIP and mqtt connecntion
enum lwIP_State
{
    lwIP_Init, lwIP_ConnectToMQTT, lwIP_MQTTReady, lwIP_CloseConnection
};
enum lwIP_State state;


// Global variables

static uint32_t ticks;      // counter for gettimeofday
char ipArray[20];           // array to save the IP address given by the DHCP server

    // buttons
volatile uint32_t g_ui32Buttons; // variable set by ButtonPoll, when buttons are pressed
int but1, but2;             // sets flag for button 1 and 2 on boosterpack
int timerI2C;               // sets flag for timer of interrupt of ambient light sensor
int timerTemp;              // sets flag for timer of interrupt of inbuilt temperature sensor of processor

    // variables for mongoose master
static struct mg_mgr mgr;   // event manager of momgoose master
static int msg_id;          // ID of message
struct mg_mqtt_topic_expression topic_expressions[3]; // topics for publish and subscribe { char[] "topic", int QoS}


int mqtt_state;             // flag to switch between publish and subscribe mode


// Prototypes

/** @brief Event Handler for mongoose master
*
* In the fucntion the event handler switches between the different events of the different communication with the broker
*
* @param *nc: points at connection to broker
* @param ev: flag of set events
* @param p: casted mg_mqtt_message
*/
static void ev_handler(struct mg_connection *nc, int ev, void *p);

/** @brief Button Handler
*
* Depending on which button was pushed, the handler sets flag for but1 or but2
*
*/
void AppButtonHandler(void);

/** @brief System Tick Interrupt Handler
*
* In this function the lwIP timer handler lwIPTimer() and the button handler AppButtonHandler() are called, the ticks for gettimeofday gets incremented and the value of ButtonPoll() is saved in g_ui32Buttons
*
*/
void SysTickIntHandler(void);

/** @brief Initialization of Timer
*
* In this function timer A gets initialized and started
*
* @param ui32SysClkFreq: System clock frequence
*/
void TimerInit(uint32_t ui32SysClkFreq);

/** @brief Handler for Ambient Light Sensor Measurement
*
* In this function the flag for the timer interrupt of the ambient light is set
*
*/
void AmbientLightHandler(void);

/** @brief Handler for Core Temperature Sensor Measurement
*
* In this function the flag for the timer interrupt of the inbuilt core temperature sensor is set
*
*/
void TemperatureHandler(void);

/** @brief Run-time Function
 *
 *The function measures the elapsed time and is needed for mongoose master to provide the ping request of the client
 *
 *@param *tp: timeval for elapsed time
 *@param *tzp: unused (was planed for time zones)
 */
int gettimeofday(struct timeval *tp, void *tzp);

/** @brief lwIP manger polling scheduler
*
* This function needs to be implemented on the TM4C129 for the mongoose.c file. There is just the prototype of the function.
*
* @param *mgr: points on the mongoose manager
*/
void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr);

/** @brief Timer Handler of the host server
*
* This function checks frequently if the ip address is still the same. By connecting it checks the state of the connection.
*
*/
void lwIPHostTimerHandler(void);

