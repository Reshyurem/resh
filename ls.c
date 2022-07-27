#include "header.h"

void list_form(char *path, char *name, int df)
{
    char temp[1024] = "", c;
    strcpy(temp, path);
    if (df)
    {
        if (temp[strlen(temp) - 1] != '/')
            strcat(temp, "/");
        strcat(temp, name);
    }
    struct stat stat_obj;
    lstat(temp, &stat_obj);
    if (S_ISDIR(stat_obj.st_mode))
    {
        c = 'd';
    }
    else if (S_ISREG(stat_obj.st_mode))
    {
        c = '-';
    }
    else if (S_ISLNK(stat_obj.st_mode))
    {
        c = 'l';
    }
    printf("%c", c);
    printf("%c", S_IRUSR & stat_obj.st_mode ? 'r' : '-');
    printf("%c", S_IWUSR & stat_obj.st_mode ? 'w' : '-');
    printf("%c", S_IXUSR & stat_obj.st_mode ? 'x' : '-');
    printf("%c", S_IRGRP & stat_obj.st_mode ? 'r' : '-');
    printf("%c", S_IWGRP & stat_obj.st_mode ? 'w' : '-');
    printf("%c", S_IXGRP & stat_obj.st_mode ? 'x' : '-');
    printf("%c", S_IROTH & stat_obj.st_mode ? 'r' : '-');
    printf("%c", S_IWOTH & stat_obj.st_mode ? 'w' : '-');
    printf("%c", S_IXOTH & stat_obj.st_mode ? 'x' : '-');
    printf(" %3lu ", stat_obj.st_nlink);
    printf("%-15s", getpwuid(stat_obj.st_uid)->pw_name);
    if (getgrgid(stat_obj.st_gid))
        printf("%-15s", getgrgid(stat_obj.st_gid)->gr_name);
    printf("%9ld", stat_obj.st_size);

    struct tm then, now;
    char time_val[256];

    time_t present = time(0);

    localtime_r(&stat_obj.st_mtime, &then);
    localtime_r(&present, &now);

    if (present - stat_obj.st_mtime < 15552000)
        strftime(time_val, sizeof(time_val), "%b %e %H:%M", localtime(&stat_obj.st_mtime));
    else
        strftime(time_val, sizeof(time_val), "%b %e %-5Y", localtime(&stat_obj.st_mtime));

    printf(" %s %-s\n", time_val, name);
}

void ls_command(int a_flag, int l_flag, int mul_flag, char *dir)
{
    DIR *directory = opendir(dir);
    struct dirent *directory_item;
    struct stat stat_buf;
    long int block_size = 0;
    if (directory == NULL)
    {
        if (access(dir, F_OK) == 0)
        {
            if (l_flag)
            {
                if (mul_flag > 0)
                    printf("\e[0;32m%s:\n", dir);
                list_form(dir, dir, 0);
            }
            else
                printf("\e[0;32m%s\n\e[0m", dir);
        }
        else
        {
            fprintf(stderr, "\e[0;91m");
            perror(dir);
            fprintf(stderr, "\e[0m");
        }
    }
    else
    {
        while ((directory_item = readdir(directory)) != NULL)
        {
            if (directory_item->d_name[0] == '.' && !a_flag)
                continue;
            lstat(directory_item->d_name, &stat_buf);
            block_size += stat_buf.st_blocks;
        }
        if (mul_flag > 0)
            printf("\e[0;34m%s:\n", dir);
        if (l_flag)
            printf("total %ld\n", block_size / 2);

        closedir(directory);

        directory = opendir(dir);

        while (directory_item = readdir(directory))
        {
            if (directory_item->d_name[0] == '.' && !a_flag)
                continue;
            if (l_flag)
            {
                printf("\e[0;32m");
                list_form(dir, directory_item->d_name, 1);
            }
            else
                printf("\e[0;32m%s\n", directory_item->d_name);
        }
        printf("\e[0m");
    }
}