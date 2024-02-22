#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    if (argc!=6) 
    {
        fprintf(stderr,"ARGS\n");
        exit(1);
    }
    int fd[2];
    pipe(fd);
    pid_t pid1,pid2;
    int st1,st2;

    if (!(pid1=fork()))
    {
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);

        execlp(argv[1],argv[1],argv[2],NULL);
        fprintf(stderr,"ERROR\n");
        exit(1);
    }


    else if (pid1<0) 
    {
        fprintf(stderr,"ERROR\n");
        exit(1);
    }


    if (!(pid2=fork()))
    {
        close(fd[1]);
        dup2(fd[0],0);
        close(fd[0]);
        int f = open(argv[5],O_WRONLY);
        dup2(f,1);
        close(f);
        execlp(argv[3],argv[3],argv[4],NULL);
        fprintf(stderr,"ERROR\n");
        exit(1);
    }
    
    close(fd[1]);
    close(fd[2]);
    waitpid(pid2,&st2,0);
    waitpid(pid1,&st1,0);
}