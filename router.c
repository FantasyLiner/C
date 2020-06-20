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

static void getNeighboursCost(Router router, int ** graph, int size);
static void sendToNeighbours(Router router, int size, Queue (*getBuffer)(int id));
static bool updateTable(Router router, int size);
static void updateAllReceived(Router router, int size);
static bool setTheMinimalForI(int n, int id, int ** table, int * neighbourCost, int size);
static bool countNewMinimal(Router router, int size);
static int getCostToNThroughI(int i, int n, int * neighbourCost, int ** table);

void rInit(Router router, int ** graph, int size, Queue (*getBuffer)(int id)) {
    getNeighboursCost(router, graph, size);
#ifdef debug
    printf("Successfully get neighboursCost%d\n", router->id);
    showTable(router->table, size);
#endif
    sendToNeighbours(router, size, getBuffer);
}

void rUpdate(Router router, int size, Queue (*getBuffer)(int id)) {
    sleep(1);
    while (true) {
        if (!queueIsEmpty(router->recvQueue)) {
            bool changed = updateTable(router, size);
            if (changed) {
                sendToNeighbours(router, size, getBuffer);
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
    router->neighbourCost = (int *) malloc(sizeof(int) * size);
    router->table = (int **) malloc(sizeof(int *) * size);
    (router->table)[router->id] = (int *) malloc(sizeof(int) * size);
    router->recvQueue = createQueue();
    router->nextHop = (int *) malloc(sizeof(int) * size);
    return router;
}

static void getNeighboursCost(Router router, int ** graph, int size) {
    router->neighbourCost = graph[router->id];
    for (int i = 0; i < size; i++) {
        (router->table)[router->id][i] = graph[router->id][i];
    }
}

static void sendToNeighbours(Router router, int size, Queue (*getBuffer)(int id)) {
    Queue bufferToSend;
    for (int i = 0; i < size; i++) {
        if (router->neighbourCost[i] != 0 && router->neighbourCost[i] != INFINITY) {
            int * minCost = (int *) malloc(sizeof(int) * size);
            for (int j = 0; j < size; j++) {
                minCost[j] = ((router->table)[router->id])[j];
            }
            Data data = createData(router->id, minCost);
            bufferToSend = getBuffer(i);
#ifdef debug
            printf("\n%d want to send update packet to %d\n", router->id, i);
            showArray(data->minimalCostFromId, size);
#endif
            appendToQueue(bufferToSend, data);
#ifdef debug
            printf("\n%d sucessfully send update packet to %d\n", router->id, i);
#endif
        }
    }
}

static bool updateTable(Router router, int size) {
    updateAllReceived(router, size);
    bool changed = false;
    changed = countNewMinimal(router, size);
#ifdef debug
    printf("Router %d table %s\n", router->id, changed? "changed!": "NOT changed");
    if (changed) {
#ifdef debug
        printf("\nSuccessfully update table %d\n", router->id);
        showTable(router->table, size);
#endif
    }
#endif
    return changed;
}

static void updateAllReceived(Router router, int size) {
    printf("check table in router %d has size %d\n", router->id, getSize(router->recvQueue));
    while (!queueIsEmpty(router->recvQueue)) {
#ifdef debug
        printf("Size of table in router %d = %d\n", router->id, getSize(router->recvQueue));
#endif
        Data data = popFromQueue(router->recvQueue);
        if (data) {
            int id = data->id;
#ifdef debug
            printf("router %d get update packet from %d\n", router->id, id);
            showData(data);
#endif
            int * cost = (router->table)[id];
            if (cost) {
                free(cost);
            }
            (router->table)[id] = data->minimalCostFromId;
            free(data);
#ifdef debug
            printf("Router %d recevice update packet from %d:\n", router->id, id);
            showArray((router->table)[id], size);
#endif

        }
    }
}

static bool countNewMinimal(Router router, int size) {
    bool changed = false;
    for (int i = 0; i < size; i++) {
        bool hasNewMin = setTheMinimalForI(i, router->id, router->table, router->neighbourCost, size);
        if (hasNewMin) {
            changed = true;
        }
    }
    return changed;
}

static bool setTheMinimalForI(int n, int id, int ** table, int * neighbourCost, int size) {
    int prev = table[id][n];
    int min = INFINITY;
    for (int i = 0; i < size; i++) {
        int cost = getCostToNThroughI(i, n, neighbourCost, table);
        if (cost != INFINITY && cost < min) {
            min = cost;
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
