#include "Merge.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Constants.h"

SCCResult *mergeResults(SCCResult *r1, SCCResult *r2)
{
    SCCResult *result = SCCResultCreate(r1->nMacroNodes + r2->nMacroNodes);
    
    int i = 0;
    for (; i < r1->nV; i++)
        listCopy(*r1->vertices[i], result->vertices[i]);
    for (; i < r1->nV + r2->nV; i++)
        listCopy(*r2->vertices[i - r1->nV], result->vertices[i]);

    result->nMacroNodes = r1->nMacroNodes + r2->nMacroNodes;
    result->offset = r1->offset < r2->offset ? r1->offset : r2->offset;

    return result;
}

// Merges two subgraphs and their SCCResult into a SubGraph
SubGraph *mergeGraphs(SubGraph *g1, SubGraph *g2, int shrink1, int shrink2, SCCResult *merged)
{
    int numEdges = g1->nE - shrink2;

    SubGraph *res = createSubGraph(g1->nV + g2->nV, numEdges, g1->offset/WORK_LOAD);

    int i,j,other, max=SCCResultGetLastElement(merged)+1;
    int diff = (g2->offset-(g1->offset+g1->nV));

    for(i=0; i<g1->nV; i++){
        for(j=0; j<g1->offset+g1->nV; j++){
            if (hasEdge(g1, i, j)){
                addEdge(res, i, j);
            }
        }

        for(; j < max - diff; j++){
            if (hasEdge(g1, i, j)){
                other = getMacronodeFromVertex(merged, j+diff);
                if (other != -1){
                    addEdge(res, i, other);
                }
            }
        }

        for(; j<g1->nE; j++){
            
            if (hasEdge(g1, i, j)){
                addEdge(res, i, j-shrink2);
            }
        }
    }

    for (i=0; i<g2->nV; i++){
        for (j=0; j<g1->offset; j++){
            if (hasEdge(g2, i, j)){
                addEdge(res, i+g1->nV, j);
            }
        }
        for (; j<g2->offset; j++){
            if(hasEdge(g2, i, j)){
                other = getMacronodeFromVertex(merged, j);
                if (other != -1){
                    addEdge(res, i+g1->nV, other);
                }
            }
        }
        for(; j<g2->nE; j++){
            if (hasEdge(g2, i, j)){
                addEdge(res, i+g1->nV, j-diff);
            }
        }
    }

    return res;
}