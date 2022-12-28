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
    printf("E1: %d E2: %d, shrink1: %d, shrink2: %d\n", g1->nE, g2->nE, shrink1, shrink2);
    // Firstly we reorder the graphs in case that g1 follows g2
    if (g1->offset > g2->offset)
    {
        // printf("Switched\n");
        SubGraph *temp = g1;
        g1 = g2;
        g2 = temp;

        int temp2 = shrink1;
        shrink1 = shrink2;
        shrink2 = temp2;
    }

    // The result's adjacency matrix should look like this:
    //
    //     \   offset1          V1         intra       V2          remaining
    //      \___________|_______________|_________|_____________|______________
    //      |           |               |         |             |             |
    //   V1 |           |               |         |             |             |
    //  ____|___________|_______________|_________|_____________|_____________|
    //      |           |               |         |             |             |
    //   V2 |           |               |         |             |             |
    //      |           |               |         |             |             |
    //      |___________|_______________|_________|_____________|_____________|
    //
    // To obtain this result we can copy for both g1 and g2 the offset1 section,
    //   then copy the V1:V1 square and retrace the edges between g1 and g2, then
    //   we can d the same thing but for the V2:V2 square and at the end we can siply
    //   copy the remeining edges.
    // Here we'll do all of above splitting the elaboration for the two input graphs

    int numEdges = g1->nE - shrink2, other_node;
    SubGraph *res = createSubGraph(g1->nV + g2->nV, numEdges, g1->offset/WORK_LOAD);

    int intra=g2->offset-g1->nV-g1->offset-shrink1;


    // Graph 1 - Copy of the edges contained in V1:offset1 and V1:V1  and V1:intra
    for (int i = 0; i < g1->nV; i++)
    {
        for (int j = 0; j < g1->offset +g1->nV; j++)
        {
            //printf("V1:offset - V1:V1:\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g1->adj[i * g1->nE + j],i,j);
            if (g1->adj[i * g1->nE + j])
            {
                addEdge(res, i, j);
            }
        }

        for (int j = g1->offset + g1->nV; j < g1->offset + g1->nV+intra; j++)
        {
            //printf("V1:intra :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g1->adj[i * g1->nE + j], i, j-shrink2);
            if (g1->adj[i * g1->nE + j])
            {
                addEdge(res, i, j);
            }
        }
    }

    // Graph 1 - Retrace of the edges contained V1:V2 edges and copy of the remaining ones
    //printf("%d - %d\n",g1->offset + g1->nV + intra,g1->nE);
    for (int i = 0; i < g1->nV; i++)
    {
        other_node = 0;
        for (int j = g1->offset + g1->nV + intra; j < g1->nE; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we only have to copy the remaining edges
            other_node = getMacronodeFromVertex(merged, j+g2->offset-g1->nV);
            //printf("get number: %d\n", j+g2->offset-g1->nV);

            if (other_node != -1){
                //printf("V1:V2 :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g1->adj[i * g1->nE + j], i, other_node);
                if (g1->adj[i * g1->nE + j])
                    addEdge(res, i, other_node);
            }else{
                //printf("V1:remaining :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g1->adj[i * g1->nE + j], i, j + g2->offset - g1->nV-shrink1-shrink2);
                if (g1->adj[i * g1->nE + j])
                    addEdge(res, i, j + g2->offset - g1->nV-shrink2-shrink1);
            }
        }
    }

    // Graph 2 - Copy of the edges contained in V2:offset1 and V2:V1
    for (int i = 0; i < g2->nV; i++)
    {
        for (int j = 0; j < g1->offset + g1->nV + shrink1; j++)
        {
            other_node = getMacronodeFromVertex(merged, j);

            if (other_node != -1){
                //printf("V2:V1 :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g2->adj[i * g2->nE + j], g1->nV+i, other_node);
                if (g2->adj[i * g2->nE + j])

                    addEdge(res, g1->nV + i, other_node);
            }
            else{
                //printf("V2:offset :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g2->adj[i * g2->nE + j], g1->nV+i, j );
                if (g2->adj[i * g2->nE + j])

                    addEdge(res, g1->nV + i, j);
            }
        }
    }

    // Graph 2 - Retrace and copy of the edges contained V2:intra and V2:V2 and the remaining ones
    for (int i = 0; i < g2->nV; i++)
    {
        for (int j = g1->offset + g1->nV + shrink1; j < g2->offset+g2->nV; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we only have to copy the remaining edges

            printf("V2:intra-V2:V2 :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g2->adj[i * g2->nE + j], g1->nV + i, j-shrink1);

            if (g2->adj[i * g2->nE + j])
            {
                addEdge(res, g1->nV + i, j - shrink1);
            }
        }

        for (int j = g2->offset + g2->nV; j < g2->nE; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we only have to copy the remaining edges
            
            //printf("V2:remaining :\nda i=%d j=%d val=%d\n a i=%d j=%d\n", i, j, g2->adj[i * g2->nE + j], g1->nV + i, j - shrink1);

            if (g2->adj[i * g2->nE + j])
            {
                addEdge(res, g1->nV + i, j - shrink1);
            }
        }
    }

    return res;
}