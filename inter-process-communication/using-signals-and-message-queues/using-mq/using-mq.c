#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>

#define MSG_QUEUE 10301
#define MAX_MSG 255

struct message {
    long type;
    char text[MAX_MSG];
};

int main() {
    int pid;
    int queueid;
    struct message buffer;
    FILE* f;
    int message;
    char tmp[MAX_MSG];

    queueid = msgget(MSG_QUEUE, IPC_CREAT | 0666);

    pid = fork();

    if(pid < 0) {
        printf("Error while creating new process. \n");
        exit(1);
    }

    if(pid == 0) {
        if(execl("./using-mq-parent", "using-mq-parent", "half1.txt", "1", NULL) < 0) {
            printf("Error while loading file!\n");
        }

        exit(1);
    }

    pid = fork();

    if(pid < 0) {
        printf("Error while creating new process. \n");
        exit(1);
    }
    if(pid == 0) {
        if(execl("./using-mq-parent", "using-mq-parent", "half2.txt", "2", NULL) < 0) {
            printf("Error while loading file!\n");
        }

        exit(1);
    }

    if(queueid < 0) {
        printf("Error while creating new process. \n");
        exit(1);
    }

    f = fopen("messages.txt", "r");
    if(f == NULL) {
        printf("Error while loading file!\n");
        exit(1);
    }

    message = 0;
    fgets(tmp, MAX_MSG, f);

    do {
        strcpy(buffer.text, tmp);

        if(message % 2 == 0)
            buffer.type = 1;
        else
            buffer.type = 2;

        if(msgsnd(queueid, &buffer, MAX_MSG, 0) < 0) {
            printf("Error while sending message. \n");
            exit(1);
        }

        message++;
        fgets(tmp, MAX_MSG, f);
    }
    while(!feof(f));

    fclose(f);

    strcpy(buffer.text, "END");
    buffer.type = 1;
    msgsnd(queueid, &buffer, MAX_MSG, 0);

    buffer.type = 1;
    msgsnd(queueid, &buffer, MAX_MSG, 0);

    wait(NULL);
    wait(NULL);
    msgctl(queueid, IPC_RMID, 0);

    return 0;
}
