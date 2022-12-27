#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SCCResult.h"

/* Creates a new SCCResult structure */
SCCResult *SCCResultCreate(int n){
    SCCResult* result = (SCCResult*)malloc(sizeof(SCCResult));
    result->nV = n;
    result->nMacroNodes = 0;
    result->offset = 0;
    result->vertices = (TList**)malloc(n * sizeof(TList*));
    for (int i = 0; i < n; i++){
        result->vertices[i] = (TList*)malloc(sizeof(TList));
        *result->vertices[i] = listCreate();
    }
    return result;
}

/* Destroys the SCCResult structure */
void SCCResultDestroy(SCCResult* result){
    if (result == NULL) return;
    for(int i = 0; i < result->nV; i++){
        listDestroy(*result->vertices[i]);
    }
    free(result->vertices);
    free(result);
}

/* Inserts a macronode-to-node match into the SCCResult structure */
bool SCCResultInsert(SCCResult* result, int key, int value){
    if (key >= result->nV || key < 0) return false;
    if(*result->vertices[key] == NULL){
        result->nMacroNodes++;
    }
    *result->vertices[key] = listInsert(*result->vertices[key], value);
    return true;
}

/* Rescales the SCCResult structure */
SCCResult* SCCResultRescale(SCCResult* result){
    
    SCCResult *temp = SCCResultCreate(result->nMacroNodes);
    temp->offset = result->offset;

    for (int i=0, j=0; i<result->nV; i++){
    
        TNode* node = *result->vertices[i];
        
        if(node != NULL){
            while(node != NULL){
                SCCResultInsert(temp, j, node->value);
                node = node->link;
            }
            j++;
        }

    }

    SCCResultDestroy(result);
    
    return temp;
}

/* Prints the SCCResult structure */
void SCCResultPrint(SCCResult* result){
    for(int i = 0; i < result->nV; i++){
        printf("MacroNode %d: ", i);
        listPrint(*result->vertices[i]);
        printf("\n");
    }
}

/* Returns the macronode associated to the vertex */
int getMacronodeFromVertex(SCCResult* result, int vertex){
    for(int i = 0; i < result->nV; i++)
        if(*result->vertices[i] != NULL){
            TNode* node = listSearch(*result->vertices[i], vertex);
            if(node != NULL) return i;
        }
    return -1;
}

/* Returns the veritces associated to the macronode */
TList *getVerticesFromMacronode(SCCResult *result, int macronode){
    assert(macronode<result->nMacroNodes);
    return result->vertices[macronode];
} 

/* Combines two SCCResult structures */
SCCResult *SCCResultCombine(SCCResult *tarjanResult, SCCResult *mergedSCC){

    SCCResult *result = SCCResultCreate(tarjanResult->nV);

    for (int i=0; i<tarjanResult->nV; i++){
    
        TNode* node = *tarjanResult->vertices[i];
        
        while(node != NULL){
            
            TNode* node2 = *mergedSCC->vertices[node->value-tarjanResult->offset];

            while(node2 != NULL){
                bool res = SCCResultInsert(result, i, node2->value);
                node2 = node2->link;
            }

            node = node->link;
        }

    }

    result->offset = tarjanResult->offset;

    SCCResultDestroy(tarjanResult);
    
    return result;
}
