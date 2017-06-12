/*
 * mt_msg_parsertest.c
 *
 *  Created on: Mar 21, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>

#include "stimulus_usb.h"
#include "debug_printf.h"
#include "stimulus_timer.h"
#include "mt_msg.h"



void MT_parserTest(void)
{
    int i;
    int packet[] = {0xfe, 0x03, 0x00, 0x11, 0x05, 0x20, 0x30, 0x47, 64,
                    0xfe, 0x02, 0x00, 0x12, 0x06, 0x01, 0x10, 7};
    int delay_timer = TIMEOUT_start();

    while (1) {
        for (i=0;i<(sizeof(packet)/4);i++) {
            MT_rxDecodeByte(packet[i]);

            //
            // DELAY
            //
            while (!TIMEOUT_expired(delay_timer, 500)) {
            }
            delay_timer = TIMEOUT_start();
            //DEBUG_printf("delay complete\n");


        }
    }
}
