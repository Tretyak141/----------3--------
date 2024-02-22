#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void create_file(char *s,long unsigned int N)
{
    int file;
    file = creat(s,0644);
    srand(time(NULL)/2);
    for (int i=0;i<N;i++)
    {
        int random_num;
        random_num = (rand()%1000000)*(-((rand()%2) == 0) ? 1:-1);
        write(file,&random_num,sizeof(int));
    }
    close(file);
}

void bin_to_text(char *s1,char *s2,int N)
{
    FILE *f1,*f2;
    if (!(f2=fopen(s2,"r")))
    {
        f2 = fopen(s2,"w");
        f1 = fopen(s1,"r");
        for (int i=0;i<N;i++)
        {
            int buf;
            fread(&buf,4,1,f1);
            fprintf(f2,"%d ",buf);
        }
        fclose(f1);
        fclose(f2);
        return;
    }
    printf("Введите Y если хотите изменить файл, иначе N\n");
    char c;
    scanf("%c",&c);
    fclose(f2);
    if (c=='N') {return;}

    f2 = fopen(s2,"w");    
    f1 = fopen(s1,"r");
    for (int i=0;i<N;i++)
    {
        int buf;
        fread(&buf,4,1,f1);
        fprintf(f2,"%d ",buf);
    }
    fclose(f1);
    fclose(f2);
}

void file_sorting(char *s, unsigned long int N)
{
    int fd = open(s,O_RDWR);
    for (int i=0;i<N-1;i++)
    {
        int now;
        read(fd,&now,4);
        for (int j=i+1;j<N;j++)
        {
            int now_n;
            read(fd,&now_n,4);
            if (now_n<now)
            {
                lseek(fd,i*4,SEEK_SET);
                write(fd,&now_n,4);
                lseek(fd,j*4,SEEK_SET);
                write(fd,&now,4);
                int tmp = now;
                now = now_n;
                now_n = tmp;
            }
        }
        lseek(fd,(i+1)*4,SEEK_SET);
    }
    close(fd);
}

int main(int argc,char **argv)
{
    if (argc!=4)
    {
        fprintf(stderr,"Ошибка в аргументах командной строки\n");
        return -1;
    }
    int N=atoi(argv[3]);
    create_file(argv[1],N);
    bin_to_text(argv[1],argv[2],N);
    file_sorting(argv[1],N);
    bin_to_text(argv[1],"test.txt",N);
}