/*
 * stimulus_gpio.c
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"

#include "stimulus_gpio.h"



int32_t map_port[] = {
                       GPIO_PORTA_BASE, GPIO_PORTA_BASE, GPIO_PORTA_BASE, GPIO_PORTA_BASE, GPIO_PORTA_BASE, GPIO_PORTA_BASE,    /* PORT A */
                       GPIO_PORTB_BASE, GPIO_PORTB_BASE, GPIO_PORTB_BASE, GPIO_PORTB_BASE, GPIO_PORTB_BASE,                     /* PORT B */
                       GPIO_PORTC_BASE, GPIO_PORTC_BASE, GPIO_PORTC_BASE, GPIO_PORTC_BASE,                                      /* PORT C */
                       GPIO_PORTD_BASE, GPIO_PORTD_BASE, GPIO_PORTD_BASE, GPIO_PORTD_BASE, GPIO_PORTD_BASE, GPIO_PORTD_BASE,    /* PORT D */
                       GPIO_PORTE_BASE, GPIO_PORTE_BASE, GPIO_PORTE_BASE, GPIO_PORTE_BASE, GPIO_PORTE_BASE, GPIO_PORTE_BASE,    /* PORT E */
                       GPIO_PORTF_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE, GPIO_PORTF_BASE,                     /* PORT F */
                       -1
};

int8_t map_pin[] =
        {
        GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,         /* PORT A=6 PINS */
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_7,                     /* PORT B=5 PINS */
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,                                 /* PORT C=4 PINS */
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_6, GPIO_PIN_7,         /* PORT D=6 PINS */
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,         /* PORT E=6 PINS */
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4,                     /* PORT F=5 PINS */
        -1
        };

struct Pinmap
{
    uint32_t host_pin;      // pin in GPIO Cmd bitmask
    uint32_t port;          // port base address
    uint32_t pin;           // register pin bitmask
};


static int GPIO_getPin(uint32_t base_address, uint8_t gpio_cmd, uint8_t value)
{
    int minor = gpio_cmd&0x0F;

    switch(minor) {
    case GPIO_REGISTER_DIRECTION:
        base_address = base_address + GPIO_O_DIR;
        break;
    case GPIO_REGISTER_OPEN_DRAIN:
        base_address = base_address + GPIO_O_ODR;
        break;
    case GPIO_REGISTER_PULLUP:
        base_address = base_address + GPIO_O_PUR;
        break;
    case GPIO_REGISTER_PULLDOWN:
        base_address = base_address + GPIO_O_PDR;
        break;
    case GPIO_REGISTER_DATA:
        base_address = base_address + (GPIO_O_DATA + (value << 2));
        //return !!HWREG(base_address);
        break;
    default:
        break;
    }
    return !!(HWREG(base_address) & value);
}


static void GPIO_doPin(uint32_t base_address, uint8_t gpio_cmd, uint8_t value)
{

    int minor = gpio_cmd&0x0F;

    switch(minor) {
    case GPIO_REGISTER_DIRECTION:
        base_address = base_address + GPIO_O_DIR;
        break;
    case GPIO_REGISTER_OPEN_DRAIN:
        base_address = base_address + GPIO_O_ODR;
        break;
    case GPIO_REGISTER_PULLUP:
        base_address = base_address + GPIO_O_PUR;
        break;
    case GPIO_REGISTER_PULLDOWN:
        base_address = base_address + GPIO_O_PDR;
        break;
    case GPIO_REGISTER_DATA:
        base_address = base_address + (GPIO_O_DATA + (value << 2));
        break;
    case GPIO_REGISTER_DIGITAL_EN:
        base_address = base_address + GPIO_O_DEN;
        break;
    case GPIO_REGISTER_DRIVE8MA:
        base_address = base_address + GPIO_O_DR8R;
        break;
    default:
        break;
    }

    int major = gpio_cmd&0xF0;
    uint32_t data = HWREG(base_address);

    switch(major) {
    case GPIO_COMMAND_SET:
        data = data | value;
        break;
    case GPIO_COMMAND_CLR:
        data = data & ~value;
        break;
    case GPIO_COMMAND_TGL:
        data = data ^ value;
        break;
    case GPIO_COMMAND_PUT:
        break;
    default:
        break;
    }

    HWREG(base_address) = data;
}


int GPIO_cmd(uint8_t gpio_cmd, uint32_t pin_bitmask)
{

    int i;
    int is_put = (gpio_cmd & 0xF0) == GPIO_COMMAND_PUT;
    int is_get = (gpio_cmd & 0xF0) == GPIO_COMMAND_GET;
    struct Pinmap current_pin;
    //
    // LOOP FOR SET, CLR, PUT, TGL
    //
    if (!is_get) {
        for(i=0;i<GPIO_PINS_USED;i++) {
            current_pin.port = map_port[i];
            current_pin.pin = map_pin[i];
            current_pin.host_pin = 1<<i; // or 1<<(31-i)

            if (map_port[i] == -1) {
                return -1;
            }

            int pin_found = current_pin.host_pin & pin_bitmask;
            int temp_cmd = gpio_cmd;

            if (pin_found && !is_put) {
                GPIO_doPin(current_pin.port, gpio_cmd, current_pin.pin);
            }
            else if (is_put) {
                if (current_pin.host_pin & pin_bitmask) {
                    temp_cmd &= ~GPIO_COMMAND_PUT;
                    temp_cmd |= GPIO_COMMAND_SET;
                    GPIO_doPin(current_pin.port, temp_cmd, current_pin.pin);
                }
                else {
                    temp_cmd &= ~GPIO_COMMAND_PUT;
                    temp_cmd |= GPIO_COMMAND_CLR;
                    GPIO_doPin(current_pin.port, temp_cmd, current_pin.pin);
                }
            }
        }
        return 0;
    }

    //
    // LOOP AGAIN FOR READ
    //
    uint32_t data = 0;
    for(i=0;i<GPIO_PINS_USED;i++) {
        current_pin.port = map_port[i];
        current_pin.pin = map_pin[i];
        current_pin.host_pin = 1<<i; // or 1<<(31-i)

        if (map_port[i] == -1) {
            return -1;
        }

        int pin_high = GPIO_getPin(current_pin.port, gpio_cmd, current_pin.pin);
        if (pin_high && (current_pin.host_pin & pin_bitmask)) {
            data |= current_pin.host_pin;
        }
    }
    return data;
}

