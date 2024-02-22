#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
//Вывести в каталоге название файлов которые начинаются с .

int main(int argc, char **argv)
{
    char *name = ".";
    char fname[256] = {0};
    if (argc==2) 
    {
        name = argv[1];
    }
    DIR* dir;
    dir = opendir(name);
    if (dir==NULL) return -1;
    struct dirent *entry; //инфа о файле
    struct stat st;
    while ((entry = readdir(dir))!=NULL)
    {
        sprintf(fname,"%s%s",name,entry->d_name);
        if (lstat(entry->d_name,&st)==-1)
        {
            perror("lstat");
            continue;
        }
        printf("Size: %ld;Name: %s\n",st.st_size,fname);
        if ((st.st_mode&__S_IFMT == __S_IFREG)&&(st.st_mode&S_IXUSR == __S_IFREG))
        printf("%ld\n",st.st_mtime);
    }
    closedir(dir);
}