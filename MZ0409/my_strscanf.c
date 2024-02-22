#include <stdio.h>
#include <stdlib.h>

char* my_strscanf()
{
    int c;
    char* s=(char*)calloc(sizeof(char),1);
    int i=0;
    
    while ((c=getchar())!=EOF)
    {
        i++;
        s = (char*)realloc(s,sizeof(char)*i);
        s[i-1] = (char)c;
    }
    s = (char*)realloc(s,sizeof(char)*(i+1));
    s[i] = 0;
    return s;
}

int main()
{
    char* s = my_strscanf();
    printf("%s\n",s);
    return 0;
}