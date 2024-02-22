#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define N 100000

void print_dir_files(const char *s)
{
    printf("\nДиректория: %s\n\n",s);
    DIR *dir = NULL;
    dir = opendir(s);
    if (!dir) return;
    struct dirent *f_o_d;
    while ((f_o_d=readdir(dir))!=NULL)
    {
        struct stat st;
        if ((!strcmp(f_o_d->d_name,".")) || (!strcmp(f_o_d->d_name,".."))) continue;
        char *dire = calloc(N,1);
        if (f_o_d->d_type == DT_REG)
        {
            strcat(dire,s);
            strcat(dire,f_o_d->d_name);
            stat(dire,&st);
            int mask = S_IXUSR| S_IXGRP;
            if ((st.st_mode&mask)==mask)
            {
                printf("%s\n",f_o_d->d_name);
            }
        }
        if (f_o_d->d_type == DT_DIR)
        {
            strcat(dire,s);
            strcat(dire,f_o_d->d_name);
            strcat(dire,"/");
            print_dir_files(dire);
        }
        free(dire);
    }
    printf("\nКонец директории %s\n\n",s);
    closedir(dir);
}

int main()
{
    char s[N] = "./";
    print_dir_files(s);
}