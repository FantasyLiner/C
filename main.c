#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "router.h"
#include "graph.h"
#include "dij.h"
#define SIZE 10

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
    struct rUpdateArgs args[SIZE];
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

    sleep(10);
    for (int i = 0; i < SIZE; i++) {
        printf("Table %d:\n", i);
        showArray(router[i]->table[i], SIZE);
    }

    for (int i = 0; i < SIZE; i++) {
        printf("%d\n", i);
        shortestPathFromStartPoint(i, graph, SIZE);
    }

}
