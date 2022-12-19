#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"

#define D 5

int main(int argc, char* argv[]){

    int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    SubGraph* sub = createSubGraph(D, D*size, rank);
    int* edges = getEdges(sub, 0);

    char filename[20], num[2];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, 10);
    strncat(filename, ".bin",14);

    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, D*D*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, D*D*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    SCCResult* result = SCC(sub);
    SCCResult* rescaled = SCCResultRescale(result);
    SubGraph* newGraph = rescaleGraph(sub,rescaled,rank);

    if (rank == 2){
        
        printf("Original graph:\n");
        printSubGraph(sub);
        printf("\nOriginal result:\n");
        SCCResultPrint(result);
        printf("\nRescaled result:\n");
        SCCResultPrint(rescaled);
        printf("\nRescaled graph:\n");
        printSubGraph(newGraph);
        //A questo punto si possono inviare entrambe le strutture riscalate su mpi

    }

    SCCResultDestroy(result);
    //SCCResultDestroy(rescaled);
    destroySubGraph(newGraph);
    destroySubGraph(sub);
    //free(edges);

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}