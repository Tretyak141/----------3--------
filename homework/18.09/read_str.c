#include <stdio.h>
#include <stdlib.h>
#define BLOCK 8

char* reader()
{
    unsigned size_of_str=0; //размер строки в байтах
    unsigned i=0; //текущая длина строки
    int* check; //переменная для проверки
    char c; 
    char* s = (char*)malloc(size_of_str+=BLOCK); 
    while (((c=getchar())!=EOF)&&(c!='\n'))
    {
        s[i]=c;
        i++;
        if (i==size_of_str)
        {
            int* check; //переменная для проверки

            check = (int*)malloc(BLOCK); //проверяем исчерпание памяти
            if (!check) {free(check);free(s);fprintf(stderr,"Sositehui\n");return NULL;}
            free(check);//удаляем заmalloc'енную память
            s = (char*)realloc(s,size_of_str+=BLOCK);//перевыделяем память под строку
        }
    }
    s[i]='\0';
    s = (char*)realloc(s,(i+1)); //освобождаем неизрасходованную память (оставшиеся блоки, в которые ничего не записали)
    return s;
}

int main()
{
    char* ans=reader();
    printf("%s\n",ans);
    free(ans);//освобождаем всю занятую память
}