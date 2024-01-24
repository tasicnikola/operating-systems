#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 4
#define M 5

#define MEM_KEY 10001
#define SEM_KEY 10002

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
};

int main() {
    int i, j;
    int semid;
    union semun semopts;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    semid = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    semopts.val = 0;
    semctl(semid, 0, SETVAL, semopts);

    if(fork() != 0) {
        int shm_id = shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
        int *shm_ptr = shmat(shm_id, NULL, 0);

        for(i = 0; i < N; i++)
            for(j = 0; j < M; j++) {
                shm_ptr[i * M + j] = rand() % 100;
                printf("Writing on position %d, %d number: %d\n", i, j, shm_ptr[i * M + j]);
            }

        semop(semid, &sem_unlock, 1);
        shmdt(shm_ptr);
        int status;
        wait(&status);

        shmctl(shm_id, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, NULL);
    } else {
        int shm_id = shmget(MEM_KEY, M*N*sizeof(int), IPC_CREAT | 0666);
        int *shm_ptr = shmat(shm_id, NULL, 0);

        semop(semid, &sem_lock, 1);

        for(i = 0; i < N; i++) {
            int maxel = 0;

            for(j = 0; j < M; j++) {
                if(shm_ptr[i * M + j] > maxel)
                    maxel = shm_ptr[i * M + j];
            }

            printf("Max element of column %d is: %d\n", i, maxel);
        }

        shmdt(shm_ptr);
    }
}
