#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "Tarjan.h"

int main(int argc, char* argv[]){

    // Test for the serial version

    SubGraph* g1 = createSubGraph(5, 5, 0);
    addEdge(g1, 1, 0);
    addEdge(g1, 0, 2);
    addEdge(g1, 2, 1);
    addEdge(g1, 0, 3);
    addEdge(g1, 3, 4);
    printf("Original Graph:\n");
    printSubGraph(g1);
    printf("\nSCC:\n");
    SCCResult* result1 = SCCResultRescale(SCC(g1));
    SCCResultPrint(result1);
    destroySubGraph(g1);
    SCCResultDestroy(result1);
    /* 
        Expected :
        4
        3
        1 2 0
    */

    SubGraph* g2 = createSubGraph(4, 4, 0);
    addEdge(g2, 0, 1);
    addEdge(g2, 1, 2);
    addEdge(g2, 2, 3);
    printf("\nOriginal Graph:\n");
    printSubGraph(g1);
    printf("\nSCC:\n");
    SCCResult* result2 = SCCResultRescale(SCC(g2));
    SCCResultPrint(result2);
    destroySubGraph(g2);
    SCCResultDestroy(result2);
    /* 
        Expected :
        3
        2
        1
        0
    */

    SubGraph* g3 = createSubGraph(7, 7, 0);
    addEdge(g3, 0, 1);
    addEdge(g3, 1, 2);
    addEdge(g3, 2, 0);
    addEdge(g3, 1, 3);
    addEdge(g3, 1, 4);
    addEdge(g3, 1, 6);
    addEdge(g3, 3, 5);
    addEdge(g3, 4, 5);
    printf("\nOriginal Graph:\n");
    printSubGraph(g3);
    printf("\nSCC:\n");
    SCCResult* result3 = SCCResultRescale(SCC(g3));
    SCCResultPrint(result3);
    destroySubGraph(g3);
    SCCResultDestroy(result3);
    /*
        Expected:
        5
        3
        4
        6
        2 1 0
    */

    SubGraph* g4 = createSubGraph(11, 11, 0);
    addEdge(g4, 0, 1);
    addEdge(g4, 0, 3);
    addEdge(g4, 1, 2);
    addEdge(g4, 1, 4);
    addEdge(g4, 2, 0);
    addEdge(g4, 2, 6);
    addEdge(g4, 3, 2);
    addEdge(g4, 4, 5);
    addEdge(g4, 4, 6);
    addEdge(g4, 5, 6);
    addEdge(g4, 5, 7);
    addEdge(g4, 5, 8);
    addEdge(g4, 5, 9);
    addEdge(g4, 6, 4);
    addEdge(g4, 7, 9);
    addEdge(g4, 8, 9);
    addEdge(g4, 9, 8);
    printf("\nOriginal Graph:\n");
    printSubGraph(g4);
    printf("\nSCC:\n");
    SCCResult* result4 = SCCResultRescale(SCC(g4));
    SCCResultPrint(result4);
    destroySubGraph(g4);
    SCCResultDestroy(result4);
    /*
        Expected:
        8 9
        7
        5 4 6
        3 2 1 0
        10
    */

    SubGraph* g5 = createSubGraph(5, 5, 0);
    addEdge(g5, 0, 1);
    addEdge(g5, 1, 2);
    addEdge(g5, 2, 3);
    addEdge(g5, 2, 4);
    addEdge(g5, 3, 0);
    addEdge(g5, 4, 2);
    printf("\nOriginal Graph:\n");
    printSubGraph(g5);
    printf("\nSCC:\n");
    SCCResult* result5 = SCCResultRescale(SCC(g5));
    SCCResultPrint(result5);
    destroySubGraph(g5);
    SCCResultDestroy(result5);
    /*
        Expected:
        4 3 2 1 0
    */

    // Test for the parallel version
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
    printf("\nSCC:\n");
    SCCResult* result7 = SCCResultRescale(SCC(g7));
    SCCResultPrint(result7);
    destroySubGraph(g7);
    SCCResultDestroy(result7);

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
    printf("\nSCC:\n");
    SCCResult* result8 = SCCResultRescale(SCC(g8));
    SCCResultPrint(result8);
    destroySubGraph(g8);
    SCCResultDestroy(result8);

    return 0;
}