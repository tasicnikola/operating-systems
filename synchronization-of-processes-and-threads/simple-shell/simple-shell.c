#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char command[100];
    char* args[20];
    int noArgs;

    printf("\nprompt> ");
    gets(command);

    noArgs = 0;
    args[noArgs++] = strtok(command, " ");
    while((args[noArgs++] = strtok(0, " ")) != NULL) {
    }

    while(strcmp(command, "logout") != 0) {
        if(fork() == 0) {
            sleep(3);

            if(strcmp(args[noArgs - 2], "&") == 0)
                args[noArgs - 2] = NULL;

            execvp(args[0], args);
            printf("\nError while executing command!\n");
            exit(-1);
        } else {
            if(strcmp(args[noArgs - 2], "&") != 0)
                wait(&retStatus);
        }

        printf("\nprompt> ");
        gets(command);
        noArgs = 0;
        args[noArgs++] = strtok(command, " ");

        while((args[noArgs++] = strtok(0," ")) != NULL) {
        }
    }
}
