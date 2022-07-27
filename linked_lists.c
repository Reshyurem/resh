#include "header.h"

void add_bgp_node(int pid, char *process_name)
{
    bgp *bgp_node = (bgp *)calloc(1, sizeof(bgp)), *traversal_node;
    int max = 0;
    traversal_node = bgp_start;
    while (traversal_node != NULL)
    {
        if (traversal_node->pos > max)
            max = traversal_node->pos;
        traversal_node = traversal_node->next;
    }
    if (max < bgp_count)
    {
        bgp_count = max;
    }
    bgp_node->pid = pid;
    bgp_count++;
    bgp_node->pos = bgp_count;
    bgp_node->process_name = strdup(process_name);
    bgp_node->next = NULL;
    bgp_node->prev = NULL;

    if (bgp_start == NULL)
    {
        bgp_start = bgp_node;
        bgp_end = bgp_node;
    }
    else if (bgp_start == bgp_end)
    {
        if (strcmp(bgp_start->process_name, process_name) <= 0)
        {
            bgp_end = bgp_node;
            bgp_start->next = bgp_end;
            bgp_end->prev = bgp_start;
        }
        else
        {
            bgp_start = bgp_node;
            bgp_start->next = bgp_end;
            bgp_end->prev = bgp_start;
        }
    }
    else
    {
        traversal_node = bgp_end;
        while (traversal_node != NULL)
        {
            if (strcmp(traversal_node->process_name, process_name) > 0)
            {
                if (traversal_node->prev == NULL)
                {
                    bgp_node->next = traversal_node;
                    traversal_node->prev = bgp_node;
                    bgp_start = bgp_node;
                    break;
                }
                else
                {
                    traversal_node = traversal_node->prev;
                }
            }
            else
            {
                if (traversal_node->next != NULL)
                {
                    traversal_node->next->prev = bgp_node;
                }
                bgp_node->next = traversal_node->next;
                traversal_node->next = bgp_node;
                bgp_node->prev = traversal_node;
                if(traversal_node == bgp_end){
                    bgp_end = bgp_node;
                }
                break;
            }
        }
    }
}

char *get_process_name(int pid)
{
    bgp *bgp_node = bgp_start;
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
    bgp *bgp_node = bgp_start;
    if (bgp_start == NULL)
    {
        fprintf(stderr, "\e[91;mNo BGP to remove\n\e[0m");
    }
    else if (bgp_start == bgp_end)
    {
        // free(bgp_list->process_name);
        bgp *temp = bgp_start;
        bgp_start = NULL;
        bgp_end = NULL;
        free(temp);
    }
    else
    {
        while (bgp_node != NULL)
        {
            if (bgp_node->pid == pid)
            {
                // free(bgp_node->process_name);
                if (bgp_node->prev != NULL)
                {
                    (bgp_node->prev)->next = bgp_node->next;
                }
                else
                {
                    bgp_start = bgp_node->next;
                }
                if (bgp_node->next != NULL)
                {
                    bgp_node->next->prev = bgp_node->prev;
                }
                else
                {
                    bgp_end = bgp_node->prev;
                }
                free(bgp_node);
                break;
            }
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
        if (colorCode == 0)
        {
            printf("\e[0;32m");
        }
        printf("%s\n", node->command);
        if (colorCode == 0)
        {
            printf("\e[0m");
        }
    }
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