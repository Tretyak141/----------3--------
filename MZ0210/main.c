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
    if (c!='\n')
    {
        fseek(stream,-1,SEEK_END);
        fputc('\n',stream);
    }
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
    fclose(stream);
    return ans;
}

int max(char *s)
{
    int *a = strings_in_file_info(s);
    int len = strings_in_file(s);
    int maxim=0;
    for (int i=0;i<len;i++)
    {
        maxim = (a[i]>maxim) ? a[i]:maxim;
    }
    free(a);
    return maxim;
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
    printf("%d\n",strings[1]);
    free(strings);
    fclose(stream);
}

void sorting(char *s1,char *s2)
{
    FILE *stream;
    int len = strings_in_file(s1);
    int *strinf = strings_in_file_info(s1);
    if (!(stream=fopen(s1,"r")))
    {
        fprintf(stderr,"Ошибка при открытии файла\n");
        free(strinf);
        exit(1);
    }
    int *writed_i;
    writed_i = malloc((len+1)*4);
    for (int i=0;i<len+1;i++) writed_i[i] = 0;
    int writed=0;
    int maxima = max(s1);
    int min_len=maxima;
    int cur=0;
    FILE *new;
    fseek(stream,0,SEEK_SET);
    new = fopen(s2,"w");
    while (writed!=len)
    {
        for (int i=0;i<len;i++)
        {
            if ((strinf[i]<=min_len)&&(!writed_i[i]))
            {
                cur=i;
                min_len = strinf[i];
            }
        }
        writed++;
        writed_i[cur]=1;
        for (int i=0;i<cur;i++)
        {
            fseek(stream,strinf[i]+1,SEEK_CUR);
        }
        int c;
        while (((c=fgetc(stream))!='\n')&&(c!=EOF)) fputc(c,new);
        fseek(stream,0,SEEK_SET);
        min_len = maxima;
        fputc(10,new);
    }
    fclose(stream);
    fclose(new);
    free(writed_i);
    free(strinf);
}

int main(int argc,char **argv)
{
    if (argc<3) 
    {
        perror("Не переданы аргументы\n");
        exit(1);
    }
    int c;
    printf("Количество строк: %d\n",c=strings_in_file(argv[1]));
    printf("Введите число\n");
    scanf("%d",&c);
    printf("Требуемая строка: ");
    print_str(argv[1],c);
    sorting(argv[1],argv[2]);
    return 0;
}