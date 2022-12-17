#ifndef TARJAN_H
#define TARJAN_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/TArray.h"
#include "../DataStructures/SCCResult.h"

TArray* matrixToArray(SubGraph* g, int i);
void SCCUtil(SubGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult* result);
SCCResult *SCC(SubGraph *g);
SubGraph *rescaleGraph(SubGraph *old, SCCResult *result, int rank);
#endif