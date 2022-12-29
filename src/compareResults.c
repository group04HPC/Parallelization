#include <stdio.h>
#include <stdlib.h>
#include "DataStructures/SCCResult.h"

void sortResult(TList** result, int size);


int main(int argc, char* argv[]){

    FILE* fp = fopen("result.txt", "r");
    if (fp == NULL){
        printf("Error opening file\n");
    }

    int nMacroNodes, value, len;
    fscanf(fp, "%d", &nMacroNodes);
    SCCResult* result = SCCResultCreate(nMacroNodes);
    for (int i=0; i<nMacroNodes; i++){
        fscanf(fp, "%d", &len);
        for (int j=0; j<len; j++){
            fscanf(fp, "%d", &value);
            SCCResultInsert(result, i, value);
        }
    }
    
    fscanf(fp, "%d", &nMacroNodes);

    SCCResult* result2 = SCCResultCreate(nMacroNodes);
    for (int i=0; i<nMacroNodes; i++){
        fscanf(fp, "%d", &len);
        for (int j=0; j<len; j++){
            fscanf(fp, "%d", &value);
            SCCResultInsert(result2, i, value);
        }
    }

    fclose(fp);

    printf("=============RESULTS=============\n");
    SCCResultPrint(result);
    printf("\n");
    SCCResultPrint(result2);

    if (result->nMacroNodes != result2->nMacroNodes){
        printf("Bad result, %d, %d\n", result->nMacroNodes, result2->nMacroNodes);
        return 1;
    }

    for (int i=0; i<result->nMacroNodes; i++){
        TList list = *result->vertices[i];
        TList list2 = *result2->vertices[i];
        while (list != NULL){
            if (list->value != list2->value){
                printf("Bad result, %d, %d\n", list->value, list2->value);
                return 1;
            }
            list = list->link;
            list2 = list2->link;
        }
    }

    printf("Completed Successfully!\n");

    return 0;
}


