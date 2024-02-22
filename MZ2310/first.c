#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc!=2) 
    {
        printf("Args\n");
        exit(1);
    }
    int N = atoi(argv[1]);
    for (int i=0;i<N;i++)
    {
        printf("%d: %d\n",i+1,getpid());
    }
    //exit(1);
    //exit(0);
    abort();
}