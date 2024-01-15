#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

#define MAX_BUF 80

int main(int argc, char* argv[]) {
    int pid;
    int pd[2];
    char buffer[MAX_BUF];
    FILE *f;
    int count = 0;

    if(pipe(pd) < 0) {
        printf("Failed while creating pipe!\n");
        exit(1);
    }

    pid = fork();

    if(pid < 0) {
        printf("Failed while creating new process! \n");
        exit(1);
    }

    if(pid == 0) {
        close(pd[0]);

        f = fopen("data", "r");
        fgets(buffer, MAX_BUF, f);

        while(!feof(f)) {
            write(pd[1], buffer, MAX_BUF);
            fgets(buffer, MAX_BUF, f);
        }

        close(pd[1]);
    } else {
        close(pd[1]);

        while(1) {
            count = read(pd[0], buffer, MAX_BUF);
            if(count == 0)
                break;
            printf("%s", buffer);
        }

        close(pd[0]);
    }
}
