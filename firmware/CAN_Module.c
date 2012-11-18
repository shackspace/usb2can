#include "CAN_Module.h"
#include <util/delay.h>

prog_uint8_t can_filter[] = 
{
	// Group 0
	MCP2515_FILTER(0),              // Filter 0
	MCP2515_FILTER(0),              // Filter 1

	// Group 1
	MCP2515_FILTER_EXTENDED(0),     // Filter 2
	MCP2515_FILTER_EXTENDED(0),     // Filter 3
	MCP2515_FILTER_EXTENDED(0),     // Filter 4
	MCP2515_FILTER_EXTENDED(0),     // Filter 5

	MCP2515_FILTER(0),              // Mask 0 (for group 0)
	MCP2515_FILTER_EXTENDED(0),     // Mask 1 (for group 1)
};

static bool can_module_initialized = false;

int CAN_Init()
{
	if(can_module_initialized)
		return 0;

	// enable SPI master mode by setting B0 as an output ping
	DDRB |= _BV(0);

	// set D0 to low and then to high to RESET the mcp
	DDRB|= _BV(7);
	PORTB ^= _BV(7);
	_delay_ms(10);
	PORTB |= _BV(7);

	if(!can_init(BITRATE_125_KBPS))
	{
		return 1;
	}
	else
	{
		
	}

	// set the filters.
	can_static_filter(can_filter);
	can_module_initialized = true;

	return 0;
}

void can2ascii(ascii_can_message_string* _string, can_t _can)
{
	
}

void ascii2can(ascii_can_message_string* _string, can_t _can)
{

}
