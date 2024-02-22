#include <stdio.h>
#include <stdlib.h>

struct list
{
    int elem;
    struct list *next;
};

void add_elem(struct list **L,int n)
{
    if (!(*L))
    {
        (*L) = malloc(sizeof(struct list));
        (*L)->elem = n;
        (*L)->next = NULL;
    }
    else add_elem(&((*L)->next),n);
}

void create_list(struct list **L)
{
    int n;
    while (scanf("%d",&n)!=-1)
    {
        add_elem(L,n);
    }
}

void print_list(struct list *L)
{
    while (L)
    {
        printf("%d ",L->elem);
        L = L->next;
    }
    putchar(10);
}

void free_list(struct list **L)
{
    if (!(*L)) return;
    free_list(&((*L)->next));
    free(*L);
    *L = NULL;
}

int list_len(struct list *L)
{
    int ans=0;
    while (L)
    {
        ans++;
        L = L->next;
    }
    return ans;
}

void reverse(struct list **L)
{
    struct list *L1 = *L;

    int n = list_len(*L);
    switch (n)
    {
    case 0:
        return;
    case 1:
        return;
    case 2:
        (*L) = (*L)->next;
        (*L)->next = L1;
        L1->next = NULL;
        break;
    default:
        struct list *L0 = NULL;
        (*L) = (*L)->next;
        while (*L)
        {
            L1->next = L0;
            L0 = L1;
            L1 = (*L);
            (*L) = (*L)->next;
        }
        L1->next = L0;
        (*L) = L1;
        break;
    }
}


int main()
{
    struct list *L = NULL;
    printf("Input list\n");
    create_list(&L);
    putchar(10);
    printf("List before reverse: \n");
    print_list(L);
    reverse(&L);
    printf("List after reverse: \n");
    print_list(L);
    free_list(&L);
}