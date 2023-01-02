#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../../Headers/Constants.h"
#include "../../Headers/SubGraph.h"

int main(int argc, char* argv[]){
    int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    generateRandomSubGraph(sub, MIN_EDGES_PARALLEL, MAX_EDGES_PARALLEL);
    int* edges = getEdges(sub, 0);

    char filename[FILENAME_LENGTH], num[NUM_LENGTH];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin", EXTENSION_LENGTH);

    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD*WORK_LOAD*size, MPI_SEEK_SET);
    MPI_File_write(fh, edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    destroySubGraph(sub);
    MPI_Finalize();
    return 0;
}