#include "Merge.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Constants.h"

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

// Merges two subgraphs and their SCCResult into a SubGraph
ListGraph *mergeGraphs(ListGraph *g1, ListGraph *g2, int shrink1, int shrink2, SCCResult *merged){

    // printf("G1 offset: %d G2offset: %d/n", g1->offset, g2->offset);
    
    ListGraph *result = ListGraphCreate(merged->nV, g1->nE, g1->offset < g2->offset ? g1->offset : g2->offset);
    // printf("Merge offset: %d", result->offset);

    int corr[g1->nE];

    int count=0;

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