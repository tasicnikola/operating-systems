#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

int main(int argc, char* argv[]) {
    int pd[2];
    int pid;
    char* args[10];
    int i;

    if(pipe(pd) < 0) {
        exit(1);
    }
    pid = fork();
    if(pid == 0) {
        close(1);
        dup(pd[1]);

        for(i=1; i<argc; i++) {
            ars[i-1] = argv[i];
        }
        args[argc-1] = NULL;

        execvp(args[0], args);
    } else {
        close(0);
        dup(pd[0]);
        ececlp("more", "more", NULL);
    }
}
