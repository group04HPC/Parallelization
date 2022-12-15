#include <stdio.h>
#include <stdlib.h>
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
SCCResult *SCCResultRescale(SCCResult* result){
    SCCResult* newResult = SCCResultCreate(result->nMacroNodes);
    int i = 0;
    for(int j = 0; j < result->nV; j++){
        if(*result->vertices[j] != NULL){
            newResult->vertices[i] = result->vertices[j];
            newResult->nMacroNodes++;
            newResult->nV++;
            i++;
        }
    }
    return newResult;
}

/* Prints the SCCResult structure */
void SCCResultPrint(SCCResult* result){
    for(int i = 0; i < result->nV; i++){
        if(result->vertices[i] != NULL){
            printf("MacroNode %d: ", i);
            listPrint(*result->vertices[i]);
            printf("\n");
        }
    }
}

/* Returns the macronode associated to the vertex */
int getMacronodeFromVertex(SCCResult* result, int vertex){
    for(int i = 0; i < result->nV; i++){
        if(result->vertices[i] != NULL){
            TNode* node = listSearch(*result->vertices[i], vertex);
            if(node != NULL) return i;
        }
    }
    return -1;
}