#include "CAN_Module.h"
#ifndef TEST
#include <util/delay.h>
#endif
#include <string.h>
#include <ctype.h>

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
    #ifndef TEST
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
	#endif
	can_module_initialized = true;

	//can_set_mode(LISTEN_ONLY_MODE); //first set adapter passive

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

void can2ascii(char* _string, can_t* _can)
{
	int i = 0;
	_string[0] = 'I'; //Start CAN Message with ID
#if SUPPORT_EXTENDED_CANID
	_string[1] = nibble_to_ascii(_can->id >> 28);
	_string[2] = nibble_to_ascii(_can->id >> 24);
	_string[3] = nibble_to_ascii(_can->id >> 20);
	_string[4] = nibble_to_ascii(_can->id >> 16);
#else
	_string[1] = 0;
	_string[2] = 0;
	_string[3] = 0;
	_string[4] = 0;
#endif
	_string[5] = nibble_to_ascii(_can->id >> 12);
	_string[6] = nibble_to_ascii(_can->id >> 8);
	_string[7] = nibble_to_ascii(_can->id >> 4);
	_string[8] = nibble_to_ascii(_can->id);
#if SUPPORT_EXTENDED_CANID
	_string[9] = _can->flags.rtr + ((unsigned char)(_can->flags.extended) << 1) + 'R';
#else
	_string[9] = _can->flags.rtr + 'R';
#endif
	_can->length = _can->length > 8 ? 8 : _can->length;
	_string[10] = _can->length + '0';
	for(i = 0; i < _can->length; i++)
	{
		_string[11 + 2*i] = nibble_to_ascii(_can->data[i] >> 4);
		_string[12 + 2*i] = nibble_to_ascii(_can->data[i]);
	}
	_string[11 + 2*i] = 0;
}

bool ascii2can(char* _string, can_t* _can)
{
	int i;
	if(_string[0] == 'I') //identifier
	{
		for(i = 1; i < 9; i++)
		{
			
			_can->id = _can->id << 4;
			if(isdigit(_string[i]))
				_can->id |= (_string[i] - '0');
			else if (_string[i] >= 'A' && _string[i] <= 'F')
				_can->id |= _string[i] - 'A' + 0x0A;
			else return false;
		}
	}
#if SUPPORT_EXTENDED_CANID
	if(_string[9] >= 'R' && _string[9] <= 'U')
	{
		_can->flags.rtr = (_string[9] - 'R') & 0x01;
		_can->flags.extended = ((_string[9] - 'R') >> 1) & 0x01;
	}
#else
	if(_string[9] >= 'R' && _string[9] <= 'U')
	{
		_can->flags.rtr = (_string[9] - 'R') & 0x01;
	}
#endif
	else
		return false;
	if(_string[10] >= '0' && _string[10] <= '4')
		_can->length = _string[10] - '0';
	for(i = 0; i < _can->length; i++)
	{
		if(_string[11 + i * 2] == 0 || _string[12 + i * 2] == 0)
			return false;
		if(isdigit(_string[11 + i*2]))
			_can->data[i] = (_string[11 + i*2] - '0') << 4;
		else if(_string[11 + i*2] >= 'A' && _string[11 + i*2] <= 'F')
			_can->data[i] = (_string[11 + i*2] - 'A' + 0x0A) << 4;
		else return false;

		if(isdigit(_string[12 + i*2]))
			_can->data[i] = _can->data[i] + (_string[12 + i*2] - '0');
		else if(_string[12 + i*2] >= 'A' && _string[12 + i*2] <= 'F')
			_can->data[i] = _can->data[i] + (_string[12 + i*2] - 'A' + 0x0A);
		else return false;		
	}
		
	return true;
}


int ascii_message_exists(char* str)
{
    int i;
    if(str[0] == 'I')
    {
        for(i=0; i <= ASCII_CAN_MESSAGE_LENGTH && str[i] != '\n'; i++);

        if(str[i] == '\n')
        {
            return i;
        }
        else
        {
            return -1;
        }
    }
    else
        return -1;
}

void get_substring(char* src, char *dest, unsigned int begin, unsigned int end)
{
    unsigned int i;
    for(i = 0; (i < (end - begin)) && (i < (strlen(src) - begin)); i++)
    {
        dest[i] = src[begin + i];
    }
    dest[i] = 0;
}
