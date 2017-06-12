/*
 * stimulus_gpio.h
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */

#ifndef STIMULUS_GPIO_H_
#define STIMULUS_GPIO_H_

#define GPIO_MAX_PINS 32
#define GPIO_PINS_USED 32
/* GPIO_BITMASK_SIZE has a max of 32 for TIVA TM4C123GXL */

#define GPIO_REGISTER_DIRECTION       0x01
#define GPIO_REGISTER_OPEN_DRAIN      0x02
#define GPIO_REGISTER_PULLUP          0x03
#define GPIO_REGISTER_PULLDOWN        0x04
#define GPIO_REGISTER_DATA            0x05
#define GPIO_REGISTER_DIGITAL_EN      0x06
#define GPIO_REGISTER_DRIVE8MA        0X07

#define GPIO_COMMAND_SET 0x10
#define GPIO_COMMAND_CLR 0x20
#define GPIO_COMMAND_GET 0x30
#define GPIO_COMMAND_TGL 0x40
#define GPIO_COMMAND_PUT 0x50


int GPIO_cmd(uint8_t gpio_cmd, uint32_t pin_bitmask);

void GPIO_test(void);

#endif /* STIMULUS_GPIO_H_ */
