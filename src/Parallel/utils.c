#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/* Builds a subgraph replacing nodes with the corresponding macronodes returned by the SCC function */
SubGraph* updateSubGraph(SubGraph* g, SCCResult* result, int rank){

    SubGraph* newGraph = createSubGraph(result->nMacroNodes, g->nE, rank);
    initSubGraph(newGraph);

    int startVertex = rank*g->nV, endVertex = rank*g->nV + g->nV;

    for(int i=startVertex; i<endVertex; i++){
        int newI = getMacronodeFromVertex(result, i);
        for(int j=0; j<g->nE; j++){
            if(g->adj[i*g->nE + j] != 0){
                int newJ = getMacronodeFromVertex(result, j);
                addEdge(newGraph, newI, newJ);
            }
        }
    }

    return newGraph;
}