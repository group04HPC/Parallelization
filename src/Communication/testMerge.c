#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "Merge.h"

int main(int argc, char* argv[]){

    printf("=========== 2 processes ===========\n");

    /* Simulating tarjan with two processes */
    SubGraph* g6 = createSubGraph(10, 10, 0);
    addEdge(g6, 0, 1);
    addEdge(g6, 0, 3);
    addEdge(g6, 1, 2);
    addEdge(g6, 1, 4);
    addEdge(g6, 2, 0);
    addEdge(g6, 2, 6);
    addEdge(g6, 3, 2);
    addEdge(g6, 4, 5);
    addEdge(g6, 4, 6);
    addEdge(g6, 5, 6);
    addEdge(g6, 5, 7);
    addEdge(g6, 5, 8);
    addEdge(g6, 5, 9);
    addEdge(g6, 6, 4);
    addEdge(g6, 7, 9);
    addEdge(g6, 8, 9);
    addEdge(g6, 9, 8);
    printf("\nOriginal Graph:\n");
    printSubGraph(g6);
    printf("\nSCC:\n");
    SCCResult* result6 = SCCResultRescale(SCC(g6));
    SCCResultPrint(result6);
    destroySubGraph(g6);
    SCCResultDestroy(result6);

    /* 1. Simulating the subgraph created by rank 0 */
    SubGraph* g7 = createSubGraph(5, 10, 0);
    addEdge(g7, 0, 1);
    addEdge(g7, 0, 3);
    addEdge(g7, 1, 2);
    addEdge(g7, 1, 4);
    addEdge(g7, 2, 0);
    addEdge(g7, 2, 6);
    addEdge(g7, 3, 2);
    addEdge(g7, 4, 5);
    addEdge(g7, 4, 6);
    printf("\nOriginal Graph:\n");
    printSubGraph(g7);
    SCCResult* result7 = SCCResultRescale(SCC(g7));
    int startDim7 = g7->nV;
    g7 = rescaleGraph(g7, result7);
    printf("\nRescaled graph:\n");
    printSubGraph(g7);
    printf("\nSCC:\n");
    SCCResultPrint(result7);
    
    /* 2. Simulating the subgraph created by rank 1 */
    SubGraph* g8 = createSubGraph(5, 10, 1);
    addEdge(g8, 0, 6);
    addEdge(g8, 0, 7);
    addEdge(g8, 0, 8);
    addEdge(g8, 0, 9);
    addEdge(g8, 1, 4);
    addEdge(g8, 2, 9);
    addEdge(g8, 3, 9);
    addEdge(g8, 4, 8);
    printf("\nOriginal Graph:\n");
    printSubGraph(g8);
    SCCResult* result8 = SCCResultRescale(SCC(g8));
    int startDim8 = g8->nV;
    g8 = rescaleGraph(g8, result8);
    printf("\nRescaled graph:\n");
    printSubGraph(g8);
    printf("\nSCC:\n");
    SCCResultPrint(result8);
    
    // Merging the two subgraphs
    SCCResult* result9 = mergeResults(result7, result8);
    printf("\nMerged SCC:\n");
    SCCResultPrint(result9);
 
    SubGraph* mergedSub = mergeGraphs(g7, g8, g7->nV-startDim7, g8->nV-startDim8, result9);
    printf("\nMerged Graph:\n");
    printSubGraph(mergedSub);

    SCCResult* result10 = SCCResultRescale(SCC(mergedSub));
    printf("\nSCC of merged graph:\n");
    SCCResultPrint(result10);

    SCCResult* combined = SCCResultCombine(result10, result9);
    printf("\nCombined SCC:\n");
    SCCResultPrint(combined);

    destroySubGraph(g7);
    SCCResultDestroy(result7);
    destroySubGraph(g8);
    SCCResultDestroy(result8);
    destroySubGraph(mergedSub);
    SCCResultDestroy(result9);
    SCCResultDestroy(result10);
    SCCResultDestroy(combined);

    // printf("\n=========== 3 processes ===========\n");

    // /* Simulating Tarjan with three processes */
    // SubGraph* g11 = createSubGraph(15, 15, 0);
    // addEdge(g11, 0, 1);
    // addEdge(g11, 0, 3);
    // addEdge(g11, 1, 2);
    // addEdge(g11, 1, 4);
    // addEdge(g11, 2, 0);
    // addEdge(g11, 2, 6);
    // addEdge(g11, 3, 2);
    // addEdge(g11, 4, 5);
    // addEdge(g11, 4, 6);
    // addEdge(g11, 5, 6);
    // addEdge(g11, 5, 7);
    // addEdge(g11, 5, 8);
    // addEdge(g11, 5, 9);
    // addEdge(g11, 6, 4);
    // addEdge(g11, 7, 9);
    // addEdge(g11, 8, 9);
    // addEdge(g11, 9, 8);
    // addEdge(g11, 10, 11);
    // addEdge(g11, 10, 13);
    // addEdge(g11, 11, 12);
    // addEdge(g11, 11, 14);
    // addEdge(g11, 12, 10);
    // addEdge(g11, 12, 16);
    // addEdge(g11, 13, 12);
    // addEdge(g11, 14, 15);
    // addEdge(g11, 14, 16);
    // printf("\nOriginal Graph:\n");
    // printSubGraph(g11);
    // printf("\nSCC:\n");
    // SCCResult* result11 = SCCResultRescale(SCC(g11));
    // SCCResultPrint(result11);
    // destroySubGraph(g11);
    // SCCResultDestroy(result11);

    // /* 1. Simulating the subgraph created by rank 0 */
    // SubGraph* g12 = createSubGraph(5, 15, 0);
    // addEdge(g12, 0, 1);
    // addEdge(g12, 0, 3);
    // addEdge(g12, 1, 2);
    // addEdge(g12, 1, 4);
    // addEdge(g12, 2, 0);
    // addEdge(g12, 2, 6);
    // addEdge(g12, 3, 2);
    // addEdge(g12, 4, 5);
    // addEdge(g12, 4, 6);
    // printf("\nOriginal Graph:\n");
    // printSubGraph(g12);
    // printf("\nSCC:\n");
    // SCCResult* result12 = SCCResultRescale(SCC(g12));
    // SCCResultPrint(result12);

    // /* 2. Simulating the subgraph created by rank 1 */
    // SubGraph* g13 = createSubGraph(5, 15, 1);
    // addEdge(g13, 0, 6);
    // addEdge(g13, 0, 7);
    // addEdge(g13, 0, 8);
    // addEdge(g13, 0, 9);
    // addEdge(g13, 1, 4);
    // addEdge(g13, 2, 9);
    // addEdge(g13, 3, 9);
    // addEdge(g13, 4, 8);
    // printf("\nOriginal Graph:\n");
    // printSubGraph(g13);
    // printf("\nSCC:\n");
    // SCCResult* result13 = SCCResultRescale(SCC(g13));
    // SCCResultPrint(result13);

    // /* 3. Simulating the subgraph created by rank 2 */
    // SubGraph* g14 = createSubGraph(5, 15, 2);
    // addEdge(g14, 0, 11);
    // addEdge(g14, 0, 13);
    // addEdge(g14, 1, 12);
    // addEdge(g14, 1, 14);
    // addEdge(g14, 2, 10);
    // addEdge(g14, 2, 16);
    // addEdge(g14, 3, 12);
    // addEdge(g14, 4, 15);
    // addEdge(g14, 4, 16);
    // printf("\nOriginal Graph:\n");
    // printSubGraph(g14);
    // printf("\nSCC:\n");
    // SCCResult* result14 = SCCResultRescale(SCC(g14));
    // SCCResultPrint(result14);
    
    // // Merging the first two subgraphs
    // SCCResult* result15 = mergeResults(result12, result13);
    // printf("\nMerged SCC 1:\n");
    // SCCResultPrint(result15);

    // SubGraph* mergedSub1 = mergeGraphs(g12, g13, result12, result13, result15);
    // printf("\nMerged Graph 1:\n");
    // printSubGraph(mergedSub1);

    // SCCResult* result16 = SCCResultRescale(SCC(mergedSub1));
    // printf("\nSCC of merged graph 1:\n");
    // SCCResultPrint(result16);

    // SCCResult* combined1 = SCCResultCombine(result16, result15);
    // printf("\nCombined SCC 1:\n");
    // SCCResultPrint(combined1);

    // // Merging the third subgraph
    // SCCResult* result17 = mergeResults(combined1, result14);
    // printf("\nMerged SCC 2:\n");
    // SCCResultPrint(result17);

    // SubGraph* mergedSub2 = mergeGraphs(mergedSub1, g14, combined1, result14, result17);
    // printf("\nMerged Graph 2:\n");
    // printSubGraph(mergedSub2);

    // SCCResult* result18 = SCCResultRescale(SCC(mergedSub2));
    // printf("\nSCC of merged graph 2:\n");
    // SCCResultPrint(result18);

    // SCCResult* combined2 = SCCResultCombine(result18, result17);
    // printf("\nCombined SCC 2:\n");
    // SCCResultPrint(combined2);

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
    // SCCResultDestroy(result18);
    // SCCResultDestroy(combined2);

    return 0;
}