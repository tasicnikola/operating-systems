#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>

#define MEM_KEY 10001
#define PROC_A_KEY 10002
#define PROC_B_KEY 10003
#define BUF_SIZE 80

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

struct rdstruct {
    char nch;
    char buf[BUF_SIZE];
};

int main(int argc, char* argv[]) {
    int i, procaid, procbid;
    union semun semopts;

    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    procaid = semget(PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget(PROC_B_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);

    printf("argv[1]: %s, argv[2]: %s\n", argv[1], argv[2]);

    if(fork() != 0) {
        int shm_id = shmget(MEM_KEY, sizeof(struct rdstruct), IPC_CREAT | 0666);
        struct rdstruct *shm_ptr = shmat(shm_id, NULL, 0);

        int fd = open(argv[1], O_RDONLY);
        shm_ptr->nch = BUF_SIZE;

        while(shm_ptr->nch == BUF_SIZE) {
            semop(procaid, &sem_lock, 1);
            shm_ptr->nch = read(fd, shm_ptr->buf, BUF_SIZE);
            semop(procbid, &sem_unlock, 1);
        }

        close(fd);
        shmdt(shm_ptr);
        int status;
        wait(&status);

        shmctl(shm_id, IPC_RMID, 0);
        semctl(procaid, IPC_RMID, 0);
        semctl(procbid, IPC_RMID, 0);
        close(fd);
    } else {
        int shm_id = shmget(MEM_KEY, sizeof(struct rdstruct), IPC_CREAT | 0666);
        struct rdstruct *shm_ptr = shmat(shm_id, NULL, 0);

        int fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 00777);
        int n = BUF_SIZE;

        while(n == BUF_SIZE) {
            semop(procbid, &sem_lock, 1);
            write(fd, shm_ptr->buf, shm_ptr->nch);
            n = shm_ptr->nch;
            semop(procaid, &sem_unlock, 1);
        }

        close(fd);
        shmdt(shm_ptr);
    }
}
