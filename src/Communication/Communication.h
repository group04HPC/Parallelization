#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"

void send_all(SubGraph *graph, SCCResult *result, int dest);
void recv_all(SubGraph **graph, SCCResult **result, int source);

#endif