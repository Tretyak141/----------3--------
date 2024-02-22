#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        perror("Invalid arguments");
        return 1;
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    int N = atoi(argv[4]);

    int semId = semget(key, 3, IPC_CREAT | 0666);

    struct sembuf semaphore_operation;
    semaphore_operation.sem_flg = 0;
    semaphore_operation.sem_num = 0;
    semaphore_operation.sem_op = 1;
    semop(semId, &semaphore_operation, 1);

    semaphore_operation.sem_num = 2;
    semaphore_operation.sem_op = -1;
    semop(semId, &semaphore_operation, 1);

    FILE *file = fopen(argv[3], "r");

    char c;
    
    for (int i=0;i<2*N;i++)
    {
        char s[50];
        fgets(s,50,file);
        printf("%s",s);
    }
    fclose(file);

    return 0;
}