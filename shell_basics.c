#include "header.h"

// char *input;
// size_t input_size;

void shell_setup()
{
    gethostname(systemname, 1024);
    strcpy(username, getenv("USER"));
    // getlogin_r(username, 1024);
    getcwd(home_dir, 1024);
    history_retrieval();
}

void shell_prompt()
{
    char cwd[1024];
    getcwd(cwd, 1024);
    rel_path(cwd);
    printf("\e[0;96m<%s@%s:\e[1;34m%s\e[0;96m>\e[0m", username, systemname, cwd);
    // int home_length, current_length;
    // home_length = strlen(home_dir);
    // current_length = strlen(cwd);
    // if (current_length >= home_length)
    // {
    //     strncpy(temp, cwd, home_length + 1);
    //     if (strcmp(temp, home_dir) == 0)
    //     {
    //         temp[0] = '~';
    //         temp[1] = '\0';
    //         strcat(temp, cwd + home_length);
    //         printf("<%s@%s:%s>", username, systemname, temp);
    //     }
    //     else
    //     {
    //         printf("<%s@%s:%s>", username, systemname, cwd);
    //     }
    // }
    // else
    // {
    //     printf("<%s@%s:%s>", username, systemname, cwd);
    // }
    fflush(stdout);
}

void child_terminate(int signal)
{
    int status;
    pid_t wpid = waitpid(-1, &status, WNOHANG);
    if (wpid > 0)
    {
        char *name = get_process_name(wpid);
        if (WIFEXITED(status))
        {
            fprintf(stderr, "\n\e[0;32m%s with pid %d exited normally\e[0m\n", name, wpid);
        }
        else
        {
            fprintf(stderr, "\n\e[0;91m%s with pid %d exited abnormally\e[0m\n", name, wpid);
        }
        free(name);
        remove_process(wpid);
        shell_prompt();
        // sleep(2);
        // fprintf(stdin, "\n");
        // fflush(stdin);
    }
}

void shell()
{
    char individual_command[1024];
    char *input;
    char *commands;
    int i = 0, no_of_commands;
    size_t input_size = 0;
    signal(SIGCHLD, child_terminate);
    // struct sigaction action;
    shell_setup();

    // action.sa_handler = child_terminate;
    // sigemptyset(&action.sa_mask);
    // sigaction(SIGCHLD, &action, NULL);
    signal(SIGCHLD, child_terminate);
    strcpy(prev_dir, home_dir);
    while (1)
    {
        i = 0;
        shell_prompt();
        input = input_line();
        if (strcmp(input, "") != 0)
        {
            if (his_start == NULL)
            {
                add_his_node(input);
                history_storage();
            }
            else if (strcmp(input, his_start->command) != 0)
            {
                add_his_node(input);
                history_storage();
            }
        }
        // getline(&input, &input_size, stdin);
        remove_spaces(input);

        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        char **commands = parse(input, ";", &no_of_commands);
        for (i = 0; i < no_of_commands; i++)
        {
            command_process(commands[i]);
        }
        freemem(&commands, no_of_commands);
    }
}