/*
 * debug_printf.c
 *
 *  Created on: Mar 10, 2017
 *      Author: a0227114
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "../stimulus_tool/debug_printf.h"


static void debug_uart_putc_raw( int c )
{
    /* write this byte to the Debug Uart */
    //UARTprintf("%c", c);
    UARTCharPut(UART0_BASE, (char)c);
}

static void debug_uart_putc( int c )
{
    if( c == '\n' ){
        debug_uart_putc_raw('\r');
    }
    debug_uart_putc_raw(c);
}

static void debug_uart_write_str( const char *cp )
{
    int c;
    for(;;){
        c = *cp++;
        if(c==0){
            return;
        }
        debug_uart_putc( c );
    }
}

void DEBUG_printf( const char *fmt, ... )
{
    va_list ap;
    va_start( ap, fmt );
    UARTvprintf( fmt, ap );
    va_end(ap);
}

void DEBUG_vprintf( const char *fmt, va_list ap )
{
    static char buf[200];
    vsnprintf( buf, sizeof(buf), fmt, ap );
    debug_uart_write_str( buf );
}
