#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int flag = 0;

pid_t ppid;
pid_t pid;

void sig_usr1(int s)
{
    printf("Son ");
    fflush(stdout);
    return;
}

void alrm_handler(int s)
{
    exit(0);
}


void sig_child(int s)
{
    waitpid(pid,0,0);
    flag = 1;
}

void sig_usr2(int s)
{
    printf("Father ");
    fflush(stdout);
    return;
}

int main()
{
    ppid = getpid();
    signal(SIGALRM,alrm_handler);
    signal(SIGUSR1,sig_usr1);
    signal(SIGUSR2,sig_usr2);
    signal(SIGCHLD,sig_child);
    pid = fork();
    if (!pid)
    {   
        alarm(2);
        
        while(1)
        {
            pause();
            kill(ppid,SIGUSR2);
        }
    }
    printf("Father ");
    while(1)
    {
        if (!flag)
        {
            kill(pid,SIGUSR1);
            pause();
            continue;
        }
        printf("Father ");
        fflush(stdout);
        sleep(1);
    }
}