#include <mpi.h>
#include <stdio.h>
#include "../../Headers/Communication.h"
#include "../../Headers/TList.h"
#include "../../Headers/Constants.h"

/*
 * Function:  send_all
 * --------------------
 * Sends all the needed informations to the "dest" node
 *
 *  *graph: the graph's matrix
 *  **internal: the array of arrays of internal vertexes
 *  *lengths: the lengths of each array of internal vertexes
 *  v: the number of vertexes of the graph
 *  k: the number of adjacent vertexes
 *  dest: the receiver of the communication
 */
void send_all(SubGraph *graph, SCCResult *result, int shrink,int dest)
{
    // Signature of the function MPI_Send that will be used here
    // int MPI_Send(const void *buf, int count, MPI_Datatype datatype,
    //              int dest,int tag, MPI_Comm comm)

    // Firstly we send the dimensions of the graph's matrix as an array
    int v = graph->nV, k = graph->nE, offset = graph->offset/WORK_LOAD;
    int size[4] = {v, k, offset,shrink};
    MPI_Send(size, 4, MPI_INT, dest, 0, MPI_COMM_WORLD);

    // Then we prepare a buffer that will contain the graph's matrix togheter
    // with the length of the internal array for each macrovertex.
    // We also count the sum of all the lengths for the next communication
    int dim = v * (k + 1);
    int *send_buf = (int*)malloc(dim * sizeof(int));
    int sum = 0, length[v];
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
}

/*
 * Function:  recv_all
 * --------------------
 * Recives all the needed informations from the "source" node
 *   and instantiates all the needed structures passed as arguments
 *
 *  **graph: a reference to the graph's matrix
 *  ***internal: a reference to the array of arrays of internal vertexes
 *  **lengths: a reference to the lengths of each array of internal vertexes
 *  *v: a reference to the number of vertexes of the graph
 *  *k: a reference to the number of adjacent vertexes
 *  source: the sender of the communication
 */
void recv_all(SubGraph **graph, SCCResult **result, int *shrink,int source)

// void recv_all(int **graph, int ***internal, int **lengths, int *v_ext, int *k_ext, int source)
{
    // Signature of the function MPI_Recv that will be used here
    // int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
    //              int source, int tag, MPI_Comm comm, MPI_Status *status)

    MPI_Status status;

    // Firstly we recive the dimensions of the graph's matrix as an array.
    //   We also update their values in the recived args and instantiate all
    //   the needed structures

    int size[4];
    MPI_Recv(size, 4, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
    int v = size[0], k = size[1];
    *shrink=size[3];
    // External var allocation
    *graph = createSubGraph(v, k, size[2]);
    *result = SCCResultCreate(v);

    // Then we prepare a buffer that will contain the graph's matrix togheter
    //   with the length of the internal array for each macrovertex and we
    //   recive what has been sent.

    int dim = v * (k + 1);
    int* recv_graph = (int*)malloc(dim * sizeof(int));
    MPI_Recv(recv_graph, v * (k + 1), MPI_INT, source, 0, MPI_COMM_WORLD, &status);

    // Then we split the recived matrix in the original graph and lengths array
    int sum = 0;
    int *matrix = (*graph)->adj, lengths[v];
    matrix[0] = recv_graph[0];
    for (int i = 1, j = 1, l = 0; i < v * (k + 1); i++)
    {
        if (i == k * (l + 1) + l)
        {
            // Adds to the lenght array the recived length
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

    // In the end we prepare a buffer where to store all the whole array containing
    //   the internal vertexes and recive the sender's message in it.
    //   Then the last step is to split each array and populate the given array

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