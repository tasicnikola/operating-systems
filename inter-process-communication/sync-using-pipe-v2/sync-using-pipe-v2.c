#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main() {
    int pd1[2];
    int pd2[2];
    int pd3[2];
    char line[80]; //buffer
    int pid1, pid2;

    if(pipe(pd1) == -1) {
        printf("Failed to create first pipe!\n");
        return -1;
    }

    if(pipe(pd2) == -1) {
        printf("Failed to create second pipe!\n");
        return -1;
    }

    if(pipe(pd3) == -1) {
        printf("Failed to create second pipe!\n");
        return -1;
    }

    if((pid1 = fork()) != 0) {
        close(pd1[0]);
        close(pd2[0]);
        close(pd2[1]);
        close(pd3[1]);
        printf("Waiting for input: \n");

        do {
            fgets(line, sizeof(line), stdin);
            write(pd1[1], line, strlen(line) + 1);
            read(pd3[0], line, 80);
            printf("Modified sentence: %s\n", line);
        }
        while(strcmp(line, "END") != 0);

        wait(NULL);
        close(pd1[1]);

        return 0;
    } else if ((pid2 = fork()) != 0) {
        char line1[80];
        close(pd1[1]);
        close(pd2[0]);
        close(pd3[0]);
        close(pd3[1]);

        do {
            read(pd1[0], line1, 80);
            line1[0] = toupper(line1[0]);
            write(pd2[1], line1, strlen(line1)+1);
        }
        while(strcmp(line1, "END") != 0);

        wait(NULL);
        close(pd2[1]);
        close(pd1[0]);
        exit(0);
    } else {
        char line2[80];
        close(pd1[0]);
        close(pd1[1]);
        close(pd2[1]);
        close(pd3[0]);

        do {
            read(pd2[0], line2, 80);

            if(line2[strlen(line2)-1] != '.') {
                line2[strlen(line2)+1] = '\0';
                line2[strlen(line2)] = '.';
            }

            write(pd3[1], line2, strlen(line2)+1);
        }
        while(strcmp(line2, "END") != 0);

        close(pd2[0]);
        close(pd3[1]);
        exit(0);
    }
}
