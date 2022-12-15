#include <stdio.h>
#include <stdlib.h>
#include "DataStructures/SubGraph.h"
#include "Tarjan.h"

#define NUM_VERTICES 5
#define MIN_EDGES 2
#define MAX_EDGES 3

int main(int argc, char* argv[]){
    SubGraph* sub = createSubGraph(NUM_VERTICES, NUM_VERTICES, 0);
    printf("Original Graph:\n");
    if (MIN_EDGES < MAX_EDGES && MAX_EDGES < NUM_VERTICES){
        generateRandomSubGraph(sub, MIN_EDGES, MAX_EDGES);
        printSubGraph(sub);
        SCC(sub);
    } else {
        printf("Invalid number of edges. Please choose a number of edges between %d and %d.", 0, NUM_VERTICES);
    }
    return 0;
}