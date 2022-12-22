#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SCCResult.h"

/* Creates a new SCCResult structure */
SCCResult *SCCResultCreate(int n){
    SCCResult* result = (SCCResult*)malloc(sizeof(SCCResult));
    result->nV = n;
    result->nMacroNodes = 0;
    result->vertices = (TList**)malloc(n * sizeof(TList*));
    for (int i = 0; i < n; i++){
        result->vertices[i] = (TList*)malloc(sizeof(TList));
        *result->vertices[i] = listCreate();
    }
    return result;
}

/* Destroys the SCCResult structure */
void SCCResultDestroy(SCCResult* result){
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
    
    SCCResult* temp = SCCResultCreate(result->nMacroNodes);

    int j=0;

    for (int i=0; i<result->nV; i++)
        if(*result->vertices[i] != NULL){
            listCopy(*result->vertices[i], temp->vertices[j]);
            j++;
        }

    temp->nMacroNodes = result->nMacroNodes;

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
