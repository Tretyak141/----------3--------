#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        return 1;
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    int N = atoi(argv[4]);

    int semid = semget(key, 3, IPC_CREAT | 0666);

    semctl(semid, 0, SETVAL, 0);
    semctl(semid, 1, SETVAL, 0);
    semctl(semid, 2, SETVAL, 0);

    int fd = open(argv[3],O_WRONLY|O_APPEND|O_TRUNC);

    struct sembuf oper;
    oper.sem_flg = 0;

    for (int i = 0; i < N; i++)
    {
        oper.sem_num = 0;
        oper.sem_op = -1;
        semop(semid,&oper,1);
        char *s = malloc(1000);
        sprintf(s,"Dad %d\n",getpid(),i);
        write(fd,s,strlen(s));
        free(s);
        oper.sem_num = 1;
        oper.sem_op = 1;
        semop(semid,&oper,1);
    }

    close(fd);
    oper.sem_num = 2;
    oper.sem_op = 1;
    semop(semid,&oper,1);
}