#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/Kosaraju.h"

// A recursive function to print DFS starting from v
void DFSUtil(ListGraph* g, int v, bool* visited, SCCResult* result, int key)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    SCCResultInsert(result, key, v+g->offset);
 
    // Recur for all the vertices adjacent to this vertex
    TList values = *g->adj[v+g->offset];
    while (values != NULL)
    {
        if (!visited[values->value]){
            DFSUtil(g, values->value, visited, result, key);
        }
        values = values->link;
    }
}
 
ListGraph* getTranspose(ListGraph* g)
{
    ListGraph* gT = ListGraphCreate(g->nE, g->nE, g->offset);
    TNode *prev = NULL, *succ = NULL; 
    for (int v = 0; v < g->nV; v++)
    {
        // Recur for all the vertices adjacent to this vertex
        TList values = *g->adj[v];
        while (values != NULL)
        {
            *gT->adj[values->value] = listInsert(*gT->adj[values->value], v);
            prev = values;
            values = values->link;
        }
    
    }

    destroyListGraph(g);

    return gT;
}
 
void fillOrder(ListGraph* g, int v, bool* visited, TArray* stack)
{
    // Mark the current node as visited and print it
    visited[v] = true;
 
    // Recur for all the vertices adjacent to this vertex
    TList values = *g->adj[v];
    
    while (values != NULL)
    {
        if (!visited[values->value-g->offset] && values->value >= g->offset && values->value < g->offset + g->nV)
            fillOrder(g, values->value-g->offset, visited, stack);
        values = values->link;
    }
 
    // All vertices reachable from v are processed by now, push v
    stackPush(stack, v);
}
 
// The main function that finds and prints all strongly connected components
SCCResult* SCC(ListGraph* g)
{
    SCCResult* result = SCCResultCreate(g->nV);

    TArray* stack = stackCreate(g->nV);
 
    // Mark all the vertices as not visited (For first DFS)
    bool *visited = (bool*)malloc(g->nV * sizeof(bool));
    for(int i = 0; i < g->nV; i++)
        visited[i] = false;
 
    // Fill vertices in stack according to their finishing times
    for(int i = 0; i < g->nV; i++)
        if(visited[i] == false)
            fillOrder(g, i, visited, stack);
 
    // Create a reversed graph
    ListGraph* gr = getTranspose(g);
 
    // Mark all the vertices as not visited (For second DFS)
    for(int i = 0; i < g->nV; i++)
        visited[i] = false;
 
    // Now process all vertices in order defined by Stack
    int i=0;
    while (!stackIsEmpty(stack))
    {
        // Pop a vertex from stack
        int v = stackPop(stack);

        // Print Strongly connected component of the popped vertex
        if (visited[v] == false)
        {
            DFSUtil(gr, v, visited, result, i);
        }
        i++;
    }

    return result;

}
 
