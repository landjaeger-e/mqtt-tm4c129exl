/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 *
 * The use of the mongoose MQTT client is implemented with the mongoose-master exmample for the TM4C129 microcontroller
 */

// header.h containes additional include files and global variables
#include "header.h"

//************************************************************************************************************************
// Event handler
//
static void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;

    // char buffers for saving the topics and contents of the incoming messages
    char buf_top[20] = "";
    char buf_msg[20] = "";

    // Console output for follow up on the events
    if (ev != MG_EV_POLL) printf("USER HANDLER GOT EVENT %d\n", ev);

    switch (ev)
    {
    case MG_EV_CONNECT:
        if (*(int *) p != 0)
        {
            UARTprintf("Failed to connect to MQTT Broker %s\n",
            MQTT_BROKER_ADDRESS);
        }
        else
        {
            UARTprintf("CONNECT to MQTT broker %s\n", MQTT_BROKER_ADDRESS);
        }
        UARTprintf("Sending MQTT handshake with client ID: TM4C129E\n");
        struct mg_send_mqtt_handshake_opts opts;
        memset(&opts, 0, sizeof(opts)); // memory set for the authentication options

        opts.user_name = MQTT_USER_NAME;
        opts.password = MQTT_USER_PWD;
        opts.keep_alive = 10; // Frequency for the PINGREQ (ping request) of the client

        // set communication protocoll on mqtt
        mg_set_protocol_mqtt(nc);

        // sends mqtt handshake with authetication options to broker
        mg_send_mqtt_handshake_opt(nc, mqtt_client_name, opts);

        break;
    case MG_EV_MQTT_CONNACK: // gives feedback on received connection acknowledge
        if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED)
            UARTprintf("Got mqtt connection error %d\n\r",
                       msg->connack_ret_code);
        else
            UARTprintf("CONNACK from MQTT broker %s\n", MQTT_BROKER_ADDRESS);
        break;
    case MG_EV_MQTT_SUBACK: // gives feedback on the received subscription acknowledge
        UARTprintf("SUBACK from MQTT broker %s\n", MQTT_BROKER_ADDRESS);
        break;
    case MG_EV_MQTT_PUBLISH: // gives feedback on receiving mqtt message from broker
        UARTprintf("PUBLISH from MQTT broker %s\n", MQTT_BROKER_ADDRESS);
        break;
    case MG_EV_CLOSE: // gives feedback that connection to broker has been closed
        UARTprintf("Connection to broker is closed\r\n");
        state = lwIP_CloseConnection;
        break;
    case MG_EV_MQTT_PINGRESP: // feedback on PINGREQ
        UARTprintf("PINGRESP from Broker\n");
    default:
        break;
    }

    // mqtt state machine
    if(mqtt_state == sub)
    {
        // writes static template on the display
        //TODO: call static subscribe UI for the display


        // subscribing to all topics declared in topic_expressions
        //TODO: use mg_mqtt_subscribe() for subscription of all topic_expressions


        // sets the mqtt_state to the dynamic state of subscribing = receiving
        mqtt_state = receive;
    }
    else if(mqtt_state == receive) // MG_EV_MQTT_PUBLISH must be set for reveiving content of mqtt message
    {
        //TODO: check core temperature flag
        if(){
            // writes own dynamic value of core temperature
            //TODO: call dynamic core temperature UI for the display and reset flag


        }


        if (ev == MG_EV_MQTT_PUBLISH)
        {
            // copies the string topic and payload of the incoming message p in the buf_top and buf_msg
            strncpy(buf_top, msg->topic.p, (int) msg->topic.len);
            strncpy(buf_msg, msg->payload.p, (int) msg->payload.len);

            // checks if the topic is coreTemperature
            int splitter = strncmp(buf_top, topic_expressions[2].topic, 4);
            if(splitter == 0){
            // writes the received coreTemp value of the publisher on the display
                //TODO: call dynamic publisher core temperature UI for the display

            }
            else{
                // checks if the topic is the name of the Publisher
                int splitter = strncmp(buf_top, topic_expressions[0].topic, 6);
                if(splitter == 0){
                // writes the received name of the publisher on the display
                //TODO: call dynamic publisher device name UI for the display

                }
                else{
                // writes the received ambientLight value of the publisher on the display
                //TODO: call dynamic publisher ambient light UI for the display

                }
            }
            UARTprintf("Incoming message on topic: %s: %s\n", buf_top, buf_msg);
        }
    }
    else if(mqtt_state == unsub)
    {
        // writes static part of the publisher template on the display
        //TODO:  call static  publish UI for the display


        printf("Unsubscribing of topic: %s\n", topic_expressions[1].topic);

        // unsubscribes the second topic
        //TODO: call mg_mqtt_unsubscribe() from any topic



        mqtt_state = pub; // sets the state at dynamic state publish
    }
    else if(mqtt_state == pub)
    {
        //TODO: check the button 2 flag
        if ()
        {
            char dataDeviceName[] = mqtt_client_name;
            // publishes the board name
            //TODO: call mg_mqtt_publish() for publishing the device name and reset flag



        }
        //TODO: check ambient light flag
        if ()
        {
            // publishes the value of the actual ambient Light value
            //TODO: call mg_mqtt_publish() for publishing the ambient light and reset flag


            // writes the published value on the display
            //TODO: call the dynamic publish ambient light UI


        }
        //TODO: check core temperature flag
        if ()
        {
            // publishes the value of the actual coreTemp
            //TODO: call mg_mqtt_publish() for publishing the core temperature and reset flag



            // writes the published value on the display
            //TODO: call the dynamic publish core temperature UI

        }
    }
}

