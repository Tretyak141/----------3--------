#include <stdio.h>
#include <stdlib.h>

int global_key = 1;

struct word
{
    char elem;
    struct word *next;
};

void add_elem(struct word **L,char c)
{
    if (!(*L))
    {
        (*L) = malloc(sizeof(struct word));
        (*L)->elem = c;
        (*L)->next = NULL;
        return;
    }
    add_elem(&((*L)->next),c);
}

int create_word(struct word **L)
{
    int ans=0;
    char c;
    while ((c=getchar())!=' ')
    {
        if ((c=='\n')||c==EOF)
            {
                global_key = 0;
                return ans;
            }
        ans+=1;
        add_elem(L,c);
    }
    return ans;
}

void print_word(struct word *L)
{
    while (L)
    {
        putchar(L->elem);
        L = L->next;
    }
    putchar(' ');
}

void free_word(struct word **L)
{
    if (!(*L)) return;
    free_word(&((*L)->next));
    free((*L));
}

struct list_words
{
    struct word *wordy;
    struct list_words *next;
};

void free_words(struct list_words **L)
{
    if (!(*L)) return;
    free_word(&((*L)->wordy));
    free_words(&((*L)->next));
    free(*L);
}

struct list_length
{
    int elem;
    struct list_length *next;
};

void free_len(struct list_length **L)
{
    if (!(*L)) return;
    free_len(&((*L)->next));
    free(*L);
}

void word_app(struct list_words **L,struct word *w)
{
    if (!(*L))
    {
        (*L) = malloc(sizeof(struct list_words));
        (*L)->wordy = w;
        w = NULL;
        (*L)->next = NULL;
        return;
    }
    if (!((*L)->next))
    {
        (*L)->next = malloc(sizeof(struct list_words));
        (*L)->next->wordy = w;
        w = NULL;
        (*L)->next->next = NULL;
        return;
    }
    word_app(&((*L)->next),w);
}

void len_app(struct list_length **L,int c)
{
    if (!(*L))
    {
        (*L) = malloc(sizeof(struct list_length));
        (*L)->elem = c;
        (*L)->next = NULL;
        return;
    }
    if (!((*L)->next))
    {
        (*L)->next = malloc(sizeof(struct list_length));
        (*L)->next->elem = c;
        (*L)->next->next = NULL;
        return;
    }
    len_app(&((*L)->next),c);
}

void create_list(struct list_words **L,struct list_length **M)
{
    struct word *w = NULL;
    int len;
    while (global_key)
    {
        struct word *w = NULL;
        len = create_word(&w);
        if (len==0)
        {
            continue;
        }
        word_app(L,w);
        len_app(M,len);
        w = NULL;
    }
}

void print_len(struct list_length *L)
{
    while (L)
    {
        printf("%d ",L->elem);
        L = L->next;
    }
    putchar(10);
}

void print_list(struct list_words *L)
{
    while (L)
    {
        print_word(L->wordy);
        L = L->next;
    }
}

void sort(struct list_words **L,struct list_length **M)
{
    struct list_words *tmp1 = *L;
    struct list_length *tmp2 = *M;
    while (tmp1->next)
    {
        struct list_words *tmp12 = tmp1->next;
        struct list_length *tmp22 = tmp2->next;
        while (tmp12)
        {
            int a=tmp2->elem,b=tmp22->elem;
            if (a>b)
            {
                struct word *tmp = tmp1->wordy;
                tmp1->wordy = tmp12->wordy;
                tmp12->wordy = tmp;
                tmp2->elem = b;
                tmp22->elem = a;
            }
            tmp12 = tmp12->next;
            tmp22 = tmp22->next;
        }
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
}

int main()
{
    struct list_words *L = NULL;
    struct list_length *M = NULL;
    create_list(&L,&M);
    printf("Before sort: \n");
    print_list(L);
    putchar(10);
    print_len(M);
    printf("\nAfter sort: ");
    sort(&L,&M);
    print_list(L);
    putchar(10);
    print_len(M);
    free_words(&L);
    free_len(&M);
}