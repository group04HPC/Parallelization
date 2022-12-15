#ifndef COMUNICATION_H
#define COMUNICATION_H

#include <mpi.h>
#include "Comunication.c"

void send_all(int *graph, int **internal, int *lengths, int v, int k, int dest);
void recv_all(int **graph, int ***internal, int **lengths, int *v_ext, int *k_ext, int source);

#endif