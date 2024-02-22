#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

/*
    >ipcs Check IPCs
    >ipcs -l
    ipcrm - del IPC object
*/

/*
    key_t ftok(char *file,char *proj); 
    {
        создание ключа
    }
    int msgget(key_t key,int flag);
    int shmget(-.-,int size,.-.-);
    int scmget(-.-.-,int nsems,-.-.-);
    0 - IPC_PRIVATE - только родственные

    флаги: 
        в восьмиричной ОС, задает права доступа к объекту IPC
        Возможно IPC_CREAT (создает файл)
        Возможно IPC_EXCL - выдаст ошибку если файл существует (использовать с IPC_CREAT)

    ctl:
    int msgctl(int id,int cmd,struct msgid_ds *buf)
    int shmctl(int id,int cmd,struct shmid_ds *buf)
    int scmctl(int id,int num,int cmd,union semun arg)
    CMD:
        {
            IPC_STAT
            IPC_SET
            IPC_RMID
        }
*/
