#ifndef COMMUNICATION_H
#define COMMUNICATION_H

void send_all(SubGraph *graph, SCCResult *result, int dest);
void recv_all(SubGraph **graph, SCCResult **result, int source);

#endif