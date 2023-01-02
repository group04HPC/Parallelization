#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "SubGraph.h"
#include "SCCResult.h"

void send_all(SubGraph *graph, SCCResult *result, int shrink,int dest);
void recv_all(SubGraph **graph, SCCResult **result, int *shrink,int source);

#endif