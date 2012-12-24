#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string.h>

#include "CAN_Module.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum InterpreterState {STATE_WAIT_FOR_COMMAND = 0x00, STATE_CONFIG_TRANCIEVER_MODE = 0x01, STATE_SEND = 0x02 };
enum Commands {TOGGLE_LISTEN = 'l', SEND_MODE = 's', TRANSCIEVER_MODE = 'm'};

int get_line(char* buffer, char* token);

int decode_command(char* line);

#ifdef __cplusplus
}
#endif
#endif