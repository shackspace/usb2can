#include "Interpreter.h"



int get_line(char* buffer, char* token)
{
    char temp[MAX_BUFFER_LENGTH];
    int i;
    for(i = 0; i < MAX_BUFFER_LENGTH && buffer[i] != 0; i++)
    {
        if(buffer[i] == '\n')
            break;
    }
    if(buffer[i] == 0 || i == MAX_BUFFER_LENGTH)
    {
        return -1;
    }
    else
    {
        get_substring(buffer, token, 0, i);
        get_substring(buffer, temp, i+1, MAX_BUFFER_LENGTH);
        strcpy(buffer, temp);
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
        switch(line[i])
        {
            case TOGGLE_LISTEN:
            case SEND_MODE:
            case TRANSCIEVER_MODE:
                return (unsigned char)(line[i]);
                break;
            default:
                return -1;
                break;
        }
    }
    else
    {
        return -1;
    }

}