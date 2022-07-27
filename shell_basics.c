#include "header.h"

// char *input;
// size_t input_size;

void shell_setup()
{
    gethostname(systemname, 1024);
    strcpy(username, getenv("USER"));
    // getlogin_r(username, 1024);
    getcwd(home_dir, 1024);
    parent = getpid();
    history_retrieval();
}

void shell_prompt()
{
    char cwd[1024];
    getcwd(cwd, 1024);
    rel_path(cwd);
    printf("\e[0;96m<%s@%s:\e[1;34m%s\e[0;96m> \e[0m", username, systemname, cwd);
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
    if (wpid > 0 && wpid != fg_proc)
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

void fg_to_bg_handler(int signal)
{
    if (fg_proc != 0)
    {
        add_bgp_node(fg_proc, fg_proc_name);
        kill(fg_proc, 20);
        printf("\r");
    }
}

void ctrl_C_handler(int signal)
{
    if (fg_proc != 0)
    {
        kill(fg_proc, 2);
        printf("\r");
    }
}

// void vim_handler(int signal){
//     command_process("gedit");
// }

void shell()
{
    char individual_command[1024];
    char *input;
    char *commands;
    int i = 0, no_of_commands;
    size_t input_size = 0;
    // signal(SIGCHLD, child_terminate);
    // signal(SIGTTIN, vim_handler);
    // signal(SIGTTOU, vim_handler);
    // signal(SIGTTOU, shell_close);
    // struct sigaction child, fg_to_bg, ctrl_C;
    shell_setup();

    // child.sa_handler = child_terminate;
    // sigemptyset(&child.sa_mask);
    // sigaction(SIGCHLD, &child, NULL);

    // fg_to_bg.sa_handler = fg_to_bg_handler;
    // sigemptyset(&fg_to_bg.sa_mask);
    // sigaction(SIGTSTP, &fg_to_bg, NULL);

    // ctrl_C.sa_handler = ctrl_C_handler;
    // sigemptyset(&ctrl_C.sa_mask);
    // sigaction(SIGINT, &ctrl_C, NULL);

    signal(SIGCHLD, child_terminate);
    signal(SIGTSTP, fg_to_bg_handler);
    signal(SIGINT, ctrl_C_handler);

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
            bripe(commands[i]);
            // command_process(commands[i]);
        }
        freemem(&commands, no_of_commands);
    }
}