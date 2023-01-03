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
 * This file contains the implementation of the SubGraph data structure. 
 * A SubGraph is a graph that is stored in a single node. 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../../Headers/SubGraph.h"
#include "../../Headers/Constants.h"

/* Create a new subgraph */
SubGraph* createSubGraph(int n_vertices, int n_edges, int rank){
    SubGraph *g = malloc(sizeof(SubGraph));
    assert(g != NULL);
    g->offset = rank*WORK_LOAD;
    g->nV = n_vertices;
    g->nE = n_edges;
    g->adj = (int*)malloc(sizeof(int)*n_vertices*n_edges);
    assert(g->adj != NULL);
    initSubGraph(g);
    return g;
}

/* Swaps two integers */
void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Shuffles an array of integers */
void shuffle(int* arr, int s_min, int s_max){
    srand(time(NULL));
    int i,j;
    for(i=s_max-1; i>s_min; i--){
        j = rand() % (i+1); 
        j %= (s_max-s_min);
        swap(&arr[j+s_min], &arr[i]);
    }
}

void initSubGraph(SubGraph* subgraph){
    /* Init matrix */
    for (int i = 0; i < subgraph->nV; i++) {
        for (int j = 0; j < subgraph->nE; j++) {
            subgraph->adj[i*subgraph->nE+j] = 0;
        }
    }
}

/* Generates a random subgraph with a number of edges for each vertex between min_edges and max_edges */
void generateRandomSubGraph(SubGraph* subgraph, int min_edges, int max_edges){
    int *array = (int*)malloc(subgraph->nV*sizeof(int));

    /* Number of edges for each vertex */
    int i,j = min_edges;
    for(i=0; i<subgraph->nV; i++){
        if (j > max_edges){
            j = min_edges;
        }
        array[i] = j;
        j++;
    }
    shuffle(array, 0, subgraph->nV);

    /* Insert random edges for each vertex */
    for (i = 0; i < subgraph->nV; i++) {
        for (j = 0; j < array[i]; j++){
            subgraph->adj[i*subgraph->nE + j] = 1;
        }
        shuffle(&subgraph->adj[i*subgraph->nE], 0, (subgraph->nE));
    }
}

/* Prints the subgraph */
void printSubGraph(SubGraph* subgraph){
    int i, j;
    for (i = 0; i < subgraph->nV; i++) {
        for (j = 0; j < subgraph->nE; j++){
            printf("%d ", subgraph->adj[i*subgraph->nE + j]);
        }
        printf("\n");
    }
}

/* Frees the memory allocated for a subgraph */
void destroySubGraph(SubGraph* subgraph){
    if (subgraph == NULL){
        return;
    }
    if(subgraph->adj != NULL){
        free(subgraph->adj);
    }
    free(subgraph);
}

/* Adds an edge to a subgraph */
void addEdge(SubGraph* subgraph, int vertex1, int vertex2){
    assert(subgraph->adj != NULL);
    if (vertex1 >= 0 && vertex1 < subgraph->nV && vertex2 >= 0 && vertex2 < subgraph->nE){
        subgraph->adj[vertex1*subgraph->nE + vertex2] = 1;
    }
}

/* Checks if a subgraph has an edge between two vertices */
bool hasEdge(SubGraph* subgraph, int vertex1, int vertex2){
    assert(subgraph->adj != NULL);
    assert(vertex1 >= 0 && vertex1 < subgraph->nV);
    assert(vertex2 >= 0 && vertex2 < subgraph->nE);
    return subgraph->adj[vertex1*subgraph->nE + vertex2] == 1;
} 

/* Removes an edge from a subgraph */
void removeEdge(SubGraph* subgraph, int vertex1, int vertex2){
    assert(subgraph->adj != NULL);
    assert(vertex1 >= 0 && vertex1 < subgraph->nV);
    assert(vertex2 >= 0 && vertex2 < subgraph->nE);
    subgraph->adj[vertex1*subgraph->nE + vertex2] = 0;
}

/* Returns the edges of a vertex in a subgraph */
int* getEdges(SubGraph* subgraph, int vertex){
    assert(subgraph->adj != NULL);
    assert(vertex >= 0 && vertex <= subgraph->nV+subgraph->offset);
    return &subgraph->adj[vertex*subgraph->nE];
}

