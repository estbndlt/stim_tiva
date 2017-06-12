/*
 * mt_msg.c
 *
 *  Created on: Mar 8, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"

#include "mt_msg.h"
#include "stimulus_gpio.h"
#include "stimulus_usb.h"
#include "stimulus_timer.h"


struct mt_msg rx_message;
struct mt_msg tx_message;

#define PERIPHERAL_CMD      0x00
#define GPIO_CMD            0x01
/* FUTURE PERIPHERAL FEATURES */
#define SPI_CMD             0x02
#define ADC_CMD             0x03

#define SIMPLE_CMD_START_VALUE 0x10

#define MT_SOF     0xFE
#define MT_ERROR   0x01
#define MT_PASS    0x00

#define PERIPHERAL_SUBSYSTEM_INDEX    0x01
#define PERIPHERAL_COMMAND_INDEX      0x00
#define GPIO_VERSION                  0x11 // (1.1)
#define GPIO_COMMANDS_SUPPORTED       0x1c // 28 commands

#define PIN_BITMASK_BYTES 4
#define GET_VERSION_BYTES 6

void MT_rxDecodeByte(int b)
{

    if (b == -1) {
        rx_message.state = 0;
        rx_message.ok = 0;
        return;
    }
    rx_timer = TIMEOUT_start();
    if (rx_message.state == 0) {
        if (b == 0xfe) {
            rx_message.state = 1;
            rx_message.fcs = 0;
            rx_message.len_expected = 0;
        }
        return;
    }

    rx_message.fcs ^= b;
    if (rx_message.state == 1) {
        rx_message.state = 2;
        rx_message.len_expected += (b);
        return;
    }
    if (rx_message.state == 2) {
        rx_message.state = 3;
        rx_message.len_expected += (b*256);
        rx_message.len_actual = 0;
        return;
    }
    if (rx_message.state == 3) {
        rx_message.state = 4;
        rx_message.cmd0 = b;
        return;
    }
    if (rx_message.state == 4) {
        rx_message.state = 5;
        rx_message.cmd1 = b;
        return;
    }
    if (rx_message.len_actual < rx_message.len_expected) {

        rx_message.data[rx_message.len_actual] = (uint8_t) b;
        rx_message.len_actual++;
        return;
    }
    rx_message.ok = (rx_message.fcs == 0);
    if (rx_message.ok) {
        // do stuff here
        tx_message = rx_message;
        rx_message.state = 0;
    }
}

void MT_tx8( uint8_t v )
{
    USB_putc(v);
}

void MT_tx16( uint16_t v )
{
    uint8_t low =  (v>>0) & 0xFF;
    uint8_t high = (v>>8) & 0xFF;

    USB_putc(low);
    USB_putc(high);
}

void MT_tx32( uint32_t v )
{
    uint8_t byte1 = (v >> 0)  & 0xFF;
    uint8_t byte2 = (v >> 8)  & 0xFF;
    uint8_t byte3 = (v >> 16) & 0xFF;
    uint8_t byte4 = (v >> 24) & 0xFF;

    USB_putc(byte1);
    USB_putc(byte2);
    USB_putc(byte3);
    USB_putc(byte4);
}

void MT_calcFcs( void )
{
    int i;

    tx_message.fcs = 0;
    tx_message.fcs ^= ((tx_message.len_actual>>0) & 0xFF);
    tx_message.fcs ^= ((tx_message.len_actual>>8) & 0xFF);
    tx_message.fcs ^= tx_message.cmd0;
    tx_message.fcs ^= tx_message.cmd1;

    for (i=0;i<tx_message.len_actual;i++) {
        tx_message.fcs ^= tx_message.data[i];
    }

    return;
}


void MT_txSendPacket( void )
{
    int i;

    MT_tx8(MT_SOF);
    MT_tx16(tx_message.len_actual);
    MT_tx8(tx_message.cmd0);
    MT_tx8(tx_message.cmd1);

    for (i=0;i<tx_message.len_actual;i++) {
        MT_tx8(tx_message.data[i]);
    }

    MT_tx8(tx_message.fcs);

    return;
}

int MT_validPacket( void )
{
    if (rx_message.ok) {
        rx_message.ok = 0;
        return 1;
    }
    return 0;
}

void MT_initPing( void ) {
    tx_message.data[0] = 0x01;
    tx_message.data[1] = 0x00;
    tx_message.data[2] = 0x00;
    tx_message.data[3] = 0x00;
    tx_message.data[4] = (uint8_t)'h';
    tx_message.data[5] = (uint8_t)'i';
    tx_message.len_actual = 6;
    tx_message.cmd0 = 0x50;
    tx_message.cmd1 = 0x00;
    tx_message.state = 0;
    tx_message.ok = 1;
    tx_message.fcs = 0;
}

