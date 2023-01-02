#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"
#include "../Tarjan/Tarjan.h"
#include "Merge.h"

int main(int argc, char* argv[]){

    double conversion_time = 0.0, reconversion_time = 0.0;
    SubGraph* g1 = createSubGraph(10, 10, 0);
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            addEdge(g1, i, j);
        }
    }
    removeEdge(g1, 0, 2);
    removeEdge(g1, 1, 2);
    removeEdge(g1, 2, 2);
    removeEdge(g1, 5, 2);
    removeEdge(g1, 6, 2);
    removeEdge(g1, 7, 2);
    printf("\nOriginal Graph:\n");
    printSubGraph(g1);

    printf("\nConverto...");
    clock_t begin = clock();
    ListGraph* list = createListGraphFromMatrix(g1);
    clock_t end = clock();
    conversion_time += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nList Graph:\n");
    printListGraph(list);

    printf("\nRi-Converto...");
    begin = clock();
    SubGraph* new = createMatrixGraphFromList(list);
    end = clock();
    reconversion_time += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nMatrix Graph:\n");
    printSubGraph(new);

    printf("\nConversion time: %f\n", conversion_time);
    printf("\nReconversion time: %f\n", reconversion_time);

    printf("\nSCC:\n");

    SCCResult* result1 = SCCResultRescale(SCC(list));
    SCCResultPrint(result1);
    destroySubGraph(g1);
    SCCResultDestroy(result1);

    // rank = 0
    SubGraph* g2 = createSubGraph(5, 10, 0);
    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            addEdge(g2, i, j);
        }
    }
    removeEdge(g2, 0, 2);
    removeEdge(g2, 1, 2);
    removeEdge(g2, 2, 2);
    printf("\nOriginal Graph:\n");
    printSubGraph(g2);
    ListGraph* list2 = createListGraphFromMatrix(g2);
    printf("\nList Graph:\n");
    printListGraph(list2);
    SCCResult* result2 = SCCResultRescale(SCC(list2));
    printf("\nSCC:\n");
    SCCResultPrint(result2);

    // rank = 1
    SubGraph* g3 = createSubGraph(5, 10, 1);
    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            addEdge(g3, i, j);
        }
    }
    removeEdge(g3, 0, 2);
    removeEdge(g3, 1, 2);
    removeEdge(g3, 2, 2);
    printf("\nOriginal Graph:\n");
    printSubGraph(g3);

    ListGraph* list3 = createListGraphFromMatrix(g3);
    printf("\nList Graph:\n");
    printListGraph(list3);
    SCCResult* result3 = SCCResultRescale(SCC(list3));
    printf("\nSCC:\n");
    SCCResultPrint(result3);

    // // Merge
    // printf("\nSCC:\n");
    // SCCResult* result4 = mergeResults(result2, result3);
    // SCCResultPrint(result4);

    // SubGraph* mergedSub = mergeGraphs(g2, g3, startDim2-g2->nV, startDim3-g3->nV, result4);
    // printf("\nMerged Graph:\n");
    // printSubGraph(mergedSub);

    // SCCResult* result5 = SCCResultRescale(SCC(mergedSub));
    // printf("\nSCC:\n");
    // SCCResultPrint(result5);

    // SCCResult* combinedSCC = SCCResultCombine(result5, result4);
    // SCCResultPrint(combinedSCC);

    // destroySubGraph(g7);
    // SCCResultDestroy(result7);
    // destroySubGraph(g8);
    // SCCResultDestroy(result8);
    // destroySubGraph(mergedSub);
    // SCCResultDestroy(result9);
    // SCCResultDestroy(result10);
    // SCCResultDestroy(combined);
    // destroySubGraph(g12);
    // SCCResultDestroy(result12);
    // destroySubGraph(g13);
    // SCCResultDestroy(result13);
    // destroySubGraph(g14);
    // SCCResultDestroy(result14);
    // destroySubGraph(mergedSub1);
    // SCCResultDestroy(result15);
    // SCCResultDestroy(result16);
    // SCCResultDestroy(combined1);
    // destroySubGraph(mergedSub2);
    // SCCResultDestroy(result17);

    return 0;
}