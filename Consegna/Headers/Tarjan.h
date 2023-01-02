#ifndef TARJAN_H
#define TARJAN_H

#include "SubGraph.h"
#include "TArray.h"
#include "TList.h"
#include "SCCResult.h"
#include "ListGraph.h"

void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult* result);
SCCResult *SCC(ListGraph **g);
ListGraph *rescaleGraph(ListGraph **oldGraph, SCCResult *tarjan);

#endif