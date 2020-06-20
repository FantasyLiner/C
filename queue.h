#include <stdbool.h>
#include <pthread.h>
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "data.h"

typedef struct node {
    Data data;
    struct node * next;
} * Node;

typedef struct queue {
    int size;
    Node first;
    pthread_mutex_t * lock;
} * Queue;


void initQueue(Queue queue);
Queue createQueue();
void appendToQueue(Queue, Data);
Data popFromQueue(Queue);
bool queueIsEmpty(Queue);
void traverse(Queue queue, void (*show)(Data data));
int getSize(Queue queue);
void destroyQueue(Queue queue);
#endif
