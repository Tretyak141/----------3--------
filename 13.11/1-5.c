#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

//1) Нет, не сбрасывается
//2) А) Блокируется
//   Б) Нет, не блокируется
//3) Да, отцу тоже отправлется

pid_t parpid;

int counter=0;

void handler_z(int s)
{
    signal(SIGTSTP,handler_z);
    pid_t pid=getpid();
    printf("\nI'm Andrew's handler for CTRL+Z %d\n",pid);
    if (pid!=parpid) exit(0);
    else return;
}

void handler_chld(int s)
{
    printf("\nCHLD handler\n");
}

int main()
{
    signal(SIGTSTP,handler_z);
    signal(SIGCHLD,SIG_IGN);
    //signal(SIGCHLD,handler_chld);
    pid_t pid1;
    parpid = getpid();
    if (!(pid1=fork()))
    {
        while(1);
    }

    else while (1);
}