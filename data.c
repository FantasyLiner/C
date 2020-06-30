#include "data.h"
#include <stdlib.h>
#include <stdio.h>

Data createData(int id, int * cost) {
    Data data = (Data) malloc(sizeof(*data));
    data->id = id;
    data->minimalCostFromId = cost;
    return data;
}

void destroyData(Data data) {
    free(data->minimalCostFromId);
    free(data);
}

void showData(Data data) {
    printf("id = %-5d\n", data->id);
}
