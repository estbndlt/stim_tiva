/*
 * stimulus_timer.h
 *
 *  Created on: Mar 9, 2017
 *      Author: a0227114
 */

#ifndef STIMULUS_TIMER_H_
#define STIMULUS_TIMER_H_


intptr_t TIMEOUT_start( void );
int TIMEOUT_expired( intptr_t start_token, uint32_t timeout);

void TIMER_test(void);

#endif /* STIMULUS_TIMER_H_ */
