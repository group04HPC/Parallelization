#ifndef MERGE_H
#define MERGE_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"

SubGraph *mergeGraphs(SubGraph *g1, SubGraph *g2,int shrink1,int shrink2, SCCResult *merged);
SCCResult *mergeResults(SCCResult *r1, SCCResult *r2);

#endif