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
 * This file contains the implementation of the functions required to comunicate all the structures between processes.
 * The provided functions are:
 *      void send_all(SubGraph *graph, SCCResult *result, int shrink,int dest)
 *      void recv_all(SubGraph **graph, SCCResult **result, int *shrink,int source)
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../Headers/Communication.h"
#include "../../Headers/TList.h"
#include "../../Headers/Constants.h"

/*
 * Function:  send_all
 * --------------------
 * Sends all the needed informations to the "dest" node
 *
 *  Parameters:
 *  graph: a reference to the SubGraph structure that has to be sent
 *  result: a referemce to the SCCResult structure that has to be sent
 *  shrink: a int representing the number of nodes that have been merged
 *  dest: the receiver of the communication
 */
void send_all(SubGraph *graph, SCCResult *result, int shrink, int dest)
{
    // Firstly we send the dimensions of the graph's matrix as an array
    int v = graph->nV, k = graph->nE, offset = graph->offset / WORK_LOAD;
    int size[4] = {v, k, offset, shrink};
    MPI_Send(size, 4, MPI_INT, dest, 0, MPI_COMM_WORLD);

    // Then we prepare a buffer that will contain the graph's matrix togheter
    // with the length of the internal array for each macrovertex.
    // We also count the sum of all the lengths for the next communication
    int dim = v * (k + 1);
    int *send_buf = (int *)malloc(dim * sizeof(int));
    int sum = 0, *length = (int *)malloc(v * sizeof(int));
    int *adjacent;
    
    for (int i = 0; i < v; i++)
    {
        adjacent = getEdges(graph, i);
        for (int j = 0; j < k; j++)
        {
            send_buf[i * (k + 1) + j] = adjacent[j];
        }
        length[i] = listCount(*(result->vertices[i]));
        send_buf[i * (k + 1) + k] = length[i];
        sum += length[i];
    }
    MPI_Send(send_buf, v * (k + 1), MPI_INT, dest, 0, MPI_COMM_WORLD);

    // In the end we collapse all the arrays of internal vertexes in one array of,
    // at this point, known size and we send it to the receiver. It's his job to rebuild
    // the structure one it arrives
    int adj[sum];
    for (int i = 0, count = 0; i < v; i++)
    {
        int *vertices = listToArray(*(result->vertices[i]));
        for (int j = 0; j < length[i]; j++, count++)
        {
            adj[count] = vertices[j];
        }
    }

    MPI_Send(adj, sum, MPI_INT, dest, 0, MPI_COMM_WORLD);

    free(send_buf);
    free(length);
}

/*
 * Function:  recv_all
 * --------------------
 * Recives all the needed informations from the "source" node
 *   and instantiates all the needed structures passed as arguments
 *
 *  Parameters:
 *  graph: a reference to the output SubGraph structure
 *  result: a referemce to the output SCCResult structure
 *  shrink: a reference to a int representing the number of nodes that have been merged
 *  source: the sender of the communication
 */
void recv_all(SubGraph **graph, SCCResult **result, int *shrink, int source)
{

    MPI_Status status;

    // Firstly we recive the dimensions of the graph's matrix as an array.
    //   We also update their values in the recived args and instantiate all
    //   the needed structures

    int size[4];
    MPI_Recv(size, 4, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
    int v = size[0], k = size[1];
    *shrink = size[3];

    // External var allocation
    *graph = createSubGraph(v, k, size[2]);
    *result = SCCResultCreate(v);

    // Then we prepare a buffer that will contain the graph's matrix togheter
    //   with the length of the internal array for each macrovertex and we
    //   recive what has been sent.

    int dim = v * (k + 1);
    int *recv_graph = (int *)malloc(dim * sizeof(int));
    MPI_Recv(recv_graph, v * (k + 1), MPI_INT, source, 0, MPI_COMM_WORLD, &status);

    // Then we split the recived matrix in the original graph and lengths array
    int sum = 0;
    int *matrix = (*graph)->adj, lengths[v];
    matrix[0] = recv_graph[0];
    for (int i = 1, j = 1, l = 0; i < v * (k + 1); i++)
    {
        if (i == k * (l + 1) + l)
        {
            // Adds to the lenght array the recived one
            int curr = recv_graph[i];
            lengths[l] = curr;
            sum += curr;
            l++;
        }
        else
        {
            // Adds to the external graph the recived values
            matrix[j] = recv_graph[i];
            j++;
        }
    }

    // In the end we prepare a buffer to store the whole array containing
    //   the internal vertexes and recive the sender's message in it.
    //   Then the last step is to split each array and populate the recived SCCResult

    int recv_internal[sum];
    MPI_Recv(recv_internal, sum, MPI_INT, source, 0, MPI_COMM_WORLD, &status);

    for (int i = 0, count = 0; i < v; i++)
    {
        for (int j = 0; j < lengths[i]; j++, count++)
        {
            SCCResultInsert(*result, i, recv_internal[count]);
        }
    }

    free(recv_graph);
}