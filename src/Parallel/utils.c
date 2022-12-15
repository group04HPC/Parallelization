#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Builds a subgraph replacing nodes with the corresponding macronodes returned by the SCC function */
SubGraph* updateGraph(SubGraph* g, SCCResult* result, int rank){

    SubGraph* newGraph = createSubGraph(result->nMacroNodes, g->nE, rank);
    int startVertex = rank*g->nV, endVertex = rank*g->nV + g->nV;

    for(int i=startVertex; i<endVertex; i++){
        
    }

    return newGraph;
}