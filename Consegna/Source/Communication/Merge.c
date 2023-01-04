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
 * This file contains the implementation of the functions required to merge the structures recived from another
 * process with the ones made by the current process.
 * The provided functions are:
 *      ListGraph *mergeGraphs(ListGraph *g1, ListGraph *g2, int shrink1, int shrink2, SCCResult *merged);
 *      SCCResult *mergeResults(SCCResult *r1, SCCResult *r2);
 */

#include "../../Headers/Merge.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../Headers/Constants.h"

/*
 * Function:  mergeResults
 * --------------------
 * Merges two SCCResults in one structure
 *
 *  *r1: a reference to the first SCCResult structure to merge
 *  *r2: a reference to the second SCCResult structure to merge
 *
 *  return: the merged SCCResult
 */
SCCResult *mergeResults(SCCResult *r1, SCCResult *r2)
{
    SCCResult *result = SCCResultCreate(r1->nMacroNodes + r2->nMacroNodes);
    
    int i = 0;
    for (; i < r1->nV; i++)
        listCopy(*r1->vertices[i], result->vertices[i]);
    for (; i < r1->nV + r2->nV; i++)
        listCopy(*r2->vertices[i - r1->nV], result->vertices[i]);

    result->nMacroNodes = r1->nMacroNodes + r2->nMacroNodes;
    result->offset = r1->offset < r2->offset ? r1->offset : r2->offset;

    return result;
}

/*
 * Function:  mergeGraphs
 * --------------------
 * Merges two ListGraphs in one ListGraph, rerouting all the edges of the old graphs
 * to the new nodes in the merged one
 *
 *  *g1: a reference to the first SCCResult structure to merge
 *  *g2: a reference to the second SCCResult structure to merge
 *  shrink1: a int representing the number of nodes that have been merged in the first graph
 *  shrink2: a int representing the number of nodes that have been merged in the second graph
 *  *merged: the merged SCCResult
 *
 *  return: the merged ListGraph
 */
ListGraph *mergeGraphs(ListGraph *g1, ListGraph *g2, int shrink1, int shrink2, SCCResult *merged){    
    ListGraph *result = ListGraphCreate(merged->nV, g1->nE, g1->offset < g2->offset ? g1->offset : g2->offset);
    int corr[g1->nE],count=0;

    for (int i=0; i<merged->nV; i++){
        TList list = *merged->vertices[i];
        while (list != NULL){
            corr[list->value-g1->offset] = i;
            list = list->link;
            count++;
        }
    }

    for (int i=0; i<g1->nV; i++){
        TList list = *g1->adj[i];
        while (list != NULL){
            if (list->value >= g1->offset && list->value < g1->offset+g1->nV){
                insertListGraph(result, i, list->value);
            }else if (list->value >= g1->offset+g1->nV && list->value < g1->offset+count){
                insertListGraph(result, i, corr[list->value-g1->offset] + g1->offset);
            }else{
                insertListGraph(result, i, list->value);
            }  
            list = list->link;
        }
    }

    for (int i=0; i<g2->nV; i++){
        TList list = *g2->adj[i];
        while (list != NULL){
            if (list->value >= g1->offset && list->value < g1->offset+count){
                insertListGraph(result, i+g1->nV, corr[list->value-g1->offset] + g1->offset);
            }else{
                insertListGraph(result, i+g1->nV, list->value);
            }
            list = list->link;
        }
    }
    
    return result;

}