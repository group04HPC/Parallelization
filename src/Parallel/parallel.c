#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"
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

    /* now we send both the rescaled structures (the SCC result and the updated graph) on MPI */
    


    SCCResultDestroy(result);
    //SCCResultDestroy(rescaled);
    destroySubGraph(newGraph);
    destroySubGraph(sub);
    //free(edges);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}