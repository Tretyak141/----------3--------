#include <stdio.h>
#include <sys/file.h>
#include <dirent.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

char *func2(int val)
{
    if (val>0777) return NULL;
    char *str = malloc(10);

    for (int i = 0; i < 9; i++)
    {
        if ((val >> (8 - i)) & 1)
        {
            if ((8 - i) % 3 == 2)
                str[i] = 'r';
            if ((8 - i) % 3 == 1)
                str[i] = 'w';
            if ((8 - i) % 3 == 0)
                str[i] = 'x';
        }
        else
            str[i] = '-';
    }
    str[9] = '\0';

    return str;
}

char* get_type(struct dirent* d){
    switch(d->d_type)
    {
        case DT_LNK: return "LNK";
        case DT_DIR: return "DIR";
        case DT_REG: return "REG";
    }
    return "other";
}

void print_dir()
    {
    DIR* dr = opendir(".");
    struct dirent* file;
    char* str_type, *str_name, *str_p;
    char filename[1024];
    int d;
    struct stat st;
    while(file= readdir(dr))
        {
            sprintf(filename, "./%s", file->d_name);
            stat(filename, &st);
            str_p = func2(st.st_mode & (S_IRWXU|S_IRWXG|S_IRWXO));
            str_name = file->d_name;
            str_type = get_type(file);
            d = st.st_ino;
            printf("Name: %s, fd: %d, Type: %s, p: %s\n", str_name, d, str_type,  str_p);
            free(str_p);
        }
    closedir(dr);
    }

int main()
{
    print_dir();
    return 0;
}
