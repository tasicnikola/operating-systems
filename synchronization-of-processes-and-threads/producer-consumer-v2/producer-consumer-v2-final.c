#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE_B1 10
#define BUFFER_SIZE_B2 5
#define BUFFER_SIZE_B3 20

typedef struct {
    int* data;
    int head;
    int tail;
    int size;
    sem_t empty;
    sem_t full;
    sem_t mutex;
} Buffer;

Buffer bufferB1, bufferB2, bufferB3;

void initializeBuffer(Buffer *buffer, int size) {
    buffer->data = (int*)malloc(size * sizeof(int));
    buffer->head = buffer->tail = 0;
    buffer->size = size;
    sem_init(&buffer->empty, 0, size);
    sem_init(&buffer->full, 0, 0);
    sem_init(&buffer->mutex, 0, 1);
}

void enqueue(Buffer *buffer, int value) {
    sem_wait(&buffer->empty);
    sem_wait(&buffer->mutex);

    buffer->data[buffer->tail] = value;
    buffer->tail = (buffer->tail + 1) % buffer->size;

    sem_post(&buffer->mutex);
    sem_post(&buffer->full);
}

int dequeue(Buffer *buffer) {
    sem_wait(&buffer->full);
    sem_wait(&buffer->mutex);

    int value = buffer->data[buffer->head];
    buffer->head = (buffer->head + 1) % buffer->size;

    sem_post(&buffer->mutex);
    sem_post(&buffer->empty);

    return value;
}

void *producer(void *arg) {
    for(int i=0; i<10; i++) {
        int data = rand() % 100;

        if(sem_trywait(&bufferB3.empty) == 0) {
            printf("Producer: enqueue %d to B3\n", data);
            enqueue(&bufferB3, data);
        } else if (sem_trywait(&bufferB1.empty) == 0) {
            printf("Producer: enqueue %d to B1\n", data);
            enqueue(&bufferB3, data);
        } else if (sem_trywait(&bufferB2.empty) == 0) {
            printf("Producer: enqueue %d to B2\n", data);
            enqueue(&bufferB3, data);
        } else {
            i--;
            usleep(100000);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *buffer) {
    Buffer* targetBuffer = (Buffer *)buffer;
    for(int i=0; i<10; i++) {
        int data = dequeue(targetBuffer);
        printf("Consumer: Dequeue %d from Buffer\n", data);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    initializeBuffer(&bufferB1, BUFFER_SIZE_B1);
    initializeBuffer(&bufferB2, BUFFER_SIZE_B2);
    initializeBuffer(&bufferB3, BUFFER_SIZE_B3);

    pthread_t producerThread, consumerThreadB1, consumerThreadB2, consumerThreadB3;

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThreadB1, NULL, consumer, &bufferB1);
    pthread_create(&consumerThreadB2, NULL, consumer, &bufferB2);
    pthread_create(&consumerThreadB3, NULL, consumer, &bufferB3);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThreadB1, NULL);
    pthread_join(consumerThreadB2, NULL);
    pthread_join(consumerThreadB3, NULL);

    sem_destroy(&bufferB1.empty);
    sem_destroy(&bufferB1.full);
    sem_destroy(&bufferB1.mutex);
    free(bufferB1.data);

    sem_destroy(&bufferB2.empty);
    sem_destroy(&bufferB2.full);
    sem_destroy(&bufferB2.mutex);
    free(bufferB2.data);

    sem_destroy(&bufferB3.empty);
    sem_destroy(&bufferB3.full);
    sem_destroy(&bufferB3.mutex);
    free(bufferB3.data);

    return 0;
}
