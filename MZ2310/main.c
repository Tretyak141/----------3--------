#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc,char **argv)
{
    if (argc!=3) 
    {
        fprintf(stderr,"Args, pidor\n");
        exit(1);
    }
    int fd[2];
    pipe(fd);
    int fd1 = open(argv[1],O_RDONLY),fd2 = open(argv[2],O_WRONLY);
    char s;
    int counter = 1;
    while (read(fd1,&s,1))
    {
        if (counter%2)
        {
            write(fd[1],&s,1);
        }
        if (s=='\n')
            counter++;
    }
    close(fd[1]);
    int pid;
    if ((pid=fork())<0) 
    {
        fprintf(stderr,"Error wth fork\n");
        exit(1);
    }
    else if (!pid)
    {
        dup2(fd[1],1);
        close(fd[1]);
        execlp("sort","sort","-o",fd1,NULL);
        exit(1);
    }
    waitpid(pid,0,0);
    close(fd1);
    close(fd2);
}