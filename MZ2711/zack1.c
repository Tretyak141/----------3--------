#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

void sig1(int s)
{
    signal(SIGUSR1,sig1);
    return;
}

int main()
{
    signal(SIGUSR1,sig1);
    int N;
    scanf("%d",&N);
    int fd[2];
    pipe(fd);
    int pid;
    if (!(pid=fork()))
    {
        pid_t pid1;
        if (!(pid1=fork()))
        {
            int buf=0;
            while (1)
            {
                printf("grandson %d\n",buf);
                fflush(stdout);
                buf++;
                if (buf>N) buf = -1;
                write(fd[1],&buf,4);
                kill(getppid(),SIGUSR1);
                if (buf<0)
                {
                    close(fd[0]);
                    close(fd[1]);
                    exit(0);
                }
                sleep(5);
                read(fd[0],&buf,4);
                if (buf<0) 
                {
                    close(fd[0]);
                    close(fd[1]);
                    exit(0);
                }
            }
        }
        int buf;
        while(1)
        {
            sleep(5);
            read(fd[0],&buf,4);
            if (buf==-1)
            {
                write(fd[1],&buf,4);
                kill(pid1,SIGUSR1);
                kill(getppid(),SIGUSR1);
                close(fd[0]);
                close(fd[1]);
                break;
            }
            printf("son %d\n",buf);
            fflush(stdout);
            buf++;
            if (buf>N) buf = -1;
            if (buf==-1)
            {
                write(fd[1],&buf,4);
                write(fd[1],&buf,4);
                close(fd[0]);
                close(fd[1]);
                kill(pid1,SIGUSR1);
                kill(getppid(),SIGUSR1);
                break;
            }
            write(fd[1],&buf,4);
            kill(getppid(),SIGUSR1);
            sleep(2);
            read(fd[0],&buf,4);
            if (buf==-1)
            {
                write(fd[1],&buf,4);
                kill(pid1,SIGUSR1);
                kill(getppid(),SIGUSR1);
                close(fd[0]);
                close(fd[1]);
                break;
            }
            printf("son %d\n",buf);
            fflush(stdout);
            buf++;
            if (buf>N) buf = -1;
            if (buf==-1)
            {
                write(fd[1],&buf,4);
                write(fd[1],&buf,4);
                close(fd[0]);
                close(fd[1]);
                kill(pid1,SIGUSR1);
                kill(getppid(),SIGUSR1);
                break;
            }
            write(fd[1],&buf,4);
            kill(pid1,SIGUSR1);
        }
        waitpid(pid1,0,0);
        exit(0);
    }
    int buf;
    while(1)
    {
        sleep(5);
        read(fd[0],&buf,4);
        if (buf==-1) 
        {
            close(fd[0]);
            close(fd[1]);
            break;
        }
        printf("father %d\n",buf);
        fflush(stdout);
        buf++;
        if (buf>N) buf = -1;
        if (buf==-1)
        {
            write(fd[1],&buf,4);
            kill(pid,SIGUSR1);
            close(fd[0]);
            close(fd[1]);
            break;
        }
        write(fd[1],&buf,4);
        kill(pid,SIGUSR1);
    }
    waitpid(pid,0,0);
    exit(0);
}