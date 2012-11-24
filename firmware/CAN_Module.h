#ifndef __CAN_H__
#define __CAN_H__

#include "lib/CAN/can.h"
#include "inttypes.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>

/** 
 * initialise the MCP2515, must be connected to
 *   - PB0 ... not connected
 *   - PB1 ... CLK
 *   - PB2 ... MOSI
 *   - PB2 ... MISO
 *   - PB7 ... Connected to !RESET
 *   - PB0 ... Connected to !CS
 *   - PB5 ... Connected to CANINT
 */

#define ASCII_CAN_MESSAGE_LENGTH 24

int CAN_Init();

void can2ascii(char* _string, can_t* _can);

bool ascii2can(char* _string, can_t* _can);

#endif
