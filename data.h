#ifndef __DATA_H__
#define __DATA_H__
struct rtpkt {
    int id;
    int * minimalCostFromId;
};

typedef struct rtpkt * Data;

Data createData(int id, int * cost);
void destroyData(Data data);
void showData(Data data);
#endif
