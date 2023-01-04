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
 * This file contains the implementation of the SCCResult structure.
 * The SCCResult structure is used to store the result of the Tarjan's algorithm.
 * The SCCResult structure is composed by an array of lists of nodes, the number
 * of macro nodes, the number of nodes and the offset. It contains all the necessary functions
 * to create and manage a SCCResult structure.
 *
 */

#ifndef SCCRESULT_H
#define SCCRESULT_H

#include <stdbool.h>
#include "TList.h"

typedef struct
{
    TList **vertices; /* The array of lists of nodes contained into the SCCResult structure */
    int nMacroNodes;  /* The number of macro nodes contained into the SCCResult structure */
    int nV;           /* The number of nodes contained into the SCCResult structure */
    int offset;       /* The offset of the SCCResult structure */
} SCCResult;

SCCResult *SCCResultCreate(int n);                                          /* Creates a new SCCResult structure */
void SCCResultDestroy(SCCResult *result);                                   /* Destroys the SCCResult structure */
bool SCCResultInsert(SCCResult *result, int key, int value);                /* Inserts a macronode-to-node match into the SCCResult structure */
SCCResult *SCCResultRescale(SCCResult *result);                             /* Rescales the SCCResult structure */
void SCCResultPrint(SCCResult *result);                                     /* Prints the SCCResult structure */
int getMacronodeFromVertex(SCCResult *result, int vertex);                  /* Returns the macronode associated to the vertex */
TList *getVerticesFromMacronode(SCCResult *result, int macronode);          /* Returns the veritces associated to the macronode */
SCCResult *SCCResultCombine(SCCResult *tarjanResult, SCCResult *mergedSCC); /* Combines two SCCResult structures */
int SCCResultGetLastElement(SCCResult *result);                             /* Return the max element of the SCC */
void SCCResultQuickSort(SCCResult *result, int first, int last);            /* Sorts the SCCResult structure */

#endif