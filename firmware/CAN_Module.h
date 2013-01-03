#ifndef __CAN_H__
#define __CAN_H__

#include "lib/CAN/can.h"
#include "inttypes.h"
#ifndef TEST
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#endif

/** 
 * initialise the MCP2515, must be connected     to
 *   - PB0 ... not connected
 *   - PB1 ... CLK
 *   - PB2 ... MOSI
 *   - PB2 ... MISO
 *   - PB7 ... Connected to !RESET
 *   - PB0 ... Connected to !CS
 *   - PB5 ... Connected to CANINT
 */

#define ASCII_CAN_MESSAGE_LENGTH 24

#define MAX_BUFFER_LENGTH 128

#define CAN_ASCII_MESSAGE_BEGIN_CHAR 'I'
#define CAN_ASCII_MESSAGE_END_CHAR ';'

#ifdef __cplusplus
extern "C"
{
#endif

int CAN_Init();

void can2ascii(char* _string, can_t* _can);

bool ascii2can(char* _string, can_t* _can);

int ascii_message_exists(char* str);

void get_substring(char* src, char* dest, unsigned int begin, unsigned int end);

#ifdef __cplusplus
}
#endif

#endif
