#include <stdio.h>
#include <limits.h>

void next(long unsigned *c,int a)
{
    *c *=10;
    *c += a;
}

int main()
{
    int c;
    long unsigned ans=0;
    while ((c=getchar())!=EOF)
    {
        int new_symb;
        if (((char)c>='0')&&(c<='9'))
        {
            new_symb = c-'0';
            long unsigned temp;
            if ((temp=ans*10 + new_symb)>UINT_MAX)
            {
                ans = 0;
                printf("New decimal created\n");
                next(&ans,new_symb);
            }
            else next(&ans,new_symb);
            printf("%lu\n",ans);
        }
    }
}