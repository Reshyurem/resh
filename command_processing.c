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
        if (colorCode == 0)
        {
            printf("\e[0;32m");
        }
        printf("%s\n", prev_dir);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
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
        if (colorCode == 0)
        {
            printf("\e[0;91m");
        }
        printf("Provide an command to repeat\n");
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
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

void jobs_command(char **argument, int no_of_parts)
{
    bgp *traversal_node;
    traversal_node = bgp_start;
    int r_flag = 0, s_flag = 0;
    if (no_of_parts == 1)
    {
        r_flag = 1;
        s_flag = 1;
    }
    for (int i = 1; i < no_of_parts; i++)
    {
        if (strcmp(argument[i], "-r") == 0)
        {
            r_flag = 1;
        }
        else if (strcmp(argument[i], "-s") == 0)
        {
            s_flag = 1;
        }
        else if (strcmp(argument[i], "-rs") == 0 || strcmp(argument[i], "-sr") == 0)
        {
            r_flag = 1;
            s_flag = 1;
        }
    }
    int state;
    while (traversal_node != NULL)
    {
        // printf("PID:%d\n", traversal_node->pid);
        state = search_state(traversal_node->pid);
        if (state == 0 && r_flag)
        {
            printf("[%d] ", traversal_node->pos);
            printf("Running ");
            printf("%s [%d]\n", traversal_node->process_name, traversal_node->pid);
        }
        if (state == 1 && s_flag)
        {
            printf("[%d] ", traversal_node->pos);
            printf("Stopped ");
            printf("%s [%d]\n", traversal_node->process_name, traversal_node->pid);
        }
        traversal_node = traversal_node->next;
        state = -1;
    }
}

void bg_command(char **argument, int no_of_parts)
{
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        setpgid(0, 0);
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
        if (colorCode == 0)
        {
            printf("\e[4;32m");
        }
        printf("%d\n", pid);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
        // usleep(10000);
        char *pname = combination(argument, 0, no_of_parts);
        add_bgp_node(pid, pname);
        free(pname);
    }
}

void fg_command(char **argument, int no_of_parts)
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        // signal(SIGINT, shell_close);
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
        fg_proc = pid;
        fg_proc_name = combination(argument, 0, no_of_parts);
        waitpid(pid, &status, WUNTRACED);
        fg_proc = 0;
        free(fg_proc_name);
        fg_proc_name = NULL;
    }
}

void make_fg_command(int proc)
{
    bgp *traversal_node = bgp_start;
    int status;
    while (traversal_node != NULL)
    {
        if (traversal_node->pos == proc)
        {
            int pid = traversal_node->pid;
            kill((pid_t)(pid), 18);
            fg_proc = pid;
            fg_proc_name = strdup(traversal_node->process_name);
            if (search_state(fg_proc) != 1)
            {
                remove_process(pid);
                waitpid(pid, &status, WUNTRACED);
            }
            fg_proc = 0;
            free(fg_proc_name);
            fg_proc_name = NULL;
            return;
        }
        traversal_node = traversal_node->next;
    }
    fprintf(stderr, "\e[0;91mNo job with number [%d] exists\e[0m\n", proc);
}

void make_bg_command(int proc)
{
    bgp *traversal_node = bgp_start;
    while (traversal_node != NULL)
    {
        if (traversal_node->pos == proc)
        {
            kill((pid_t)(traversal_node->pid), 18);
            return;
        }
        traversal_node = traversal_node->next;
    }
    fprintf(stderr, "\e[0;91mNo job with number [%d] exists\e[0m\n", proc);
}

void sig_command(int proc, int signal)
{
    if (signal > 0 && signal < 65)
    {
        bgp *traversal_node = bgp_start;
        while (traversal_node != NULL)
        {
            if (traversal_node->pos == proc)
            {
                kill((pid_t)(traversal_node->pid), signal);
                return;
            }
            traversal_node = traversal_node->next;
        }
        fprintf(stderr, "\e[0;91mNo job with number [%d] exists\e[0m\n", proc);
    }
    else
    {
        fprintf(stderr, "\e[0;91mSignal with value %d does not exist\n\e[0m", signal);
    }
}

