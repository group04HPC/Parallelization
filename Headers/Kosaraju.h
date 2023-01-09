#ifndef KOSARAJU_H
#define KOSARAJU_H

#include "SubGraph.h"
#include "TArray.h"
#include "TList.h"
#include "SCCResult.h"
#include "ListGraph.h"

// Fills Stack with vertices (in increasing order of finishing times). The top element of stack has the maximum finishing time
void fillOrder(ListGraph* g, int v, bool* visited, TArray* stack);

// A recursive function to print DFS starting from v
void DFSUtil(ListGraph* g, int v, bool* visited, SCCResult* result, int key);

// The main function that finds and prints strongly connected
// components
SCCResult* SCC(ListGraph* g);

// Function that returns reverse (or transpose) of this graph
ListGraph* getTranspose(ListGraph* g);

#endif