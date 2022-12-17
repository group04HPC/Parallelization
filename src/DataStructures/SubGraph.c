#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SubGraph.h"

/* Create a new subgraph */
SubGraph* createSubGraph(int n_vertices, int n_edges, int rank){
    SubGraph *g = malloc(sizeof(SubGraph));
    assert(g != NULL);
    g->offset = rank*n_vertices;
    g->nV = n_vertices;
    g->nE = n_edges;
    g->adj = (int*)malloc(sizeof(int)*n_vertices*n_edges);
    assert(g->adj != NULL);
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
    initSubGraph(subgraph);

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
    if(subgraph->adj != NULL){
        free(subgraph->adj);
    }
    free(subgraph);
}

/* Adds an edge to a subgraph */
void addEdge(SubGraph* subgraph, int vertex1, int vertex2){
    assert(subgraph->adj != NULL);
    //printf("vertex1: %d, vertex2: %d\n", vertex1, vertex2);
    assert(vertex1 >= 0 && vertex1 < subgraph->nV);
    assert(vertex2 >= 0 && vertex2 < subgraph->nE);
    subgraph->adj[vertex1*subgraph->nE + vertex2] = 1;
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
    printf("vertex: %d, nV: %d, nE: %d\n", vertex, subgraph->nV, subgraph->nE);
    assert(vertex >= 0 && vertex <= subgraph->nV+subgraph->offset);
    return &subgraph->adj[vertex*subgraph->nE];
}

