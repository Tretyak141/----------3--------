#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    int ch[2];
    pipe(ch);

    int pid1,pid2,pid3;
    if (!(pid1=fork()))
    {
        if (!(pid2=fork()))
        {
            if (!(pid3=fork()))
            {
                close(ch[0]);
                time_t now;
                time(&now);
                write(ch[1],&now,sizeof(time_t));
                write(ch[1],&now,sizeof(time_t));
                write(ch[1],&now,sizeof(time_t));

                close(ch[1]);
                exit(0);
            }
            waitpid(pid3,0,0);
            close(ch[1]);
            time_t now;
            read(ch[0],&now,sizeof(time_t));
            struct tm *s = localtime(&now);
            int day = s->tm_mday;
            if (day<10)
                printf("D: 0%d\n",day);
            else
                printf("D: %d\n",day);
            close(ch[0]);
            exit(0);
        }
        waitpid(pid2,0,0);
        close(ch[1]);
        time_t now;
        read(ch[0],&now,sizeof(time_t));
        struct tm *s = localtime(&now);
        int month = s->tm_mon + 1;
        if (month<10)
            printf("M: 0%d\n",month);
        else
            printf("M: %d\n",month);
        
        close(ch[0]);
        exit(0);
    }
    while (wait(0)!=-1);
    close(ch[1]);

    time_t now;
    read(ch[0],&now,sizeof(time_t));
    struct tm *s = localtime(&now);
    int year = s->tm_year + 1900;
    printf("Y: %d\n",year);
    close(ch[0]);
    exit(0);
}