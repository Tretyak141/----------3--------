#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>
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

    int msqId = msgget(key, IPC_CREAT | 0666);

    if (msqId == -1)
    {
        perror("Failed to get semaphores");
        return 1;
    }

    FILE *file = fopen(argv[3], "a");
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    struct message
    {
        long mtype;
        char mtext[1];
    } msg, receivedMsg;
    msg.mtext[0] = '\0';

    for (int i = 0; i < N; i++)
    {
        msgrcv(msqId, &receivedMsg, sizeof(char*), 2, 0);

        fprintf(file, "%d something\n", getpid());
        fflush(file);


        msg.mtype = 3;
        msgsnd(msqId, &msg, sizeof(char*), 0);
    }

    msgctl(msqId,IPC_RMID,0);


    fclose(file);

    return 0;
}