#include "header.h"

void cd_command(char **argument)
{
    if (argument[1] == NULL || strcmp(argument[1], "~") == 0)
    {
        getcwd(prev_dir, 1024);
        if (chdir(home_dir) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
    }
    else if (strcmp(argument[1], "-") == 0)
    {
        char temp[1024];
        getcwd(temp, 1024);
        printf("\e[0;32m%s\n\e[0m", prev_dir);
        if (chdir(prev_dir) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
        strcpy(prev_dir, temp);
    }
    else
    {
        getcwd(prev_dir, 1024);
        if (chdir(argument[1]) == -1)
        {
            perror("\e[0;91mcd");
            fprintf(stderr, "\e[0m");
        }
    }
}

void echo_command(char **argument, int no_of_parts)
{
    int i;
    for (i = 1; i < no_of_parts; i++)
    {
        printf("%s ", argument[i]);
    }
    printf("\n");
}

void pwd_command()
{
    char temp[1024];
    getcwd(temp, 1024);
    printf("%s\n", temp);
}

void repeat_command(char **argument, int no_of_parts)
{
    char *temp = combination(argument, 2, no_of_parts);
    int n = atoi(argument[1]);
    if (strcmp(temp, "") == 0)
    {
        printf("\e[0;91mProvide an command to repeat\n\e[0m");
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            command_process(temp);
        }
    }
    free(temp);
}

void bg_command(char **argument)
{
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        if (execvp(argument[0], argument) == -1)
        {
            fprintf(stderr, "\e[0;91m");
            perror("Shell");
            fprintf(stderr, "\e[0m");
        }
        exit(0);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "\e[0;91m");
        perror("Fork");
        fprintf(stderr, "\e[0m");
    }
    else
    {
        printf("\e[4;32m%d\e[0m\n", pid);
        // usleep(10000);
        add_bgp_node(pid, argument[0]);
    }
}

void fg_command(char **argument, int no_of_parts)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        if (execvp(argument[0], argument) == -1)
        {
            fprintf(stderr, "\e[0;91m");
            perror("Shell");
            fprintf(stderr, "\e[0m");
        }
        exit(1);
    }
    else if (pid < 0)
    {
        perror("Fork");
    }
    else
    {
        // printf("%d", pid);
        waitpid(pid, &status, WUNTRACED);
        // do
        // {
        //     wpid = waitpid(pid, &status, WUNTRACED);
        // } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void command_process(char command[])
{
    int no_of_parts;
    char **argument = parse(command, " ", &no_of_parts);
    if (argument != NULL)
    {
        if (strcmp(argument[0], "repeat") == 0)
        {
            repeat_command(argument, no_of_parts);
        }
        else if (strcmp(argument[0], "cd") == 0)
        {
            if (no_of_parts > 2)
            {
                fprintf(stderr, "\e[0;91mError: Invalid number of arguments\n\e[0m");
            }
            else
                cd_command(argument);
        }
        else if (strcmp(argument[0], "echo") == 0)
        {
            echo_command(argument, no_of_parts);
        }
        else if (strcmp(argument[0], "pwd") == 0)
        {
            pwd_command();
        }
        else if (strcmp(argument[0], "history") == 0)
        {
            if (no_of_parts == 1)
                history_command(his_start, 10);
            else
                history_command(his_start, atoi(argument[1]));
        }
        else if (strcmp(argument[0], "pinfo") == 0)
        {
            if (no_of_parts == 1)
            {
                pinfo_command(getpid());
            }
            else
            {
                pinfo_command(atoi(argument[1]));
            }
        }
        else if (strcmp(argument[0], "ls") == 0)
        {
            int a = 0, l = 0, no_of_flags = 0, no_of_dir = 0;
            for (int i = 1; i < no_of_parts; i++)
            {
                if (strcmp(argument[i], "-l") == 0)
                {
                    no_of_flags++;
                    l = 1;
                }
                else if (strcmp(argument[i], "-a") == 0)
                {
                    no_of_flags++;
                    a = 1;
                }
                else if ((strcmp(argument[i], "-al") == 0) || (strcmp(argument[i], "-la") == 0))
                {
                    no_of_flags++;
                    l = a = 1;
                }
            }
            printf("\e[0;32m");
            no_of_dir = no_of_parts - no_of_flags - 1;
            if (no_of_dir == 0)
            {
                ls_command(a, l, 0, ".");
            }
            else
            {
                for (int i = 1; i < no_of_parts; i++)
                {
                    if (strcmp(argument[i], "-al") != 0 && strcmp(argument[i], "-la") != 0 && strcmp(argument[i], "-a") != 0 && strcmp(argument[i], "-l") != 0)
                    {
                        ls_command(a, l, no_of_dir, argument[i]);
                        if (no_of_dir > 1)
                            printf("\n");
                        no_of_dir--;
                    }
                }
            }
            printf("\e[0m");
        }
        else if (strcmp(argument[0], "exit") == 0)
        {
            printf("exit\n");
            exit(0);
        }
        // else if (strcmp(argument[0], "ls") == 0)
        // {
        //     system("ls");
        // }
        else
        {
            if (strcmp(argument[no_of_parts - 1], "&") == 0)
            {
                free(argument[no_of_parts - 1]);
                argument[no_of_parts - 1] = NULL;
                no_of_parts--;
                bg_command(argument);
            }
            else
            {
                fg_command(argument, no_of_parts);
            }
        }
    }
    freemem(&argument, no_of_parts);
}