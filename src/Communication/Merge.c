#include "Merge.h"
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"

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
    // Reorders the graphs in case that g1 comes after g2
    if(g1->offset>g2->offset){
        printf("Switched\n");
        SubGraph* temp=g1;
        g1=g2;
        g2=temp;
    }
    int numEdges=g1->nE;
    if(g1->nE>g2->nE)
        numEdges=g2->nE;

    SubGraph *res = createSubGraph(g1->nV + g2->nV, numEdges, g1->offset);

    //printf("%d %d %d\n",res->nE,res->nV,res->offset);
    // The first part of both adjacences matrixes should be copied as
    //   it is because Tarjan's algo only worked in the midddle piece of the subgraph

    int *matrix=g1->adj;
    for (int i = 0; i < res->nV; i++)
    {
        for (int j = 0; j < g1->offset; j++)
        {
            if (i < g1->nV && g1->adj[i * g1->nE + j]){
                addEdge(res, i, j);
            }

            if (i >= g1->nV && g2->adj[(i - g1->nV) * g2->nE + j])
            {
                addEdge(res, i, j);
            }
        }
    }

    // Now we have to merge the modified part of the graph
    
    int other_node;
    for (int i = 0; i < g1->nV; i++)
    {
        other_node = 0;
        for (int j = g1->offset; j < g1->nE; j++)
        {   
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed, 
            //   so we can avoid recalling that function
            if(other_node!=-1)
                other_node = getMacronodeFromVertex(r2, j);
            
            if (other_node!=-1)
                addEdge(res, i, other_node);
            else
                addEdge(res, i, j);
        }
    }

    for (int i = 0; i < g2->nV; i++)
    {
        other_node = 0;
        for (int j = g1->offset; j < g2->nE; j++)
        {
            // If the previous result of getMacronodeFromVertex is -1 it means that
            //   we've surpassed the part in which tarjan has been executed,
            //   so we can avoid recalling that function
            if (other_node != -1)
                other_node = getMacronodeFromVertex(r1, j);

            if (other_node != -1)
                addEdge(res, i, other_node);
            else
                addEdge(res, i, j);
        }
    }

    return res;
}
