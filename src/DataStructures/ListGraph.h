#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "SubGraph.h"
#include "TList.h"

typedef struct listGraph {
    int offset; /* Starting vertex of the subgraph */
    int nV; /* Number of vertices */
    int nE; /* Number of edges */
    TList** adj; /* Adjacency list */
} ListGraph;

ListGraph* createListGraphFromMatrix(SubGraph* sub); /* Creates a list graph from a matrix graph */
SubGraph* createMatrixGraphFromList(ListGraph* list); /* Creates a matrix graph from a list graph */
void printListGraph(ListGraph* graph); /* Prints a list graph */

#endif