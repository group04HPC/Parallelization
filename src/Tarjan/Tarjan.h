#ifndef TARJAN_H
#define TARJAN_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/TArray.h"
#include "../DataStructures/TList.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"

void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult* result);
SCCResult *SCC(ListGraph **g);
ListGraph *rescaleGraph(ListGraph **oldGraph, SCCResult *tarjan);

#endif