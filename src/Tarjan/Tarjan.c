#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/TList.h"
#include "Tarjan.h"
#include "../Constants.h"

#define NIL -1
#define TRUE 1
#define FALSE 0

/**
 * A recursive function that finds and prints strongly connected components using DFS traversal.
 * g -> the SubGraph
 * u --> The vertex to be visited next
 * disc[] --> Stores discovery times of visited vertices
 * low[] -- >> earliest visited vertex (the vertex with minimum discovery time) that can be reached from subtree rooted with current vertex
 * *st -- >> To store all the connected ancestors (could be part of SCC)
 * stackMember[] --> bit/index array for faster check whether a node is in stack
 * *result --> the resulting SCCs
 */
void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult *result){
    
    /* A static variable is used for simplicity, we can avoid the use of static variable by passing a pointer.*/
    static int time = 0;
    
    if(u < g->offset || u >= g->offset+g->nV){
        disc[u] = NIL;
        low[u] = NIL;
        printf("Error: Vertex %d is out of bounds\n", u);
        return;
    }
        
    /* Initialize discovery time and low value */
    disc[u] = low[u] = ++time;
    stackPush(st, u);
    stackMember[u] = TRUE;
    
    /* Go through all vertices adjacent to this */
    TList values = *g->adj[u-g->offset];

    for (int i = 0; i < listCount(values); ++i){

        int v = listGet(values, i); /* v is the current adjacent of 'u' */

        if (v >= g->offset && v < g->offset+g->nV){
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

    }

    /* Head node found, pop the stack and print an SCC */
    int w = g->offset; /* To store stack extracted vertices */
    if (low[u] == disc[u]){
        while (stackTop(st) != u){
            w = (int)stackPop(st);
            SCCResultInsert(result, u-g->offset, w);
            stackMember[w] = FALSE;
        }
        w = (int)stackPop(st);
        SCCResultInsert(result, u-g->offset, w);
        stackMember[w] = FALSE;
    }
}

/**
 * The function to do DFS traversal, by means of the SCCUtil().
 */
SCCResult* SCC(ListGraph** graph){

    ListGraph* g = *graph;
    int V = g->nV, disc[g->nE], low[g->nE], stackMember[g->nE];
    TArray *st = stackCreate(V);

    /* Initialize disc and low, and stackMember arrays */
    for (int i = 0; i < g->nE; i++){
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = FALSE;
    }

    SCCResult* result = SCCResultCreate(V);
    result->offset = g->offset;

    /* Call the recursive helper function to find strongly connected components in DFS tree with vertex 'i' */
    for (int i = g->offset; i < g->offset+V; i++){

        if (disc[i] == NIL)
            SCCUtil(g, i, disc, low, st, stackMember, result);
    }

    stackDestroy(st);

    result = SCCResultRescale(result);
 
    *graph = rescaleGraph(&g, result);

    return result;
}

/**
 *  Rescales the graph, merging all the SCC in a macronode and destroys the old one.
 */
ListGraph *rescaleGraph(ListGraph **oldGraph, SCCResult *tarjan){

    ListGraph *old = *oldGraph;
    ListGraph *new = ListGraphCreate(tarjan->nV, old->nE - (old->nV-tarjan->nV), old->offset);

    int corr[old->nV];

    for (int i=0; i<tarjan->nV; i++){
        TList list = *tarjan->vertices[i];
        while (list != NULL){
            corr[list->value-old->offset] = i;
            list = list->link;
        }
    }

    for (int i=0; i<old->nV; i++){

        TList list = *old->adj[i];
                
        while (list != NULL){
            if (list->value >= old->offset && list->value < old->offset+old->nV)
                insertListGraph(new, corr[i], corr[list->value-old->offset]+old->offset);
            else
                insertListGraph(new, corr[i], list->value);
            list = list->link;
        }
    }

    // se non da segmentation fault fai la destroy
    destroyListGraph(old);

    return new;
}
