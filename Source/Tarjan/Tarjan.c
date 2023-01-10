/*
 * Course: High Performance Computing 2022/2023
 *
 * Lecturer: Francesco Moscato    fmoscato@unisa.it
 *
 * Group:
 * Ferrara Grazia   0622701901  g.ferrara75@studenti.unisa.it
 * Franco Paolo     0622701993  p.franco9@studenti.unisa.it
 *
 * Copyright (C) 2023 - All Rights Reserved
 *
 * This file is part of Project Assignment 2022/2023.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Project Assignment 2022/2023.  If not, see http://www.gnu.org/licenses/.
 *
 * Requirements of the assignment:
 * Provide a parallell version of the Tarjan's algorithm to find Strongly Connected Components in a Graph.
 * The implementation MUST use a message passing paradigm, and has to be implemented by using MPI.
 * Students MUST store and load the input graph FROM FILES. The whole graph MUST be distributed on files
 * on each node (i.e.: the whole graph cannot be stored on a single (even replicated) file). Good Graph
 * dimensions are greater than 4GB of data. Students have to choose the proper data structure to
 * represent the graph in memory.
 *
 * Purpose of the file:
 * This file contains the implementation of the functions required to execute Tarjan's algorithm.
 * The provided functions are:
 *      void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult* result);
 *      SCCResult *SCC(ListGraph **g);
 *      ListGraph *rescaleGraph(ListGraph **oldGraph, SCCResult *tarjan);
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../Headers/TList.h"
#include "../../Headers/Tarjan.h"
#include "../../Headers/Constants.h"

#define NIL -1
#define TRUE 1
#define FALSE 0

/*
 * Function:  SCCUtil
 * --------------------
 *  Locally finds all the strongly connected components using DFS traversa, inserting them in a SCCResult structure.
 *
 *  Parameters:
 *  g: the ListGraph to analyze
 *  u: the vertex to visit in this execution
 *  disc: an array storing the discovery times of the visited vertices
 *  low: an array storing the vertices with minimum discovery time
 *  stackMember: an array used to fastly check whether a node is in stack
 *  st: a stack used to store all the connected ancestors
 *  result: the resulting SCCResult structure
 */
void SCCUtil(ListGraph *g, int u, int disc[], int low[], TArray *st, int stackMember[], SCCResult *result)
{

    /* A static variable is used for simplicity, we can avoid the use of static variable by passing a pointer.*/
    static int time = 0;

    if (u < g->offset || u >= g->offset + g->nV)
    {
        disc[u] = NIL;
        low[u] = NIL;
        return;
    }

    /* Initialize discovery time and low value */
    disc[u] = low[u] = ++time;
    stackPush(st, u);
    stackMember[u] = TRUE;

    /* Go through all vertices adjacent to this */
    TList values = *g->adj[u - g->offset];

    while(values!=NULL)
    {

        int v = values->value; /* v is the current adjacent of 'u' */

        if (v >= g->offset && v < g->offset + g->nV)
        {
            /* If v is not visited yet, then recur for it */
            if (disc[v] == -1)
            {
                SCCUtil(g, v, disc, low, st, stackMember, result);
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
        values = values->link;
    }

    /* Head node found, pop the stack and print an SCC */
    int w = g->offset; /* To store stack extracted vertices */
    if (low[u] == disc[u])
    {
        while (stackTop(st) != u)
        {
            w = (int)stackPop(st);
            SCCResultInsert(result, u - g->offset, w);
            stackMember[w] = FALSE;
        }
        w = (int)stackPop(st);
        SCCResultInsert(result, u - g->offset, w);
        stackMember[w] = FALSE;
    }
}

/*
 * Function:  SCC
 * --------------------
 *  Finds all the strongly connected components invoking multiple times the SCCUtil function.
 *
 *  Parameters:
 *  graph: the ListGraph to analyze
 *
 *  Returns:
 *  resultt: the resulting SCCResult structure
 */

SCCResult *SCC(ListGraph **graph)
{

    ListGraph *g = *graph;
    int V = g->nV, disc[g->nE], low[g->nE], stackMember[g->nE];
    TArray *st = stackCreate(V);

    /* Initialize disc and low, and stackMember arrays */
    for (int i = 0; i < g->nE; i++)
    {
        disc[i] = NIL;
        low[i] = NIL;
        stackMember[i] = FALSE;
    }

    SCCResult *result = SCCResultCreate(V);
    result->offset = g->offset;

    /* Call the recursive helper function to find strongly connected components in DFS tree with vertex 'i' */
    for (int i = g->offset; i < g->offset + V; i++)
    {

        if (disc[i] == NIL)
            SCCUtil(g, i, disc, low, st, stackMember, result);
    }

    stackDestroy(st);
    result = SCCResultRescale(result);
    *graph = rescaleGraph(&g, result);

    return result;
}

/*
 * Function:  rescaleGraph
 * --------------------
 *  Rescales the graph, merging all the SCC in a macronode and destroying the old one.
 *
 *  Parameters:
 *  oldGraph: the ListGraph to rescale
 *  tarjan: the SCCResult to use during the rescale process
 *
 *  Returns:
 *  new: the rescaled ListGraph structure
 *
 */
ListGraph *rescaleGraph(ListGraph **oldGraph, SCCResult *tarjan)
{

    ListGraph *old = *oldGraph;
    ListGraph *new = ListGraphCreate(tarjan->nV, old->nE, old->offset);
    int corr[old->nV];

    int count = 0;
    for (int i = 0; i < tarjan->nV; i++)
    {
        TList list = *tarjan->vertices[i];
        while (list != NULL)
        {
            corr[list->value - old->offset] = i;
            list = list->link;
            count++;
        }
    }

    for (int i = 0; i < old->nV; i++)
    {
        TList list = *old->adj[i];
        while (list != NULL)
        {

            if (list->value >= old->offset && list->value < old->offset + old->nV)
            {
                insertListGraph(new, corr[i], corr[list->value - old->offset] + old->offset);
            }
            else
            {
                insertListGraph(new, corr[i], list->value);
            }

            list = list->link;
        }
    }

    destroyListGraph(old);

    return new;
}
