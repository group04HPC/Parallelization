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
    if (sub->nV != result->nV)
    sub = rescaleGraph(sub, result);
    shrink = sub->nV - result->nV;


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

            mergedResult = mergeResults(receivedResult, result);

            mergedGraph = mergeGraphs(receivedGraph, sub, recivedShrink, shrink, mergedResult);

            if (even && i+1 == size/2){
                sub = mergedGraph;
                result = mergedResult;
                break;
            }

            result = SCCResultRescale(SCC(mergedGraph));
            shrink=sub->nV-result->nV;

            result = SCCResultCombine(result, mergedResult);

            if (sub->nV != result->nV){
                sub = rescaleGraph(sub, result);
                shrink = sub->nV - result->nV;
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

            mergedResult = mergeResults(receivedResult, result);
            SCCResultDestroy(result);
            result = mergedResult;

            mergedGraph = mergeGraphs(receivedGraph, sub, recivedShrink, shrink, mergedResult);
            destroySubGraph(sub);
            sub = mergedGraph;

        }
    }

    if (rank == size-1){
        printf("Final graph:\n");
        printSubGraph(sub);
        printf("\nFinal result:\n");
        SCCResultPrint(result);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
