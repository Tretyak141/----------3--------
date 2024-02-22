#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
    if (argc!=4) 
    {
        printf("ARGS!!!\n");
        exit(1);
    }
    #define MAXIMA atoi(argv[3])
    #define N1 atoi(argv[1])
    int fd[N1-1][2];
    int father[2];
    int N = N1;
    for (int i=0;i<N1-1;i++)
    {
        pipe(fd[i]);
    }
    pipe(father);
    pid_t *pids;
    int flag=1;
    pids = (pid_t*)malloc(sizeof(pid_t)*N);
    int num=-1;
    for (int i=0;i<N;i++)
    {
        pid_t tmp_pid = fork();
        if (!tmp_pid) 
        {
            flag=0;
            num=i;
            break;
        }
        pids[i] = tmp_pid;
    }
    if (num>=0)
    {
        if (num)
        {        
            if (num==(N-1)) close(father[0]);
            else
            {
                close(father[0]);
                close(father[1]);
            }
        }
        else
        {
            close(father[1]);
        }
        for (int i=0;i<num-1;i++)
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        if (num>0) close(fd[num-1][1]);
        for (int i=num+1;i<N;i++)
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        close(fd[num][0]);
        while (1)
        {
            int buf;
            if (num)
                read(fd[num-1][0],&buf,4);
            else read(father[0],&buf,4);
            if (buf)
            {
                printf("My number: %d; My PID: %d\n",buf,getpid());
                buf++;
                if (buf<=MAXIMA)
                    if (num!=N-1) write(fd[num][1],&buf,4);
                    else write(father[1],&buf,4);
                else
                {
                    buf = 0;
                    if (num==N-1) write(father[1],&buf,4);
                    else write(fd[num][1],&buf,4);
                    if (!num) close(father[0]);
                    if (num==N-1) close(father[1]);
                    else close(fd[num][1]);
                    exit(0);
                }
            }
            else
            {
                buf = 0;
                if (num==N-1) write(father[1],&buf,4);
                else write(fd[num][1],&buf,4);
                if (!num) close(father[0]);
                if (num==N-1) close(father[1]);
                else close(fd[num][1]);
                exit(0);
            }
        }
    }
    else
    {
        int buf=atoi(argv[2]);
        for (int i=0;i<N-1;i++) 
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        close(father[0]);
        write(father[1],&buf,4);
        close(father[1]);
        for (int i=0;i<N;i++)
            waitpid(pids[i],0,0);
        free(pids);
        printf("END\n");
        exit(0);
    }
}