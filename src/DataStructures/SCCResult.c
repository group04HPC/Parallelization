#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SCCResult.h"

/* Creates a new SCCResult structure */
SCCResult *SCCResultCreate(int n)
{
    SCCResult *result = (SCCResult *)malloc(sizeof(SCCResult));
    result->nV = n;
    result->nMacroNodes = 0;
    result->offset = 0;
    result->vertices = (TList **)malloc(n * sizeof(TList *));
    for (int i = 0; i < n; i++)
    {
        result->vertices[i] = (TList *)malloc(sizeof(TList));
        *result->vertices[i] = listCreate();
    }
    return result;
}

/* Destroys the SCCResult structure */
void SCCResultDestroy(SCCResult *result)
{
    if (result == NULL)
        return;
    for (int i = 0; i < result->nV; i++)
    {
        listDestroy(*result->vertices[i]);
    }
    free(result->vertices);
    free(result);
}

/* Inserts a macronode-to-node match into the SCCResult structure */
bool SCCResultInsert(SCCResult *result, int key, int value)
{
    if (key >= result->nV || key < 0)
        return false;
    if (*result->vertices[key] == NULL)
    {
        result->nMacroNodes++;
    }
    *result->vertices[key] = listInsert(*result->vertices[key], value);
    return true;
}

/* Rescales the SCCResult structure */
SCCResult *SCCResultRescale(SCCResult *result)
{

    SCCResult *temp = SCCResultCreate(result->nMacroNodes);
    int lastValue = -1;
    bool sort = false;
    temp->offset = result->offset;

    for (int i = 0, j = 0; i < result->nV; i++)
    {

        TNode *node = *result->vertices[i];

        if (node != NULL)
        {
            if (lastValue > node->value)
                sort = true;
            lastValue = node->value;

            while (node != NULL)
            {
                SCCResultInsert(temp, j, node->value);
                node = node->link;
            }
            j++;
        }
    }

    if (sort) SCCResultQuickSort(temp, 0, temp->nV - 1);
    SCCResultDestroy(result);

    return temp;
}

void SCCResultQuickSort(SCCResult *result, int first, int last)
{
    if (last <= first)
        return;
    int i = first, j = last, pivot = first;
    TList *temp, curr_i, curr_j, curr_pivot = *result->vertices[pivot];

    while (i < j)
    {
        curr_i = *result->vertices[i];
        while (i < last && curr_i->value <= curr_pivot->value)
        {

            i++;
            curr_i = *result->vertices[i];
        }
        curr_j = *result->vertices[j];
        while (curr_j->value > curr_pivot->value)
        {
            j--;
            curr_j = *result->vertices[j];
        }
        if (i < j)
        {
            temp = result->vertices[i];
            result->vertices[i] = result->vertices[j];
            result->vertices[j] = temp;
        }
    }

    temp = result->vertices[pivot];
    result->vertices[pivot] = result->vertices[j];
    result->vertices[j] = temp;

    SCCResultQuickSort(result, first, j - 1);
    SCCResultQuickSort(result, j + 1, last);
}

void SCCResultSort(SCCResult *result)
{
    for (int i = 0; i < result->nV; i++)
    {
        if (*result->vertices[i] == NULL)
        {
            *result->vertices[i] = listInsert(*result->vertices[i], -1);
        }
    }
    SCCResultQuickSort(result, 0, result->nV - 1);
}

/* Prints the SCCResult structure */
void SCCResultPrint(SCCResult *result)
{
    for (int i = 0; i < result->nV; i++)
    {
        printf("MacroNode %d: ", i);
        listPrint(*result->vertices[i]);
        printf("\n");
    }
}

/* Returns the macronode associated to the vertex */
int getMacronodeFromVertex(SCCResult *result, int vertex)
{
    for (int i = 0; i < result->nV; i++)
        if (*result->vertices[i] != NULL)
        {
            TNode *node = listSearch(*result->vertices[i], vertex);
            if (node != NULL)
                return i;
        }
    return -1;
}

/* Returns the veritces associated to the macronode */
TList *getVerticesFromMacronode(SCCResult *result, int macronode)
{
    assert(macronode < result->nMacroNodes);
    return result->vertices[macronode];
}

/* Combines two SCCResult structures */
SCCResult *SCCResultCombine(SCCResult *tarjanResult, SCCResult *mergedSCC)
{

    SCCResult *result = SCCResultCreate(tarjanResult->nV);

    for (int i = 0; i < tarjanResult->nV; i++)
    {

        TNode *node = *tarjanResult->vertices[i];

        while (node != NULL)
        {

            TNode *node2 = *mergedSCC->vertices[node->value - tarjanResult->offset];

            while (node2 != NULL)
            {
                SCCResultInsert(result, i, node2->value);
                node2 = node2->link;
            }

            node = node->link;
        }
    }

    result->offset = tarjanResult->offset;

    return result;
}

int SCCResultGetLastElement(SCCResult *result)
{

    int max = 0;

    for (int i = 0; i < result->nV; i++)
    {

        TNode *node = *result->vertices[i];

        while (node != NULL)
        {
            if (node->value > max)
                max = node->value;
            node = node->link;
        }
    }

    return max;
}

