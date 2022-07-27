#include "header.h"

void pinfo_command(int pid)
{
    FILE *fp;
    char filename[300];
    sprintf(filename, "/proc/%d/stat", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        char buf[600], **stats;
        int no_of_parts, len, bufsize = 600;
        fread(buf, sizeof(char), sizeof(buf), fp);
        stats = parse(buf, " ", &no_of_parts);
        // printf("%d", no_of_parts);
        // if (no_of_parts == 52)
        // {
        if (colorCode == 0)
        {
            printf("\e[0;32m");
        }
        printf("pid -- %d\nProcess Status -- %s", pid, stats[2]);
        if (strcmp(stats[7], stats[0]) == 0)
        {
            printf("+");
        }
        buf[0] = '\0';
        sprintf(filename, "/proc/%d/exe", pid);
        len = readlink(filename, buf, bufsize);
        buf[len] = '\0';
        // printf("executable -- %s\n", buf);
        rel_path(buf);
        printf("\nmemory -- %s {Virtual Memory}\nExecutable Path -- %s\n", stats[22], buf);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
        // }
        freemem(&stats, no_of_parts);
        fclose(fp);
    }
    else
    {
        perror("\e[0;91mFile Opening");
        fprintf(stderr, "\e[0m");
    }
}

int search_state(int pid)
{
    FILE *fp;
    int ret;
    char filename[300];
    sprintf(filename, "/proc/%d/stat", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        char buf[600], **stats;
        int no_of_parts, len, bufsize = 600;
        fread(buf, sizeof(char), sizeof(buf), fp);
        stats = parse(buf, " ", &no_of_parts);
        // if (no_of_parts == 52)
        // {
        // printf("%s\n", stats[2]);
        if (strcmp(stats[2], "R") == 0 || strcmp(stats[2], "S") == 0 || strcmp(stats[2], "D") == 0)
        {
            ret = 0;
        }
        else if (strcmp(stats[2], "T") == 0)
        {
            ret = 1;
        }
        else
        {
            ret = -1;
        }
        // printf("[%s, %d]", stats[2], ret);
        // }
        freemem(&stats, no_of_parts);
        fclose(fp);
    }
    else
    {
        ret = -1;
        perror("\e[0;91mFile Opening");
        fprintf(stderr, "\e[0m");
    }
    return ret;
}