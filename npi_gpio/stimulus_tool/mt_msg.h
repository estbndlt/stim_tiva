/*
 * mt_msg.h
 *
 *  Created on: Mar 8, 2017
 *      Author: a0227114
 */
#ifndef MT_MSG_H_
#define MT_MSG_H_



struct mt_msg {
    int state;
    int ok;
    int cursor;
    uint8_t cmd1;
    uint8_t cmd0;
    uint16_t len_expected;
    uint16_t len_actual;
    uint8_t data[256];
    uint8_t fcs;
};

extern struct mt_msg rx_message;
extern struct mt_msg tx_message;

void MT_rxDecodeByte(int b);
void MT_processPacket( void );
void MT_sendPing( void );
int MT_validPacket( void );

uint8_t MT_rx8( void );
uint16_t MT_rx16( void );
uint32_t MT_rx32( void );

void MT_tx8( uint8_t v );
void MT_tx16( uint16_t v );
void MT_tx32( uint32_t v );

void MT_parserTest(void);
void MT_responseTest(void);

intptr_t rx_timer;
intptr_t ping_timer;
intptr_t delay_timer;

struct pingStruct {
     uint8_t count;
     uint8_t repeat;
};

struct pingStruct pingCmd;

#endif /* MT_MSG_H_ */
