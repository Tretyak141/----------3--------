#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main() {
    


    pid_t pid1;

    int fd[2];

    int st;

    pipe(fd);

    if ((pid1 = fork()) < 0) {
        perror("FORK ERROR\n");
        exit(1);
    } else if (!pid1) {

        close(fd[1]);

        pid_t pid2;

        if ((pid2 = fork()) < 0) {
            perror("FORK ERROR\n");
            exit(1);
        } else if (!pid2) {

            close(fd[1]);

            pid_t pid3;

            if ((pid3 = fork()) < 0) {
                perror("FORK ERROR\n");
                exit(1);
            } else if (!pid3) {

                close(fd[0]);
                time_t now;


                time(&now);

                for(int i = 0; i < 3; i++) 
                    write(fd[1], &now, sizeof(time_t));
                printf("%d\n",now);
                close(fd[1]);

                exit(0);
            }

            waitpid(pid3, &st, 0);
            if (st) 
                exit(1);
            time_t now;

            struct tm * timeinfo;
            read(fd[0], &now, sizeof(time_t));
            printf("%d\n",now);
            timeinfo = localtime(&now);

            printf("D:%02d\n", timeinfo -> tm_mday);

            close(fd[0]);

            exit(0);        
        }

        waitpid(pid2, &st, 0);
        struct tm * timeinfo;

        if (st) 
            exit(1);
        time_t now;

        read(fd[0], &now, sizeof(time_t));

        timeinfo = localtime(&now);

        printf("M:%02d %d\n", timeinfo -> tm_mon + 1,now);

        close(fd[0]);

        exit(0);
    }
    struct tm * timeinfo;

    close(fd[1]);
    waitpid(pid1, &st, 0);

    if (st) 
        exit(1);
    time_t now;

    read(fd[0], &now, sizeof(time_t));

    timeinfo = localtime(&now);

    printf("Y:%d %d\n", (timeinfo->tm_year),now);

    close(fd[0]);

    return 0;
}