#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "queue.h"
#include "data.h"

#define SIZE 3
#define RATIO 6

void reader(void * args) {
    Queue q = (Queue) args;
    while (true) {
        while (!queueIsEmpty(q)) {
            Data d = popFromQueue(q);
            printf("\b \b");
            fflush(stdout);
            destroyData(d);
        }
        sleep(3);
    }
}

void writer(void * args) {
    Queue q = (Queue) args;
    srand((unsigned)time(0));
    while (true) {
        int id = rand() % 100;
        int * ar = (int *) malloc(sizeof(int) * SIZE);
        Data d = createData(id, ar);
        printf("%c", rand() % 26 + 97);
        fflush(stdout);
        appendToQueue(q, d);
        sleep(1);
    }
}

int main() {
    pthread_t thread[RATIO * SIZE];
    Queue q[SIZE];

    for (int i = 0; i < SIZE; i++) {
        q[i] = createQueue();
    }

    printf("-----\n");
    for (int i = 0; i < SIZE * RATIO; i += RATIO) {
        printf("i = %d\n", i);
        pthread_create(&thread[i], NULL, (void *)reader, (void *)q[i / RATIO]);
        printf("Created reader %d\n", i);
        for (int j = i + 1; j < i + RATIO; j++) {
            printf("j = %d\n", j);
            pthread_create(&thread[j], NULL, (void *)writer, (void *)q[i / RATIO]);
            printf("Created writer %d\n", j);
        }
    }
    puts("all created!");

    while (true) {
        sleep(100);
    }
}
