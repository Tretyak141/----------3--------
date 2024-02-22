#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#define LENIN 256

int main(int argc,char **argv)
{
    if (argc!=2)
    {
        fprintf(stderr,"Error wth argc\n");
        exit(1);
    }
    FILE* f = fopen(argv[1],"w");
    int fd1[2],fd2[2];
    char s[LENIN];
    pid_t pids;
    pipe(fd1);
    pipe(fd2);
    pids = fork();
    if (pids>0)
    {
        close(fd1[0]);
        close(fd2[1]);
        char c;
        while ((c=getchar())!=EOF)
        {
            fgets(s,LENIN,stdin);
            for (int i=0;s[i]!=0;i++)
            {
                if (s[i]=='\n') 
                {
                    s[i] = 0;
                    break;
                }
            }
            fprintf(f,"%c%s",c,s);
            fflush(f);
            //int i=0;
            write(fd1[1],&c,1);
            write(fd1[1],s,strlen(s)+1);
            int num;
            read(fd2[0],&num,4);
            if (num==-1)
            {
                fclose(f);
                waitpid(pids,0,0);
                return 0;
            }
            fflush(f);
        }
        fclose(f);
        close(fd1[1]);
        close(fd2[0]);
        return 0;
    }
    else
    {
        close(fd1[1]);
        close(fd2[0]);
        //fclose(f);
        while (1)
        {
            char c;
            read(fd1[0],&c,1);
            char s[LENIN];
            read(fd1[0],&s,LENIN);
            int i=0;
            int counter = 0;
            for (int i=0;s[i]!=0;i++)
            {
                //printf("%c\n",s[i]);
                if (s[i]==c) counter++;
                if (s[i]=='\n')
                {
                    s[i] = 0;
                    break;
                }
            }
            if (!strcmp(s," exit"))
            {
                int buf = -1;
                write(fd2[1],&buf,4);
                close(fd1[0]);
                close(fd2[1]);
                exit(0);
            }
            //printf("%d\n",counter);
            fprintf(f," %d\n",counter);
            fflush(f);
            write(fd2[1],&counter,4);

        }
    }
}