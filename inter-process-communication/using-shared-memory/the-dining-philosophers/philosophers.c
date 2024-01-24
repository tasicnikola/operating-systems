#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMAPHORES_KEY 10001
#define MUTEX_KEY 10002
#define STATUS_KEY 10003

#define N 5

enum status {THINKING, HUNGRY, EATING};
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo * __buf;
    void * __pad;
};

int s, mutex, statuses;
enum status* shmem;
struct sembuf lock = {0, -1, 0};
struct sembuf unlock = {0, 1, 0};

void think(int no);
void take_forks(int no);
void put_forks(int no);
void eat(int no);
void test(int no);

int main(int argc, char* argv[]) {
    int i;
    int no = atoi(argv[1]);

    s = semget(SEMAPHORES_KEY, N, 0666);
    mutex = semget(MUTEX_KEY, N, 0666);
    statuses = shmget(STATUS_KEY, N * sizeof(enum status), 0666);
    shmem = (enum status *)shmat(statuses, NULL, 0);

    for(i = 0; i < 1000; i++) {
        think(no);
        take_forks(no);
        eat(no);
        put_forks(no);
    }

    shmdt(shmat);

    return 0;
}

void think(int no) {
    printf("Philosopher %d thinking. \n", no);
    sleep(rand() % 11);
}

void eat(int no) {
    printf("Philosopher %d eats. \n", no);
    sleep(rand() % 6);
}

void take_forks(int no) {
    lock.sem_num = 0;
    semop(mutex, &lock, 1);

    shmem[no] = HUNGRY;
    test(no);

    unlock.sem_num = 0;
    semop(mutex, &unlock, 1);

    lock.sem_num = no;
    semop(s, &lock, 1);
}

void test(int no) {
    if(shmem[no] == HUNGRY
        && shmem[(no + N - 1) % N] != EATING
        && shmem[(no + 1) % N] != EATING
    ) {
        shmem[no] = EATING;
        unlock.sem_num = no;
        semop(s, &unlock, 1);
    }
}

void put_forks(int no) {
    lock.sem_num = 0;
    semop(mutex, &lock, 1);

    shmem[no] = THINKING;

    test((no + N - 1) % N);
    test((no + 1) % N);

    unlock.sem_num = 0;
    semop(mutex, &unlock, 1);
}