void command_process(char command[])
{
    int no_of_parts;
    char **argument = parse(command, " ", &no_of_parts);
    int stdinCopy = dup(STDIN_FILENO);
    int stdoutCopy = dup(STDOUT_FILENO);
    for (int i = 0; i < no_of_parts; i++)
    {
        if (strcmp(argument[i], "<") == 0 && i < no_of_parts - 1)
        {
            int inputFile = open(argument[i + 1], O_RDONLY);
            if (dup2(inputFile, STDIN_FILENO) < 0)
            {
                perror("\e[0;91mdup2");
                fprintf(stderr, "\e[0m");
                return;
            }
            close(inputFile);
            // free(argument[i]);
            // free(argument[i + 1]);
            for (int j = i; j < no_of_parts - 1; j++)
            {
                argument[j] = argument[j + 2];
            }
            no_of_parts -= 2;
            i--;
            // colorCode = 1;
        }
        else if (strcmp(argument[i], ">") == 0 && i < no_of_parts - 1)
        {
            int outputFile = open(argument[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (dup2(outputFile, STDOUT_FILENO) < 0)
            {
                perror("\e[0;91mdup2");
                fprintf(stderr, "\e[0m");
                return;
            }
            close(outputFile);
            // free(argument[i]);
            // free(argument[i + 1]);
            for (int j = i; j < no_of_parts - 1; j++)
            {
                argument[j] = argument[j + 2];
            }
            no_of_parts -= 2;
            i--;
            colorCode = 1;
        }
        else if (strcmp(argument[i], ">>") == 0 && i < no_of_parts - 1)
        {
            int outputFile = open(argument[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
            if (dup2(outputFile, STDOUT_FILENO) < 0)
            {
                perror("\e[0;91mdup2");
                fprintf(stderr, "\e[0m");
                return;
            }
            close(outputFile);
            // free(argument[i]);
            // free(argument[i + 1]);
            for (int j = i; j < no_of_parts - 1; j++)
            {
                argument[j] = argument[j + 2];
            }
            no_of_parts -= 2;
            i--;
            colorCode = 1;
        }
    }
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
                else if (strcmp(argument[i], "~") == 0)
                {
                    argument[i] = strdup(home_dir);
                }
            }
            if (colorCode == 0)
            {
                printf("\e[0;32m");
            }
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
            if (colorCode == 0)
            {
                printf("\e[0m");
            }
        }
        else if (strcmp(argument[0], "jobs") == 0)
        {
            jobs_command(argument, no_of_parts);
        }
        else if (strcmp(argument[0], "sig") == 0)
        {
            if (no_of_parts > 3)
            {
                fprintf(stderr, "\e[0;91mToo many arguments\e[0m\n");
            }
            else
            {
                sig_command(atoi(argument[1]), atoi(argument[2]));
            }
        }
        else if (strcmp(argument[0], "fg") == 0)
        {
            for (int i = 1; i < no_of_parts; i++)
            {
                make_fg_command(atoi(argument[i]));
            }
        }
        else if (strcmp(argument[0], "bg") == 0)
        {
            for (int i = 1; i < no_of_parts; i++)
            {
                make_bg_command(atoi(argument[i]));
            }
        }
        else if (strcmp(argument[0], "replay") == 0)
        {
            action_replay(argument, no_of_parts);
        }
        else if (strcmp(argument[0], "baywatch") == 0)
        {
            int sleep_period = 1, flag = 0;
            for (int i = 1; i < no_of_parts; i++)
            {
                if (strcmp(argument[i], "-n") == 0)
                {
                    if (i < no_of_parts - 1)
                        sleep_period = atoi(argument[++i]);
                }
                if (strcmp(argument[i], "dirty") == 0)
                {
                    flag = 1;
                }
            }
            if(flag == 1){
                quitter(sleep_period);
            }
            else {
                fprintf(stderr, "\e[0;91mCommand Missing: Only the command 'dirty' is available\n\e[0;m");
            }
        }
        else if (strcmp(argument[0], "exit") == 0)
        {
            printf("exit\n");
            exit(0);
        }
        else
        {
            if (strcmp(argument[no_of_parts - 1], "&") == 0)
            {
                free(argument[no_of_parts - 1]);
                argument[no_of_parts - 1] = NULL;
                no_of_parts--;
                bg_command(argument, no_of_parts);
            }
            else
            {
                fg_command(argument, no_of_parts);
            }
        }
    }
    if (dup2(stdinCopy, STDIN_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdinCopy);
    if (dup2(stdoutCopy, STDOUT_FILENO) < 0)
    {
        perror("\e[0;91mdup2");
        fprintf(stderr, "\e[0m");
        return;
    }
    close(stdoutCopy);
    freemem(&argument, no_of_parts);
    colorCode = 0;
}