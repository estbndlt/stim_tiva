/*
 * usb_uart.h
 *
 *  Created on: Mar 8, 2017
 *      Author: a0227114
 */
#ifndef STIMULUS_USB_H_
#define STIMULUS_USB_H_

#include <stdio.h>


extern void USB_init(void);

int  USB_getc( void );
void USB_write( const uint8_t *pBuffer, size_t nbytes );
void USB_putc( int c );
int USB_RxAvailable( void );
int UART_RxAvailable( void );
int UART_getc( void );
void UART_putc( int c );

void USB_test(void);


#endif /* STIMULUS_USB_H_ */
