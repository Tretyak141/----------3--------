#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

volatile int c_z=0;
int c_c=0;

void handler_z(int s)
{
    signal(SIGTSTP,handler_z);
    putchar(10);
    if (c_c>=2 && (c_c<4))
    {
        c_z++;
        return;
    }
    else return;
}

void handler_c(int s)
{
    signal(SIGINT,handler_c);
    c_c++;
    if (c_c==6)
    {
        printf("\n%d\n",c_z);
        exit(0);
    }
    putchar(10);
    return;
}

int main()
{
    signal(SIGINT,handler_c);
    signal(SIGTSTP,handler_z);
    while(1);
}