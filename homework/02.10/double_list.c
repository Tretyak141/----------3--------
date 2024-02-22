#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"



int main(int argc,char **argv)
{
    struct dlist *L = NULL;
    create_list(&L);
    print_list(L);
    void (*f[2])(struct dlist**);
    f[0] = insert;
    f[1] = del_num;
    printf("\n\nВыберите действие\n0 - вставить элемент\n1 - удалить элемент \n2 - вывести весь список в двух видах\nЛюбая другая клавиша - завершить обработку списка\n\n");

    while(1)
    {
        char c;
        scanf("%c",&c);
        if ((c=='0')||(c=='1')) f[c-'0'](&L);
        else if (c=='2') print_list(L);
        else if (c=='\n') continue;
        else
        {
            printf("Обработка списка закончена. Вот итог обработки\n");
            print_list(L);
            break;
        }
    }
    del_list(&L);
}