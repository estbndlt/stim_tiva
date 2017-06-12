/*
 * debug_printf.h
 *
 *  Created on: Mar 10, 2017
 *      Author: a0227114
 */
#ifndef DEBUG_PRINTF_H_
#define DEBUG_PRINTF_H_

#include <stdarg.h>

void DEBUG_printf( const char *fmt, ... );
void DEBUG_vprintf( const char *fmt, va_list ap );

#endif /* DEBUG_PRINTF_H_ */
