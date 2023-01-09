#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/Kosaraju.h"

// A recursive function to print DFS starting from v
void DFSUtil(ListGraph* g, int v, bool* visited, SCCResult* result, int key)
{
    // Mark the current node as visited and print it
    visited[v] = true;
    SCCResultInsert(result, key, v);
 
    // Recur for all the vertices adjacent to this vertex
    TList values = *g->adj[v];
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
    ListGraph* gT = ListGraphCreate(g->nV, g->nE, g->offset);
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
        if (!visited[values->value])
            fillOrder(g, values->value, visited, stack);
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
    // Create a graph given in the above diagram
    SubGraph* g = createSubGraph(7, 7, 0);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 0);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 1, 6);
    addEdge(g, 3, 5);
    addEdge(g, 4, 5);
    ListGraph* list = createListGraphFromMatrix(g);
    printf("Graph: \n");
    printListGraph(list);
    
    printf("\nSCC \n");
    SCCResult* result = SCCResultRescale(SCC(list));
    SCCResultPrint(result);
 
    return 0;
}