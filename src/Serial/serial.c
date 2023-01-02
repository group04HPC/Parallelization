#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"
#include "../Tarjan/Tarjan.h"

int main(int argc, char* argv[]){

    int size;

    double time_spent = 0.0;
 
    clock_t begin = clock();

    FILE* fp = fopen("matrix.txt", "r");
    if (fp == NULL){
        printf("Error opening file \n");
    }
    fscanf(fp, "%d", &size);
    int* matrix = (int*)malloc(size*size*sizeof(int));
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            fscanf(fp, "%d", &matrix[i*size+j]);
    fclose(fp);

    SubGraph* sub = createSubGraph(size, size, 0);
    sub->adj = matrix;
    ListGraph* list = createListGraphFromMatrix(sub);
    
    SCCResult* result = SCC(&list);
    // SCCResultPrint(result);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Tarjan excution time serial: %f\n", time_spent);

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