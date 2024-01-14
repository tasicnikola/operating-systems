#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pd1[2];
    int pd2[2];
    char line[80];
    int pid1, pid2;

    if(pipe(pd1) == -1) {
        printf("Failed to create first pipe! \n");
        return -1;
    }

    if(pipe(pd2) == -1) {
        printf("Failed to create second pipe! \n");
        return -1;
    }

    if(pid1 = fork() != 0) {
        close(pd1[0]);
        close(pd2[0]);
        close(pd2[1]);
        do {
            gets(line);

            write(pd1[1], line, strlen(line)+1);
            printf("First process sent to pipe1: %s\n", line);
        }
        while(strcmp(line, "END") != 0);

        printf("First process recived END\n");
        wait(NULL);
        close(pd1[1]);
        printf("First process exits! \n");
        return 0;
    } else if(pid2 = fork() != 0) {
        printf("Second process created!\n");
        char line1[80];
        close(pd1[1]);
        close(pd2[0]);

        do {
            read(pd1[0], line1, 80);
            printf("Second process recived: %s\n", line1);
            line1[0] = toupper(line1[0]);
            write(pd2[1], line1, strlen(line1)+1);
            printf("Second process sent: %s\n", line1);
        }
        while(strcmp(line1, "END") != 0);

        printf("Second process recived END\n");
        wait(NULL);
        close(pd1[0]);
        close(pd2[1]);
        printf("Second process exits! \n");
        exit(0);
    } else {
        char line2[80];

        printf("Third process created!\n");
        close(pd1[0]);
        close(pd1[1]);
        close(pd2[1]);

        do {
            read(pd2[0], line2, 80);
            printf("Third process recived: %s\n", line2);

            if(line2[strlen(line2)-1] != '.') {
                line2[strlen(line2)+1] = '\0';
                line2[strlen(line2)] = '.';
            }
            printf("Modified sentence: %s\n", line2);
        }
        while(strcmp(line2, "END") != 0);

        printf("Third process recived END!\n");
        close(pd2[0]);
        printf("Third process exits! \n");
        exit(0);
    }
}
