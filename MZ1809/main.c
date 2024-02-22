#include <stdio.h>
#include <stdlib.h>

//Деревья

struct tree
{
    int elem;
    struct tree *right,*left;
};

void insert_to_tree(struct tree **T,int N)
{
    if (!(*T)) {(*T) = malloc(sizeof(struct tree));(*T)->elem = N; (*T)->right = NULL; (*T)->left = NULL;return;}
    if (N>((*T)->elem)) insert_to_tree(&((*T)->right),N);
    if (N<((*T)->elem)) insert_to_tree(&((*T)->left),N);
}

void create_tree(struct tree **T)
{
    int N;
    while (scanf("%d",&N)!=-1) insert_to_tree(T,N);
}

void print_tree(struct tree *T)
{
    if (!T) {printf("_");return;}
    printf("%d(Left: ",T->elem);
    print_tree(T->left);
    printf(")(Right: ");
    print_tree(T->right);
    printf(")");
}

void free_tree(struct tree **T)
{
    if (!(*T)) return;
    free_tree(&((*T)->right));
    free_tree(&((*T)->left));
    free(*T);
    (*T) = NULL;
}


struct list
{
    int elem;
    struct list *next;
};

void push_list(struct list **l, int N)
{
    if (!(*l)) 
    {
        (*l) = malloc(sizeof(struct list));
        (*l)->elem = N;
        (*l)->next = NULL;
        }
    else push_list(&((*l)->next),N);
}

void free_list(struct list **L)
{
    if (!(*L)) return;
    free_list(&((*L)->next));
    free((*L));
}

void convert(struct list **L,struct tree *T)
{
    if (!T) return;
    if ((!(T->left))&&(!(T->right))) {push_list(L,T->elem);return;}
    convert(L,T->left);
    push_list(L,T->elem);
    convert(L,T->right);
}

void tr_to_li_to_tr(struct tree **T)
{
    struct list *L = NULL;
    convert(&L,*T);
    struct list *L1 = L;
    int length = 0;
    while (L1)
    {
        length++;
        L1 = L1->next;
    }
    int *a = calloc(sizeof(int),length);
    L1 = L;
    int i=0;
    while (L1)
    {
        a[i] = L1->elem;
        i++;
        L1 = L1->next;
    }
    free_list(&L);
    free_tree(T);
    for (int i=(length/2);i<length;i++)
    {
        insert_to_tree(T,a[i]);
    }
    for (int i=(length/2)-1;i>=0;i--) insert_to_tree(T,a[i]);
    free(a);
}

int max(int a,int b)
{
    return (a>b) ? a:b;
}

int height(struct tree *T)
{
    if (!T) return 0;
    return 1+max(height(T->left),height(T->right));
}

int is_balanced(struct tree *T)
{
    if (!T) return 1;
    return (abs(height(T->right)-height(T->left))<2);
}

void balance(struct tree** T)
{
    if (!(*T)) return;
    if (!is_balanced(*T)) tr_to_li_to_tr(T);
    tr_to_li_to_tr(&((*T)->right));
    tr_to_li_to_tr(&((*T)->left));
}

int main()
{
    struct tree *T = NULL;
    create_tree(&T);
    printf("\nStarted tree: ");
    print_tree(T);
    printf("\n");
    printf("Tree after balance: \n");
    balance(&T);
    print_tree(T);
    printf("\n");
    free_tree(&T);
}