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
    printf("Директория: %s\n\n",s);
    DIR *dir = NULL;
    dir = opendir(s);
    if (!dir) return;
    struct dirent *f_o_d;
    while ((f_o_d=readdir(dir))!=NULL)
    {
        struct stat st;
        lstat(f_o_d->d_name,&st);
        if ((!strcmp(f_o_d->d_name,".")) || (!strcmp(f_o_d->d_name,".."))) continue;
        char *dire = calloc(N,1);
        if ((st.st_mode & (S_IXGRP | S_IXUSR))&&(st.st_mode & __S_IFREG))
        {
            printf("%s\n",f_o_d->d_name);
        }
        else if (st.st_mode&__S_IFREG) printf("%s %d\n",f_o_d->d_name,st.st_mode);
        if (f_o_d->d_type == 4)
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