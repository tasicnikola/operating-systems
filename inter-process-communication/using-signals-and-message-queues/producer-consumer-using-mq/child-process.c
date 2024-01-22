#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MSG_QUEUE 10001
#define MAX_MSG 10

struct message {
    long type;
    char text[MAX_MSG];
};

int main() {
    int N;
    int queueid;
    int i;
    struct message buffer;

    srand(time(NULL));

    queueid = msgget(MSG_QUEUE, IPC_CREAT | 0666);
    if(queueid < 0) {
        printf("Error while creating message queue. \n");
        exit(1);
    }

    N = rand() % 20;
    for(i = 0; i<N; i++) {
        if(i == N - 1) {
            sprintf(buffer.text, "%d", -1);
        } else {
            sprintf(buffer.text, "%d", rand() % 100);
        }
        buffer.type = 1;

        if(msgsnd(queueid, &buffer, sizeof(buffer.text), 0) < -1) {
            printf("Error while sending message. \n");
            exit(1);
        }
    }

    return 0;
}
