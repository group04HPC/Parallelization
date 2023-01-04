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
 * This file contains the main function of the parallel version of the program. By means of MPI, it loads the
 * graph from files, distributes it among the nodes, and then executes the Tarjan's algorithm to find the
 * Strongly Connected Components.
 * Firstly each node executes Tarjan's algorithm on its own subgraph, and then it sends the results to the
 * following node. The following node merges the results with its own subgraph, applies Tarjan's algorithm
 * on the merged subgraph, and then sends the results to the next node. This process is repeated until the
 * last node receives the results from the previous node.
 * The last node computes the time required to execute the whole process, saves it on a file and saves the result
 * on another file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>
#include "../../Headers/SubGraph.h"
#include "../../Headers/SCCResult.h"
#include "../../Headers/Tarjan.h"
#include "../../Headers/Constants.h"
#include "../../Headers/Merge.h"
#include "../../Headers/Communication.h"

int nextAvailableRank(bool *values, int size, int rank); // returns the next available rank
int prevAvailableRank(bool *values, int size, int rank); // returns the previous available rank
void updateAvailableRanks(bool *values, int size);       // updates the array of available ranks
int countAvailableRanks(bool *values, int size);         // counts the number of available ranks

int main(int argc, char *argv[])
{

    /* MPI init */
    int rank, size;
    MPI_Status status;
    MPI_File fh;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start, end;

    /* subgraph creation */
    SubGraph *sub = createSubGraph(WORK_LOAD, WORK_LOAD * size, rank);
    int *edges = getEdges(sub, 0);

    /* filenames init */
    char filename[FILENAME_LENGTH], num[NUM_LENGTH];
    strcpy(filename, "Data/file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin", EXTENSION_LENGTH);

    start = MPI_Wtime();

    /* each process reads its own subgraph from its own file */
    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD * WORK_LOAD * size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, WORK_LOAD * WORK_LOAD * size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    /* Tarjan algorithm on the subgraph converted in list */
    ListGraph *list = createListGraphFromMatrix(sub);
    SCCResult *result = SCC(&list);
    destroySubGraph(sub);

    /*
     * If the number of processes is equal to 1 then it has already finished
     * otherwise it executes the content of this statement
     */
    if (size > 1)
    {
        int shrink = list->nV - result->nV, recivedShrink = 0, next, prev, curr_av = 1;
        SubGraph *receivedGraph = NULL;
        SCCResult *receivedResult = NULL, *mergedResult = NULL;
        ListGraph *receivedList = NULL, *mergedList = NULL;

        /* Creates an array with the available processes to which send the structures */
        bool values[size];
        for (int i = 0; i < size; i++)
            values[i] = true;

        /*
         * This loop is executed until all the processes have finished their execution
         * and have sent their results to the last process
         */
        while (curr_av > 0)
        {

            curr_av = countAvailableRanks(values, rank);

            if (curr_av % 2 == 0)
            {
                // send to next
                next = nextAvailableRank(values, size, rank);
                if (next != -1)
                {
                    /* generates the matrix subgraph from the list graph in order to send it */
                    sub = createMatrixGraphFromList(list);
                    send_all(sub, result, shrink, next);
                    break;
                }
            }
            else
            {
                // rcv from prev
                prev = prevAvailableRank(values, size, rank);

                recv_all(&receivedGraph, &receivedResult, &recivedShrink, prev);
                receivedGraph->offset = receivedGraph->offset / WORK_LOAD;
                receivedResult->offset = receivedGraph->offset;
                /* generates the list graph from the subgraph in order to work on it */
                receivedList = createListGraphFromMatrix(receivedGraph);

                /* merges both the result and the graph */
                mergedResult = mergeResults(receivedResult, result);
                mergedList = mergeGraphs(receivedList, list, recivedShrink, shrink, mergedResult);

                /* 
                 * applies Tarjan's algorithm on the merged graph and rescales both the result
                 * and the graph 
                 */
                result = SCC(&mergedList);

                /* combined the result of tarjan with the merged result */
                result = SCCResultCombine(result, mergedResult);

                list = mergedList;
                shrink = list->nV - result->nV;
            }

            updateAvailableRanks(values, size);
        }
    }

    end = MPI_Wtime() - start;

    // last process
    if (rank == size - 1)
    {
        printf("Tarjan excution time parallel: %f\n", end);

        /* saves the result on a file */
        FILE *f = fopen("Data/result.txt", "a+");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        fprintf(f, "\n%d\n", result->nMacroNodes);
        for (int i = 0; i < result->nMacroNodes; i++)
        {
            TList list = *result->vertices[i];
            fprintf(f, "%d ", listCount(list));
            while (list != NULL)
            {
                fprintf(f, "%d ", list->value);
                list = list->link;
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}

/**
 * Function: nextAvailableRank
 * ---------------------------
 * Returns the next available rank in the array of ranks
 *
 * Parameters:
 * values: array of ranks
 * size: size of the array
 * rank: current rank
 *
 * Returns:
 * the next available rank
 */
int nextAvailableRank(bool *values, int size, int rank)
{
    for (int i = rank + 1; i < size; i++)
    {
        if (values[i])
        {
            return i;
        }
    }
    return -1;
}

/**
 * Function: prevAvailableRank
 * ---------------------------
 * Returns the previous available rank in the array of ranks
 *
 * Parameters:
 * values: array of ranks
 * size: size of the array
 * rank: current rank
 *
 * Returns:
 * the previous available rank
 */
int prevAvailableRank(bool *values, int size, int rank)
{
    for (int i = rank - 1; i >= 0; i--)
    {
        if (values[i])
        {
            return i;
        }
    }
    return -1;
}

/**
 * Function: updateAvailableRanks
 * ------------------------------
 * Updates the array of ranks by setting to false the ranks that have already sent their results
 *
 * Parameters:
 * values: array of ranks
 * size: size of the array
 *
 */
void updateAvailableRanks(bool *values, int size)
{
    int lastTrue = -1;

    for (int i = 0; i < size; i++)
    {
        if (values[i] && lastTrue == -1)
        {
            lastTrue = i;
        }
        else if (values[i] && lastTrue != -1)
        {
            values[lastTrue] = false;
            lastTrue = -1;
        }
    }
}

/**
 * Function: countAvailableRanks
 * -----------------------------
 * Counts the number of available ranks in the array of ranks
 *
 * Parameters:
 * values: array of ranks
 * size: size of the array
 *
 * Returns:
 * the number of available ranks
 *
 */
int countAvailableRanks(bool *values, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (values[i])
        {
            count++;
        }
    }
    return count;
}