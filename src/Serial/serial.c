#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"

int main(int argc, char* argv[]){
    SubGraph* sub = createSubGraph(NUM_VERTICES, NUM_VERTICES, 0);
    printf("Original Graph:\n");
    generateRandomSubGraph(sub, MIN_EDGES_SERIAL, MAX_EDGES_SERIAL);
    printSubGraph(sub);
    SCCResult* result = SCC(sub);
    printf("\nSCC:\n");
    SCCResultPrint(result);
    return 0;
}