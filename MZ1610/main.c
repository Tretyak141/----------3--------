#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    FILE *f = fopen("test.c","w");
    fprintf(f,"#include <stdio.h>\n int main(){printf(\"hello world\");}");
    fclose(f);
    pid_t pid;
    if (!(pid=fork())) {execlp("gcc","gcc","test.c","-o","test",NULL);}
    int st;
    waitpid(pid,&st,0);
    if (!(pid=fork())) {execl("test","test",NULL);}
    waitpid(pid,&st,0);
    return 0;
}