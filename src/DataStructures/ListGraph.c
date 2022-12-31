#include <stdio.h>
#include <stdlib.h>
#include "ListGraph.h"
#include <assert.h>

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
    SubGraph* sub = (SubGraph*)malloc(sizeof(SubGraph));
    assert(sub != NULL);
    sub->offset = list->offset;
    sub->nV = list->nV;
    sub->nE = list->nE;
    sub->adj = (int*)malloc(sizeof(int)*sub->nV*sub->nE);
    assert(sub->adj != NULL);
    initSubGraph(sub);
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