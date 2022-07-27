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
        if (no_of_parts == 52)
        {
            printf("\e[0;32mpid -- %d\nProcess Status -- %s", pid, stats[2], stats[22]);
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
            printf("\nmemory -- %s {Virtual Memory}\nExecutable Path -- %s\n\e[0m", stats[22], buf);
        }
        freemem(&stats, no_of_parts);
        fclose(fp);
    }
    else
    {
        perror("\e[0;91mFile Opening");
        fprintf(stderr, "\e[0m");
    }
}