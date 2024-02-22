#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void sort(char* s)
{
    int i=0;
    while (s[i])
    {
        char min=s[i];
        int ind=i;
        int j=i+1;
        while (s[j])
        {
            if (s[j]<min)
            {
                min = s[j];
                ind=j;
            }
            j++;
        }
        char temp=s[i];
        s[i]=s[ind];
        s[ind] = temp;
        i++;
    }
}


double rev_d(double x)
{
    char *s = (char*)calloc(sizeof(char),308);
    sprintf(s,"%f",x);
    int sign;
    if (x>0) sign=1;
    else sign=0;
    char *s_new = (char*)calloc(sizeof(char),308);
    if (!sign) s_new[0] = '-';
    else s_new[0]='+';
    int length=strlen(s);
    int i=1;
    for (i=1;i<length;i++)
    {
        s_new[i] = s[length-i];

    }
    if (x>0) s_new[i]=s[0];     
    return strtod(s_new,0);
}


int main(int argc,char**argv)
{
    long size = 0;

    for (int i=1;i<argc;i++)
    {
        size+=strlen(argv[i]);
    }


    char* ans1=(char*)calloc(sizeof(char),size);
    
    double ans2 = 0;
    for (int i=1;i<argc;i++)
    {
        double temp;
        char* check;
        temp = strtod(argv[i],&check);
        if (check[0]!='\0') {strcat(ans1,argv[i]);}
        else ans2+=temp;
    }
    sort(ans1);
    printf("%s\nBefore %f\n",ans1,ans2);
    printf("Reversed ans2: %f\n",rev_d(ans2));
}