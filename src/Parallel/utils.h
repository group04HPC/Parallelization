#ifndef UTILS_H
#define UTILS_H

#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"

SubGraph* updateGraph(SubGraph* g, SCCResult* result, int rank); /* Builds a subgraph replacing nodes with the corresponding macronodes returned by the SCC function */

#endif