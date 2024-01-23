#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define FIRST_KEY 10001
#define SECOND_KEY 10002
#define THIRD_KEY 10003
#define MEM_KEY 10004
#define N 1024

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
    struct  seminfo* __buf;
    void* __pad;
};

int main() {
    int sem1, sem2, sem3, memid;
    int i, j;
    FILE* f;
    char* shmem;
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};
    union semun opts;

    sem1 = semget(FIRST_KEY, 1, IPC_CREAT | 0666);
    sem2 = semget(SECOND_KEY, 1, IPC_CREAT | 0666);
    sem3 = semget(THIRD_KEY, 1, IPC_CREAT | 0666);

    opts.val = 1;
    semctl(sem1, 0, SETVAL, opts);

    opts.val = 0;
    semctl(sem2, 0, SETVAL, opts);

    opts.val = 0;
    semctl(sem3, 0, SETVAL, opts);

    memid = shmget(MEM_KEY, N * sizeof(char), IPC_CREAT | 0666);

    if(fork() == 0) {
        shmem = (char *)shmat(memid, NULL, 0);
        for(i = 0; i < 10; i++) {
            semop(sem1, &lock, 1);
            for(j = 0; j < N/2; j++) {
                shmem[j] = 'a' + rand() % ('z'-'a' + 1);
                semop(sem2, &unlock, 1);
            }

            shmdt(shmem);

            return 0;
        }
    }

    if(fork() == 0) {
        shmem = (char *)shmat(memid, NULL, 0);
        for(i = 0; i < 10; i++) {
            semop(sem2, &lock, 1);
            for(j = N/2; j < N; j++) {
                shmem[j] = '0' + rand() % ('9'-'0' + 1);
                semop(sem3, &unlock, 1);
            }

            shmdt(shmem);

            return 0;
        }
    }

    shmem = (char *)shmat(memid, NULL, SHM_RDONLY);
    for(i = 0; i < 10; i++) {
        semop(sem3, &lock, 1);
        char buff[N + 1];
        strncpy(buff, shmem, N);
        buff[N] = '\0';
        f = fopen("data", "a");
        fprintf(f, "%s\n", buff);
        fclose(f);
        semop(sem1, &unlock, 1);
    }

    shmdt(shmem);
    semctl(sem1, 0, IPC_RMID, opts);
    semctl(sem2, 0, IPC_RMID, opts);
    semctl(sem3, 0, IPC_RMID, opts);
    shmctl(memid, IPC_RMID, 0);

    return 0;
}
