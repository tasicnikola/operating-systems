#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define CTRL_C_THRESHOLD 5
int ctrl_c_count = 0;

void catch_int(int sig_num) {
    signal(SIGINT, catch_int);
    ctrl_c_count++;

    if(ctrl_c_count >= CTRL_C_THRESHOLD) {
        char answer[30];
        printf("Are you sure you want to exit? [y/n]\n");
        fflush(stdout);
        fgets(answer, sizeof(answer), stdin);

        if(answer[0] == 'y' || answer[0] == 'Y') {
            printf("Exiting...\n");
            fflush(stdout);
            exit(0);
        } else {
            printf("\nContinuing...\n");
            fflush(stdout);
            ctrl_c_count = 0;
        }
    }
}

void catch_suspend(int sig_num) {
    signal(SIGTSTP, catch_suspend);
    printf("\nCTRL-C pressed %d times\n", ctrl_c_count);
    fflush(stdout);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, catch_int);
    signal(SIGTSTP, catch_suspend);

    for( ;; )
        pause();

    return 0;
}
