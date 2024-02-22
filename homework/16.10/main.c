#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int strings_in_file(char *name)//Задача номер 1, семинар
{
    FILE *stream;
    if (!(stream = fopen(name,"r+")))
    {
        fprintf(stderr,"Ошибка при открытии файла\n");
        exit(1);
    }
    int ans = 1;
    int pos=0;
    int c;
    while ((c=fgetc(stream))!=EOF)
    {
        if (c=='\n') ans++;
        pos++;
    }
    if (!pos) return 0;
    fseek(stream,-1,SEEK_END);
    c = fgetc(stream);
    if (c==EOF) ans-=1;
    fclose(stream);
    return ans;
}

int *strings_in_file_info(char *s)
{
    int *ans = calloc(strings_in_file(s),sizeof(int));
    int strings = 0;
    FILE *stream;
    if (!(stream=fopen(s,"r")))
    {
        fprintf(stderr,"Ошибка при открытии файла\n");
        exit(1);
    }
    int now_length=0;
    int c;
    while ((c=fgetc(stream))!=EOF)
    {
        if (c!='\n') now_length++;
        else
        {
            strings++;
            ans[strings-1] = now_length;
            now_length = 0;
        }
    }
    if (strings==1) ans[0] = 0;
    if (now_length) ans[strings] = now_length;
    fclose(stream);
    return ans;
}

void print_str(char *s,int num_of_str)
{
    int str_info = strings_in_file(s);
    int *strings = strings_in_file_info(s);
    if (str_info<=num_of_str)
    {
        fprintf(stderr,"Нет такой строки в файле\n");
        free(strings);        
        return;
    }
    FILE *stream;
    if (!(stream=fopen(s,"r")))
    {
        fprintf(stderr,"Ошибка при открытии файла\n");
        free(strings);
        exit(1);
    }
    for (int i=0;i<num_of_str;i++)
    {
        fseek(stream,strings[i]+1,SEEK_CUR);        
    }
    int c;
    while (((c=fgetc(stream))!='\n')&&(c!=EOF)) putchar(c);
    putchar(10);
    free(strings);
    fclose(stream);
}

void my_more(int s,int num,int linenum,char *name)
{
    int len = strings_in_file(name);
    if (len==0)
    {
        printf("Файл пуст\n");
        return;
    }
    int *str_info = strings_in_file_info(name);
    char c;
    if ((linenum-1+num)>=len)
    {
        if (!s)
        {
            for (int i=linenum-1;i<len;i++) print_str(name,i);
            free(str_info);
            return;
        }
        int curr = linenum-1;
        while (curr<len)
        {
            if (curr==(linenum-1))
            {
                print_str(name,curr);
                curr++;
                continue;
            }
            if ((str_info[curr]==0)&&(str_info[curr-1]==0)) {curr++;continue;}
            print_str(name,curr);
            curr++;
        }
        free(str_info);
        return;
    }
    int curr = linenum-1;
    if (!s)
        for (int i=linenum-1;i<linenum-1+num;i++)
        {
            print_str(name,i);
            curr++;
        }
    else
    {
        int str_c = 0;
        while ((str_c<num)&&(curr<len))
        {
            if (curr==(linenum-1))
            {
                print_str(name,curr);
                curr++;
                str_c++;
                continue;
            }
            if ((str_info[curr]==0)&&(str_info[curr-1]==0)) {curr++;continue;}
            print_str(name,curr);
            curr++;
            str_c++;
        }
    }
    
    while (curr<len)
    {
        scanf("%c",&c);
        if (c=='q')
        {
            free(str_info);
            return;
        }
        if (c!=' ') continue;
        else
        {
            int str_c = 0;
            if (s==0)
            {
                for (int i=curr;i<curr+num;i++){
                    if (i>=len) 
                    {
                        free(str_info);
                        return;
                    }
                    print_str(name,i);
                }
                curr = curr+num;
                continue;
            }
            while ((str_c<num)&&(curr<len))
            {
                if ((str_info[curr]==0)&&(str_info[curr-1]==0)) {curr++;continue;}
                print_str(name,curr);
                curr++;
                str_c++;
            }
        }
    }
    free(str_info);
}

int main(int argc,char **argv)
{
    int s1,s2,s3;
    int s;
    switch (argc){
        case 5:
            s = strcmp(argv[1],"-s");
            if (s)
            {
                fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                return 1;
            }
            int num = atoi(argv[2]);
            if (num>=0)
            {
                fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                return 1;
            }
            int linenum = atoi(argv[3]);
            if (linenum<0) 
            {
                fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                return 1;
            }
            my_more(1,-num,linenum,argv[4]);
            break;
        
        case 4:
            int s = strcmp(argv[1],"-s");
            if (s==0) s1=1;
            else
            {
                s1 = 0;
            }
            if (!s1)
            {
                s2 = atoi(argv[1]);
                if (s2>=0)
                {
                    fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                    return 1;
                }
                s3 = atoi(argv[2]);
                if (s3<=0)
                {
                    fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                    return 1;
                }
            }
            else
            {
                int tmp = atoi(argv[2]);
                if (tmp>0)
                {
                    s2 = -10;
                    s3 = tmp;
                }
                if (tmp<0) 
                {
                    s2 = tmp;
                    s3 = 1;
                }
                if (!tmp)
                {
                    fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                    return 1;
                }
            }
            my_more(s1,-s2,s3,argv[3]);
            break;
        case 3:
            s = strcmp(argv[1],"-s");
            if (s==0) 
            {   
                s1=1;
                my_more(s1,10,1,argv[2]);
                break;
            }
            else
            {
                s1 = 0;
                int tmp=atoi(argv[1]);
                if (tmp>0)
                {
                    s2 = -10;
                    s3 = tmp;
                }
                if (tmp<0)
                {
                    s2 = tmp;
                    s3 = 1;
                }
                if (tmp==0)
                {
                    fprintf(stderr,"Неправильно введены аргументы командной строки\n");
                    return 1;                    
                }
                my_more(s1,-s2,s3,argv[2]);
                break;
            }
        case 2:
            my_more(0,10,1,argv[1]);
            break;
        default:
            fprintf(stderr,"Неправильно введены аргументы командной строки\n");
            return 1;    
    }
}