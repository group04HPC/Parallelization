#ifndef TARJAN_H
#define TARJAN_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/TArray.h"
#include "../DataStructures/TList.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"

TArray* listToArray(ListGraph* g, int i);
void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult* result);
SCCResult *SCC(ListGraph *g);
SubGraph *rescaleGraph(SubGraph *old, SCCResult *result, SCCResult *result2, int flag);

#endif