#include "header.h"

char username[1024] = "";
char systemname[1024] = "";
char home_dir[1024] = "";
char prev_dir[1024] = "";
bgp *bgp_start = NULL;
bgp *bgp_end = NULL;
his *his_start = NULL;
his *his_end = NULL;
int his_count = 0;
int bgp_count = 0;
int fg_proc = 0;
char* fg_proc_name = NULL;
char colorCode = 0;
int parent = 0;
int baywatch = 1;

int main()
{
    shell();
}