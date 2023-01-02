#ifndef MERGE_H
#define MERGE_H

#include "SubGraph.h"
#include "SCCResult.h"
#include "ListGraph.h"

ListGraph *mergeGraphs(ListGraph *g1, ListGraph *g2, int shrink1, int shrink2, SCCResult *merged);
SCCResult *mergeResults(SCCResult *r1, SCCResult *r2);

#endif