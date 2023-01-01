#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../DataStructures/ListGraph.h"
#include "Tarjan.h"

int main(int argc, char* argv[]){

    double conversion_time = 0.0;
    // Test for the serial version

    SubGraph* g1 = createSubGraph(5, 5, 0);
    addEdge(g1, 1, 0);
    addEdge(g1, 0, 2);
    addEdge(g1, 2, 1);
    addEdge(g1, 0, 3);
    addEdge(g1, 3, 4);
    printf("Original Graph:\n");
    printSubGraph(g1);
    ListGraph* list = createListGraphFromMatrix(g1);
    printf("\nList Graph:\n");
    printListGraph(list);
    
    printf("\nSCC:\n");
    SCCResult* result1 = SCC(&list);
    SCCResultPrint(result1);
    destroySubGraph(g1);
    SCCResultDestroy(result1);
    destroyListGraph(list);
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
    printSubGraph(g2);
    ListGraph* list2 = createListGraphFromMatrix(g2);
    printf("\nSCC:\n");
    SCCResult* result2 = SCC(&list2);
    SCCResultPrint(result2);
    destroySubGraph(g2);
    SCCResultDestroy(result2);
    destroyListGraph(list2);
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
    ListGraph* list3 = createListGraphFromMatrix(g3);
    printf("\nSCC:\n");
    SCCResult* result3 = SCC(&list3);
    SCCResultPrint(result3);
    destroySubGraph(g3);
    SCCResultDestroy(result3);
    destroyListGraph(list3);
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
    ListGraph* list4 = createListGraphFromMatrix(g4);
    printf("\nSCC:\n");
    SCCResult* result4 = SCC(&list4);
    SCCResultPrint(result4);
    destroySubGraph(g4);
    SCCResultDestroy(result4);
    destroyListGraph(list4);
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
    ListGraph* list5 = createListGraphFromMatrix(g5);
    printf("\nSCC:\n");
    SCCResult* result5 = SCC(&list5);
    SCCResultPrint(result5);
    destroySubGraph(g5);
    SCCResultDestroy(result5);
    destroyListGraph(list5);
    /*
        Expected:
        4 3 2 1 0
    */

    // Test for the parallel version

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
    ListGraph* list6 = createListGraphFromMatrix(g6);
    printf("\nSCC:\n");
    SCCResult* result6 = SCC(&list6);
    SCCResultPrint(result6);
    destroySubGraph(g6);
    SCCResultDestroy(result6);
    destroyListGraph(list6);

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
    ListGraph* list7 = createListGraphFromMatrix(g7);
    printf("\nSCC:\n");
    SCCResult* result7 = SCC(&list7);
    SCCResultPrint(result7);
    destroySubGraph(g7);
    SCCResultDestroy(result7);
    destroyListGraph(list7);

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
    ListGraph* list8 = createListGraphFromMatrix(g8);
    printf("\nSCC:\n");
    SCCResult* result8 = SCC(&list8);
    SCCResultPrint(result8);
    destroySubGraph(g8);
    SCCResultDestroy(result8);
    destroyListGraph(list8);

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
    destroySubGraph(g9);
    SCCResultDestroy(result9);
    destroyListGraph(list9);

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
    printListGraph(list10);
    printf("\nSCC:\n");
    SCCResult* result10 = SCC(&list10);
    SCCResultPrint(result10);
    printf("\nRescaled Graph:\n");
    printListGraph(list10);
    destroySubGraph(g10);
    SCCResultDestroy(result10);
    destroyListGraph(list10);

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
    printListGraph(list11);
    printf("\nSCC:\n");
    SCCResult* result11 = SCC(&list11);
    SCCResultPrint(result11);
    printf("\nRescaled Graph:\n");
    printListGraph(list11);
    destroySubGraph(g11);
    SCCResultDestroy(result11);
    destroyListGraph(list11);

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
    destroySubGraph(g12);
    destroyListGraph(list12);

    return 0;
}