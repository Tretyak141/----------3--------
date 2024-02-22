#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

int main()
{
 
    int N;
    scanf("%d",&N);

    int f = open("IPC",O_CREAT|O_TRUNC,0666);
    close(f);

    key_t key = ftok("IPC",1);

    int semid = semget(key, 3, IPC_CREAT | 0666);
    


    int fd[2];
    pipe(fd);

    semctl(semid, 0, SETVAL, 0);
    semctl(semid, 1, SETVAL, 0);
    semctl(semid, 2, SETVAL, 0);
    if (!fork())
    {
        if (!fork())
        {
            struct sembuf oper;
            oper.sem_flg = 0;
            while (1)
            {
                oper.sem_op = -1;
                oper.sem_num = 2;
                semop(semid,&oper,1);
                int buf;
                read(fd[0],&buf,4);
                if (buf==-1)
                {
                    write(fd[1],&buf,4);
                    oper.sem_op = 1;
                    oper.sem_num = 0;
                    semop(semid,&oper,1);
                    close(fd[0]);
                    close(fd[1]);
                    exit(0);
                }
                printf("grandson %d\n",buf);
                fflush(stdout);
                buf++;
                if (buf==(N+1)) buf = -1;
                write(fd[1],&buf,4);
                oper.sem_op = 1;
                oper.sem_num = 0;
                semop(semid,&oper,1);
                if (buf==-1) 
                {
                    close(fd[0]);
                    close(fd[1]);

                    exit(0);
                }
            }
        }
        struct sembuf oper;
        oper.sem_flg = 0;
        while(1)
        {
            oper.sem_op = -1;
            oper.sem_num = 1;
            semop(semid,&oper,1);
            fflush(stdout);
            int buf;
            read(fd[0],&buf,4);
            if (buf==-1)
            {
                write(fd[1],&buf,4);
                oper.sem_op = 1;
                oper.sem_num = 2;
                semop(semid,&oper,1);
                close(fd[0]);
                close(fd[1]);
                break;
            }
            printf("son %d\n",buf);
            fflush(stdout);
            buf++;
            if (buf==(N+1)) buf = -1;
            write(fd[1],&buf,4);
            oper.sem_op = 1;
            oper.sem_num = 2;
            semop(semid,&oper,1);
            if (buf==-1) 
            {
                close(fd[0]);
                close(fd[1]);
                break;
            }
        }

        wait(0);
        exit(0);
    }
    struct sembuf oper;
    oper.sem_flg = 0;

    int buf = 0;
    write(fd[1],&buf,4);
    oper.sem_op = 1;
    oper.sem_num = 1;
    semop(semid,&oper,1);
    while(1)
    {
        oper.sem_op = -1;
        oper.sem_num = 0;
        semop(semid,&oper,1);
        read(fd[0],&buf,4);
        if (buf==-1)
        {
            oper.sem_op = 1;
            oper.sem_num = 1;
            semop(semid,&oper,1);
            write(fd[1],&buf,4);
            close(fd[0]);
            close(fd[1]);
            break;
        }
        printf("father %d\n",buf);
        fflush(stdout);
        buf++;
        if (buf==(N+1)) buf = -1;
        write(fd[1],&buf,4);
        oper.sem_op = 1;
        oper.sem_num = 1;
        semop(semid,&oper,1);
        fflush(stdout);
        if (buf==-1)
        {
            close(fd[0]);
            close(fd[1]);
            break;
        }
    }
    wait(0);
    semctl(semid,IPC_RMID,0);
}