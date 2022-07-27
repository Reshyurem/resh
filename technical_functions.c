#include "header.h"

void remove_spaces(char string[])
{
    char temp[strlen(string) + 1];
    int i = 0, j = 0;

    while (string[i] != '\0')
    {
        if (string[i] == '\t')
            string[i] = ' ';
        i++;
    }

    i = 0;

    while (string[i] != '\0')
    {
        while (string[i] == ' ' && string[i + 1] == ' ')
            i++;

        temp[j] = string[i];
        j++;
        i++;
    }

    i = 0;

    // while (temp[i] != '\0')
    // {
    //     if (temp[i] == ';')
    //         temp[i] = ' ';
    //     i++;
    // }

    temp[j] = '\0';
    strcpy(string, temp);
}

char *combination(char **argument, int start_part, int end_part)
{
    char temp[1024] = "";
    for (int i = start_part; i < end_part; i++)
    {
        strcat(temp, argument[i]);
        strcat(temp, " ");
    }
    temp[strlen(temp) - 1] = '\0';
    return strdup(temp);
}

void backspace(char str[], int *position)
{
    if (*position > 0)
    {
        if (str[(*position) - 1] == 9)
        {
            for (int i = 0; i < 7; i++)
            {
                printf("\b");
            }
        }
        str[--(*position)] = '\0';
        printf("\b \b");
    }
}

void rel_path(char *path)
{
    char temp[1024] = "";
    int home_length, current_length;
    home_length = strlen(home_dir);
    // printf("home:%s\ncurr:%s\n", home_dir, path);
    current_length = strlen(path);
    // printf("hlen:%d\nclen:%d\n", home_length, current_length);
    if (current_length >= home_length)
    {
        strncpy(temp, path, home_length);
        // printf("temp:%s\n", temp);
        if (strcmp(temp, home_dir) == 0)
        {
            temp[0] = '~';
            // temp[1] = '/';
            temp[1] = '\0';
            strcat(temp, path + home_length);
            strcpy(path, temp);
            // printf("sent:%s\n", path);
        }
    }
}

char *input_line()
{
    char temp_string[1024] = "", c;
    int i = 0, x = 0;
    his *node = NULL;
    setbuf(stdout, NULL);
    enableRawMode();
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c)) // control characters
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                if (read(STDIN_FILENO, buf, 2) == 2)
                {
                    if (buf[0] == 91 && buf[1] == 65)
                    {
                        // printf("[%d%d%d]\n", c, buf[0], buf[1]);
                        while (x > 0)
                        {
                            backspace(temp_string, &x);
                        }
                        if (node == NULL)
                        {
                            node = his_start;
                        }
                        else if (node->prev != NULL)
                        {
                            node = node->prev;
                        }

                        if (node != NULL)
                        {
                            printf("%s", node->command);
                            x += strlen(node->command);
                            strcpy(temp_string, node->command);
                        }
                    }
                    if (buf[0] == 91 && buf[1] == 66)
                    {
                        while (x > 0)
                        {
                            backspace(temp_string, &x);
                        }
                        if (node != NULL)
                        {
                            if (node->next != NULL)
                            {
                                node = node->next;
                                printf("%s", node->command);
                                x += strlen(node->command);
                                strcpy(temp_string, node->command);
                            }
                        }
                        else
                        {
                            node = NULL;
                            strcpy(temp_string, "");
                        }
                    }
                }
            }
            else if (c == 127)
            { // backspace
                backspace(temp_string, &x);
                if (x == 0)
                {
                    node = NULL;
                }
            }
            else if (c == 9)
            { // TAB character
                temp_string[x++] = c;
                for (int i = 0; i < 8; i++)
                { // TABS should be 8 spaces
                    printf(" ");
                }
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            temp_string[x++] = c;
            printf("%c", c);
        }
    }
    printf("\n");
    disableRawMode();
    // strcat(temp_string, "\n");
    return strdup(temp_string);

    // if ((c = getch()) == '\033')
    // {
    //     getch();
    //     char *return_string = (char *)malloc(sizeof(char) * i + 1);
    //     strcpy(return_string, "");
    //     if (getch() == 'A')
    //     {
    //         printf("Go Up");
    //         strcpy(return_string, "echo go up\n");
    //     }
    //     return return_string;
    // }
    // else
    // {
    //     temp_string[0] = c;
    //     i++;
    //     while ((c = getch()) != '\n')
    //     {
    //         temp_string[i] = c;
    //         i++;
    //     }
    //     temp_string[i] = '\n';
    //     char *return_string = (char *)malloc(sizeof(char) * i + 1);
    //     strcpy(return_string, temp_string);
    //     return return_string;
    // }
}

int quitter(int sleeper)
{
    char c;
    int ret;
    int i = 1;
    clock_t current = clock();
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    setbuf(stdout, NULL);
    enableRawMode();
    struct pollfd pfds;
    pfds.fd = STDIN_FILENO;
    pfds.events = POLLIN;
    while (1)
    {
        if (clock() - current > 1000000 * sleeper)
        {
            // FILE *fp;
            // fp = fopen("/proc/meminfo", "r");
            // if (fp != NULL)
            // {
            //     char **stats;
            //     char *buf = (char *)malloc(1800);
            //     int no_of_parts, len, bufsize = 1800;
            //     fread(buf, sizeof(char), sizeof(buf), fp);
            //     printf("%s", buf);
            //     stats = parse(buf, " \n", &no_of_parts);
            //     for(int i = 0; i < no_of_parts; i++){
            //         // printf("%d", no_of_parts);
            //         if(strcmp(stats[i], "Dirty:") == 0){
            //             printf("%s %s\n", stats[i + 1], stats[i + 2]);
            //             break;
            //         }
            //     }
            //     free(buf);
            //     freemem(&stats, no_of_parts);
            //     fclose(fp);
            bripe("cat /proc/meminfo | grep Dirty:");
            // }
            // else
            // {
            //     perror("\e[0;91mFile Opening");
            //     fprintf(stderr, "\e[0m");
            //     break;
            // }
            current = clock();
        }
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (c == 113)
            {
                break;
            }
        }
    }
    printf("\n");
    disableRawMode();
    fcntl(STDIN_FILENO, F_SETFL, flags);
    return ret;
}