//******************************************************************************************************************************
// Main
//

int main(void)
{
    ticks = 0;

    // define first state
    state = lwIP_Init;

    mqtt_state = sub; // default state is subscribing

    // mqtt topics for subscribe and publish  {topic name, QoS}
    //TODO: define three topics for topic_expression[] with deviceName, ambientLight and coreTemperature




    // default values of flags for the buttons and timers
    g_ui32Buttons = 0;
    but1 = 0, but2 = 0, timerI2C = 0, timerTemp = 0;

    // Make sure the main oscillator is enabled because this is required by
    // the PHY.  The system must have a 25MHz crystal attached to the OSC
    // pins. The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal
    // frequency is 10MHz or higher.
    SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

    // Run from the PLL at 120 MHz.
    uint32_t sys_clk =
    MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                           120000000);

    // Configure the device pins for Ethernet (Ethernet, USB)
    PinoutSet(true, false);

    // Configures the display on the boosterpack 1 position
    CFAF128128B0145T_init(1, sys_clk, 20000000);


    // Configure UART.
    UARTStdioConfig(0, 115200, sys_clk);
    UARTprintf("\n-----------------------------\n");
    UARTprintf("MQTT example with Mongoose\n");
    UARTprintf("-----------------------------\n");

    // Configure board's LEDs
    MAP_GPIOPinTypeGPIOOutput(CLP_D1_PORT, CLP_D1_PIN);
    MAP_GPIOPinTypeGPIOOutput(CLP_D2_PORT, CLP_D2_PIN);
    LEDWrite(CLP_D1, 0);
    LEDWrite(CLP_D2, 0);

    // Configure the hardware MAC address for Ethernet Controller filtering of
    // incoming packets.  The MAC address will be stored in the non-volatile
    // USER0 and USER1 registers.
    uint32_t u0, u1;
    MAP_FlashUserGet(&u0, &u1);
    uint8_t mac[6];
    mac[0] = ((u0 >> 0) & 0xff);
    mac[1] = ((u0 >> 8) & 0xff);
    mac[2] = ((u0 >> 16) & 0xff);
    mac[3] = ((u1 >> 0) & 0xff);
    mac[4] = ((u1 >> 8) & 0xff);
    mac[5] = ((u1 >> 16) & 0xff);



    // Initialize the lwIP library, using DHCP.
    lwIPInit(sys_clk, mac, 0, 0, 0, IPADDR_USE_DHCP);
    UARTprintf("Initializing lwIP Stack...\n");

    // Initialize the buttons
    ButtonsInit();

    // Initialize Ambient Light Sensor
    OPT3001Init(sys_clk);
    // Configure Ambient Light Sensor
    OPT3001Config();

    // Initialize Temperature Sensor
    temperatureInit();

    //Timer
    I2CTimer(sys_clk);
    tempTimer(sys_clk);

    // Configure SysTick for a periodic interrupt.
    MAP_SysTickPeriodSet(sys_clk / SYSTICKHZ);
    MAP_SysTickEnable();
    MAP_SysTickIntEnable();

    // Loop forever
    while (true)
    {
        // provisioning state machine
        if (state == lwIP_ConnectToMQTT)
        {
            // Initialize the mongoose master
            mg_mgr_init(&mgr, NULL);

            // checks wheter connection to broker was successfully established
            if (mg_connect(&mgr, MQTT_BROKER_ADDRESS, ev_handler) == NULL)
            {
                UARTprintf("Failed to create TCP connection\n\r");
            }
            else
            {
                UARTprintf("TCP connection established\n\r");
                state = lwIP_MQTTReady;
            }
        }

        if (state == lwIP_MQTTReady)
        {
            // Setting the polling state every second
            mg_mgr_poll(&mgr, 1000);
            if (but1)
            {
                if(mqtt_state == receive){
                    mqtt_state = unsub; // mqtt mode switches from subscribing to publishing
                }
                if(mqtt_state == pub){
                    mqtt_state = sub;   // mqtt mode switches from publishing to subscribing
                }
                but1 = 0;
            }
        }
        if (state == lwIP_CloseConnection)
        {
            mg_mgr_free(&mgr); //mongoose-master releases used storage
            UARTprintf("Example finished\n\r");
        }
    }
}

