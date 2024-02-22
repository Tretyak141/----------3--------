#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#define LENIN 256

//Вариант программы, при котором процесс-сын и процесс-внук

int main(int argc,char **argv)
{
    if (argc!=4) 
    {
        fprintf(stderr,"Args\n");
        exit(1);
    }
    int fd1 = open(argv[1],O_RDONLY),
        fd2 = open(argv[2],O_RDONLY),
        fd3 = open(argv[3],O_CREAT|O_WRONLY|O_TRUNC,0666);
    char ch1,ch2;

    if (!read(fd1,&ch1,1) || !read(fd2,&ch2,1))
    {
        fprintf(stderr,"Один из файлов пуст\n");
        close(fd1);
        close(fd2);
        close(fd3);
        exit(0);
    }

    lseek(fd1,0,SEEK_SET);
    lseek(fd2,0,SEEK_SET);

    int gs[2],sg[2],f[2];

    pipe(gs);
    pipe(sg);
    pipe(f);

    pid_t pid1,pid2;

    if (!(pid1=fork()))
    {
        close(sg[0]);
        
        close(gs[1]);
        
        close(f[0]);
        
        close(fd2);

        while (1)
        {
            int counter_symb=0;
            int count_check;
            char read_buf;
            while ((count_check=read(fd1,&read_buf,1))&&(read_buf!='\n'))
            {
                write(fd3,&read_buf,1);
                counter_symb++;
            }
            if (!counter_symb && !count_check)
            {
                int buf=0;
                write(sg[1],&buf,4);
                read(gs[0],&buf,4);
                write(f[1],&buf,4);
                buf = 2;
                write(f[1],&buf,4);
                waitpid(pid2,0,0);
                close(sg[1]);
                close(gs[0]);
                close(f[1]);
                close(fd1);
                close(fd3);
                exit(0);
            }
            read_buf = '\n';
            write(fd3,&read_buf,1);
            int buf = 1;
            write(sg[1],&buf,4);
            read(gs[0],&buf,4);
            if (!buf)
            {
                int counter_symbs=0,counter=0;
                while (read(fd1,&read_buf,1))
                {
                    if (read_buf=='\n') 
                        counter++;
                    counter_symbs++;
                }
                if (counter_symbs) counter++;
                waitpid(pid2,0,0);
                write(f[1],&counter,4);
                buf = 1;
                write(f[1],&buf,4);
                close(sg[1]);
                close(gs[0]);
                close(fd1);
                close(fd3);
                exit(0);
            }
        }
        
    }

    else if (!(pid2=fork()))
    {
        close(sg[1]);

        close(gs[0]);

        close(f[1]);
        close(f[0]);

        close(fd1);

        while(1)
        {
            int buf;
            read(sg[0],&buf,4);
            if (!buf)
            {
                char read_buf;
                int counter_symbs=0,counter=0;
                while (read(fd2,&read_buf,1))
                {
                    if (read_buf=='\n') 
                        counter++;
                    counter_symbs++;
                }
                if (counter_symbs) counter++;
                write(gs[1],&counter,4);
                close(sg[0]);
                close(gs[1]);
                close(fd2);
                close(fd3);
                exit(0);   
            }

            char read_buf;
            int check;
            int counter_s = 0;
            while ((check=read(fd2,&read_buf,1))&&(read_buf!='\n'))
            {
                write(fd3,&read_buf,1);
                counter_s++;
            }
            if (!check)
            {
                buf = 0;
                write(gs[1],&buf,4);
                close(gs[1]);
                close(sg[0]);
                close(fd2);
                close(fd3);
                exit(0);
            }
            read_buf = '\n';
            write(fd3,&read_buf,1);
            buf = 1;
            write(gs[1],&buf,4);
        }
    }

    else
    {
        close(sg[0]);
        close(sg[1]);

        close(gs[0]);
        close(gs[1]);

        close(f[1]);

        close(fd1);
        close(fd2);
        close(fd3);

        int ans1,ans2;
        read(f[0],&ans1,4);
        read(f[0],&ans2,4);
        if (!ans1)
        {
            printf("Оба файла исчерпаны\n");
            exit(0);
        }
        printf("Незанесенных в файл 3 строк из файла %d: %d\n",ans2,ans1);
        waitpid(pid1,0,0);
        waitpid(pid2,0,0);
    }
}