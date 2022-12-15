#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"

#define NUM_VERTICES 5

int main(int argc, char* argv[]){

int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    SubGraph* sub = createSubGraph(NUM_VERTICES, NUM_VERTICES*size, rank);
    int* edges = getEdges(sub, 0);

    char filename[MAX_FILENAME_LENGTH], num[MEDIUM_FILENAME_LENGTH-MIN_FILENAME_LENGTH];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin",MAX_FILENAME_LENGTH);

    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, NUM_VERTICES*NUM_VERTICES*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, NUM_VERTICES*NUM_VERTICES*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    if (rank == 0){
        printf("Original Graph:\n");
        printSubGraph(sub);
        SCCResult* result = SCC(sub);
        printf("\nSCC:\n");
        SCCResultPrint(result);
    }

    destroySubGraph(sub);
    MPI_Finalize();
    return 0;

}