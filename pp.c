#include <stdio.h>
#include <pthread.h>

int n;

pthread_t tid1,tid2;


void *second(void *a)
{
    pthread_join(tid1,NULL);
    while(*(int*)a < n)
    {
        printf("%d ",*(int*)a);
        fflush(stdout);
        *(int*)a++;
        pthread_join(tid1,NULL);
    }
    return NULL;
}

void *first(void *a)
{
    int k = 1;
    fflush(stdout);
    pthread_create(&tid2,NULL,second,&k);
    while(k<n)
    {
        printf("%d ",k);
            fflush(stdout);

        k++;
    }
    return NULL;
}


int main()
{
    scanf("%d",&n);
    pthread_create(&tid1,NULL,first,&n);
    pthread_join(tid1,NULL);
}