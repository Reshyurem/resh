#include "header.h"
void action_replay(char **argument, int no_of_parts)
{

    int i = 0, command_start = -1, command_end = -1, period = -1, interval = -1;
    for (; i < no_of_parts; i++)
    {
        if (strcmp(argument[i], "-command") == 0)
        {
            command_start = i + 1;
        }
        else if (strcmp(argument[i], "-period") == 0)
        {
            if (i + 1 < no_of_parts)
                period = atoi(argument[i + 1]);
            if (command_start != -1 && command_end == -1)
            {
                command_end = i;
            }
            i++;
        }
        else if (strcmp(argument[i], "-interval") == 0)
        {
            if (i + 1 < no_of_parts)
                interval = atoi(argument[i + 1]);
            if (command_start != -1 && command_end == -1)
            {
                command_end = i;
            }
            i++;
        }
    }
    if (command_start != -1 && command_end == -1)
    {
        command_end = no_of_parts;
    }

    if(command_start > 0 && command_end > 0 && interval > 0 && period > 0)
    {
        char *command = combination(argument, command_start, command_end);
        for(int j = interval; j <= period; j += interval){
            sleep(interval);
            command_process(strdup(command));
        }
    }
    else
    {
        fprintf(stderr, "\e[0;91mInvalid arguments\n\e[0m");
    }
}