#ifndef MERGE_H
#define MERGE_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"

ListGraph *mergeGraphs(ListGraph *g1, ListGraph *g2, int shrink1, int shrink2, SCCResult *merged);
SCCResult *mergeResults(SCCResult *r1, SCCResult *r2);

#endif