#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

static Node createNode(Data data);

Queue createQueue() {
    Queue queue = (Queue) malloc(sizeof(struct queue));
    initQueue(queue);
    return queue;
}

int getSize(Queue queue) {
    return queue->size;
}

void initQueue(Queue queue) {
    queue->size = 0;
    queue->first = NULL;
    pthread_mutex_t * lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    queue->lock = lock;
    pthread_mutex_init(lock, NULL);
}

void appendToQueue(Queue queue, Data data) {
    pthread_mutex_lock(queue->lock);
    Node * pnode = &(queue->first);
    while (*pnode != NULL) {
        pnode = &((*pnode)->next);
    }
    *pnode = createNode(data);
    (*pnode)->next = NULL;
    queue->size++;
    pthread_mutex_unlock(queue->lock);
}

Data popFromQueue(Queue queue) {
    pthread_mutex_lock(queue->lock);
    if (queueIsEmpty(queue)) {
        printf("Queue is EMPTY! Nothing to pop!\n");
        pthread_mutex_unlock(queue->lock);
        return NULL;
    }

    Node node = queue->first;
    Data data = node->data;
    queue->first = node->next;
    free(node);
    queue->size--;
    pthread_mutex_unlock(queue->lock);
    return data;
}

bool queueIsEmpty(Queue queue) {
    return queue->size == 0;
}

static Node createNode(Data data) {
    Node newNode = (Node)malloc(sizeof(struct node));
    newNode->data = data;
    return newNode;
}

void traverse(Queue queue, void (*show)(Data data)) {
    for (Node node = queue->first; node != NULL; node = node->next) {
        show(node->data);
    }
}

void destroyQueue(Queue queue) {
    while (!queueIsEmpty(queue)) {
        destroyData(popFromQueue(queue));
    }
    pthread_mutex_destroy(queue->lock);
    free(queue);
}
