#ifndef MERGE_H
#define MERGE_H

#include "../DataStructures/ListGraph.h"
#include "../DataStructures/SCCResult.h"

SubGraph *mergeGraphs(ListGraph *g1, ListGraph *g2,int shrink1,int shrink2, SCCResult *merged);
SCCResult *mergeResults(SCCResult *r1, SCCResult *r2);

#endif