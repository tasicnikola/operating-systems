#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void catch_int(int sig_num) {
    signal(SIGINT, SIG_DFL);
    printf("Error while stopping program.\n");
    fflush(stdout);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, catch_int);
    for( ;; )
        pause();
}
