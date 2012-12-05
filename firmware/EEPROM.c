#include "EEPROM.h"

Config config;

void read_config()
{
   _EEGET(config.bitrate, 0x00);
}

void write_config()
{
    _EEPUT(config.bitrate, 0x00);
}
