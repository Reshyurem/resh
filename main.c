#include "header.h"

char username[1024] = "";
char systemname[1024] = "";
char home_dir[1024] = "";
char prev_dir[1024] = "";
bgp *bgp_list = NULL;
his *his_start = NULL;
his *his_end = NULL;
int his_count = 0;

int main()
{
    shell();
}