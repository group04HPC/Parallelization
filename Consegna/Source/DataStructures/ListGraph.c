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
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../Headers/ListGraph.h"

/* Creates a list graph */
ListGraph* ListGraphCreate(int nV, int nE, int offset){
    ListGraph* list = (ListGraph*)malloc(sizeof(ListGraph));
    assert(list != NULL);
    list->offset = offset;
    list->nV = nV;
    list->nE = nE;
    
    list->adj = (TList**)malloc(sizeof(TList*)*list->nV);
    for(int i = 0; i < list->nV; i++){
        list->adj[i] = (TList*)malloc(sizeof(TList));
        assert(list->adj[i] != NULL);
        *list->adj[i] = listCreate();
    }
    return list;
}

/* Creates a list graph from a matrix graph */
ListGraph* createListGraphFromMatrix(SubGraph* sub){
    ListGraph* list = (ListGraph*)malloc(sizeof(ListGraph));
    assert(list != NULL);
    list->offset = sub->offset;
    list->nV = sub->nV;
    list->nE = sub->nE;
    
    list->adj = (TList**)malloc(sizeof(TList*)*list->nV);
    for(int i = 0; i < list->nV; i++){
        list->adj[i] = (TList*)malloc(sizeof(TList));
        assert(list->adj[i] != NULL);
        *list->adj[i] = listCreate();
    }
    
    for(int i = 0; i < list->nV; i++){
        for(int j = 0; j < list->nE; j++){
            if(hasEdge(sub, i, j)){
                *list->adj[i] = listInsert(*list->adj[i], j);
            }
        }
    }
    return list;
}

/* Creates a matrix graph from a list graph */
SubGraph* createMatrixGraphFromList(ListGraph* list){
    SubGraph* sub = createSubGraph(list->nV, list->nE, list->offset);
    assert(sub != NULL);
    for(int i = 0; i < list->nV; i++){
        TNode* node = *list->adj[i];
        while(node != NULL){
            addEdge(sub, i, node->value);
            node = node->link;
        }
    }
    return sub;
} 

/* Prints a list graph */
void printListGraph(ListGraph* graph){
    for(int i = 0; i < graph->nV; i++){
        printf("Node %d: ", i+graph->offset);
        listPrint(*graph->adj[i]);
        printf("\n");
    }
}

/* Destroys a list graph */
void destroyListGraph(ListGraph* graph){
    if (graph == NULL) return;
    for(int i = 0; i < graph->nV; i++){
        if (graph->adj[i] != NULL){
            listDestroy(*graph->adj[i]);
            free(graph->adj[i]);
        }
    }
    free(graph->adj);
    free(graph);
}

/* Inserts a new vertex into the list graph */
void insertListGraph(ListGraph* newList, int v, int u){
    *newList->adj[v] = listInsert(*newList->adj[v], u);
}