/*
 * stimulus_gpio_test.c
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>

#include "debug_printf.h"
#include "stimulus_gpio.h"
#include "driverlib/gpio.h"
#include "inc/hw_types.h"


void GPIO_delay(int x)
{
    while (x--) {}
}

void GPIO_test(void)
{
    int i;
    int status;

    DEBUG_printf("GPIO Test Initializing...\n");

    //testing input
    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_DIRECTION, 0x0000003f);
    GPIO_cmd(GPIO_COMMAND_SET|GPIO_REGISTER_DRIVE8MA, 0x0000003f);
    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_OPEN_DRAIN, 0x0000003f);
    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_PULLUP, 0x0000003f);
    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_PULLDOWN, 0x0000003f);
    GPIO_cmd(GPIO_COMMAND_SET|GPIO_REGISTER_DIGITAL_EN, 0x0000003f);

//    HWREG(0x40004000 + 0x00000500) |= 0xfc;  // 2ma
//    HWREG(0x40004000 + 0x0000050C) &= ~0xfc; // odr
//    HWREG(0x40004000 + 0x00000510) &= ~0xfc; // pur
//    HWREG(0x40004000 + 0x00000514) &= ~0xfc; // pdr
//    HWREG(0x40004000 + 0x0000051C) |= 0xfc;  // den
//    HWREG(0x40004000 + 0x00000528) &= ~0xfc; // amsel

//    GPIOPinTypeGPIOInput(0x40004000, 0xfc);

    DEBUG_printf("led\n");
    int led;
    while (1) {
        led = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0x0000003f);

        if (led) {
            GPIO_cmd(GPIO_COMMAND_SET|GPIO_REGISTER_DATA, 0x20000000);
            DEBUG_printf("HIGH\n");
        }
        else {
            GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_DATA, 0x20000000);
            DEBUG_printf("LOW\n");
        }
        GPIO_delay(3000000);

    }
    //testing input end

//    DEBUG_printf("Test 1: Set Commands\n");
//    for (i=0;i<8;i++) {
//        GPIO_cmd(GPIO_COMMAND_SET|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0xffffffff);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0x00000000);
//        GPIO_delay(1000000);
//    }
//    DEBUG_printf("Test 2: Toggle Commands\n");
//    for (i=0;i<8;i++) {
//        GPIO_cmd(GPIO_COMMAND_TGL|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0xffffffff);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0x00000000);
//        GPIO_delay(1000000);
//    }
//    DEBUG_printf("Test 3: Get Commands\n");
//    DEBUG_printf("Step through code to view \"status\" variable\n");
//    for (i=0;i<8;i++) {
//        GPIO_cmd(GPIO_COMMAND_PUT|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, i<<28);
//        status = GPIO_cmd(GPIO_COMMAND_GET|GPIO_REGISTER_DATA, 0xffffffff);
//        GPIO_delay(1000000);
//    }
//    DEBUG_printf("Test 4: Put and Clear Commands\n");
//    GPIO_cmd(GPIO_COMMAND_PUT|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);
//    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);
//    GPIO_cmd(GPIO_COMMAND_PUT|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);
//    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);
//    GPIO_cmd(GPIO_COMMAND_PUT|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);
//    GPIO_cmd(GPIO_COMMAND_CLR|GPIO_REGISTER_DATA, 0x70000000);
//    GPIO_delay(1000000);

    DEBUG_printf("GPIO Test Complete\n\n");
}

