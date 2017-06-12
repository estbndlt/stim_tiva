/*
 * stimulus_timer.c
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"

#include "stimulus_timer.h"



extern intptr_t global_timer = 0;

intptr_t TIMEOUT_start(void)
{
    return global_timer;
}

int TIMEOUT_expired(intptr_t start_token, uint32_t timeout)
{
    uint32_t current_time = TIMEOUT_start();
    uint32_t elapsed = current_time - start_token;
    if (elapsed < timeout) {
        return 0;
    }
    else {
        return 1;
    }
}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    global_timer += 1;
}


