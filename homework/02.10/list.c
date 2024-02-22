#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


void push_list(struct dlist **L,char *c)
{
    struct dlist *L1 = (*L);
    
    if (!(*L)) 
    {
        (*L) = malloc(sizeof(struct dlist));
        (*L)->elem = malloc(256);
        strcpy((*L)->elem,c);
        (*L)->back = NULL;
        (*L)->forward = NULL;
    }
    else
    {
        while (L1->forward) L1 = L1->forward;
        L1->forward = malloc(sizeof(struct dlist));
        L1->forward->elem = malloc(256);
        strcpy(L1->forward->elem,c);
        L1->forward->forward = NULL;
        L1->forward->back = L1;
    }
}

void create_list(struct dlist **L)
{
    printf("Какого размера список вы хотите задать?\n");
    int n;
    scanf("%d",&n);
    printf("Введите элементы списка\n");
    for (int i=0;i<n;i++) 
    {
        char *s;
        s = malloc(256);
        scanf("%s",s);
        push_list(L,s);
        free(s);
    }
}

void del_list(struct dlist **L)
{
    if (!(*L)) return;
    if (!((*L)->forward)) 
    {
        free((*L)->elem);
        free(*L);
    }
    else
    {
        free((*L)->elem);
        del_list(&((*L)->forward));
        free(*L);
    }
    (*L) = NULL;
}

void print_list(struct dlist *L)
{
    if (!L) {printf("Список пуст\n");return;}

    printf("\nСписок: ");
    while (L->forward)
    {
        printf("%s ",L->elem);
        L = L->forward;
    }
    printf("%s\n",L->elem);
    printf("Перевернутый список: ");
    while (L->back)
    {
        printf("%s ",L->elem);
        L = L->back;
    }
    printf("%s\n",L->elem);
}

int length(struct dlist *L)
{
    int ans = 0;
    while (L) 
    {
        ans++;
        L = L->forward;
    }
    return ans;
}

void insert(struct dlist **L)
{
    int num;
    int len = length(*L);
    char *s;
    s = malloc(256);
    printf("Введите элемент, который нужно вставить\n");
    scanf("%s",s);
    printf("Введите номер, после которого надо вставить (нумерация от 0 до n-1)\n");
    scanf("%d",&num);
    if ((len==0)||(num==0)) 
    {
        push_list(L,s);
        free(s);
        return;
    }
    if (num>=len) 
    {
        fprintf(stderr,"Нет элемента с таким номером\n");
        free(s);
        return;
    }
    if (num==(len-1))
    {
        push_list(L,s);
        free(s);
        return;
    }
    int iter=0;
    struct dlist *L1 = (*L);
    while (iter!=num)
    {
        iter++;
        L1 = L1->forward;
    }
    struct dlist *new_elem;
    new_elem = malloc(sizeof(struct dlist));
    new_elem->elem = malloc(256);
    strcpy(new_elem->elem,s);
    free(s);
    new_elem->forward = L1->forward;
    L1->forward->back = new_elem;
    new_elem->back = L1;
    L1->forward = new_elem;
}

void del_num(struct dlist **L)
{
    printf("Введите удаляемый номер\n");
    int num;
    scanf("%d",&num);
    int len=length(*L);
    if (num>=len)
    {
        fprintf(stderr,"Нет элемента с таким номером");
        return;
    }
    if (num==0)
    {
        struct dlist *L1 = *L;
        (*L) = (*L)->forward;
        free(L1->elem);
        free(L1);
        if (*L) (*L)->back = NULL;
        return;
    }
    if (num==(len-1))
    {
        struct dlist *L1 = (*L);
        while (L1->forward) L1 = L1->forward;
        L1->back->forward = NULL;
        free(L1->elem);
        free(L1);
        return;
    }
    int iter=0;
    struct dlist *L1 = (*L);
    while (iter!=num)
    {
        iter++;
        L1 = L1->forward;
    }
    struct dlist *tmp = L1;
    tmp->back->forward = tmp->forward;
    tmp->forward->back = tmp->back;
    free(tmp->elem);
    free(tmp);
}