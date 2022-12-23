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
    if (sub->nV != result->nV)
    {
        sub = rescaleGraph(sub, result);
    }
    // SubGraph* rescaledGraph = rescaleGraph(sub, result, rank);
    // destroySubGraph(sub);
    // sub = rescaledGraph;

    int start = 0, stop = even ? size/2 : (size-1)/2,recivedShrink;
    SubGraph *receivedGraph = NULL, *mergedGraph = NULL;
    SCCResult *receivedResult = NULL, *mergedResult = NULL, *combinedResult=NULL;

    /* All the processes will be considered */
    int i=0,shrink=sub->nE-; 
    while(rank >= start && start != stop){

        if (rank >= start && rank < stop){
            /* These ranks will send */
            send_all(sub, result, shrink, rank + (stop - start));
            destroySubGraph(sub);
            SCCResultDestroy(result);
        }

        if (rank >= stop && rank < stop+(stop-start)){
            /* These ranks will receive */
            if (receivedGraph != NULL)
                destroySubGraph(receivedGraph);
            if (receivedResult != NULL)
                SCCResultDestroy(receivedResult);
            if (mergedGraph != NULL)
                destroySubGraph(mergedGraph);
            if (mergedResult != NULL)
                SCCResultDestroy(mergedResult);
            if (combinedResult != NULL)
                SCCResultDestroy(combinedResult);

            recv_all(&receivedGraph, &receivedResult,&recivedShrink rank-(stop-start));

            printf("My graph:\n");
            printSubGraph(sub);
            printf("My result:\n");
            SCCResultPrint(result);

            printf("Received graph:\n");
            printSubGraph(receivedGraph);
            printf("Received result:\n");
            SCCResultPrint(receivedResult);


            mergedResult = mergeResults(receivedResult, result,recivedShrink,shrink);
            printf("Merged result:\n");
            SCCResultPrint(mergedResult);

            mergedGraph = mergeGraphs(receivedGraph, sub, receivedResult, result, mergedResult);
            destroySubGraph(sub);
            sub = mergedGraph;
            printf("Merged graph:\n");
            printSubGraph(sub);

            SCCResultDestroy(result);
            result = SCCResultRescale(SCC(mergedGraph));
            printf("Tarjan result:\n");
            SCCResultPrint(result);
            shrink=sub->nV-result->nV;

            combinedResult = SCCResultCombine(result, mergedResult);
            printf("Combined result:\n");
            SCCResultPrint(combinedResult);

            if (sub->nV != combinedResult->nV){
                sub = rescaleGraph(sub, result, rank-(stop-start));
            }
            printf("Rescaled graph:\n");
            printSubGraph(sub);

            SCCResultDestroy(result);
            result = combinedResult;
        }

        if ((even ? size : size-1) == stop+1)
            break;

        start = stop;
        
        stop += ((even ? size : size-1) - stop)/2;

        i++;
    }

    printf("========================================\n");

    
    if (!even && size > 1){

        if (rank == size - 2){
            send_all(sub, combinedResult,shrink, size-1);
        }

        if (rank == size - 1){

            recv_all(&receivedGraph, &receivedResult,&recivedShrink, size-2);
            printf("Sub:\n");
            printSubGraph(sub);
            printf("Received graph:\n");
            printSubGraph(receivedGraph);

            mergedResult = mergeResults(receivedResult, result, recivedShrink,shrink);
            printf("Merged result:\n");
            SCCResultPrint(mergedResult);

            mergedGraph = mergeGraphs(receivedGraph, sub, receivedResult, result, mergedResult);
            destroySubGraph(sub);
            sub = mergedGraph;
            printf("Final graph:\n");
            printSubGraph(sub);

            SCCResultDestroy(result);
            result = SCCResultRescale(SCC(mergedGraph));
            printf("Result:\n");
            SCCResultPrint(result);

            combinedResult = SCCResultCombine(result, mergedResult);
            printf("Combined result:\n");
            SCCResultPrint(combinedResult);

            if (sub->nV != combinedResult->nV){
                sub = rescaleGraph(sub, result);
            }
            printf("Rescaled graph:\n");
            printSubGraph(sub);

            SCCResultDestroy(result);
            result = combinedResult;
        
        }
    }

    // if (rank == size-1){
    //     printf("Final graph:\n");
    //     printSubGraph(sub);
    //     printf("\nFinal result:\n");
    //     SCCResultPrint(combinedResult);
    // }

    SCCResultDestroy(result);
    destroySubGraph(sub);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
