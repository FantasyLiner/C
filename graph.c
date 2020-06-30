#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
            else if (rand() % 10 < 7) {
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
