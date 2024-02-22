#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

//Вариант программы, при котором два процесса-сына

int main(int argc,char **argv)
{
    if (argc!=4) 
    {
        fprintf(stderr,"Error wth args\n");
        exit(1);
    }
    int fd1 = open(argv[1],O_RDONLY),fd2 = open(argv[2],O_RDONLY),fd3 = open(argv[3],O_CREAT|O_WRONLY|O_TRUNC,0666);
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
    pid_t pid1,pid2;
    int f1s[2],f2s[2],s1f[2],s2f[2];
    pipe(f1s);
    pipe(f2s);
    pipe(s1f);
    pipe(s2f);
    if (!(pid1=fork()))
    {
        close(f1s[1]);
        close(s1f[0]);

        close(f2s[0]);
        close(f2s[1]);
        
        close(s2f[0]);
        close(s2f[1]);

        close(fd2);
    
        while (1)
        {
            int buf;
            read(f1s[0],&buf,4);
            if (!buf) 
            {
                char c;
                int counter=0,count_s = 0;;
                while (read(fd1,&c,1))
                {
                    if (c=='\n')
                        counter++;
                    count_s++;
                }
                if (count_s) counter++;
                write(s1f[1],&counter,4);
                close(s1f[1]);
                close(f1s[0]);
                close(fd1);
                close(fd3);
                exit(0);
            }
            int counter_symbs = 0;
            int read_check;
            char read_buf;
            while ((read_check=read(fd1,&read_buf,1))&&(read_buf!='\n')) //Считываем либо до конца файла, либо до '\n'
            {
                write(fd3,&read_buf,1);
                counter_symbs++;
            }
            if (!read_check && !counter_symbs) //Если оказалось, что не было считано ни одного символа - возвращаем в отца 0 как сигнал об окончании файла
            {                                  //И завершаем процесс 
                int buf = 0;
                write(s1f[1],&buf,4);
                close(s1f[1]);
                close(f1s[0]);
                close(fd1);
                close(fd3);
                exit(0);
            }
            read_buf = '\n';
            write(fd3,&read_buf,1);
            buf=1;
            write(s1f[1],&buf,4);
        }
    }
    else if (pid1<0)
    {
        close(f1s[0]);
        close(f2s[0]);
        close(f1s[1]);
        close(f2s[1]);
        close(s1f[0]);
        close(s2f[0]);
        close(s1f[1]);
        close(s2f[1]);
        close(fd1);
        close(fd2);
        close(fd3);
        printf("ERROR WTH 1 FORK\n");
        exit(0);
    }
    else if (!(pid2=fork()))
    {
        close(f1s[0]);
        close(f1s[1]);

        close(s1f[0]);
        close(s1f[1]);

        close(f2s[1]);
        close(s2f[0]);

        close(fd1);

        while (1)
        {
            int buf;
            read(f2s[0],&buf,4); //Получаем от отца сигнал о начале обработки второго сына
            if (!buf) //Если в канал передан 0 - выдаем в канал общения отца с сыном количество оставшихся строк
            {
                char c;
                int counter=0,count_s = 0;;
                while (read(fd2,&c,1))
                {
                    if (c=='\n')
                        counter++;
                    count_s++;
                }
                if (count_s) counter++;
                write(s2f[1],&counter,4);
                close(f2s[0]);
                close(s2f[1]);
                close(fd2);
                close(fd3);
                exit(0);
            }
            int counter_symbs = 0;
            int read_check;
            char read_buf;
            while ((read_check=read(fd2,&read_buf,1))&&(read_buf!='\n')) //считываем до конца файла или до '\n'
            {
                write(fd3,&read_buf,1);
                counter_symbs++;
            }
            if (!read_check)//Если по итогу файл-2 кончился, нет смысла обрабатывать первый файл; возвращаем 0 в канал к отцу
            {
                int buf = 0;
                write(s2f[1],&buf,4);
                close(s2f[1]);
                close(f2s[0]);
                close(fd2);
                close(fd3);
                exit(0);
            }
            read_buf = '\n';
            write(fd3,&read_buf,1);
            buf = 1;
            write(s2f[1],&buf,4);
        }
    }

    else if (pid2<0)
    {
        int buf=0;
        write(f1s[1],&buf,4);
        close(f1s[0]);
        close(f2s[0]);
        close(f1s[1]);
        close(f2s[1]);
        close(s1f[0]);
        close(s2f[0]);
        close(s1f[1]);
        close(s2f[1]);
        close(fd1);
        close(fd2);
        close(fd3);
        waitpid(pid1,0,0);
        printf("ERROR WTH 2 FORK\n");
        exit(0);
    }

    else if (pid1 && pid2)
    {
        close(fd1);
        close(fd2);
        close(fd3);
        
        close(f1s[0]);
        close(f2s[0]);

        close(s1f[1]);
        close(s2f[1]);

        //По очереди отправляем процессам-сыновьям сигналы через каналы и обрабатываем то, что сыновья отправили отцу

        while (1)
        {
            int buf=1;
            write(f1s[1],&buf,4);
            read(s1f[0],&buf,4);
            if (!buf)
            {
                write(f2s[1],&buf,4);
                read(s2f[0],&buf,4);
                printf("Незанесенных в файл 3 строк из файла 2: %d\n",buf);
                close(f1s[1]);
                close(f2s[1]);

                close(s1f[0]);
                close(s2f[0]);

                waitpid(pid1,0,0);
                waitpid(pid2,0,0);

                exit(0);
            }
            write(f2s[1],&buf,4);
            read(s2f[0],&buf,4);
            if (!buf)
            {
                write(f1s[1],&buf,4);
                read(s1f[0],&buf,4);
                if (!buf)
                    printf("Оба файла исчерпаны\n");
                else
                    printf("Незанесенных в файл 3 строк из файла 1: %d\n",buf);
                
                close(f1s[1]);
                close(f2s[1]);

                close(s1f[0]);
                close(s2f[0]);

                waitpid(pid1,0,0);
                waitpid(pid2,0,0);

                exit(0);
            }
        }
    }
}