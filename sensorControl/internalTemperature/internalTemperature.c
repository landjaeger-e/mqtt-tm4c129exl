/*
 * internalTemperature.c
 *
 *  Created on: May 28, 2019
 *      Author: sebas
 */
#include "internalTemperature.h"
#include "header.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <driverlib/fpu.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/debug.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>

uint32_t ui32SysClock;
uint32_t ui32ACCValues[1];
uint32_t ui32Temperature;
float_t flTempCelsius;
char strBuffer[30];

extern void TemperatureHandler();

void temperatureInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
}

char* getTemperature(){
    ADCProcessorTrigger(ADC0_BASE, 3);
    while( !ADCIntStatus(ADC0_BASE, 3, false))
    {
    }
    ADCSequenceDataGet(ADC0_BASE, 3, ui32ACCValues);
    flTempCelsius = 147.5 - ((75.0 * 3.3 * ui32ACCValues[0]) / 4096.0);
    sprintf(strBuffer, "%.2f", flTempCelsius);
    return strBuffer;
}

void tempTimer(int ui32SysClkFreq){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);   // enable Timer 0 peripheral

    // Set up TIMER
    // Configure Timer 0 to 32 bit periodic. When Timer 0 is configured as a 32-bit timer,
    // it combines the two 16-bit timers Timer 0A and Timer 0B.
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    // This calculated period is loaded into the Timer’s Interval Load register using the
    // TimerLoadSet function of the driverLib Timer API. Note that you have to subtract
    // one from the timer period since the interrupt fires at the zero count.
    TimerLoadSet(TIMER1_BASE, TIMER_A, ((ui32SysClkFreq * 1) - 1));

    // Next, we have to enable the interrupt. Not only in the timer module, but also in the NVIC!
    // TimerIntEnable() enables a specific event within the timer to generate an interrupt.
    // In this case we are enabling an interrupt to be generated on a timeout of Timer0A.
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // IntEnable() enables the specific vector associated with Timer0A.
    IntEnable(INT_TIMER1A);
    TimerIntRegister(TIMER1_BASE, TIMER_A, &TemperatureHandler);
    IntMasterEnable();

    // Finally we enable the timer. This will start the timer and interrupts will immediately (!)
    // begin triggering on the timeouts.
    TimerEnable(TIMER1_BASE, TIMER_A);
}


