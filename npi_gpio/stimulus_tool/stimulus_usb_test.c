/*
 * stimulus_usb_test.c
 *
 *  Created on: Mar 8, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>

#include "stimulus_usb.h"
#include "debug_printf.h"
#include "stimulus_timer.h"
#include "mt_msg.h"


int random( int seed )
{
    int prime = 3;
    int constant = 6;
    int mask = 0xFF;

    seed = (seed * prime + constant);
    return (seed&mask);
}


void USB_test(void)
{
    int random_number = random(12);
    int timeout_timer = TIMEOUT_start();
    int delay_timer = TIMEOUT_start();

    int newVal;
    int usbVal;
    int random_delay;

    for (;;) {
        //
        // RANDOM NUMBER DELAY
        //
        if (random_number > 10) {
            random_delay = 0;
        }
        else {
            random_delay = random_number * 20;
        }
        random_number = random(random_number);

        //
        // DELAY
        //
        while (!TIMEOUT_expired(delay_timer, random_delay)) {
        }
        delay_timer = TIMEOUT_start();
        //DEBUG_printf("delay complete\n");

        //
        // RX/TX USB AND UART
        //
        if (UART_RxAvailable()) {
            newVal = UART_getc();
            USB_putc(newVal);
            UART_putc(newVal);
        }

        if (USB_RxAvailable()) {
            usbVal = USB_getc();
            UART_putc(usbVal);
            USB_putc(usbVal);
        }
    }
}
