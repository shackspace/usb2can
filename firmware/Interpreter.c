#include "Interpreter.h"

enum Commands {TOGGLE_LISTEN = 'l', SEND_MODE = 's', TRANSCIEVER_MODE = 'm'};

int get_line(char* buffer, char* token)
{
    int i;
    for(i = 0; i < MAX_BUFFER_LENGTH && buffer[i] != 0; i++)
    {
        if(buffer[i] == '\n')
            break;
    }
    if(buffer[i] == 0 || i == MAX_BUFFER_LENGTH)
        return -1;
    else
    {
        get_substring(buffer, token, 0, i);
        return i;
    }
}

int decode_command(char* line)
{
    int i = 0;
    int length = strlen(line);
    while((line[0] == ' ') && (i < length)) //strip whitespaces
    {
        i++;
    }

    if(i != length)
    {

    }
    else
    {
        return -1;
    }

}