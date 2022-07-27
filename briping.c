#include "header.h"

void bripe(char *command)
{
    int no_of_parts = 0, pipeFile[2], prevPipeFile[2], i = 0;
    int stdinCopy = dup(STDIN_FILENO);
    int stdoutCopy = dup(STDOUT_FILENO);
    char **pipes = parse(command, "|", &no_of_parts);
    for (i = 0; i < no_of_parts - 1; i++)
    {
        pipe(pipeFile);
        if (dup2(pipeFile[1], STDOUT_FILENO) < 0)
        {
            perror("\e[0;91mdup2");
            fprintf(stderr, "\e[0m");
            return;
        }
        close(pipeFile[1]);
        command_process(pipes[i]);
        if (dup2(pipeFile[0], STDIN_FILENO) < 0)
        {
            perror("\e[0;91mdup2");
            fprintf(stderr, "\e[0m");
            return;
        }
        close(pipeFile[0]);
    }
    if (dup2(stdoutCopy, STDOUT_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdoutCopy);
    command_process(pipes[i]);
    if (dup2(stdinCopy, STDIN_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdinCopy);
}