#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


int func(const char *s)
{
    int len;
    if ((len=strlen(s))!=9)
    {
        fprintf(stderr,"-1\n");
        exit(1);
    }
    char modes[] = {'r','w','x'};
    int ans = 0;
    for (int i=0;i<len;i++)
    {
        if (((modes[i%3]!=s[i])&&(s[i]!='-')))
        {
            fprintf(stderr,"-1\n");
            exit(1);
        }
        char c = (modes[i%3]==s[i]) ? 1:0;
        ans = ans*2 + c;
    }
    return ans;
}

char *rwx(int mode)
{
    char modes[] = {'r','w','x'};
    if ((mode>0777)) return NULL;
    char *ans;
    ans = calloc(10,1);
    for (int i=0;i<9;i++) ans[i] = '-';
    int iter=8;
    while (mode)
    {
        ans[iter] = (mode%2) ? modes[iter%3]:ans[iter];
        iter--;
        mode = mode >> 1;
    }

    return ans;
}

void output()
{
    char *namedir = "./";
    DIR *dir = NULL;
    dir = opendir(namedir);
    if (!dir)
    {
        fprintf(stderr,"Ошибка при открытии директории\n");
        exit(1);
    }
    struct dirent *cur;
    while ((cur = readdir(dir))!=NULL)
    {
            struct stat st;
            stat(cur->d_name,&st);
            int d = st.st_ino;
            int mask = st.st_mode & 0777;
            char *mode = rwx(mask);
            char *type;
            if (cur->d_type==DT_REG) type = "Regular file";
            if (cur->d_type==DT_LNK) type = "Symb link";
            if (cur->d_type==DT_DIR) type = "Directory";
            printf("File_name: %s\nFile_descriptor: %d\nMode: %s\nType: %s\n\n",cur->d_name,d,mode,type);
            free(mode);
    }
    closedir(dir);
}

int main()
{
    printf("Input number\n");
    int N;
    scanf("%d",&N);
    char *s = rwx(N);
    if (!s) {printf("Error\n");exit(1);}
    printf("%s\n",s);
    printf("Check answer: %d\n",func(s));
    printf("Начат обход директории\n\n");
    output();
    free(s);
}