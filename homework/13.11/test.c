#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
    if (argc!=4) return 1;
    int f1 = open(argv[1],O_RDONLY), 
        f2 = open(argv[2],O_RDONLY), 
        f3 = open(argv[3],O_WRONLY|O_TRUNC|O_CREAT,0666);
    while (1)
    {
        int pid1 = fork();
        if (!pid1)
        {
            int flag;
            int counter_symbs=0;
            char buf;
            while ((flag=read(f1,&buf,1))&&(buf!='\n'))
            {
                write(f3,&buf,1);
                counter_symbs++;
            }
            if ((flag==0)&&(counter_symbs==0))
            {
                abort();
            }
            buf = '\n';
            write(f3,&buf,1);
            exit(0);
        }
        else
        {
            int st;
            waitpid(pid1,&st,0);
            if (!WIFEXITED(st))
            {
                int counter=0,counter_s=0;
                char buf;
                while (read(f2,&buf,1))
                {
                    if (buf=='\n') counter++;
                    counter_s++;
                }
                if (counter_s) counter++;
                close(f1);
                close(f2);
                close(f3);
                printf("Во втором файле осталось %d строк\n",counter);
                return 0;
            }
        }
        int pid2=fork();
        if (!pid2)
        {
            int flag;
            int counter_symbs=0;
            char buf;
            while ((flag=read(f2,&buf,1))&&(buf!='\n'))
            {
                write(f3,&buf,1);
                counter_symbs++;
            }
            if (flag==0)
            {
                printf("HUI\n");
                abort();
            }
            buf = '\n';
            write(f3,&buf,1);
            exit(0);
        }
        else
        {
            int st;
            waitpid(pid2,&st,0);
            if (!WIFEXITED(st))
            {
                int counter=0,counter_s=0;
                char buf;
                while (read(f1,&buf,1))
                {
                    if (buf=='\n') counter++;
                    counter_s++;
                }
                if (counter_s) counter++;
                close(f1);
                close(f2);
                close(f3);
                printf("Во первом файле осталось %d строк\n",counter);
                return 0;
            }
        }
    }
}