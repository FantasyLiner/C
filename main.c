#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "router.h"
#define SIZE 10

int ** createGraph(int size) {
    int ** graph = (int **) malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++) {
        graph[i] = (int *) malloc(sizeof(int) * size);
    }
    srand((unsigned)time(0));
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {
                graph[i][j] = 0;
            }
            //else if (rand() % 10 < 7) {
            else if (true) {
                graph[i][j] = graph[j][i] = rand() % 10 + 1;
            }
            else {
                graph[i][j] = graph[j][i] = INFINITY;
            }
        }
    }
    return graph;
}

void showGraph(int ** graph, int size) {
    printf("%5s", "");
    for (int i = 0; i < size; i++)
        printf("%6d", i);
    putchar('\n');
    for (int i = 0; i < size; i++) {
        printf("%5d", i);
        for (int j = 0; j < size; j++) {
            printf("%6d", graph[i][j]);
        }
        putchar('\n');
    }
}

Router router[SIZE];

Queue getBuffer(int id) {
    return (router[id])->recvQueue;
}

void routerUpdate(void * args) {
    RUpdateArgs updateArgs = (RUpdateArgs) args;
    rUpdate(updateArgs->router, updateArgs->size, updateArgs->getBuffer);
}

int main() {
    int ** graph = createGraph(SIZE);
    pthread_t thread[SIZE];
    puts("This is graph:");
    showGraph(graph, SIZE);
    putchar('\n');
    struct rUpdateArgs args[3];
    for (int i = 0; i < SIZE; i++) {
        router[i] = createRouter(i, SIZE);
    }
    for (int i = 0; i < SIZE; i++) {
        rInit(router[i], graph, SIZE, getBuffer);
    }
    for (int i = 0; i < SIZE; i++) {
        args[i].router = router[i];
        args[i].size = SIZE;
        args[i].getBuffer = getBuffer;
        pthread_create(&thread[i], NULL, (void *)routerUpdate, &(args[i]));
    }

    while (true) {
        sleep(1);
        for (int i = 0; i < SIZE; i++) {
            printf("table %d:\n", i);
            showTable(router[i]->table, SIZE);
        }
    }
}
