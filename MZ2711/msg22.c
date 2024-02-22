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
        perror("Invalid arguments");
        return 1;
    }

    key_t key = ftok(argv[1], atoi(argv[2]));
    int N = atoi(argv[4]);

    int semId = semget(key, 3, IPC_CREAT | 0666);

    if (semId == -1)
    {
        perror("Failed to get semaphores");
        return 1;
    }

    int fd = open(argv[3],O_WRONLY|O_APPEND);

    struct sembuf oper;
    oper.sem_flg = 0;

    for (int i = 0; i < N; i++)
    {
        oper.sem_num = 1;
        oper.sem_op = -1;
        semop(semId, &oper, 1);
        char *s = malloc(1000);
        sprintf(s,"%d MESSAGE\n",getpid());
        write(fd,s,strlen(s));
        oper.sem_num = 0;
        oper.sem_op = 1;        
        semop(semId, &oper, 1); 
    }

    close(fd);

    return 0;
}