#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
    char c = argv[2][0];
    FILE *f1 = fopen(argv[1],"r");
    FILE *tmp = fopen("_ABACABA_","w");
    char ch;
    while (!feof(f1))
    {
        ch = fgetc(f1);
        if (ch!=c)
        {
            fputc(ch,tmp);
        }
    }
    fclose(f1);
    remove(argv[1]);
    rename("_ABACABA_",argv[1]);
}