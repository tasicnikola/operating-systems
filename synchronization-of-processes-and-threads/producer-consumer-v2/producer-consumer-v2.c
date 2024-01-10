#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N1 10
#define N2 5
#define N3 20

int B1[N1], B2[N2], B3[N3];

int empty1 = N1;
int empty2 = N2;
int empty3 = N3;

int tail1 = 0;
int head1 = 0;
int head2 = 0;
int tail2 = 0;
int head3 = 0;
int tail3 = 0;

sem_t mutex_B1, mutex_B2, mutex_B3, mutex_empty;

void* nit0(void* arg) {
    int data;

    while(1) {
        sleep(rand() % 3);
        sem_wait(&mutex_empty);
        sem_wait(&mutex_B3);
        if(empty3 > 0) {
            B3[tail3] = rand() % 1000;
            tail3 = (tail3+1) % N3;
            empty3--;
            sem_post(&mutex_B3);
            continue;
        } else
            sem_post(&mutex_B3);

        if(empty1 > 0) {
            B1[tail1] = rand() % 1000;
            tail1 = (tail1 + 1) % N1;
            empty1--;
            sem_post(&mutex_B1);
            continue;
        } else
            sem_post(&mutex_B1);

        if(empty2 > 0) {
            B2[tail2] = rand() % 1000;
            tail2 = (tail2 + 1) % N2;
            empty2--;
            sem_post(&mutex_B2);
        }
        sem_post(&mutex_B2);
    }
}

void* nit1(void* arg) {
    while(1) {
        sem_wait(&mutex_B1);
        if(empty1 < N1) {
            printf("Thread T1 read %d", B1[head1]);
            head1 = (head1 + 1) % N1;
            empty1++;
            sem_post(&mutex_empty);
        }
        sem_post(&mutex_B1);
        sleep(rand() % 5);
    }
}

void* nit2(void* arg) {
    while(1) {
        sem_wait(&mutex_B2);

        if(empty2 < N2) {
            printf("Thread T2 read %d", B2[head2]);
            head2 = (head2 + 1) % N2;
            empty2++;
            sem_post(&mutex_empty);
        }
        sem_post(&mutex_B2);
        sleep(rand() % 7);
    }
}

void * nit3(void * arg)
{
    while(1) {
        sem_wait(&mutex_B3);
        if (empty3 < N3) {
            printf("Thread T3 read %d\n", B3[head3]);;
            head3 = (head3 + 1) % N3;
            empty3++;
            sem_post(&mutex_empty);
        }
        sem_post(&mutex_B3);
        sleep(rand() % 9);
    }
}

int main()
{
    int i;
    pthread_t niti[4];

    srand(3232234);
    sem_init(&mutex_B1, 0, 1);
    sem_init(&mutex_B2, 0, 1);
    sem_init(&mutex_B3, 0, 1);
    sem_init(&mutex_empty, 0, N1+N2+N3);

    pthread_create(&niti[0], NULL, &nit0, NULL);
    pthread_create(&niti[1], NULL, &nit1, NULL);
    pthread_create(&niti[2], NULL, &nit2, NULL);
    pthread_create(&niti[3], NULL, &nit3, NULL);

    for(i = 0; i<4; i++) {
        if(pthread_join(niti[i], NULL) != 0) {
            perror("Failed to join threads!");
        }
    }

    sem_destroy(&mutex_B1);
    sem_destroy(&mutex_B2);
    sem_destroy(&mutex_B3);
    sem_destroy(&mutex_empty);

    return 0;
}
