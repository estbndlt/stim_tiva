/*
 * mt_msg_responsetest.c
 *
 *  Created on: Mar 23, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>

#include "stimulus_usb.h"
#include "debug_printf.h"
#include "stimulus_timer.h"
#include "mt_msg.h"
#include "stimulus_gpio.h"

#define RX_TIMEOUT_VALUE    15
#define PING_TIMEOUT_VALUE  100

void TIMEOUT_init( void )
{
    rx_timer = TIMEOUT_start();
    ping_timer = TIMEOUT_start();
}


int TIMEOUT_rx( void )
{
    if (TIMEOUT_expired(rx_timer, RX_TIMEOUT_VALUE)) {
        rx_timer = TIMEOUT_start();
        return 1;
    }
    else {
        return 0;
    }
}

int TIMEOUT_ping( void )
{
    if (TIMEOUT_expired(ping_timer, PING_TIMEOUT_VALUE)) {
        ping_timer = TIMEOUT_start();
        return 1;
    }
    else {
        return 0;
    }
}

void TIMER_delay(int milliseconds)
{
    delay_timer = TIMEOUT_start();
    while (!TIMEOUT_expired(delay_timer, milliseconds)) {
    }
    delay_timer = TIMEOUT_start();
}

void MT_responseTest(void)
{
    TIMEOUT_init();

    int usbVal;

//    struct pingStruct pingCmd = {0,0};

    for (;;) {
        //
        // DELAY
        //
        //TIMER_delay(500);
        //GPIO_cmd(GPIO_COMMAND_TGL|GPIO_REGISTER_DATA, 0x20000000);

        //
        // RX/TX USB ECHO
        //
        if (USB_RxAvailable()) {
            usbVal = USB_getc();
            // UART_putc(usbVal);
            //USB_putc(usbVal);
            MT_rxDecodeByte(usbVal);
        }

        //
        // PROCESS INVALID PACKET
        //
        if(TIMEOUT_rx()) {
//            GPIO_cmd(GPIO_COMMAND_TGL|GPIO_REGISTER_DATA, 0x40000000);
            MT_rxDecodeByte(-1);
        }

        //
        // PROCESS VALID PACKET
        //
        if (MT_validPacket()) {
            //GPIO_cmd(GPIO_COMMAND_TGL|GPIO_REGISTER_DATA, 0x20000000);
            MT_processPacket();
        }

        //
        // PING RESPONSE AFTER TIMEOUT
        //
//        if (/*(pingCmd.repeat != 0) &&*/ TIMEOUT_ping()) {
//            MT_sendPing();
////            GPIO_cmd(GPIO_COMMAND_TGL|GPIO_REGISTER_DATA, 0x40000000);
//        }
    }
}



