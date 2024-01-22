#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_QUEUE 10001
#define MAX_MSG 10

struct message {
    long type;
    char text[MAX_MSG];
};

int main() {
    int pid;
    int queueid;
    int number = 0;
    struct message buffer;

    pid = fork();

    if(pid < 0) {
        printf("Error while creating new process. \n");
        exit(1);
    }

    if(pid == 0) {
        if(execl("./child-process", "child-process", NULL) < 0) {
            printf("Error while loading file. \n");
        }

        exit(1);
    }

    queueid = msgget(MSG_QUEUE, IPC_CREAT | 0666);
    if(queueid < 0) {
        printf("Error while creating message queue. \n");
        exit(1);
    }

    do {
        if(msgrcv(queueid, &buffer, MAX_MSG, 0, 0) < -1) {
            printf("Error while receiving message. \n");
            exit(1);
        }

        number = atoi(buffer.text);
        printf("%d\n", number);
    }
    while(number > -1);

    wait(NULL);
    msgctl(queueid, IPC_RMID, 0);

    return 0;
}