// Driver program to test above functions
int main()
{
    double conversion_time = 0.0;
    // Test for the parallel version

    /* Simulating tarjan with two processes */
    // SubGraph* g6 = createSubGraph(10, 10, 0);
    // addEdge(g6, 0, 1);
    // addEdge(g6, 0, 3);
    // addEdge(g6, 1, 2);
    // addEdge(g6, 1, 4);
    // addEdge(g6, 2, 0);
    // addEdge(g6, 2, 6);
    // addEdge(g6, 3, 2);
    // addEdge(g6, 4, 5);
    // addEdge(g6, 4, 6);
    // addEdge(g6, 5, 6);
    // addEdge(g6, 5, 7);
    // addEdge(g6, 5, 8);
    // addEdge(g6, 5, 9);
    // addEdge(g6, 6, 4);
    // addEdge(g6, 7, 9);
    // addEdge(g6, 8, 9);
    // addEdge(g6, 9, 8);
    // printf("\nOriginal Graph:\n");
    // printSubGraph(g6);
    // ListGraph* list6 = createListGraphFromMatrix(g6);
    // printf("\nSCC:\n");
    // SCCResult* result6 = SCCResultRescale(SCC(list6));
    // SCCResultPrint(result6);
    // destroySubGraph(g6);
    // SCCResultDestroy(result6);
    // destroyListGraph(list6);

    /* 1. Simulating the subgraph created by rank 0 */
    SubGraph* g7 = createSubGraph(5, 10, 0);
    addEdge(g7, 0, 1);
    addEdge(g7, 0, 3);
    addEdge(g7, 1, 2);
    addEdge(g7, 1, 4);
    addEdge(g7, 2, 0);
    addEdge(g7, 2, 6);
    addEdge(g7, 3, 2);
    addEdge(g7, 4, 5);
    addEdge(g7, 4, 6);
    ListGraph* list7 = createListGraphFromMatrix(g7);
    printf("\nSCC:\n");
    SCCResult* result7 = SCCResultRescale(SCC(list7));
    SCCResultPrint(result7);
    // destroySubGraph(g7);
    // SCCResultDestroy(result7);
    // destroyListGraph(list7);

    /* 2. Simulating the subgraph created by rank 1 */
    SubGraph* g8 = createSubGraph(5, 10, 1);
    addEdge(g8, 0, 6);
    addEdge(g8, 0, 7);
    addEdge(g8, 0, 8);
    addEdge(g8, 0, 9);
    addEdge(g8, 1, 4);
    addEdge(g8, 2, 9);
    addEdge(g8, 3, 9);
    addEdge(g8, 4, 8);
    ListGraph* list8 = createListGraphFromMatrix(g8);
    printf("\nSCC:\n");
    SCCResult* result8 = SCCResultRescale(SCC(list8));
    SCCResultPrint(result8);
    // destroySubGraph(g8);
    // SCCResultDestroy(result8);
    // destroyListGraph(list8);

    // /* Simulating Tarjan with three processes */
    SubGraph* g9 = createSubGraph(15, 15, 0);
    addEdge(g9, 0, 1);
    addEdge(g9, 0, 3);
    addEdge(g9, 1, 2);
    addEdge(g9, 1, 4);
    addEdge(g9, 2, 0);
    addEdge(g9, 2, 6);
    addEdge(g9, 3, 2);
    addEdge(g9, 4, 5);
    addEdge(g9, 4, 6);
    addEdge(g9, 5, 6);
    addEdge(g9, 5, 7);
    addEdge(g9, 5, 8);
    addEdge(g9, 5, 9);
    addEdge(g9, 6, 4);
    addEdge(g9, 7, 9);
    addEdge(g9, 8, 9);
    addEdge(g9, 9, 8);
    addEdge(g9, 10, 11);
    addEdge(g9, 10, 13);
    addEdge(g9, 11, 12);
    addEdge(g9, 11, 14);
    addEdge(g9, 12, 10);
    addEdge(g9, 12, 16);
    addEdge(g9, 13, 12);
    addEdge(g9, 14, 15);
    addEdge(g9, 14, 16);
    ListGraph* list9 = createListGraphFromMatrix(g9);
    printf("\nSCC:\n");
    SCCResult* result9 = SCCResultRescale(SCC(list9));
    SCCResultPrint(result9);
    destroySubGraph(g9);
    SCCResultDestroy(result9);
    // destroyListGraph(list9);

    // /* 1. Simulating the subgraph created by rank 0 */
    SubGraph* g10 = createSubGraph(5, 15, 0);
    addEdge(g10, 0, 1);
    addEdge(g10, 0, 3);
    addEdge(g10, 1, 2);
    addEdge(g10, 1, 4);
    addEdge(g10, 2, 0);
    addEdge(g10, 2, 6);
    addEdge(g10, 3, 2);
    addEdge(g10, 4, 5);
    addEdge(g10, 4, 6);
    ListGraph* list10 = createListGraphFromMatrix(g10);
    printf("\nSCC:\n");
    SCCResult* result10 = SCCResultRescale(SCC(list10));
    SCCResultPrint(result10);
    // destroySubGraph(g10);
    // SCCResultDestroy(result10);
    // destroyListGraph(list10);

    // /* 2. Simulating the subgraph created by rank 1 */
    SubGraph* g11 = createSubGraph(5, 15, 1);
    addEdge(g11, 0, 6);
    addEdge(g11, 0, 7);
    addEdge(g11, 0, 8);
    addEdge(g11, 0, 9);
    addEdge(g11, 1, 4);
    addEdge(g11, 2, 9);
    addEdge(g11, 3, 9);
    addEdge(g11, 4, 8);
    ListGraph* list11 = createListGraphFromMatrix(g11);
    printf("\nSCC:\n");
    SCCResult* result11 = SCCResultRescale(SCC(list11));
    SCCResultPrint(result11);
    // printf("\nRescaled Graph:\n");
    // printListGraph(list11);
    // destroySubGraph(g11);
    // SCCResultDestroy(result11);
    // destroyListGraph(list11);

    // /* 3. Simulating the subgraph created by rank 2 */
    SubGraph* g12 = createSubGraph(5, 15, 2);
    addEdge(g12, 0, 11);
    addEdge(g12, 0, 13);
    addEdge(g12, 1, 12);
    addEdge(g12, 1, 14);
    addEdge(g12, 2, 10);
    addEdge(g12, 2, 16);
    addEdge(g12, 3, 12);
    addEdge(g12, 4, 15);
    addEdge(g12, 4, 16);
    ListGraph* list12 = createListGraphFromMatrix(g12);
    printf("\nSCC:\n");
    SCCResult* result12 = SCCResultRescale(SCC(list12));
    SCCResultPrint(result12);
    // printf("\nRescaled Graph:\n");
    // printListGraph(list12);
    // destroySubGraph(g12);
    // destroyListGraph(list12);

    return 0;
}