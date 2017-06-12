/*
 * hw_init.c
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "hw_init.h"


static void GPIO_init( void )
{

    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                   SYSCTL_OSC_MAIN);

    //
    // Enable and wait for the port to be ready for access
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}

    //
    // CONFIGURE PORT F
    //
    HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + (0xFF << 2))) = 0xFF;
    HWREG(GPIO_PORTF_BASE+GPIO_O_DIR) = HWREG(GPIO_PORTF_BASE+GPIO_O_DIR)|0xFF; // output
    HWREG(GPIO_PORTF_BASE+GPIO_O_AFSEL) = HWREG(GPIO_PORTF_BASE+GPIO_O_AFSEL)&~0xFF; // alt func
    HWREG(GPIO_PORTF_BASE+GPIO_O_DR8R) = HWREG(GPIO_PORTF_BASE+GPIO_O_DR8R)|0xFF; // set 8mA

    //
    // PLAYING AROUND
    //
    HWREG(GPIO_PORTF_BASE+GPIO_O_PUR) = HWREG(GPIO_PORTF_BASE+GPIO_O_PUR)|0x04; // set pur
    HWREG(GPIO_PORTF_BASE+GPIO_O_PUR) = HWREG(GPIO_PORTF_BASE+GPIO_O_PUR)&~0x04; // set pur
    HWREG(GPIO_PORTF_BASE+GPIO_O_PUR) = HWREG(GPIO_PORTF_BASE+GPIO_O_PUR)|0x02; // set pur
    HWREG(GPIO_PORTF_BASE+GPIO_O_PUR) = HWREG(GPIO_PORTF_BASE+GPIO_O_PUR)|0x08; // set pur
    HWREG(GPIO_PORTF_BASE+GPIO_O_PDR) = HWREG(GPIO_PORTF_BASE+GPIO_O_PDR)|0x0A; // set pdr
    HWREG(GPIO_PORTF_BASE+GPIO_O_DEN) = HWREG(GPIO_PORTF_BASE+GPIO_O_DEN)|0x0E; // set den
    //HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + (0x0E << 2))) = 0x00;

}


static void TIMER_init( void )
{

    //
    // CONFIGURE TIMER TO GO OFF AFTER X AMOUNT OF TIME
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {}
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    uint32_t ui32Period;
    ui32Period = (SysCtlClockGet()/1000);// 1ms timer // / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);

}

static void UART_init(void)
{

    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);

}

void HW_init( void )
{
    GPIO_init();
    TIMER_init();
    UART_init();
    USB_init();
}
