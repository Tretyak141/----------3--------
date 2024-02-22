#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int flag = 0;

int input_file;

pid_t ppid;
pid_t pid;

void sig_usr1(int s)
{
    printf("Son ");
    fflush(stdout);
    char buf;
    int check = 1;
    while ((check=read(input_file,&buf,1))&&(buf!=' ')&&(buf!='\n')&&(buf!='\t')) 
    {
        putchar(buf);
        fflush(stdout);
    }
    printf(" ");
    if (!check)
    {
        close(input_file);
        exit(0);
    }
    fflush(stdout);
    return;
}

void alrm_handler(int s)
{
    close(input_file);
    exit(0);
}

void sigch(int s)
{
    wait(0);
    close(input_file);
    exit(0);
}

void sig_usr2(int s)
{
    printf("Father ");
    fflush(stdout);
    char buf;
    int check = 1;
    while ((check=read(input_file,&buf,1))&&(buf!=' ')) 
    {
        putchar(buf);
        fflush(stdout);

    }
    printf(" ");
    fflush(stdout);

    if (!check) 
    {
        kill(pid,SIGALRM);
        waitpid(pid,0,0);
        exit(0);
    }
    fflush(stdout);
    return;
}

int main(int argc,char **argv)
{
    input_file = open("main2.c",O_RDONLY);
    ppid = getpid();
    signal(SIGALRM,alrm_handler);
    signal(SIGUSR1,sig_usr1);
    signal(SIGUSR2,sig_usr2);
    signal(SIGCHLD,sigch);
    alarm(1);
    pid = fork();
    if (!pid)
    {
        kill(ppid,SIGUSR2);
        while(1)
        {
            pause();
            kill(ppid,SIGUSR2);
        }
    }
    pause();
    while(1)
    {
        kill(pid,SIGUSR1);
        pause();
    }
}