#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

char user[40];

void catch_alarm(int sig_num) {
    printf("Timeout. Exiting... \n\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    signal(SIGALRM, catch_alarm);
    printf("Username: ");
    fflush(stdout);
    alarm(30);
    fgets(user, sizeof(user), stdin);
    alarm(0);
    printf("Username: '%s'\n", user);

    return 0;
}
