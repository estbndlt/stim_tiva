//*****************************************************************************
//
// main.c - Simple MT interface.
//
// Copyright (c) 2012-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.3.156 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

//
// INCLUDES
//
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_types.h"



#include "stimulus_tool/debug_printf.h"
#include "stimulus_tool/hw_init.h"
#include "stimulus_tool/mt_msg.h"
#include "stimulus_tool/stimulus_gpio.h"
#include "stimulus_tool/stimulus_timer.h"
#include "stimulus_tool/stimulus_usb.h"

//
// MACROS AND DEFINITIONS
//
#define RED_LED   0x10000000
#define BLUE_LED  0x20000000
#define GREEN_LED 0x40000000


//*****************************************************************************
//
// MAIN FUNCTION
//
//*****************************************************************************
int main(void)
{
    //
    // INIT SECTION
    //
    HW_init();


    //
    // TEST SECTION
    //
    // GPIO_test();    // toggles 7 LED colors three times and then blink white three times
    // TIMER_test();   // blinks green LED three times at one second intervals
    // MT_parserTest();      // ensures the parser can read multiple packets without errors
    // USB_test();     // echoes chars on UART and USB with random delay between bytes
    MT_responseTest(); // sends command response packets and ping response when timeout occurs
    // Stim_device();  // TODO



    //
    // DECLARATIONS
    //


    //
    // MAIN LOOP
    //
    while (1)
    {

    }
}
