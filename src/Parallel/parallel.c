#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"
#include "../Communication/Merge.h"
#include "../Communication/Communication.h"

int main(int argc, char* argv[]){

    /* MPI init */
    int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    bool even = size % 2 == 0 ? true : false;

    /* subgraph creation */
    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    int* edges = getEdges(sub, 0);

    /* filenames init */
    char filename[FILENAME_LENGTH], num[NUM_LENGTH];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin", EXTENSION_LENGTH);

    /* each process reads its own subgraph from its own file */
    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD*WORK_LOAD*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    /* Tarjan algorithm */
    SCCResult* result = SCCResultRescale(SCC(sub));
    int shrink = 0;
    //printf("%d nV, %d nMacroNodes, %d rank\n", sub->nV, result->nV, rank);
    shrink = sub->nV - result->nV;
    if (sub->nV != result->nV)
        sub = rescaleGraph(sub, result);
    //printf("%d shrink, %d rank\n", shrink, rank);

    int start = 0, stop = even ? size/2 : (size-1)/2, recivedShrink=0;
    SubGraph *receivedGraph = NULL, *mergedGraph = NULL;
    SCCResult *receivedResult = NULL, *mergedResult = NULL;

    /* All the processes will be considered */
    int i=0; 
    while(rank >= start && start != stop){

        if (rank >= start && rank < stop){
            /* These ranks will send */
            send_all(sub, result, shrink, rank + (stop - start));
            destroySubGraph(sub);
            SCCResultDestroy(result);
        }

        if (rank >= stop && rank < stop+(stop-start)){
            /* These ranks will receive */

            recv_all(&receivedGraph, &receivedResult, &recivedShrink, rank-(stop-start));

            // printf("My subgraph:\n");
            // printSubGraph(sub);

            receivedResult->offset = receivedGraph->offset;
            // printf("Received Result: %d\n", receivedResult->offset);
            // SCCResultPrint(receivedResult);
            // printf("Received graph: %d\n", receivedGraph->offset);
            // printSubGraph(receivedGraph);
            mergedResult = mergeResults(receivedResult, result);

            // printf("Merged Result: %d\n", mergedResult->offset);
            // SCCResultPrint(mergedResult);

            mergedGraph = mergeGraphs(receivedGraph, sub, recivedShrink, shrink, mergedResult);
            sub = mergedGraph;

            // printf("Merged graph:, %d\n", mergedGraph->offset);
            // printSubGraph(mergedGraph);

            result = SCCResultRescale(SCC(mergedGraph));
            shrink = sub->nV-result->nV;

            // printf("Tarjan:\n");
            // SCCResultPrint(result);

            result = SCCResultCombine(result, mergedResult);

            // printf("Combined result:\n");
            // SCCResultPrint(result);

            if (sub->nV != result->nV){
                sub = rescaleGraph(mergedGraph, result);
                // printf("Rescaled graph:\n");
                // printSubGraph(sub);
            }else{
                destroySubGraph(sub);
                sub = mergedGraph;
            }

        }

        if ((even ? size : size-1) == stop+1){
            break;
        }

        start = stop;
        
        stop += ((even ? size : size-1) - stop)/2;

        i++;
    }
    
    if (!even && size > 1){

        if (rank == size - 2){
            send_all(sub, result, shrink, size-1);
            destroySubGraph(sub);
            SCCResultDestroy(result);
        }

        if (rank == size - 1){

            recv_all(&receivedGraph, &receivedResult, &recivedShrink, size-2);

            printf("Received shrink: %d\n", recivedShrink);

            printf("Received Result: \n");
            SCCResultPrint(receivedResult);

            printf("Received graph: \n");
            printSubGraph(receivedGraph);


            printf("My subgraph:\n");
            printSubGraph(sub);

            printf("My result:\n");
            SCCResultPrint(result);

            receivedResult->offset = receivedGraph->offset;
            mergedResult = mergeResults(receivedResult, result);
            
            printf("Merged result:\n");
            SCCResultPrint(mergedResult);

            mergedGraph = mergeGraphs(receivedGraph, sub, recivedShrink, shrink, mergedResult);

            printf("Merged graph:, %d\n", mergedGraph->offset);
            printSubGraph(mergedGraph);

            result = SCCResultRescale(SCC(mergedGraph));
            printf("Tarjan:\n");
            SCCResultPrint(result);
            shrink=mergedGraph->nV-result->nV;

            result = SCCResultCombine(result, mergedResult);

            if (sub->nV != result->nV){
                sub = rescaleGraph(mergedGraph, result);
            }else{
                destroySubGraph(sub);
                sub = mergedGraph;
            }

        }
    }

    if (rank == size-1){
        printf("Final graph:\n");
        printSubGraph(sub);
        printf("\nFinal result:\n");
        int max = SCCResultGetLastElement(result);
        printf("Max: %d\n", max);
        SCCResultPrint(result);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
