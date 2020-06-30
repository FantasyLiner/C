#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"
#include "dij.h"

static void initStartPoint(int start, int ** graph, int size, int shortestCostTable[], int previous[], bool alreadyFound[]);
static int findShortestPointNotFound(int size, int shortestCostTable[], int previous[], bool alreadyFound[]);
static void updateTableForNewNode(int ** graph, int newNode, int size, int shortestCostTable[], int previous[], bool alreadyFound[]);
static void showArray(int ar[], int size);


void shortestPathFromStartPoint(int start, int ** graph, int size) {
    int shortestCostTable[size];
    int previous[size];
    bool alreadyFound[size];
    initStartPoint(start, graph, size, shortestCostTable, previous, alreadyFound);

    for (int i = 0; i < size - 1; i++) {
        int newNode = findShortestPointNotFound(size, shortestCostTable, previous, alreadyFound);
        if (newNode == -1)
            break;
        alreadyFound[newNode] = true;
        updateTableForNewNode(graph, newNode, size, shortestCostTable, previous, alreadyFound);
    }
    showArray(shortestCostTable, size);
}

static void initStartPoint(int start, int ** graph, int size, int shortestCostTable[], int previous[], bool alreadyFound[]) {
    for (int i = 0; i < size; i++) {
        shortestCostTable[i] = graph[start][i];
        previous[i] = start;
        alreadyFound[i] = false;
    }
    alreadyFound[start] = true;
}

static int findShortestPointNotFound(int size, int shortestCostTable[], int previous[], bool alreadyFound[]) {
    int shortest = INFINITY;
    int shortestNode = -1;
    for (int i = 0; i < size; i++) {
        if (!alreadyFound[i] && shortestCostTable[i] != INFINITY && (shortest == INFINITY || shortestCostTable[i] < shortest)) {
            shortest = shortestCostTable[i];
            shortestNode = i;
        }
    }
    return shortestNode;
}

static void updateTableForNewNode(int ** graph, int newNode, int size, int shortestCostTable[], int previous[], bool alreadyFound[]) {
    int newNodeCostFromStart = shortestCostTable[newNode];
    for (int i = 0; i < size; i++) {
        if (!alreadyFound[i] && graph[newNode][i] != INFINITY) {
            int newCost = newNodeCostFromStart + graph[newNode][i];
            if (shortestCostTable[i] == INFINITY || shortestCostTable[i] > newCost) {
                shortestCostTable[i] = newCost;
                previous[i] = newNode;
            }
        }
    }
}

static void showArray(int ar[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%5d", ar[i]);
    }
    putchar('\n');
}
