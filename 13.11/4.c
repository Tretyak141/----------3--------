#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int counter=0;

void Z(int s)
{
    counter+=3;
    printf("\n +3 %d \n",counter);
}

void C(int s)
{
    counter-=4;
    if (counter<0)
    {
        printf("\nStop, counter = %d\n",counter);
        exit(0);
    }
    else printf("\n -4 %d \n",counter);
}

int main(int argc,char **argv)
{
    if (argc!=2)
    {
        printf("ARGS!!!\n");
        exit(1);
    }
    int N=atoi(argv[1]);
    counter = N;
    signal(SIGTSTP,Z);
    signal(SIGINT,C);
    while(1);
}