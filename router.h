#ifndef __ROUTER_H__
#define __ROUTER_H__
#include "graph.h"
#include "queue.h"

typedef struct router {
    int id;
    int * neighbourCost;
    int ** table;
    Queue recvQueue;
    int * nextHop;
} * Router;

typedef struct rUpdateArgs {
    Router router;
    int size;
    Queue (*getBuffer)(int id);
} * RUpdateArgs;

void rInit(Router router, int ** graph, int size, Queue (*getBuffer)(int id));
Router createRouter(int id, int size);
void rUpdate(Router router, int size, Queue (*getBuffer)(int id));

#endif
