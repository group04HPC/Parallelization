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
 * This file contains the implementation of the functions required to execute Kosaraju's algorithm.
 * The provided functions are:
 *      void fillOrder(ListGraph* g, int v, bool* visited, TArray* stack);
 *      void DFSUtil(ListGraph* g, int v, bool* visited, SCCResult* result, int key);
 *      SCCResult* SCC_K(ListGraph** graph);
 *      ListGraph* getTranspose(ListGraph* g);
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/Kosaraju.h"
#include "../../Headers/Tarjan.h"
#include <time.h>

/**
 * Function: DFSUtil
 * -----------------
 * This function is a recursive function that uses the DFS starting from v.
 * 
 * Parameters:
 * g: the graph
 * v: the starting vertex
 * visited: the array of visited vertices
 * result: the result of the algorithm
 * key: the key of the SCC
*/
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

/**
 * Function: getTranspose
 * ----------------------
 * This function trnsposes the graph g.
 * 
 * Parameters:
 * g: the graph
*/
ListGraph* getTranspose(ListGraph* g)
{
    ListGraph* gT = ListGraphCreate(g->nE, g->nV, g->offset);
    for (int v = 0; v < g->nV; v++)
    {
        // Recur for all the vertices adjacent to this vertex
        TList values = *g->adj[v];
        while (values != NULL)
        {
            *gT->adj[values->value] = listInsert(*gT->adj[values->value], v);
            values = values->link;
        }
    
    }

    return gT;
}
 
/**
 * Function: fillOrder
 * -------------------
 * This function fills the stack with the vertices in the order of their finishing times.
 * 
 * Parameters:
 * g: the graph
 * v: the starting vertex
 * visited: the array of visited vertices
 * stack: the stack
*/
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
 
/**
 * Function: SCC_K
 * ---------------
 * This function finds and inserts in a SCCResult all strongly connected components in the graph.
 * 
 * Parameters:
 * graph: the graph
 * 
 * Returns:
 * the result of the algorithm
*/

SCCResult* SCC_K(ListGraph** graph)
{
    ListGraph* g = *graph;
    SCCResult* result = SCCResultCreate(g->nV);
    result->offset = g->offset;
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

    stackDestroy(stack);
    result = SCCResultRescale(result);
    *graph = rescaleGraph(&g, result);

    return result;

}