#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_bit(char s[],unsigned int x)
{
    int n=32;
    for (int i=0;i<n;i++) s[i]='0';
    int i=n-1;
    while (x)
    {    
        s[i] = (char)(x%2)+'0';
        x/=2;
        i--;
    }
    s[n]=0;
}

unsigned int rev_or_nul(unsigned int x)
{
    char c[32];
    print_bit(c,x);
    printf("Двоичное представление числа: %s\n",c);
    int star = (x & 0xFF000000)>>24;
    int mlad = (x & 0x000000FF);
    if (star==mlad) return (x & 0xFF0000FF);
    mlad = mlad << 24;
    unsigned int ans = x & 0x00FFFF00;
    return (ans|star)|mlad;
}


int main()
{
    unsigned long int num;
    printf("Введите целое беззнаковое 32битное число (от 0 до (2^32 -1)) ");
    scanf("%lu",&num);
    if (num>0xFFFFFFFFu) {fprintf(stderr,"Число выходит за границы UINT или введено отрицательное число\n");exit(1);}
    unsigned int new_num = rev_or_nul(num);
    printf("Измененное беззнаковое число: %u\n",new_num);
    char c[32];
    print_bit(c,new_num);
    printf("Двоичная форма полученного числа: %s\n",c);
}