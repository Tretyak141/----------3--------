#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>


void sig_usr(int s)
{
    signal(SIGUSR1,sig_usr);
}

int main(int argc,char **argv)
{
    if (argc!=4) 
    {
        printf("ARGS!!!\n");
        exit(1);
    }
    #define MAXIMA atoi(argv[3])
    #define N1 atoi(argv[1])
    signal(SIGUSR1,sig_usr);
    int fd[2];
    pipe(fd);
    pid_t *pids;
    int N = atoi(argv[1]);
    pids = (pid_t*)malloc(sizeof(pid_t)*N);
    int num=-1;
    for (int i=0;i<N;i++)
    {
        pid_t tmp_pid = fork();
        if (!tmp_pid) 
        {
            num=i;
            break;
        }
        pids[i] = tmp_pid;
    }
    if (num>-1)
    {
        int buf;
        while(1)
        {
            pause();
            read(fd[0],&buf,4);
            if (!buf)
            {
                close(fd[0]);
                close(fd[1]);
                free(pids);
                exit(0);
            }
            printf("My num: %d; My pid: %d\n",buf,getpid());
            buf++;
            if (buf<=MAXIMA)
                write(fd[1],&buf,4);
            else
            {
                buf = 0;
                write(fd[1],&buf,4);
                kill(getppid(),SIGUSR1);
                free(pids);
                close(fd[0]);
                close(fd[1]);
                exit(0);
            }
            kill(getppid(),SIGUSR1);
        }
    }
    else
    {
        int buf = atoi(argv[2]);
        while (1)
        {
            int flag = 1;
            for (int i=0;i<N;i++)
            {                
                kill(pids[i],SIGUSR1);
                write(fd[1],&buf,4);
                pause();
                read(fd[0],&buf,4);
                if (!buf)
                {
                    waitpid(pids[i],0,0);
                    flag = 0;
                    num = i;
                    break;
                }

            }
            if (!flag)
            {
                break;
            }
        }
        close(fd[0]);
        for (int i=0;i<N;i++) 
        {
            buf = 0;
            write(fd[1],&buf,4);
            kill(pids[i],SIGUSR1);
            waitpid(pids[i],0,0);
        }
        close(fd[1]);
        free(pids);
        exit(0);
    }
}