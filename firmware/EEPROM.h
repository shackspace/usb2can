#include <avr/eeprom.h>

typedef struct
{
	char bitrate;
	char transciever_mode;
	char listening_enabled;
} Config;

extern Config config;

void read_config();

void write_config();

