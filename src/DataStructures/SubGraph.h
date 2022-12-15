#ifndef SUBGRAPH_H
#define SUBGRAPH_H

#include <stdbool.h>

typedef struct {
    int offset; /* Starting vertex of the subgraph */
    int nV; /* Number of vertices inside the subgraph */
    int nE; /* Max number of edges */
    int *adj; /* Adjacency Matrix */
} SubGraph;

SubGraph* createSubGraph(int n_vertices, int n_edges, int rank); /* Genartes a random subgraph with a number of edges for each vertex between min_edges and max_edges */
void swap(int* a, int* b); /* Swaps two integers */
void shuffle(int* array, int min, int max); /* Shuffles an array of integers */
void generateRandomSubGraph(SubGraph* subgraph, int min_edges, int max_edges); /* Generates a random subgraph with a number of edges for each vertex between min_edges and max_edges */
void destroySubGraph(SubGraph* subgraph); /* Frees the memory allocated for a subgraph */
void printSubGraph(SubGraph* subgraph); /* Prints a subgraph */
void addEdge(SubGraph* subgraph, int vertex1, int vertex2); /* Adds an edge to a subgraph */
void removeEdge(SubGraph* subgraph, int vertex1, int vertex2); /* Removes an edge from a subgraph */
bool hasEdge(SubGraph* subgraph, int vertex1, int vertex2); /* Checks if a subgraph has an edge between two vertices */
int* getEdges(SubGraph* subgraph, int vertex); /* Returns the edges of a vertex in a subgraph */

#endif
