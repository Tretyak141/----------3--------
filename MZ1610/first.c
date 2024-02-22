#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int func(const char *s)
{
    int len;
    if ((len=strlen(s))!=9)
    {
        fprintf(stderr,"-1\n");
        exit(1);
    }
    char modes[] = {'r','w','x'};
    int ans = 0;
    for (int i=0;i<len;i++)
    {
        if (((modes[i%3]!=s[i])&&(s[i]!='-')))
        {
            fprintf(stderr,"-1\n");
            exit(1);
        }
        char c = (modes[i%3]==s[i]) ? 1:0;
        ans = ans*2 + c;
    }
    return ans;
}

int main()
{
    char *s = malloc(10);
    scanf("%s",s);
    printf("%o\n",func(s));
    free(s);
}