#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "header.h"

char **parse(char string[], char delim[], int *no_of_parts)
{
    char *part, *saveptr;
    char temp[1024];
    int i = 0;
    strcpy(temp, string);
    *no_of_parts = 0;
    for (part = strtok_r(temp, delim, &saveptr); part != NULL; part = strtok_r(NULL, delim, &saveptr))
    {
        (*no_of_parts)++;
    }

    strcpy(temp, string);

    char **parts = (char **)calloc((*no_of_parts + 1), sizeof(char *));

    // walk through the tokenized buffer again
    part = string;

    for (part = strtok_r(temp, delim, &saveptr); part != NULL; part = strtok_r(NULL, delim, &saveptr))
    {
        parts[i] = (char *)calloc(strlen(part) + 2, sizeof(char));
        strcpy(parts[i], part);
        i++;
    }
    parts[*no_of_parts] = NULL;
    return parts;
}

void freemem(char ***mem, int no)
{
    for (int i = 0; i <= no; i++)
    {
        free((*mem)[i]);
    }
    free(*mem);
}