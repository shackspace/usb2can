#include <avr/eeprom.h>

typedef struct
{
	int bitrate;
} Config;

extern Config config;

void read_config();

void write_config();

