#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"
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
    // if (rank == 0){
        
    //     printf("Original graph:\n");
    //     printSubGraph(sub);
    //     printf("\nOriginal result:\n");
    //     SCCResultPrint(result);
    //     printf("\nRescaled result:\n");
    //     SCCResultPrint(rescaled);
    //     printf("\nRescaled graph:\n");
    //     printSubGraph(newGraph);
        
    // }

    int start = 0, stop = size/2;
    SubGraph* receivedGraph;
    SCCResult* receivedResult;

    /* All the processes will be considered */
    for (int i = 0; i < size/2 && start<=stop; i++){
        if (rank >= start && rank < stop){
            /* These ranks will send */
            send_all(newGraph, rescaled, rank+(stop-start));
        }
        
        if (rank >= stop && rank < size){
            /* These ranks will receive */
            if (receivedGraph != NULL){
                destroySubGraph(receivedGraph);
            }
            if (receivedResult != NULL){
                SCCResultDestroy(receivedResult);
            }
            recv_all(&receivedGraph, &receivedResult, rank-(stop-start));
            printf("\nReceived graph:\n");
            printSubGraph(receivedGraph);
        }
        start = stop;
        stop += (size-stop)/2;
    }

    if (rank == size-2){
        send_all(newGraph, rescaled, rank+(stop-start));
    }

    if(rank == size-1){
        if (receivedGraph != NULL){
        destroySubGraph(receivedGraph);
        }
        if (receivedResult != NULL){
            SCCResultDestroy(receivedResult);
        }
        recv_all(&receivedGraph, &receivedResult, rank-(stop-start));
        printf("\nReceived graph:\n");
        printSubGraph(receivedGraph);
    }

    printf("okok %d\n", rank);

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