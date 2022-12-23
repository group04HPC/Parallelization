#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/TList.h"
#include "Tarjan.h"

#define NIL -1
#define TRUE 1
#define FALSE 0

/**
 * Convert a matrix row into an array 
 */
TArray* matrixToArray(SubGraph* g, int i){
    TArray* array = stackCreate(g->nE);
    for (int j = g->offset; j < g->offset+g->nV; j++)
        if (g->adj[(i-g->offset)*g->nE+j] != 0)
            stackPush(array, j);
    return array;
}

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
void SCCUtil(SubGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult *result){
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
        stackDestroy(values);
        return;
    }
}

/**
 * The function to do DFS traversal, by means of the SCCUtil().
 */
SCCResult* SCC(SubGraph *g){

    int V = g->nV, disc[g->nE], low[g->nE], stackMember[g->nE];
    TArray *st = stackCreate(V);

    /* Initialize disc and low, and stackMember arrays */
    for (int i = 0; i < g->nE; i++){
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = FALSE;
    }

    SCCResult* result = SCCResultCreate(V);

    /* Call the recursive helper function to find strongly connected components in DFS tree with vertex 'i' */
    for (int i = g->offset; i < g->offset+V; i++)
        if (disc[i] == NIL)
            SCCUtil(g, i, disc, low, st, stackMember, result);

    stackDestroy(st);
    return result;
}

/**
 *  Rescales the graph, merging all the SCC in a macronode 
 */
SubGraph *rescaleGraph(SubGraph *old, SCCResult *result, int rank){

    int mergedNodes = old->nV - result->nV, numEdges = old->nE;
    SubGraph* new = createSubGraph(result->nV, numEdges-mergedNodes, rank);
    
    for(int i=0; i<result->nV; i++){

        TList *curr = getVerticesFromMacronode(result, i);

        int *nodes = listToArray(*curr),
            length = listCount(*curr);

        for(int j=0; j< length; j++){
            int *edges = getEdges(old,nodes[j]);

            /* Scaling nodes that come before the scc part */
            for(int k=0;k<old->offset;k++)
                if(edges[k]) addEdge(new,i,k);

            /* Scaling nodes that are inside the scc part */
            for(int k=old->offset; k<old->offset+old->nV; k++)
                /* Non facciamo leggere questo ad Auletta altrimenti si spara */
                if (edges[k]) addEdge(new, i, getMacronodeFromVertex(result, k));

            /* Scaling nodes that come after the scc part */
            for (int k=old->offset + old->nV; k<old->nE; k++)
                if (edges[k])addEdge(new, i, k-mergedNodes);
        }
        
    }
    return new;
}

/**
 *  Retraces the SCCResult, including in each macronode its internal nodes
 */
SCCResult *retraceResult(SCCResult *res,SCCResult* original, int rank){

    if (rank == 5){
        printf("Sono dentro\n");
        SCCResultPrint(res);
        SCCResultPrint(original);
    }

    SCCResult *result=SCCResultCreate(res->nV);

    if (rank == 5){
        printf("ho creato il result\n");
    }

    for(int i=0;i<res->nV;i++){
        if (rank == 5) printf("i = %d\n",i);
        TList curr = *res->vertices[i];
        if (rank == 5) printf("ho preso la lista\n");
        while(curr!=NULL){
            if (rank == 5) printf("curr->value = %d\n",curr->value);
            listCopy(*original->vertices[curr->value],result->vertices[i]);
            if (rank == 5) printf("ho copiato\n");
            curr = curr->link;
            if (rank == 5) printf("ho fatto il passo\n");
        }
        if (rank == 5) printf("ho finito la lista\n");
    }

    return result;
}