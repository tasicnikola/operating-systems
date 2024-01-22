#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_QUEUE 10301
#define MAX_MSG 255

struct message {
    long type;
    char text[MAX_MSG];
};

int main(int argc, char* argv[]) {
    int queueid;
    struct message buffer;
    FILE* f;
    int type;

    if(argc < 3) {
        printf("Not enough arguments.\n");
        exit(1);
    }

    queueid = msgget(MSG_QUEUE, IPC_CREAT | 0666);

    if(queueid < 0) {
        printf("Error while creating message queue. \n");
        exit(1);
    }

    type = atoi(argv[2]);
    if(type == 0) {
        printf("Wrong message type! \n");
    }

    f = fopen(argv[1], "w");
    if(f == NULL) {
        printf("Error while opening file. \n");
        exit(1);
    }

    while(strcmp(buffer.text, "END") != 0) {
        if(msgrcv(queueid, &buffer, MAX_MSG, type, 0) < 0) {
            printf("Error while receiving message.\n");
            break;
        }

        if(strcmp(buffer.text, "END") != 0) {
            fprintf(f, "%s\n", buffer.text);
            fflush(f);
        }
    }

    fclose(f);

    return 0;
}