//*************************************************************************************************************************
// Buttons
//

void AppButtonHandler(void)
{
    static uint32_t ui32TickCounter;
    ui32TickCounter++;

// switch upon which button was pressed
    switch (g_ui32Buttons & ALL_BUTTONS)
    {
    case TOP_BUTTON:
        // Check if the button has been held int32_t enough
        if ((ui32TickCounter % APP_BUTTON_POLL_DIVIDER) == 0)
        {
            UARTprintf("USR_SW1 button pressed.\n");
            but1 = 1;
        }
        break;

    case BOTTOM_BUTTON:
        // Check if the button has been held int32_t enough
        if ((ui32TickCounter % APP_BUTTON_POLL_DIVIDER) == 0)
        {
            UARTprintf("USR_SW2 button pressed.\n");
            but2 = 1;
        }
        break;

    case ALL_BUTTONS:
        // do nothing
        UARTprintf("Both buttons pressed.\n");
        break;

    default:
        // reset the tick counter when no buttons are pressed
        // this makes the first button reaction speed quicker
        ui32TickCounter = APP_BUTTON_POLL_DIVIDER - 1;
        break;
    }
}



void TimerInit(uint32_t ui32SysClkFreq)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   // enable Timer 0 peripheral

// Set up TIMER
// Configure Timer 0 to 32 bit periodic. When Timer 0 is configured as a 32-bit timer,
// it combines the two 16-bit timers Timer 0A and Timer 0B.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

// This calculated period is loaded into the Timer’s Interval Load register using the
// TimerLoadSet function of the driverLib Timer API. Note that you have to subtract
// one from the timer period since the interrupt fires at the zero count.
    TimerLoadSet(TIMER0_BASE, TIMER_A, ((ui32SysClkFreq * 2) - 1));

// Next, we have to enable the interrupt. Not only in the timer module, but also in the NVIC!
// TimerIntEnable() enables a specific event within the timer to generate an interrupt.
// In this case we are enabling an interrupt to be generated on a timeout of Timer0A.
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

// IntEnable() enables the specific vector associated with Timer0A.
    IntEnable(INT_TIMER0A);

// IntMasterEnable() is the master interrupt enable API for all interrupts.
    IntMasterEnable();

// Finally we enable the timer. This will start the timer and interrupts will immediately (!)
// begin triggering on the timeouts.
    TimerEnable(TIMER0_BASE, TIMER_A);
}

//*************************************************************************************************************************
// Timer Handler for sensors (AmbientLIght on boosterpack and inbuilt core temperature sesnor)
//
void AmbientLightHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timerI2C = 1; //sets flag for ambient light sensor publish
}
void TemperatureHandler(void){
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    timerTemp = 1; //sets flag for core temperature  sensor publish
}

//*************************************************************************************************************************
// Function needed by mongoose-master
//
int gettimeofday(struct timeval *tp, void *tzp)
{
    tp->tv_sec = ticks / SYSTICKHZ;
    tp->tv_usec = (ticks % SYSTICKHZ) * 1000;
    return 0;
}

void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr)
{
}


//************************************************************************************************************************
// lwIP
//

void SysTickIntHandler(void)
{
// Call the lwIP timer handler.
    lwIPTimer(SYSTICKMS);
    ticks++; // up counter for gettimeofday()

    g_ui32Buttons = ButtonsPoll(0, 0); // get buttons
    AppButtonHandler();               // debounce and do something
}

// Invoked by lwiplib every HOST_TMR_INTERVAL ms (defined in lwipopts.h).
void lwIPHostTimerHandler(void)
{
    static uint32_t old_ip = IP_LINK_DOWN;
    uint32_t ip = lwIPLocalIPAddrGet(); //gets to IP address
    if (ip != old_ip)
    {
        switch (ip)
        {
        case IP_LINK_DOWN:
        {
            UARTprintf("Ethernet link down.\r\n");
            LEDWrite(CLP_D2, 0); // sets LED output on the board
            break;
        }
        case IP_LINK_UP:
        {
            UARTprintf("Ethernet link up. Waiting for IP address...\n");
            LEDWrite(CLP_D2, 0);
            break;
        }
        default:
        {
            UARTprintf("Got IP from DHCP: %s\n",
                       ipaddr_ntoa((const ip_addr_t *) &ip));
            strncpy(ipArray, (ipaddr_ntoa((const ip_addr_t *) &ip)), 16);

            LEDWrite(CLP_D2, CLP_D2);

            // if IP from dhcp server received then change state to lqwIP_ConnectToMQTT
            state = lwIP_ConnectToMQTT;
            break;
        }
        }
        old_ip = ip;
    }
}

