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
    SCCResult* result = SCC(sub);
    /* macro-nodes creation */
    SCCResult* rescaled = SCCResultRescale(result);
    /* new updated subgraph replacing nodes with the obtained macro-nodes */
    SubGraph* newGraph = rescaleGraph(sub,rescaled,rank);

    /* debug print to check if it's all ok */
    if (rank == 1){
        
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
    SubGraph *receivedGraph = NULL, *mergedGraph = NULL;
    SCCResult *receivedResult = NULL, *mergedResult = NULL, *retracedResult=NULL;

    /* All the processes will be considered */
    int i=0; 
    while(rank >= start && start != stop){
        
        if (rank == 5){
            printf("Sono 5  %d\n", i);
        }
        
        printf("INVIO-----i:%d, rank:%d, prima condizione: %d, seconda condizione: %d\n", i, rank, rank >= start, rank < stop);
        if (rank >= start && rank < stop){
            printf("INVIO-------i:%d, start: %d, stop: %d, rank:%d\n", i, start, stop, rank);
            /* These ranks will send */
            if (rank == 5){
                printf("Sono 5 e ho sto per inviare - %d\n", i);
            }
            send_all(newGraph, rescaled, rank+(stop-start));
            if (rank == 5){
                printf("Sono 5 e ho inviato - %d\n", i);
            }
        }

        printf("RICEVO-----i:%d, rank:%d, prima condizione: %d, seconda condizione: %d\n", i, rank, rank >= stop, rank < stop+(stop-start));
        if (rank >= stop && rank < stop+(stop-start)){
            /* These ranks will receive */
            printf("RICEVO-------i:%d, start: %d, stop: %d, rank:%d\n",i, start, stop, rank);
            if (receivedGraph != NULL){
                destroySubGraph(receivedGraph);
            }

            if (rank == 5){
                printf("SONO 5 E distruggo il grafo - %d\n", i);
            }
            
            if (receivedResult != NULL){
                SCCResultDestroy(receivedResult);
            }

            if (rank == 5){
                printf("SONO 5 E  distruggo il result - %d\n", i);
            }

            printf("RANK: %d\n", rank-(stop-start));
            recv_all(&receivedGraph, &receivedResult, rank-(stop-start));

            if (rank == 5){
                printf("SONO 5 E faccio la merge - %d\n", i);
            }

            mergedGraph = mergeGraphs(receivedGraph, newGraph, receivedResult, rescaled);
            mergedResult = mergeResults(receivedResult, rescaled);
            
            if (rank == 5){
                printf("SONO 5 E faccio Tarjan - %d\n", i);
            }
            result = SCC(mergedGraph);
            if (rank == 5){
                printf("SONO 5 E ho fatto Tarjan - %d\n", i);
            }
            rescaled = SCCResultRescale(result);
            if (rank == 5){
                printf("SONO 5 E ho riscalato Tarjan - %d\n", i);
            }
            retracedResult = retraceResult(rescaled, mergedResult, rank);

            if (rank == 5){
                printf("SONO 5 E ho ritracciato il grafo - %d\n", i);
            }
            newGraph = rescaleGraph(mergedGraph, rescaled, rank);

            if (rank == 5){
                printf("SONO 5 e ho finito - %d\n", i);
            }
        }

        if ((even ? size : size-1) == stop+1) {
            break;
        }

        start = stop;
        
        stop += ((even ? size : size-1) - stop)/2;

        i++;
    }

    printf("SONO FUORI DAL WHILE %d\n", rank);

    if (!even && size > 1){
        if (rank == size - 2){
            printf("INVIO-------rank:%d\n", rank);
            printf("SONO QIO DENTRO");
            send_all(newGraph, rescaled, size-1);
        }
        if (rank == size - 1){
            printf("RICEVO-------rank:%d\n", rank);
            recv_all(&receivedGraph, &receivedResult, size-2);

            mergedGraph = mergeGraphs(receivedGraph, newGraph, receivedResult, rescaled);
            mergedResult = mergeResults(receivedResult, rescaled);
            
            result = SCC(mergedGraph);
            rescaled = SCCResultRescale(result);
            retracedResult = retraceResult(rescaled, mergedResult, rank);
            
            newGraph = rescaleGraph(mergedGraph, rescaled, rank);
        }
    }

    SCCResultDestroy(result);
    SCCResultDestroy(rescaled);
    destroySubGraph(newGraph);
    destroySubGraph(sub);
    //free(edges);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
