#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define THREAD_NUM 2;
#define TRUE 1
#define FALSE 0

pthread_mutex_t mutex;
pthread_cond_t condVarEmpty;
pthread_cond_t condVarFull;
int bufferFull = FALSE;
int buffer[2];

void* producer(void *arg) {
        long int rnd;
        float normalisedRnd;
        int sleepTime;
        int i;
        int bufferPointer;
        bufferPointer=0;
        for(i=1 ; i<=10 ; i++)
        {
        rnd = random();
        normalisedRnd = (float)rnd/(float)RAND_MAX;
        sleepTime = (int)(normalisedRnd * 5);
        pthread_mutex_lock(&mutex);
        while (bufferFull)
        pthread_cond_wait(&condVarEmpty, &mutex);
        buffer[bufferPointer] = i;
        printf("Thread wrote into buffer[%d] value of:%d\n",
        bufferPointer, i);
        bufferPointer = (bufferPointer + 1) % 2;
        if (bufferPointer == 0)
        {
        bufferFull = TRUE;
        }
        pthread_cond_signal(&condVarFull);
        pthread_mutex_unlock(&mutex);
        sleep(sleepTime);
        }
}

int main()
{
    pthread_t threadID;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condVarEmpty, NULL);
    pthread_cond_init(&condVarFull, NULL);

    pthread_create(&threadID, NULL, producer, NULL);

    for(int i=0; i<5; i++) {
        pthread_mutex_lock(&mutex);
        while(!bufferFull)
            pthread_cond_wait(&condVarFull, &mutex);

        printf("Sum of numbers from buffer %d + %d = %d\n", buffer[0], buffer[1], buffer[0]+buffer[1]);

        bufferFull = FALSE;

        pthread_cond_signal(&condVarEmpty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condVarEmpty);
    pthread_cond_destroy(&condVarFull);
    return 0;
}
