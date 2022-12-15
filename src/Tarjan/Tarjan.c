#include <stdio.h>
#include <stdlib.h>
#include "Tarjan.h"

#define NIL -1
#define TRUE 1
#define FALSE 0

/**
 * Convert a matrix row into an array 
 */
TArray* matrixToArray(SubGraph* g, int i){
    TArray* array = stackCreate(g->nE);
    for (int j = 0; j < g->nE; j++) {
        if (g->adj[i*g->nE+j] != 0){
            stackPush(array, j);
        }
    }
    return array;
}

/**
 * A recursive function that finds and prints strongly connected components using DFS traversal.
 * g -> the SuubGraph
 * u --> The vertex to be visited next
 * disc[] --> Stores discovery times of visited vertices
 * low[] -- >> earliest visited vertex (the vertex with minimum discovery time) that can be reached from subtree rooted with current vertex
 * *st -- >> To store all the connected ancestors (could be part of SCC)
 * stackMember[] --> bit/index array for faster check whether a node is in stack
 * *result --> the resulting SCCs
 */
void SCCUtil(SubGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult *result){
    /* A static variable is used for simplicity, we can avoid the use of static variable by passing a pointer.*/
    static int time = 0;

    /* Initialize discovery time and low value */
    disc[u] = low[u] = ++time;
    stackPush(st, u);
    stackMember[u] = TRUE;
    
    /* Go through all vertices adjacent to this */
    TArray *values = matrixToArray(g, u);

    for (int i = 0; i < values->size; ++i){
        int v = arrayGet(values, i); /* v is the current adjacent of 'u' */

        /* If v is not visited yet, then recur for it */
        if (disc[v] == -1){
            SCCUtil(g, v, disc, low, st, stackMember,result);
            /* 
             * Check if the subtree rooted with 'v' has a connection to one of the ancestors of 'u'
             * Case 1 (per above discussion on Disc and Low value) 
             */
            low[u] = (low[u] < low[v]) ? low[u] : low[v];
        }
        /* 
         * Update low value of 'u' only of 'v' is still in stack (i.e. it's a back edge, not cross edge). 
         * Case 2 (per above discussion on Disc and Low value)
         */
        else if (stackMember[v] == TRUE)
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
    }

    /* Head node found, pop the stack and print an SCC */
    int w = 0; /* To store stack extracted vertices */
    if (low[u] == disc[u]){
        while (stackTop(st) != u){
            w = (int)stackPop(st);
            SCCResultInsert(result, u, w);
            stackMember[w] = FALSE;
        }
        w = (int)stackPop(st);
        SCCResultInsert(result, u, w);
        stackMember[w] = FALSE;
        return;
    }
}

/**
 * The function to do DFS traversal, by means of the SCCUtil().
 */
SCCResult* SCC(SubGraph *g){
    int V = g->nV, disc[V], low[V], stackMember[V];
    TArray *st = stackCreate(V);

    /* Initialize disc and low, and stackMember arrays */
    for (int i = 0; i < V; i++){
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = FALSE;
    }

    SCCResult* result = SCCResultCreate(V);

    /* Call the recursive helper function to find strongly connected components in DFS tree with vertex 'i' */
    for (int i = 0; i < V; i++)
        if (disc[i] == NIL)
            SCCUtil(g, i, disc, low, st, stackMember, result);

    return result;
}