#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "Merge.h"
#include "../DataStructures/TArray.h"
#include "../DataStructures/TList.h"

int main(int argc, char* argv[]){

    printf("=========== 2 processes ===========\n");

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
    printf("\nSCC:\n");
    SCCResult* result1 = SCCResultRescale(SCC(g1));
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
    SCCResult* result2 = SCCResultRescale(SCC(g2));
    int startDim2 = g2->nV;
    g2 = rescaleGraph(g2, result2);
    printf("\nRescaled graph: %d\n",g2->offset);
    printSubGraph(g2);
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
    SCCResult* result3 = SCCResultRescale(SCC(g3));
    int startDim3 = g3->nV;
    g3 = rescaleGraph(g3, result3);
    printf("\nRescaled graph: %d\n",g3->offset);
    printSubGraph(g3);
    printf("\nSCC:\n");
    SCCResultPrint(result3);

    // Merge
    printf("\nSCC:\n");
    SCCResult* result4 = mergeResults(result2, result3);
    SCCResultPrint(result4);

    SubGraph* mergedSub = mergeGraphs(g2, g3, startDim2-g2->nV, startDim3-g3->nV, result4);
    printf("\nMerged Graph:\n");
    printSubGraph(mergedSub);

    SCCResult* result5 = SCCResultRescale(SCC(mergedSub));
    printf("\nSCC:\n");
    SCCResultPrint(result5);

    SCCResult* combinedSCC = SCCResultCombine(result5, result4);
    SCCResultPrint(combinedSCC);

    // Simulating tarjan with two processes 
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
    // printSubGraph(g6);
    // printf("\nSCC:\n");
    // SCCResult* result6 = SCCResultRescale(SCC(g6));
    // SCCResultPrint(result6);
    // destroySubGraph(g6);
    // SCCResultDestroy(result6);

    // // 1. Simulating the subgraph created by rank 0 
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
    // //printf("\nOriginal Graph:\n");
    // //printSubGraph(g7);
    // SCCResult* result7 = SCCResultRescale(SCC(g7));
    // int startDim7 = g7->nV;
    // g7 = rescaleGraph(g7, result7);
    // //printf("\nRescaled graph: %d\n",g7->offset);
    // //printSubGraph(g7);
    // //printf("\nSCC:\n");
    // //SCCResultPrint(result7);
    
    // // 2. Simulating the subgraph created by rank 1 
    // SubGraph* g8 = createSubGraph(5, 10, 1);
    // addEdge(g8, 0, 6);
    // addEdge(g8, 0, 7);
    // addEdge(g8, 0, 8);
    // addEdge(g8, 0, 9);
    // addEdge(g8, 1, 4);
    // addEdge(g8, 2, 9);
    // addEdge(g8, 3, 9);
    // addEdge(g8, 4, 8);
    // //printf("\nOriginal Graph:\n");
    // //printSubGraph(g8);
    // SCCResult* result8 = SCCResultRescale(SCC(g8));
    // int startDim8 = g8->nV;
    // g8 = rescaleGraph(g8, result8);
    // //printf("\nRescaled graph: %d\n",g8->offset);
    // //printSubGraph(g8);
    // //printf("\nSCC:\n");
    // //SCCResultPrint(result8);
    
    // // Merging the two subgraphs
    // SCCResult* result9 = mergeResults(result7, result8);
    // //printf("\nMerged SCC:\n");
    // //SCCResultPrint(result9);
 
    // SubGraph* mergedSub = mergeGraphs(g7, g8, startDim7-g7->nV, startDim8-g8->nV, result9);
    // printf("\nMerged Graph:\n");
    // printSubGraph(mergedSub);

    // SCCResult* result10 = SCCResultRescale(SCC(mergedSub));
    // //printf("\nSCC of merged graph:\n");
    // //SCCResultPrint(result10);

    // SCCResult* combined = SCCResultCombine(result10, result9);
    // printf("\nCombined SCC:\n");
    // SCCResultPrint(combined);

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
    // int startDim11=g11->nV;
    // g11 = rescaleGraph(g11, result11);
    // SCCResultPrint(result11);

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
    // //printf("\nOriginal Graph:\n");
    // //printSubGraph(g12);
    // SCCResult* result12 = SCCResultRescale(SCC(g12));
    // int startDim12=g12->nV;
    // g12=rescaleGraph(g12,result12);
    // //printf("\nRescaled graph: %d\n", g12->offset);
    // //printSubGraph(g12);
    // //printf("\nSCC:\n");
    // //SCCResultPrint(result12);

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
    // //printf("\nOriginal Graph:\n");
    // //printSubGraph(g13);
    // SCCResult *result13 = SCCResultRescale(SCC(g13));
    // int startDim13 = g13->nV;
    // g13 = rescaleGraph(g13, result13);
    // //printf("\nRescaled graph: %d\n", g13->offset);
    // //printSubGraph(g13);
    // //printf("\nSCC:\n");
    // //SCCResultPrint(result13);

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
    // SCCResult *result14 = SCCResultRescale(SCC(g14));
    // int startDim14 = g14->nV;
    // g14 = rescaleGraph(g14, result14);
    // printf("\nRescaled graph: %d\n", g14->offset);
    // printSubGraph(g14);
    // printf("\nSCC:\n");
    // SCCResultPrint(result14);

    // // Merging the first two subgraphs
    // SCCResult* result15 = mergeResults(result12, result13);
    // printf("\nMerged SCC 1:\n");
    // SCCResultPrint(result15);

    // SubGraph* mergedSub1 = mergeGraphs(g12, g13,startDim12-g12->nV, startDim13-g13->nV, result15);
    // printf("\nMerged Graph 1:\n");
    // printSubGraph(mergedSub1);

    // SCCResult* result16 = SCCResultRescale(SCC(mergedSub1));
    // printf("\nSCC of merged graph 1:\n");
    // SCCResultPrint(result16);
    // int startDim1=result16->nV;
    // SCCResult* combined1 = SCCResultCombine(result16, result15);
    // printf("\nCombined SCC 1:\n");
    // SCCResultPrint(combined1);

    // // Merging the third subgraph
    // SCCResult* result17 = mergeResults(combined1, result14);
    // printf("\nMerged SCC 2:\n");
    // SCCResultPrint(result17);

    // SubGraph* mergedSub2 = mergeGraphs(mergedSub1, g14, startDim1-mergedSub1->nV, startDim14-g14->nV, result17);
    // printf("\nMerged Graph 2:\n");
    // printSubGraph(mergedSub2);

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