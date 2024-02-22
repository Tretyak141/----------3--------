#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void p(char *p1,char *par1,char *p2,char *par2)
{
    int pid1,pid2;
    if (!(pid1=fork()) || !(pid2=fork()))
    {
        if (!pid1)
        {
            execl(p1,p1,par1,NULL);
            exit(1);
        }
        if (!pid2)
        {
            execl(p2,p2,par2,NULL);
            exit(1);
        }
    }
    int st1,st2;
    waitpid(pid1,&st1,0);
    waitpid(pid2,&st2,0);
    if (WIFEXITED(st1))
        if (!__W_EXITCODE(st1,0)) printf("Error wth first process\n");
        else printf("Process 1 is OK\n");
    else printf("Process 1 was aborted\n");
    if (WIFEXITED(st2))
        if (!__W_EXITCODE(st2,0)) printf("Error wth second process\n");
        else printf("Process 2 is OK\n");
    else printf("Process 2 was aborted\n");
}

void s(char *p1,char *par1,char *p2,char *par2)
{
    char *s[2] = {p1,p2};
    char *pars[2] = {par1,par2};
    for (int i=0;i<2;i++)
    {
        pid_t pid;
        if (!(pid=fork()))
        {
            char *argv[1] = {pars[i]};
            execv(s[i],argv);
            exit(1);
        }
        int st1;
        waitpid(pid,&st1,0);
        if (WIFEXITED(st1))
            if (!__W_EXITCODE(st1,1)) printf("Error wth %d process\n",i);
            else printf("Process %d is OK\n",i);
        else printf("Process %d was aborted\n",i);
    }
}

int main(int argc,char **argv)
{
    if (argc!=6)
    {
        fprintf(stderr,"ARGS\n");
        exit(1);
    }
    if (!strcmp(argv[5],"p")) p(argv[1],argv[3],argv[2],argv[4]);
    else if (!strcmp(argv[5],"s")) s(argv[1],argv[3],argv[2],argv[4]);
    else printf("Error wth 5 parametr\n");
}