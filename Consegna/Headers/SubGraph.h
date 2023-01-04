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
 * This file contains the implementation of the SubGraph data structure
 * by means of the adjacency matrix representation.
 * A SubGraph is a graph that is stored in a single node. It contains all
 * the necessary functions to create and manage a subgraph.
 *
 */

#ifndef SUBGRAPH_H
#define SUBGRAPH_H

#include <stdbool.h>

typedef struct
{
    int offset; /* Starting vertex of the subgraph */
    int nV;     /* Number of vertices inside the subgraph */
    int nE;     /* Max number of edges */
    int *adj;   /* Adjacency Matrix */
} SubGraph;

SubGraph *createSubGraph(int n_vertices, int n_edges, int rank);               /* Genartes a random subgraph with a number of edges for each vertex between min_edges and max_edges */
void swap(int *a, int *b);                                                     /* Swaps two integers */
void shuffle(int *array, int min, int max);                                    /* Shuffles an array of integers */
void initSubGraph(SubGraph *subgraph);                                         /* Initializes a subgraph */
void generateRandomSubGraph(SubGraph *subgraph, int min_edges, int max_edges); /* Generates a random subgraph with a number of edges for each vertex between min_edges and max_edges */
void destroySubGraph(SubGraph *subgraph);                                      /* Frees the memory allocated for a subgraph */
void printSubGraph(SubGraph *subgraph);                                        /* Prints a subgraph */
void addEdge(SubGraph *subgraph, int vertex1, int vertex2);                    /* Adds an edge to a subgraph */
void removeEdge(SubGraph *subgraph, int vertex1, int vertex2);                 /* Removes an edge from a subgraph */
bool hasEdge(SubGraph *subgraph, int vertex1, int vertex2);                    /* Checks if a subgraph has an edge between two vertices */
int *getEdges(SubGraph *subgraph, int vertex);                                 /* Returns the edges of a vertex in a subgraph */

#endif
