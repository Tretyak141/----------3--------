#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc,char** argv)
{
    long sum=0,count=0;
    char* tmp;
    for (int i=1;i<argc;i++)
    {
        long temper = strtol(argv[i],&tmp,10);
        if (!tmp&&(temper==(pow(2,32)-1)||temper==(-pow(2,32)-1))) continue;
        else {count++;sum+=temper;};
    }
    printf("ans: %f\n",(double)sum/count);
    return 0;
}