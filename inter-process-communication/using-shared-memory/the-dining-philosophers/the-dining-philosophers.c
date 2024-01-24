#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SEMAPHORES_KEY 10001
#define MUTEX_KEY 10002
#define STATUS_KEY 10003

#define N 5

enum status {THINKING, HUNGRY, EATING};

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
    struct seminfo* __buf;
    void* __pad;
};

int main() {
    int s, mutex, statuses;
    int i;
    enum status* shmem;
    union semun opts;

    s = semget(SEMAPHORES_KEY, N, IPC_CREAT | 0666);
    opts.val = 1;
    for(i = 0; i < N; i++) {
        semctl(s, i, SETVAL, opts);
    }

    mutex = semget(MUTEX_KEY, N, IPC_CREAT | 0666);
    opts.val = 1;
    semctl(mutex, 0, SETVAL, opts);

    statuses = shmget(STATUS_KEY, N * sizeof(enum status), IPC_CREAT | 0666);

    shmem = (enum status*)shmat(statuses, NULL, 0);

    for(i = 0; i < N; i++) {
        shmem[i] = THINKING;
    }

    shmdt(shmem);

    for(i = 0; i < N; i++) {
        if(fork() == 0) {
            char buff[2];
            sprintf(buff, "%d", i);

            execl("./philosophers", "philosophers", buff, NULL);
        }
    }

    for(i = 0; i < N; i++) {
        wait(NULL);
    }

    semctl(s, 0, IPC_RMID, opts);
    semctl(mutex, 0, IPC_RMID, opts);
    shmctl(statuses, IPC_RMID, 0);

    return 0;
}
