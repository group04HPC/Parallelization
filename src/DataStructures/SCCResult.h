#ifndef SCCRESULT_H
#define SCCRESULT_H

#include <stdbool.h>
#include "TList.h"

typedef struct {
    TList** vertices; /* The array of lists of nodes contained into the SCCResult structure */
    int nMacroNodes; /* The number of macro nodes contained into the SCCResult structure */
    int nV; /* The number of nodes contained into the SCCResult structure */
    int offset; /* The offset of the SCCResult structure */
} SCCResult;

SCCResult *SCCResultCreate(int n); /* Creates a new SCCResult structure */
void SCCResultDestroy(SCCResult* result); /* Destroys the SCCResult structure */
bool SCCResultInsert(SCCResult* result, int key, int value); /* Inserts a macronode-to-node match into the SCCResult structure */
SCCResult *SCCResultRescale(SCCResult* result); /* Rescales the SCCResult structure */
void SCCResultPrint(SCCResult* result); /* Prints the SCCResult structure */
int getMacronodeFromVertex(SCCResult* result, int vertex); /* Returns the macronode associated to the vertex */
TList *getVerticesFromMacronode(SCCResult *result, int macronode); /* Returns the veritces associated to the macronode */
SCCResult *SCCResultCombine(SCCResult *tarjanResult, SCCResult *mergedSCC); /* Combines two SCCResult structures */
int SCCResultGetLastElement(SCCResult *result); /* Return the max element of the SCC */
void SCCResultQuickSort(SCCResult *result, int first, int last); /* Sorts the SCCResult structure */

#endif