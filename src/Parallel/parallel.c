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

int power(int base, int exp);

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
    SCCResult* result = SCC(sub);
    /* macro-nodes creation */
    SCCResult* rescaled = SCCResultRescale(result);
    /* new updated subgraph replacing nodes with the obtained macro-nodes */
    SubGraph* newGraph = rescaleGraph(sub,rescaled,rank);

    /* debug print to check if it's all ok */
    if (rank == 0){
        
        printf("Original graph:\n");
        printSubGraph(sub);
        printf("\nOriginal result:\n");
        SCCResultPrint(result);
        printf("\nRescaled result:\n");
        SCCResultPrint(rescaled);
        printf("\nRescaled graph:\n");
        printSubGraph(newGraph);
        
    }

    int start = 0, stop = even ? size/2 : (size-1)/2;
    SubGraph* receivedGraph;
    SCCResult* receivedResult;

    // /* All the processes will be considered */
    // int i=0; 
    // while(rank >= start && start != stop){
        
    //     printf("INVIO-----i:%d, rank:%d, prima condizione: %d, seconda condizione: %d\n", i, rank, rank >= start, rank < stop);
    //     if (rank >= start && rank < stop){
    //         printf("INVIO-------i:%d, start: %d, stop: %d, rank:%d\n", i, start, stop, rank);
    //         /* These ranks will send */
    //         send_all(newGraph, rescaled, rank+(stop-start));
    //     }

    //     printf("RICEVO-----i:%d, rank:%d, prima condizione: %d, seconda condizione: %d\n", i, rank, rank >= stop, rank < size);
    //     if (rank >= stop && rank < stop+(stop-start)){
    //         /* These ranks will receive */
    //         printf("RICEVO-------i:%d, start: %d, stop: %d, rank:%d\n",i, start, stop, rank);
    //         if (receivedGraph != NULL){
    //             destroySubGraph(receivedGraph);
    //         }
    //         if (receivedResult != NULL){
    //             SCCResultDestroy(receivedResult);
    //         }
    //         recv_all(&receivedGraph, &receivedResult, rank-(stop-start));
    //         printf("\nReceived graph:\n");
    //         printSubGraph(receivedGraph);
    //     }

    //     if ((even ? size : size-1) == stop+1) {
    //         break;
    //     }

    //     start = stop;
        
    //     stop += ((even ? size : size-1) - stop)/2;

    //     i++;
    // }

    // MPI_Barrier(MPI_COMM_WORLD);

    // if (!even && size > 1){
        
    //     if (rank == size - 2){
    //         printf("INVIO-------rank:%d\n", rank);
    //         send_all(newGraph, rescaled, size-1);
    //     }
    //     if (rank == size - 1){
    //         printf("RICEVO-------rank:%d\n", rank);
    //         recv_all(&receivedGraph, &receivedResult, size-2);
    //         printf("\nReceived graph:\n");
    //         printSubGraph(receivedGraph);
    //     }
    // }

    if(rank == 0){
        send_all(newGraph, rescaled, 1);
    }

    if(rank == 1){
        SubGraph* receivedGraph;
        SCCResult* receivedResult;
        recv_all(&receivedGraph, &receivedResult, 0);
        SCCResult* mergedResult = mergeResults(receivedResult, rescaled);
        SubGraph* mergedGraph = mergeGraphs(receivedGraph, newGraph, receivedResult, rescaled);
        printf("\nMerged graph:\n");
        printSubGraph(mergedGraph);
        printf("\nMerged result:\n");
        SCCResultPrint(mergedResult);
    }

    SCCResultDestroy(result);
    //SCCResultDestroy(rescaled);
    destroySubGraph(newGraph);
    destroySubGraph(sub);
    //free(edges);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}

int power(int base, int exp){
    int result = 1;
    for (int i = 0; i < exp; i++){
        result *= base;
    }
    return result;
}