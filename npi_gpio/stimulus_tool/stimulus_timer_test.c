/*
 * stimulus_timer_test.c
 *
 *  Created on: Mar 8, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"

#include "debug_printf.h"
#include "stimulus_timer.h"


void TIMER_test(void)
{
    DEBUG_printf("Timer Test Initializing...\n");
    uint32_t timer_restart = TIMEOUT_start();
    int blink_count = 0;

    while (blink_count < 6) {
        if (TIMEOUT_expired(timer_restart, 1000)) {
            // print at each second
            DEBUG_printf("Time (seconds): %d\n", blink_count+1);
            //HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + (0x0E << 2))) ^= 0x08;
            timer_restart = TIMEOUT_start();
            blink_count++;
        }
    }
    DEBUG_printf("Timer Test Complete\n\n");
}
