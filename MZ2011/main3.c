#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

pid_t pid1;
pid_t pid2;
pid_t father;

int symb;

void sig_usr1(int s)
{
    symb = 0;
    return;
}

void sig_usr2(int s)
{
    symb = 1;

    return;
}

void sig_alrm(int s)
{
}

void sigint(int s)
{
    wait(0);
    wait(0);
    exit(0);
}

void sigio(int s)
{
    kill(father,SIGINT);
    exit(0);
}

int main(int argc,char **argv)
{
    if (argc!=2) return 1;
    signal(SIGALRM,sig_alrm);
    signal(SIGIO,sigio);
    signal(SIGUSR1,sig_usr1);
    signal(SIGUSR2,sig_usr2);
    signal(SIGINT,sigint);
    father = getpid();
    if (!(pid1 = fork()))
    {
        int i=0;
        int c[8];
        while(1)
        {
            pause();
            c[i] = symb;
            i++;
            
            if (i==8)
            {
                char s=0;
                for (int j=0;j<8;j++)
                {
                    s+=(c[j]<<(7-j));
                }
                printf("%c",s);
                fflush(stdout);
                i = 0;
            }
            kill(0,SIGALRM);
        }
    }
    else if (!(pid2=fork()))
    {
        char buf;
        int fd = open(argv[1],O_RDONLY);
        while (read(fd,&buf,1))
        {

            int arr[8];
            for (int i=0;i<8;i++)
            {
                arr[7-i] = buf%2;
                buf/=2;
            }
            for(int i=0;i<8;i++)
            {
                if (arr[i])
                {
                    kill(pid1,SIGUSR2);
                }
                else
                {
                    kill(pid1,SIGUSR1);
                }
                pause();
            }
        }
        kill(pid1,SIGIO);
        close(fd);
        exit(0);
    }
    signal(SIGALRM,SIG_IGN);
}