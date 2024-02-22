#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(struct dirent *f)
{
    char *s = calloc(256,1);
    char *last = calloc(256,1);
    int i=0;
    readlink(f->d_name,s,256);
    while (s[0])
    {
        strcpy(last,s);
        free(s);
        s = calloc(256,1);
        readlink(last,s,256);
        i++;
    }
    if (i>0)
    printf("Name: %s;\nLink to: %s\n\n",f->d_name,last);
    free(s);
    free(last);
}

void dir1(DIR *dir)
{
    struct dirent *f_o_d;
    while ((f_o_d = readdir(dir))!=NULL)
    {
        print_file(f_o_d);
    }
    free(f_o_d);
}

int main()
{
    DIR *dir = opendir("./");
    dir1(dir);
    closedir(dir);
}