#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "router.h"

#define debug
#ifdef debug
void showTable(int ** graph, int size) {
    printf("%5s", "");
    for (int i = 0; i < size; i++)
        printf("%6d", i);
    putchar('\n');
    for (int i = 0; i < size; i++) {
        printf("%5d", i);
        if (graph[i] == NULL) {
            printf("empty line\n");
            continue;
        }
        for (int j = 0; j < size; j++) {
            printf("%6d", graph[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void showArray(int * ar, int size) {
    putchar('\n');
    for (int i = 0; i < size; i++) {
        printf("%5d", ar[i]);
    }
    putchar('\n');
}
#endif

static bool isNeighbours(int cost);
static void getNeighboursCost(Router router, int ** graph, int size);
static void sendToNeighbours(Router router, int size, Queue (*getBuffer)(int id));
static bool updateTable(Router router, int size);
static void updateAllReceived(Router router, int size);
static bool setTheMinimalForI(int n, int id, int ** table, int * neighbourCost, int size, int * nextHop);
static bool countNewMinimal(Router router, int size);
static int getCostToNThroughI(int i, int n, int * neighbourCost, int ** table);

void rInit(Router router, int ** graph, int size, Queue (*getBuffer)(int id)) {
    getNeighboursCost(router, graph, size);
    sendToNeighbours(router, size, getBuffer);
}

void rUpdate(Router router, int size, Queue (*getBuffer)(int id)) {
    sleep(1);
    while (true) {
        if (!queueIsEmpty(router->recvQueue)) {
            bool changed = updateTable(router, size);
            if (changed) {
                sendToNeighbours(router, size, getBuffer);
#ifdef debug
                printf("Min cost table changed for router %d:\n", router->id);
                showTable(router->table, size);
                printf("next Hop changed for router %d:\n", router->id);
                showArray(router->nextHop, size);
#endif
            }
        }
#ifdef debug
        printf("Router %d in process!\n", router->id);
#endif
        sleep(1);
    }
}

Router createRouter(int id, int size) {
    Router router = (Router) malloc(sizeof(struct router));
    router->id = id;
    router->table = (int **) malloc(sizeof(int *) * size);
    (router->table)[id] = (int *) malloc(sizeof(int) * size);
    router->recvQueue = createQueue();
    router->nextHop = (int *) malloc(sizeof(int) * size);
    return router;
}

static void getNeighboursCost(Router router, int ** graph, int size) {
//    router->neighbourCost = (int *) malloc(sizeof(int) * size);
    router->neighbourCost = graph[router->id];
    for (int i = 0; i < size; i++) {
        (router->table)[router->id][i] = graph[router->id][i];
        (router->nextHop)[i] = router->id;
    }
}

static bool isNeighbours(int cost) {
    return (cost != 0 && cost != INFINITY);
}

static void sendToNeighbours(Router router, int size, Queue (*getBuffer)(int id)) {
    Queue bufferToSend;
    for (int i = 0; i < size; i++) {
        if (isNeighbours(router->neighbourCost[i])) {
            int * minCost = (int *) malloc(sizeof(int) * size);
            for (int j = 0; j < size; j++) {
                minCost[j] = ((router->table)[router->id])[j];
            }
            Data data = createData(router->id, minCost);
            bufferToSend = getBuffer(i);
            appendToQueue(bufferToSend, data);
        }
    }
}

static bool updateTable(Router router, int size) {
    updateAllReceived(router, size);
    bool changed = false;
    changed = countNewMinimal(router, size);
    return changed;
}

static void updateAllReceived(Router router, int size) {
    while (!queueIsEmpty(router->recvQueue)) {
        Data data = popFromQueue(router->recvQueue);
        if (data) {
            int id = data->id;
            int * cost = (router->table)[id];
            if (cost) {
                free(cost);
            }
            (router->table)[id] = data->minimalCostFromId;
            free(data);
        }
    }
}

static bool countNewMinimal(Router router, int size) {
    bool changed = false;
    for (int i = 0; i < size; i++) {
        bool hasNewMin = setTheMinimalForI(i, router->id, router->table, router->neighbourCost, size, router->nextHop);
        if (hasNewMin) {
            changed = true;
        }
    }
    return changed;
}

static bool setTheMinimalForI(int n, int id, int ** table, int * neighbourCost, int size, int * nextHop) {
    int prev = table[id][n];
    int min = INFINITY;
    for (int i = 0; i < size; i++) {
        int cost = getCostToNThroughI(i, n, neighbourCost, table);
        if (cost != INFINITY && (min == INFINITY || cost < min)) {
            min = cost;
            nextHop[n] = i;
        }
    }
    table[id][n] = min;
    return prev != min;
}

static int getCostToNThroughI(int i, int n, int * neighbourCost, int ** table) {
    if (neighbourCost[i] == INFINITY || table[i] == NULL || table[i][n] == INFINITY)
        return INFINITY;
    else
        return neighbourCost[i] + table[i][n];
}
