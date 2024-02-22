#ifndef _list_h_
#define _list_h_

struct dlist
{
    char *elem;
    struct dlist *forward,*back;
};
void push_list(struct dlist **L,char *c);
void create_list(struct dlist **L);
void del_list(struct dlist **L);
void print_list(struct dlist *L);
int length(struct dlist *L);
void insert(struct dlist **L);
void del_num(struct dlist **L);
#endif