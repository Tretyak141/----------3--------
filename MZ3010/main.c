#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    int f1 = open(argv[2],O_RDONLY);
    int fd[2];
    pipe(fd);
    pid_t pid1,pid2;
    if ((pid1=fork())<0)
    {
        fprintf(stderr,"ERR\n");
        exit(1);
    }
    if (pid1==0)
    {
        dup2(f1,0);
        dup2(fd[1],1);
        close(fd[1]);
        close(fd[0]);
        close(f1);
        execlp(argv[1],argv[1],NULL);
        exit(1);
    }
    else
    {
        int st;
        waitpid(pid1,&st,0);
        if (!st)
        {
            if (!(pid2=fork()))
            {
                dup2(fd[1],1);
                close(fd[1]);
                close(fd[0]);
                execlp(argv[3],argv[3],NULL);
            }
        }
    }
    pid_t pid3 = fork();
    if (!pid3)
    {
        int f2 = open(argv[5],O_APPEND|O_WRONLY|O_CREAT,0777);
        dup2(fd[0],0);
        close(fd[0]);
        dup2(f2,1);
        close(f1);
        execlp(argv[4],argv[4],NULL);
        exit(1);
    }
}