/* 
 * Course: High Performance Computing 2022/2023 
 * 
 * Lecturer: Francesco Moscato    fmoscato@unisa.it 
 *
 * Group: 
 * Ferrara Grazia   0622701901  g.ferrara75@studenti.unisa.it
 * Franco Paolo     0622701993  p.franco9@studenti.unisa.it
 *
 * Copyright (C) 2023 - All Rights Reserved 
 *
 * This file is part of Project Assignment 2022/2023. 
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with Project Assignment 2022/2023.  If not, see http://www.gnu.org/licenses/. 
 * 
 * Requirements of the assignment:
 * Provide a parallell version of the Tarjan's algorithm to find Strongly Connected Components in a Graph. 
 * The implementation MUST use a message passing paradigm, and has to be implemented by using MPI. 
 * Students MUST store and load the input graph FROM FILES. The whole graph MUST be distributed on files 
 * on each node (i.e.: the whole graph cannot be stored on a single (even replicated) file). Good Graph 
 * dimensions are greater than 4GB of data. Students have to choose the proper data structure to 
 * represent the graph in memory.
 * 
 * Purpose of the file:
 * This file contains a main to generate a random graph and store it in a file by means of MPI. Each process 
 * generates a random subgraph and stores it in a binary file. 
 * 
 */

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

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* each process creates its own subgraph */
    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    generateRandomSubGraph(sub, MIN_EDGES_PARALLEL, MAX_EDGES_PARALLEL);
    int* edges = getEdges(sub, 0);

    char filename[sizeof "mpidir/Data/file10.bin\0"];
    snprintf(filename, sizeof filename, "mpidir/Data/file%02d.bin", rank);

    /* Each process saves its own subgraph on a binary file */
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