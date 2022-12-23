#include <stdio.h>
#include "../DataStructures/TList.h"
#include "../Constants.h"
#include "Merge.h"

SCCResult *mergeResults(SCCResult *r1, SCCResult *r2)
{
    SCCResult* result = SCCResultCreate(r1->nMacroNodes + r2->nMacroNodes);

    int i=0; 

    for (; i<r1->nV; i++)
        listCopy(*r1->vertices[i], result->vertices[i]);

    for (; i<r1->nV+r2->nV; i++)
        listCopy(*r2->vertices[i-r1->nV], result->vertices[i]);

    result->nMacroNodes = r1->nMacroNodes + r2->nMacroNodes;

    return result;
}

// Merges two subgraphs and their SCCResult into a SubGraph
SubGraph *mergeGraphs(SubGraph *g1, SubGraph *g2, SCCResult *r1, SCCResult *r2, SCCResult *merged)
{
    /* offset of the new merged graph */
    int offset = (g1->offset < g2->offset) ? g1->offset : g2->offset, nV = merged->nV, nE = merged->nV;

    SubGraph *g = createSubGraph(nV, nE, offset);

    /* transfer the first adj matrix values into the new graph */
    for (int i = g1->offset; i < g1->offset+g1->nV; i++)
        for (int j = 0; j < g1->nE; j++)
            if (g1->adj[(i-g1->offset) * g1->nE + j] == 1){
                int newI = getMacronodeFromVertex(merged,i),
                newJ = getMacronodeFromVertex(merged,j);
                if (g->adj[newI * g->nE + newJ] != 1) g->adj[newI * g->nE + newJ] = 1;
            }

    /* transfer the second adj matrix values into the new graph */
    for (int i = g2->offset; i < g2->offset+g2->nV; i++)
        for (int j = 0; j < g2->nE; j++)
            if (g2->adj[(i-g2->offset) * g2->nE + j] == 1){
                int newI = getMacronodeFromVertex(merged,i),
                newJ = getMacronodeFromVertex(merged,j);
                if (g->adj[newI * g->nE + newJ] != 1) g->adj[newI * g->nE + newJ] = 1;
            }

    return g;
    
}
