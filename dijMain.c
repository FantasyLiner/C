#include "graph.h"
#include "dij.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int main() {
    int ** graph = createGraph(SIZE);
    puts("This is the graph:");
    showGraph(graph, SIZE);

    for (int i = 0; i < SIZE; i++) {
        printf("%d\n", i);
        shortestPathFromStartPoint(i, graph, SIZE);
    }
}
