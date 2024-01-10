#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 20

int arr[N];

pthread_mutex_t mutex1, mutex2;

void* thread1(void* arg)
{
    int pos;

    while(1) {
        pthread_mutex_lock(&mutex1);
        pos = rand() % 10;
        arr[pos] = rand() % 100;

        printf("First thread on the position of: %d wrote a value of %d\n", pos, arr[pos]);

        pthread_mutex_unlock(&mutex1);

        sleep(2);
    }
}

void* thread2(void* arg)
{
    int pos;
    while(1) {
        pthread_mutex_lock(&mutex2);
        pos = rand() % 10 + 10;
        arr[pos] = rand() % 100;

        printf("Second thread on the position of: %d wrote a value of %d\n", pos, arr[pos]);

        pthread_mutex_unlock(&mutex2);
        sleep(4);
    }
}

void* thread3(void* arg)
{
    int pos;

    while(1) {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);

        pos = rand() % 20;
        arr[pos] = rand() % 100;

        printf("Third thread on the position of: %d wrote a value of %d\n", pos, arr[pos]);

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);

        sleep(8);
    }
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&t1, NULL, &thread1, NULL);
    pthread_create(&t2, NULL, &thread2, NULL);
    pthread_create(&t3, NULL, &thread3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
