#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"

int main(int argc, char* argv[]){

    int size;
    FILE* fp = fopen("../Parallel/matrix.txt", "r");
    if (fp == NULL){
        printf("Error opening file\n");
    }
    fscanf(fp, "%d", &size);
    int* matrix = (int*)malloc(size*size*sizeof(int));
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            fscanf(fp, "%d", &matrix[i*size+j]);
    fclose(fp);

    SubGraph* sub = createSubGraph(size, size, 0);
    sub->adj = matrix;
    
    SCCResult* result = SCCResultRescale(SCC(sub));

    printf("Original graph:\n");
    printSubGraph(sub);
    printf("\nResult:\n");
    SCCResultPrint(result);

    FILE* fp2 = fopen("result.txt", "w+");
    if (fp2 == NULL){
        printf("Error opening file\n");
    }
    fprintf(fp2, "%d\n", result->nMacroNodes);
    for (int i=0; i<result->nMacroNodes; i++){
        TList list = *result->vertices[i];
        fprintf(fp2, "%d ", listCount(list));
        while (list != NULL){
            fprintf(fp2, "%d ", list->value);
            list = list->link;
        }
        fprintf(fp2, "\n");
    }
    fclose(fp2);

    SCCResultDestroy(result);
    destroySubGraph(sub);

    return 0;
}