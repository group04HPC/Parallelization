#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "Tarjan.h"

int main(int argc, char* argv[]){

    SubGraph* g1 = createSubGraph(5, 5, 0);
    addEdge(g1, 1, 0);
    addEdge(g1, 0, 2);
    addEdge(g1, 2, 1);
    addEdge(g1, 0, 3);
    addEdge(g1, 3, 4);
    printf("Original Graph:\n");
    printSubGraph(g1);
    printf("\nSCC:\n");
    SCCResultPrint(SCCResultRescale(SCC(g1)));

    SubGraph* g2 = createSubGraph(4, 4, 0);
    addEdge(g2, 0, 1);
    addEdge(g2, 1, 2);
    addEdge(g2, 2, 3);
    printf("\nOriginal Graph:\n");
    printSubGraph(g1);
    printf("\nSCC:\n");
    SCCResultPrint(SCCResultRescale(SCC(g2)));

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
    SCCResultPrint(SCCResultRescale(SCC(g3)));

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
    SCCResultPrint(SCCResultRescale(SCC(g4)));

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
    SCCResultPrint(SCCResultRescale(SCC(g5)));

    return 0;
}