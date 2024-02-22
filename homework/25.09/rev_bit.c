#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define SIZE sizeof(STYPE)*8

typedef char  STYPE;
typedef unsigned char UTYPE;

void print_bit(char s[],STYPE x)
{
    STYPE n=sizeof(x)*8;
    for (STYPE i=0;i<n;i++) s[i]='0';
    STYPE i=n-1;
    UTYPE tmp=x;
    while (tmp)
    {    

        s[i] = (char)(tmp%2)+'0';
        tmp/=2;
        i--;
    }
    s[n]=0;
}

STYPE rev_bit(STYPE value)
{
    STYPE n=sizeof(value)*8;
    char c[SIZE];
    print_bit(c,value);
    for (int i=0;i<(n/2);i++)
    {
        char tmp = c[i];
        c[i]=c[n-1-i];
        c[n-1-i]=tmp;
    }
    STYPE new_value=0;
    printf("Двоичная форма измененного числа: %s\n",c);
    for (STYPE i=0;i<n;i++)
        new_value += pow(2,n-i-1)*(c[i]-'0');
    return new_value;
}

int main()
{
    long int num;
    printf("Введите целое число ");
    scanf("%lu",&num);
    if (sizeof(STYPE)==4)
        if ((num>INT_MAX)||(num<INT_MIN)){fprintf(stderr,"Число не входит в тип INT\n");exit(1);}
    if (sizeof(STYPE)==1)
        if ((num>CHAR_MAX)||(num<CHAR_MIN)) {fprintf(stderr,"Число не входит в тип CHAR\n");exit(1);}
    char c[SIZE];
    print_bit(c,num);
    printf("Двоичная форма числа: %s\n",c);
    printf("%d\n",rev_bit(num));
}