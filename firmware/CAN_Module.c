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


inline char nibble_to_ascii(uint8_t nibble)
{
	nibble = nibble & 0x0F;
	if(nibble < 0x0A)
		return nibble + '0';
	else
		return nibble + 'A' - 0x0A;
}

void can2ascii(ascii_can_message_string _string, can_t _can)
{
	int i = 0;
	_string[0] = 'I'; //Start CAN Message with ID
	#if SUPPORT_EXTENDED_CANID
	_string[1] = nibble_to_ascii(_can.id >> 28);
	_string[2] = nibble_to_ascii(_can.id >> 24);
	_string[3] = nibble_to_ascii(_can.id >> 20);
	_string[4] = nibble_to_ascii(_can.id >> 16);
	#else
	_string[1] = 0;
	_string[2] = 0;
	_string[3] = 0;
	_string[4] = 0;
	#endif
	_string[5] = nibble_to_ascii(_can.id >> 12);
	_string[6] = nibble_to_ascii(_can.id >> 8);
	_string[7] = nibble_to_ascii(_can.id >> 4);
	_string[8] = nibble_to_ascii(_can.id);
	_string[9] = _can.flags.rtr + (_can.flags.extended << 1) + 'G';
	_can.length = _can.length > 8 ? 8 : _can.length;
	_string[10] = _can.length + '0';
	for(i = 0; i < _can.length; i++)
	{
		_string[11 + 2*i] = nibble_to_ascii(_can.data[i] >> 4);
		_string[12 + 2*i] = nibble_to_ascii(_can.data[i]);
	}
	_string[11 + 2*i] = 0;
}

void ascii2can(ascii_can_message_string _string, can_t _can)
{

}
