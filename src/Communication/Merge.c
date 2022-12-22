#include <stdio.h>
#include "Merge.h"

void printBella(SubGraph* g){
    for(int i=0;i<g->nV;i++){
        for(int j=0;j<g->nE;j++){
            if(j==g->offset|| j==g->offset+g->nV)
                printf("| ");
            printf("%d ",g->adj[i*g->nE+j]);
        }
        printf("\n");
    }
}

SCCResult *mergeResults(SCCResult *r1, SCCResult *r2)
{
    SCCResult *result = SCCResultCreate(r1->nV + r2->nV);
    int j = 0;

    for (int i = 0; i < r1->nV; i++, j++)
    {
        result->vertices[j] = r1->vertices[i];
    }
    for (int i = 0; i < r2->nV; i++, j++)
    {
        result->vertices[j] = r2->vertices[i];
    }

    // Now the final result should be tutt'appost
    return result;
}

// Merges two subgraphs and their SCCResult into a SubGraph
SubGraph *mergeGraphs(SubGraph *g1, SubGraph *g2,SCCResult *r1, SCCResult *r2)
{
    // Firstly we reorder the graphs in case that g1 follows g2
    if(g1->offset>g2->offset){
        printf("Switched\n");
        SubGraph* temp=g1;
        g1=g2;
        g2=temp;
    }

    // The result's adjacency matrix should look like this:
    //
    //     \   offset1          V1            V2          remaining
    //      \___________|_______________|_____________|______________
    //      |           |               |             |             |
    //   V1 |           |               |             |             |
    //  ____|___________|_______________|_____________|_____________|
    //      |           |               |             |             |
    //   V2 |           |               |             |             |
    //      |           |               |             |             |
    //      |___________|_______________|_____________|_____________|
    //
    // To obtain this result we can copy for both g1 and g2 the offset1 section,
    //   then copy the V1:V1 square and retrace the edges between g1 and g2, then
    //   we can d the same thing but for the V2:V2 square and at the end we can siply
    //   copy the remeining edges. 
    // Here we'll do all of above splitting the elaboration for the two input graphs
    
    int numEdges = g1->offset + g1->nV + g2->nE - g2->offset,other_node;
    SubGraph *res = createSubGraph(g1->nV + g2->nV, numEdges, g1->offset);
    // printf("Grafo 1:\n");
    // printBella(g1);
    // printSubGraph(g1);
    // printf("Grafo 2:\n");
    // printBella(g2);
    // printSubGraph(g2);

    int reduced_nodes_1= g2->offset-g1->offset-g1->nV;
    //int reduced_nodes_2= g1->nE-g2->nE-2*(g2->offset-g2->nV);
    int reduced_nodes_2 = g1->nE + g2->nV + g2->offset - g1->nV - g1->offset - g2->nE - reduced_nodes_1;
    // printf("Reduced nodes: %d\t%d\n",reduced_nodes_1,reduced_nodes_2);
    // printf("Edges: %d\n",numEdges);
    
    //printf("1~%d\t2~%d\n",reduced_nodes_1,reduced_nodes_2);

    // Graph 1 - Copy of the edges contained in V1:offset1 and V1:V1 
    for (int i = 0; i < g1->nV; i++)
    {
        for (int j = 0; j < g1->offset+g1->nV; j++)
        {
            if (g1->adj[i * g1->nE + j]){
                addEdge(res, i, j);
            }
        }
    }

    // Graph 1 - Retrace of the edges contained V1:V2 edges and copy of the remaining ones
    for (int i = 0; i < g1->nV; i++)
    {
        other_node=0;
        for (int j = g1->offset+g1->nV; j < g1->nE; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we only have to copy the remaining edges

            other_node = getMacronodeFromVertex(r2, j);

            if (other_node != -1)
                addEdge(res, i, other_node);
            else
                addEdge(res, i, j-reduced_nodes_2);
        }
    }
    // Verificare che non serva calcolare uno reduced_nodes fra i due grafi
    //   es: reduced_nodes_1= offset2-offset1-nV1  -> se g1 si è rimpicciolito durante tarjan
    //       reduced_nodes_2= nE1-nE2-2*(off2-v2)

    // Graph 2 - Copy of the edges contained in V2:offset1
    for (int i = 0; i < g2->nV; i++)
    {
        for (int j = 0; j < g1->offset; j++)
        {
            if (g2->adj[i * g1->nE + j])
            {
                addEdge(res, g1->nV+i, j);
            }
        }
    }

    // Graph 2 - Retrace of the edges contained V2:V1 edges and copy of all the remaining ones
    for (int i = 0; i < g2->nV; i++)
    {
        other_node = 0;
        for (int j = g1->offset; j < g2->nE; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we only have to copy the remaining edges

            other_node = getMacronodeFromVertex(r1, j);

            if (other_node != -1)
                addEdge(res, g1->nV + i, other_node);
            else
                addEdge(res, g1->nV + i, j-reduced_nodes_1);
        }
    }

    return res;
}
