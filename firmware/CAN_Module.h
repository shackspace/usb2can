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

int CAN_Init();

typedef char ascii_can_message_string[24];

void can2ascii(ascii_can_message_string _string, can_t _can);

bool ascii2can(ascii_can_message_string _string, can_t* _can);

#endif
