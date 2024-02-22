#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    FILE *f1,*f2,*f3;
    if (!(f1 = fopen(argv[1],"r"))) 
    {
        fprintf(stderr,"Error with 1 file\n");
        exit(1);
    }
    if (!(f2 = fopen(argv[2],"r"))) 
    {
        fprintf(stderr,"Error with 2 file\n");
        exit(1);
    }
    if (!(f3 = fopen(argv[3],"a"))) 
    {
        fprintf(stderr,"Error with 3 file\n");
        exit(1);
    }
    int N = atoi(argv[4]);
    char *s1,*s2;
    s1 = malloc(N+1);
    s2 = malloc(N+1);
    while ((!feof(f1))||(!feof(f2)))
    {
        char *c1,*c2;
        c1 = fgets(s1,N,f1);
        c2 = fgets(s2,N,f2);
        if (!c1)
        {
            fputs(s2,f3);
            continue;
        }
        if (!c2)
        {
            fputs(s1,f3);
            continue;
        }
        fputs(s1,f3);
        fputs(s2,f3);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    free(s1);
    free(s2);
    return 0;
}