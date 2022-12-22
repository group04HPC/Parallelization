#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../Constants.h"
#include "../DataStructures/SubGraph.h"

int main(int argc, char* argv[]){
    int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    int* edges = getEdges(sub, 0);
    int *matrix = NULL;
    if (rank==0){
        matrix = (int*)malloc(WORK_LOAD*WORK_LOAD*size*size*sizeof(int));
    }

    char filename[20], num[2];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, 10);
    strncat(filename, ".bin",14);

    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD*WORK_LOAD*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    MPI_Gather(edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, matrix, WORK_LOAD*WORK_LOAD*size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank==0){
        FILE* fp = fopen("matrix.txt", "w+");
        if (fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }
        fprintf(fp, "%d\n", WORK_LOAD*size);
        for (int i=0; i<WORK_LOAD*size; i++)
            for (int j=0; j<WORK_LOAD*size; j++)
                fprintf(fp, "%d ", matrix[i*WORK_LOAD*size+j]);
            fprintf(fp, "\n");
        fclose(fp);
    }

    destroySubGraph(sub);
    MPI_Finalize();
    return 0;
}