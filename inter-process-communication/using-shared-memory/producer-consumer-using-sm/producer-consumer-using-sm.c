#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define FULL_KEY 10001
#define EMPTY_KEY 10002
#define MUTEX_KEY 10003
#define MEM_KEY 10004
#define N 10

union semun {
    int val;
    struct semid_ds* buf;
    ushort* array;
    void* pad;
};

int main() {
    int fullid, emptyid, mutexid;
    int memid;
    int* shmem;
    int i;
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};
    union semun opts;

    fullid = semget(FULL_KEY, 1, IPC_CREAT | 0666);
    if(fullid < 0) {
        printf("Error while creating semaphore FULL. \n");
        exit(1);
    }
    opts.val = 0;

    if(semctl(fullid, 0, SETVAL, opts) < 0) {
        printf("Error while init semaphore FULL. \n");
        exit(1);
    }

    emptyid = semget(EMPTY_KEY, 1, IPC_CREAT | 0666);
    if(emptyid < 0) {
        printf("Error while creating semaphore EMPTY. \n");
        exit(1);
    }
    opts.val = N;
    if(semctl(emptyid, 0, SETVAL, opts) < 0) {
        printf("Error while init semaphore EMPTY. \n");
        exit(1);
    }

    mutexid = semget(MUTEX_KEY, 1, IPC_CREAT | 0666);
    if(mutexid < 0) {
        printf("Error while creating semaphore MUTEX. \n");
        exit(1);
    }
    opts.val = 1;
    if(semctl(mutexid, 0, SETVAL, opts) < 0) {
        printf("Error while init semaphore MUTEX. \n");
        exit(1);
    }

    memid = shmget(MEM_KEY, N * sizeof(int), IPC_CREAT | 0666);
    if(memid < 0) {
        printf("Error while creating shared memory. \n");
        exit(1);
    }

    if(fork() == 0) {
        int next_write = 0;
        shmem = shmat(memid, NULL, 0);
        if(shmem == NULL) {
            printf("Error while mapping shared memory. \n");
            exit(1);
        }

        for(i = 0; i < 20; i++) {
            semop(emptyid, &lock, 1);
            semop(mutexid, &lock, 1);
            int p = rand() % 100;
            shmem[next_write] = p;
            next_write = (next_write++) % N;
            printf("Generated value: %d \n", p);
            semop(mutexid, &unlock, 1);
            semop(fullid, &unlock, 1);
            sleep(2);
        }

        shmdt(shmem);
    } else {
        int next_read = 0;
        shmem = shmat(memid, NULL, 0);
        if(shmem == NULL) {
            printf("Error while mapping shared memory. \n");
            exit(1);
        }

        for(i = 0; i < 20; i++) {
            semop(fullid, &lock, 1);
            semop(mutexid, &lock, 1);
            printf("Read value: %d \n", shmem[next_read]);
            next_read = (next_read++) % N;
            semop(mutexid, &unlock, 1);
            semop(emptyid, &unlock, 1);
            sleep(5);
        }

        shmdt(shmem);
        semctl(fullid, 0, IPC_RMID, 0);
        semctl(emptyid, 0, IPC_RMID, 0);
        semctl(mutexid, 0, IPC_RMID, 0);
        semctl(memid, 0, IPC_RMID, 0);
    }

    return 0;
}
