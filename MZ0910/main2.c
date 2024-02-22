#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
    struct stat st;
    if ((st.st_mode&__S_IFMT == __S_IFREG)&&(st.st_mode&S_IXUSR == __S_IFREG))
        printf("%d\n",st.st_atime);
}