void MT_sendPing( void )
{
//    if (tx_message.data[0]!=0x01 && tx_message.data[1]!=0x00) {
//        MT_initPing();
//    }
    MT_initPing();
    MT_calcFcs();
    MT_txSendPacket();
//    do {
//        MT_calcFcs();
//        MT_txSendPacket();
//        if (repeat) {
//            tx_message.data[2] += 0x01;
//        }
//    } while (tx_message.data[2] != tx_message.data[3]);

    return;
}

void MT_sendOk( void ) {
    // send simple response
    tx_message.len_actual -= 3;
    tx_message.data[2] = MT_PASS;

    MT_calcFcs();
    MT_txSendPacket();
}

void MT_sendErr( void ) {
    // send error response
    tx_message.len_actual -= 3;
    tx_message.data[2] = MT_ERROR;

    MT_calcFcs();
    MT_txSendPacket();
}

void MT_sendOkBitmask( void ) {
    // send get response
    tx_message.len_actual += 1;
    tx_message.data[2] = MT_PASS;

    MT_calcFcs();
    MT_txSendPacket();
}

void MT_sendOkVersion( void ) {
    int i;
    // send get response

    for (i=tx_message.len_actual;i>1;i--) {
        tx_message.data[i] = tx_message.data[i-1];
    }
    tx_message.len_actual += 1;
    tx_message.data[2] = MT_PASS;

    MT_calcFcs();
    MT_txSendPacket();
}

void MT_gpioProcess( void )
{
    uint16_t command = (rx_message.data[PERIPHERAL_COMMAND_INDEX]);

    int packet_response;
    int simple_command = ((command > 0x10) && (command < 0x58));
    int get_version = (command == 0x00);
    int set_pulse = (command == 0x01);
    int set_indication = (command == 0x02);
    int get_command = ((command > 0x2f) && (command <0x3f));

    if (get_version) {
        tx_message.len_actual = 7;
        tx_message.data[2] = GPIO_COMMANDS_SUPPORTED;
        tx_message.data[3] = 0xff;
        tx_message.data[4] = 0xff;
        tx_message.data[5] = 0xff;
        tx_message.data[6] = 0x0f;
        MT_calcFcs();
        MT_txSendPacket();
    }
    if (set_pulse) {

    }
    if (set_indication) {

    }
    if (simple_command) {
        if (rx_message.len_actual>2) {
            uint32_t pin_bitmask =  (rx_message.data[2]<<0) +
                    (rx_message.data[3]<<8) +
                    (rx_message.data[4]<<16) +
                    (rx_message.data[5]<<24);
            packet_response = GPIO_cmd(command, pin_bitmask);

        }
        if (get_command) {
            tx_message.data[3] = (packet_response >> 0)  & 0x0ff;
            tx_message.data[4] = (packet_response >> 8)  & 0x0ff;
            tx_message.data[5] = (packet_response >> 16) & 0x0ff;
            tx_message.data[6] = (packet_response >> 24) & 0x0ff;
            MT_sendOkBitmask();
        }
        else if (!packet_response) {
            MT_sendOk();
        }
        else {
            MT_sendErr();
        }
    }
    return;
}

void MT_peripheralProcess( void )
{
    uint16_t command = (rx_message.data[PERIPHERAL_COMMAND_INDEX]);
    int version_command = (command == 0x00);
    int ping_command = (command == 0x01);

    if (version_command) {
        tx_message.len_actual = 0x0a;
        tx_message.data[2] = 0x01; // build
        tx_message.data[3] = 0x01; // patch
        tx_message.data[4] = 0x01; // minor
        tx_message.data[5] = 0x01; // major
        tx_message.data[6] = 0x01; // peripherals supported bitmask
        tx_message.data[7] = 0x00; // currently only gpio
        tx_message.data[8] = 0x00;
        tx_message.data[9] = 0x00;
        MT_calcFcs();
        MT_txSendPacket();
    }
    else if (ping_command) {
        MT_sendPing();
//        if (pingCmd.count != pingCmd.repeat) {
//            tx_message.data[0] = pingCmd.count;
//            tx_message.data[1] = pingCmd.repeat;
//            pingCmd.count++;
//            MT_sendPing();
//        }

    }
    else {
        MT_sendErr();
    }

    return;
}

void MT_processPacket( void )
{
    struct MT_packet {
        int8_t command;
        void (*cmdProcess)(void);
    };

    struct MT_packet MT_packetList[] = {
        {PERIPHERAL_CMD, MT_peripheralProcess},
        {GPIO_CMD, MT_gpioProcess},
        {-1, NULL},
    };

    int elements = sizeof(MT_packetList)/sizeof(MT_packetList[0]);
    int i;

    for (i=0;i<elements;i++) {
        if (rx_message.data[PERIPHERAL_SUBSYSTEM_INDEX] == MT_packetList[i].command) {
            MT_packetList[i].cmdProcess();
            return;
        }
    }
    return;
}


