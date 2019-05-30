/*
 * Opt3001.c
 *
 *  Created on: May 9, 2019
 *      Author: sebas
 */
#include "Opt3001.h"
#include "header.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <inc/hw_ints.h>
#include <inc/hw_i2c.h>
#include <driverlib/sysctl.h>
#include <driverlib/debug.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>

#define OPT3001_I2C_ADDRESS             0x44
#define REG_RESULT                      0x00
#define REG_CONFIGURATION               0x01
#define REG_MANUFACTURER_ID             0x7E
#define REG_DEVICE_ID                   0x7F

uint16_t ui16Result;        // raw value of ambient light intensity
uint16_t ui16Exponent;      // raw value of ambient light intensity (exponent)
uint32_t ui32SysClkFreq;    // SysClock frequency
uint8_t buffer[2];          // buffer for reading data
char strBuffer[30];         // for itoa()
char id[3]={0};             // manufacturer ID


void OPT3001Init(int ui32SysClkFreq){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
            SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);                        // reset I2C if it has already been activated
            SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            SysCtlDelay(3); // wait a bit before accessing

            GPIOPinConfigure(GPIO_PB2_I2C0SCL);
            GPIOPinConfigure(GPIO_PB3_I2C0SDA);
            GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
            GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

            // Configure GPIOs (LEDs)
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
            SysCtlDelay(3); // wait a bit before accessing

            GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x00); // switch off LEDs

            // Initialize I2C master peripheral, false = 100 kBits/s, true = 400 kBit/s
            I2CMasterInitExpClk(I2C0_BASE, ui32SysClkFreq, false);

            SysCtlDelay(10000);         // wait a bit
}

void OPT3001ReadManufacturerID(char id[3])
{
    // Set register address to manufacturer ID  (single send)
    I2CMasterSlaveAddrSet(I2C0_BASE, OPT3001_I2C_ADDRESS, false);
    I2CMasterDataPut(I2C0_BASE, REG_MANUFACTURER_ID);                 // manuf. ID reg.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy

    SysCtlDelay(100000);                        // wait >20 ms before reading

    // Read content (2x8 bit) of selected register
    I2CMasterSlaveAddrSet(I2C0_BASE, OPT3001_I2C_ADDRESS, true);
    // Receive first data byte
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy

    id[0] = I2CMasterDataGet(I2C0_BASE);           // first data byte (MSB)

    // Receive second/last data byte
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy

    id[1] = I2CMasterDataGet(I2C0_BASE);                // second data byte (LSB)
}

void OPT3001Config(void)
{
    // Set register address to config register (burst send)
    I2CMasterSlaveAddrSet(I2C0_BASE, OPT3001_I2C_ADDRESS, false);
    I2CMasterDataPut(I2C0_BASE, REG_CONFIGURATION);                 // config reg.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy

    // Send first data byte
    I2CMasterDataPut(I2C0_BASE, 0b11001110);                // first config byte: see above
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy

    // Send second/last data byte
    I2CMasterDataPut(I2C0_BASE, 0b00010000);                 // second config byte: see above
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    SysCtlDelay(500);                           // wait 500
    while (I2CMasterBusy(I2C0_BASE)) { }        // check if I2C still busy
}
char* itoa(int val, char *a)
{
    char *e=a,*o=a,sign=val<0;
    do *e++="0123456789"[abs(val%10)]; while( val/=10 );
    for( sign?*e++='-':1,*e--=0; a < e; *a ^= *e,*e ^= *a,*a++ ^= *e-- );
    return o;
}

char* OPT3001Value(void){
            // bit 7 was set, so lets read out the light intensity
            // Set register address to result register (single send)
            I2CMasterSlaveAddrSet(I2C0_BASE, OPT3001_I2C_ADDRESS, false);
            I2CMasterDataPut(I2C0_BASE, REG_RESULT);                    // result reg.
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
            SysCtlDelay(500);                               // wait 500
            while (I2CMasterBusy(I2C0_BASE)) { }            // check if I2C still busy

            SysCtlDelay(100000);                            // wait >20 ms before reading

            // Read content (2x8 bit) of selected register
            I2CMasterSlaveAddrSet(I2C0_BASE, OPT3001_I2C_ADDRESS, true);
            // receive the data back (first byte)
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
            SysCtlDelay(500);
            while (I2CMasterBusy(I2C0_BASE)) { }

            buffer[0] = I2CMasterDataGet(I2C0_BASE);                                 // read out first data byte

            // last data byte
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
            SysCtlDelay(500);                               // wait 500
            while (I2CMasterBusy(I2C0_BASE)) { }            // check if I2C still busy

            buffer[1] = I2CMasterDataGet(I2C0_BASE);                     // read out second data byte

            // data is read out, switch on both LEDs
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x03);

            // combine 2x8 bit data into 16 bit value
            ui16Result = buffer[0];
            ui16Result <<= 8;
            ui16Result |= buffer[1];
            ui16Exponent = (ui16Result >> 12) & 0x000F;
            ui16Result = ui16Result & 0x0FFF;

            //convert raw readings to LUX
            switch(ui16Exponent)
            {
                case 0: //*0.015625
                    ui16Result = ui16Result>>6;
                    break;
                case 1: //*0.03125
                    ui16Result = ui16Result>>5;
                    break;
                case 2: //*0.0625
                    ui16Result = ui16Result>>4;
                    break;
                case 3: //*0.125
                    ui16Result = ui16Result>>3;
                    break;
                case 4: //*0.25
                    ui16Result = ui16Result>>2;
                    break;
                case 5: //*0.5
                    ui16Result = ui16Result>>1;
                    break;
                case 6:
                    ui16Result = ui16Result;
                    break;
                case 7: //*2
                    ui16Result = ui16Result<<1;
                    break;
                case 8: //*4
                    ui16Result = ui16Result<<2;
                    break;
                case 9: //*8
                    ui16Result = ui16Result<<3;
                    break;
                case 10: //*16
                    ui16Result = ui16Result<<4;
                    break;
                case 11: //*32
                    ui16Result = ui16Result<<5;
                    break;
            }
            sprintf(strBuffer, "%u    ", ui16Result);
   return strBuffer;
}
void I2CTimer(int ui32SysClkFreq)
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

    // Finally we enable the timer. This will start the timer and interrupts will immediately (!)
    // begin triggering on the timeouts.
    TimerEnable(TIMER0_BASE, TIMER_A);
}
