#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int database = 0;
int readers = 0;

sem_t mutex_DB, mutex_N;

//Simulation of database reading
void readDatabase(int i) {
    printf("Thread %d read a value of %d\n",i, database);
    sleep(rand() % 3);
}

//Simulation of database writing
void writeDatabase() {
    printf("Writer changes values into database. \n");
    database = database + rand() % 10;
    sleep(rand() % 5);
}

void* writer(void* arg) {
    while(1) {
        sem_wait(&mutex_DB);
        writeDatabase();
        sem_post(&mutex_DB);

        sleep(rand() % 10);
    }
}

void* reader(void* arg) {
    int i;
    i = *((int *)arg);

    while(1) {
        sem_wait(&mutex_N);
        readers++;
        if(readers == 1)
            sem_wait(&mutex_DB);
        sem_post(&mutex_N);
        readDatabase(i);
        sem_wait(&mutex_N);
        readers--;
        if(readers == 0)
            sem_post(&mutex_DB);
        sem_post(&mutex_N);

        sleep(rand() % 7);
    }
}

int main() {
    int i;
    int red_br[4];
    pthread_t threads[5];
    srand(3232234);

    sem_init(&mutex_N, 0, 1);
    sem_init(&mutex_DB, 0, 1);

    for(i=0; i<4; i++) {
        red_br[i] = i;
        pthread_create(&threads[i], NULL, &reader, &red_br[i]);
    }
    pthread_create(&threads[4], NULL, &writer, NULL);

    for(i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&mutex_N);
    sem_destroy(&mutex_DB);
}
