#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string.h>

#include "CAN_Module.h"

#ifdef __cplusplus
extern "C"
{
#endif

int get_line(char* buffer, char* token);

#ifdef __cplusplus
}
#endif
#endif