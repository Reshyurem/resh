#include "header.h"

void add_bgp_node(int pid, char *process_name)
{
    bgp *bgp_node = (bgp *)calloc(1, sizeof(bgp));
    bgp_node->pid = pid;
    bgp_node->process_name = strdup(process_name);
    bgp_node->next = NULL;
    if (bgp_list == NULL)
    {
        bgp_list = bgp_node;
    }
    else
    {
        bgp_node->next = bgp_list;
        bgp_list = bgp_node;
    }

    // char *name = (char *)calloc(1024, sizeof(char));
    // char *fname = (char *)calloc(1024, sizeof(char));
    // if (name && fname)
    // {
    //     sprintf(name, "/proc/%d/cmdline", pid);
    //     FILE *f;
    //     f = fopen(name, "r");
    //     if (f)
    //     {
    //         size_t size;
    //         size = fread(fname, sizeof(char), 1024, f);
    //         if (size > 0)
    //         {
    //             if ('\n' == fname[size - 1])
    //                 fname[size - 1] = '\0';
    //         }
    //         fclose(f);
    //     }
    // }
    // bgp *bgp_node = (bgp *)calloc(1, sizeof(bgp));
    // bgp_node->pid = pid;
    // bgp_node->process_name = strdup(fname);
    // bgp_node->next = NULL;
    // if (bgp_list == NULL)
    // {
    //     bgp_list = bgp_node;
    // }
    // else
    // {
    //     bgp_node->next = bgp_list;
    //     bgp_list = bgp_node;
    // }
    // free(name);
    // free(fname);
}

char *get_process_name(int pid)
{
    bgp *bgp_node = bgp_list;
    while (bgp_node != NULL)
    {
        if (bgp_node->pid == pid)
        {
            return bgp_node->process_name;
        }
        bgp_node = bgp_node->next;
    }
    return "";
}

void remove_process(int pid)
{
    bgp *bgp_node = bgp_list, *prev = bgp_list;
    if (bgp_list->pid == pid)
    {
        // free(bgp_list->process_name);
        bgp *temp = bgp_list;
        bgp_list = bgp_list->next;
        free(temp);
    }
    else
    {
        while (bgp_node != NULL)
        {
            if (bgp_node->pid == pid)
            {
                // free(bgp_node->process_name);
                prev->next = bgp_node->next;
                free(bgp_node);
                break;
            }
            prev = bgp_node;
            bgp_node = bgp_node->next;
        }
    }
}

void add_his_node(char *command)
{
    his *his_node = (his *)calloc(1, sizeof(his));
    his_node->command = strdup(command);
    his_node->next = NULL;
    his_node->prev = NULL;
    if (his_start == NULL && his_end == NULL)
    {
        his_start = his_node;
        his_end = his_node;
    }
    else if (his_start == his_end)
    {
        his_end->next = his_node;
        his_node->prev = his_end;
        his_start = his_node;
    }
    else
    {
        his_node->prev = his_start;
        his_start->next = his_node;
        his_start = his_node;
    }
    his_count++;

    if (his_count > 20)
    {
        his *temp = his_end;
        his_end = his_end->next;
        free(temp->command);
        free(temp);
        his_count--;
    }
}

void history_command(his *node, int no)
{
    if (node != NULL && no > 0)
    {
        history_command(node->prev, no - 1);
        printf("\e[0;32m%s\n", node->command);
    }
    printf("\e[0m");
}

void history_storage()
{
    his *temp = his_end;
    FILE *fp = fopen("/tmp/history.txt", "w");
    while (temp != NULL)
    {
        fprintf(fp, "%s\n", temp->command);
        temp = temp->next;
    }
    fclose(fp);
}

void history_retrieval()
{
    FILE *fp = fopen("/tmp/history.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    if (fp != NULL)
    {
        while ((read = getline(&line, &len, fp)) != -1)
        {
            if (read > 0)
            {
                if (line[read - 1] == '\n')
                {
                    line[read - 1] = '\0';
                }
                add_his_node(line);
            }
        }
        fclose(fp);
    }

    if (line != NULL)
    {
        free(line);
    }
}