#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"
#include "../Tarjan/Tarjan.h"
#include "Merge.h"

int main(int argc, char* argv[]){

    // SubGraph* g1 = createSubGraph(10, 10, 0);
    // for (int i=0; i<10; i++){
    //     for (int j=0; j<10; j++){
    //         addEdge(g1, i, j);
    //     }
    // }
    // removeEdge(g1, 0, 2);
    // removeEdge(g1, 1, 2);
    // removeEdge(g1, 2, 2);
    // removeEdge(g1, 5, 2);
    // removeEdge(g1, 6, 2);
    // removeEdge(g1, 7, 2);

    // ListGraph* list = createListGraphFromMatrix(g1);
    // printf("\nList Graph:\n");
    // printListGraph(list);
    // destroySubGraph(g1);

    // printf("\nSCC:\n");
    // SCCResult* result1 = SCC(&list);
    // SCCResultPrint(result1);
    // SCCResultDestroy(result1);

    // // rank = 0
    // SubGraph* g2 = createSubGraph(5, 10, 0);
    // for (int i=0; i<5; i++){
    //     for (int j=0; j<10; j++){
    //         addEdge(g2, i, j);
    //     }
    // }
    // removeEdge(g2, 0, 2);
    // removeEdge(g2, 1, 2);
    // removeEdge(g2, 2, 2);

    // ListGraph* list2 = createListGraphFromMatrix(g2);
    // int startDim2 = list2->nV;
    // printf("\nList Graph:\n");
    // printListGraph(list2);
    // SCCResult* result2 = SCC(&list2);
    // printf("\nSCC:\n");
    // SCCResultPrint(result2);
    // printf("\nList Graph:\n");
    // printListGraph(list2);

    // // rank = 1
    // SubGraph* g3 = createSubGraph(5, 10, 1);
    // for (int i=0; i<5; i++){
    //     for (int j=0; j<10; j++){
    //         addEdge(g3, i, j);
    //     }
    // }
    // removeEdge(g3, 0, 2);
    // removeEdge(g3, 1, 2);
    // removeEdge(g3, 2, 2);

    // ListGraph* list3 = createListGraphFromMatrix(g3);
    // int startDim3 = list3->nV;
    // printf("\nList Graph:\n");
    // printListGraph(list3);
    // SCCResult* result3 = SCC(&list3);
    // printf("\nSCC:\n");
    // SCCResultPrint(result3);
    // printf("\nList Graph:\n");
    // printListGraph(list3);

    // // Merge
    // printf("\nSCC:\n");
    // SCCResult* result4 = mergeResults(result2, result3);
    // SCCResultPrint(result4);

    // ListGraph* mergedSub = mergeGraphs(list2, list3, startDim2-g2->nV, startDim3-g3->nV, result4);
    // printf("\nMerged Graph:\n");
    // printListGraph(mergedSub);

    // SCCResult* result5 = SCC(&mergedSub);
    // printf("\nSCC:\n");
    // SCCResultPrint(result5);

    // SCCResult* combinedSCC = SCCResultCombine(result5, result4);
    // printf("\nCombined SCC:\n");
    // SCCResultPrint(combinedSCC);

    // destroySubGraph(g2);
    // destroySubGraph(g3);
    // destroyListGraph(list);
    // destroyListGraph(list2);
    // destroyListGraph(list3);
    // destroyListGraph(mergedSub);
    // SCCResultDestroy(result2);
    // SCCResultDestroy(result3);
    // SCCResultDestroy(result4);
    // SCCResultDestroy(result5);
    // SCCResultDestroy(combinedSCC);

    // /* Simulating tarjan with two processes */
    // SubGraph* g6 = createSubGraph(10, 10, 0);
    // addEdge(g6, 0, 1);
    // addEdge(g6, 0, 3);
    // addEdge(g6, 1, 2);
    // addEdge(g6, 1, 4);
    // addEdge(g6, 2, 0);
    // addEdge(g6, 2, 6);
    // addEdge(g6, 3, 2);
    // addEdge(g6, 4, 5);
    // addEdge(g6, 4, 6);
    // addEdge(g6, 5, 6);
    // addEdge(g6, 5, 7);
    // addEdge(g6, 5, 8);
    // addEdge(g6, 5, 9);
    // addEdge(g6, 6, 4);
    // addEdge(g6, 7, 9);
    // addEdge(g6, 8, 9);
    // addEdge(g6, 9, 8);
    // printf("\nOriginal Graph:\n");
    // //printSubGraph(g6);
    // ListGraph* list6 = createListGraphFromMatrix(g6);
    // printListGraph(list6);
    // printf("\nSCC:\n");
    // SCCResult* result6 = SCC(&list6);
    // SCCResultPrint(result6);

    // /* 1. Simulating the subgraph created by rank 0 */
    // SubGraph* g7 = createSubGraph(5, 10, 0);
    // addEdge(g7, 0, 1);
    // addEdge(g7, 0, 3);
    // addEdge(g7, 1, 2);
    // addEdge(g7, 1, 4);
    // addEdge(g7, 2, 0);
    // addEdge(g7, 2, 6);
    // addEdge(g7, 3, 2);
    // addEdge(g7, 4, 5);
    // addEdge(g7, 4, 6);
    // printf("\nOriginal Graph:\n");
    // //printSubGraph(g7);
    // ListGraph* list7 = createListGraphFromMatrix(g7);
    // printListGraph(list7);
    // int startDim7 = list7->nV;
    // printf("\nSCC:\n");
    // SCCResult* result7 = SCC(&list7);
    // SCCResultPrint(result7);

    // /* 2. Simulating the subgraph created by rank 1 */
    // SubGraph* g8 = createSubGraph(5, 10, 1);
    // addEdge(g8, 0, 6);
    // addEdge(g8, 0, 7);
    // addEdge(g8, 0, 8);
    // addEdge(g8, 0, 9);
    // addEdge(g8, 1, 4);
    // addEdge(g8, 2, 9);
    // addEdge(g8, 3, 9);
    // addEdge(g8, 4, 8);
    // printf("\nOriginal Graph:\n");
    // // printSubGraph(g8);
    // ListGraph* list8 = createListGraphFromMatrix(g8);
    // printListGraph(list8);
    // int startDim8 = list8->nV;
    // printf("\nSCC:\n");
    // SCCResult* result8 = SCC(&list8);
    // SCCResultPrint(result8);

    // /* Merge */
    // printf("\nSCC:\n");
    // SCCResult* result9 = mergeResults(result7, result8);
    // SCCResultPrint(result9);

    // printf("\nList Graph 1:\n");
    // printListGraph(list7);
    // printf("\nList Graph 2:\n");
    // printListGraph(list8);
    // ListGraph* mergedSub2 = mergeGraphs(list7, list8, startDim7-g7->nV, startDim8-g8->nV, result9);
    // printf("\nMerged Graph:\n");
    // printListGraph(mergedSub2);

    // SCCResult* result10 = SCC(&mergedSub2);
    // printf("\nSCC:\n");
    // SCCResultPrint(result10);

    // SCCResult* combinedSCC2 = SCCResultCombine(result10, result9);
    // printf("\nCombined SCC:\n");
    // SCCResultPrint(combinedSCC2);

    // destroySubGraph(g6);
    // SCCResultDestroy(result6);
    // destroyListGraph(list6);
    // destroySubGraph(g7);
    // SCCResultDestroy(result7);
    // destroyListGraph(list7);
    // destroySubGraph(g8);
    // SCCResultDestroy(result8);
    // destroyListGraph(list8);
    // destroyListGraph(mergedSub2);
    // SCCResultDestroy(result9);
    // SCCResultDestroy(result10);
    // SCCResultDestroy(combinedSCC2);

    /* Simulating Tarjan with three processes */
    SubGraph* g9 = createSubGraph(15, 15, 0);
    addEdge(g9, 0, 1);
    addEdge(g9, 0, 3);
    addEdge(g9, 1, 2);
    addEdge(g9, 1, 4);
    addEdge(g9, 2, 0);
    addEdge(g9, 2, 6);
    addEdge(g9, 3, 2);
    addEdge(g9, 4, 5);
    addEdge(g9, 4, 6);
    addEdge(g9, 5, 6);
    addEdge(g9, 5, 7);
    addEdge(g9, 5, 8);
    addEdge(g9, 5, 9);
    addEdge(g9, 6, 4);
    addEdge(g9, 7, 9);
    addEdge(g9, 8, 9);
    addEdge(g9, 9, 8);
    addEdge(g9, 10, 11);
    addEdge(g9, 10, 13);
    addEdge(g9, 11, 12);
    addEdge(g9, 11, 14);
    addEdge(g9, 12, 10);
    addEdge(g9, 12, 16);
    addEdge(g9, 13, 12);
    addEdge(g9, 14, 15);
    addEdge(g9, 14, 16);
    printf("\nOriginal Graph:\n");
    printSubGraph(g9);
    ListGraph* list9 = createListGraphFromMatrix(g9);
    printf("\nSCC:\n");
    SCCResult* result9 = SCC(&list9);
    SCCResultPrint(result9);

    /* 1. Simulating the subgraph created by rank 0 */
    SubGraph* g10 = createSubGraph(5, 15, 0);
    addEdge(g10, 0, 1);
    addEdge(g10, 0, 3);
    addEdge(g10, 1, 2);
    addEdge(g10, 1, 4);
    addEdge(g10, 2, 0);
    addEdge(g10, 2, 6);
    addEdge(g10, 3, 2);
    addEdge(g10, 4, 5);
    addEdge(g10, 4, 6);
    printf("\nOriginal Graph:\n");
    ListGraph* list10 = createListGraphFromMatrix(g10);
    int startDim10 = list10->nV;
    printListGraph(list10);
    printf("\nSCC:\n");
    SCCResult* result10 = SCC(&list10);
    SCCResultPrint(result10);
    printf("\nRescaled Graph:\n");
    printListGraph(list10);

    /* 2. Simulating the subgraph created by rank 1 */
    SubGraph* g11 = createSubGraph(5, 15, 1);
    addEdge(g11, 0, 6);
    addEdge(g11, 0, 7);
    addEdge(g11, 0, 8);
    addEdge(g11, 0, 9);
    addEdge(g11, 1, 4);
    addEdge(g11, 2, 9);
    addEdge(g11, 3, 9);
    addEdge(g11, 4, 8);
    printf("\nOriginal Graph:\n");
    ListGraph* list11 = createListGraphFromMatrix(g11);
    int startDim11 = list11->nV;
    printListGraph(list11);
    printf("\nSCC:\n");
    SCCResult* result11 = SCC(&list11);
    SCCResultPrint(result11);
    printf("\nRescaled Graph:\n");
    printListGraph(list11);

    SCCResult* mergedResults2 = mergeResults(result10, result11);
    printf("\nMerged SCC:\n");
    SCCResultPrint(mergedResults2);

    ListGraph* mergedSub2 = mergeGraphs(list10, list11, startDim10-g10->nV, startDim11-g11->nV, mergedResults2);
    printf("\nMerged Graph:\n");
    printListGraph(mergedSub2);

    SCCResult* tarjan12 = SCC(&mergedSub2);
    printf("\nSCC:\n");
    SCCResultPrint(tarjan12);

    SCCResult* combinedSCC2 = SCCResultCombine(tarjan12, mergedResults2);
    printf("\nCombined SCC:\n");
    SCCResultPrint(combinedSCC2);

    /* 3. Simulating the subgraph created by rank 2 */
    SubGraph* g12 = createSubGraph(5, 15, 2);
    addEdge(g12, 0, 11);
    addEdge(g12, 0, 13);
    addEdge(g12, 1, 12);
    addEdge(g12, 1, 14);
    addEdge(g12, 2, 10);
    addEdge(g12, 2, 16);
    addEdge(g12, 3, 12);
    addEdge(g12, 4, 15);
    addEdge(g12, 4, 16);
    printf("\nOriginal Graph:\n");
    ListGraph* list12 = createListGraphFromMatrix(g12);
    printListGraph(list12);
    printf("\nSCC:\n");
    SCCResult* result12 = SCC(&list12);
    SCCResultPrint(result12);
    printf("\nRescaled Graph:\n");
    printListGraph(list12);

    SCCResult* mergedResults3 = mergeResults(combinedSCC2, result12);
    printf("\nMerged SCC:\n");
    SCCResultPrint(mergedResults3);

    destroySubGraph(g9);
    SCCResultDestroy(result9);
    destroyListGraph(list9);
    destroySubGraph(g10);
    SCCResultDestroy(result10);
    destroyListGraph(list10);
    destroySubGraph(g11);
    SCCResultDestroy(result11);
    destroyListGraph(list11);
    destroySubGraph(g12);
    SCCResultDestroy(result12);
    destroyListGraph(list12);
    SCCResultDestroy(mergedResults2);
    destroyListGraph(mergedSub2);
    SCCResultDestroy(tarjan12);
    SCCResultDestroy(combinedSCC2);
    SCCResultDestroy(mergedResults3);

    return 0;
}