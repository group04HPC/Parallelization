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
    printf("Original Graph:\n");
    printSubGraph(sub);
    printf("\nSCC Result:\n");
    
    SCCResult* result = SCCResultRescale(SCC(sub));
    SCCResultPrint(result);
    SCCResultDestroy(result);
    destroySubGraph(sub);

    return 0;
}