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
 *  The main function used to execute Tarjan's algorithm in serial.
 * The provided functions are:
 *      int main(int argc, char* argv[])
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Headers/SubGraph.h"
#include "../../Headers/SCCResult.h"
#include "../../Headers/Tarjan.h"
#include "../../Headers/Constants.h"

/*
 * Function:  main
 * --------------------
 *  The main function used to execute Tarjan's algorithm in serial.
 *
 *  Parameters:
 *  argc: the number of arguments
 *  argv: the arguments
 *
 *  Returns: the exit code of the main function
 */
int main(int argc, char *argv[])
{

    int size, value;

    double total_time_spent = 0.0, read_time_spent = 0.0, tarjan_time_spent = 0.0;

    clock_t begin = clock();

    FILE *fp = fopen("mpidir/Data/matrix.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file in Serial.c\n");
        return 1;
    }
    fscanf(fp, "%d", &size);
    ListGraph *list = ListGraphCreate(size, size, 0);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++){
            fscanf(fp, "%d", &value);
            if (value == 1)
                insertListGraph(list, i, j);
        }
    fclose(fp);

    clock_t end = clock();
    read_time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    begin = clock();
    SCCResult *result = SCC(&list);
    end = clock();
    tarjan_time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    // FILE *fp2 = fopen("mpidir/Data/result.txt", "w+");
    // if (fp2 == NULL)
    // {
    //     printf("Error opening file in Serial.c\n");
    //     return 1;
    // }
    // fprintf(fp2, "%d\n", result->nMacroNodes);
    // for (int i = 0; i < result->nMacroNodes; i++)
    // {
    //     TList list = *result->vertices[i];
    //     fprintf(fp2, "%d ", listCount(list));
    //     while (list != NULL)
    //     {
    //         fprintf(fp2, "%d ", list->value);
    //         list = list->link;
    //     }
    //     fprintf(fp2, "\n");
    // }
    // fclose(fp2);

    total_time_spent = read_time_spent + tarjan_time_spent;

    printf("%f,%f,%f", read_time_spent, tarjan_time_spent, total_time_spent);

    // FILE *fp3 = fopen("mpidir/Data/time.txt", "a+");
    // if (fp3 == NULL)
    // {
    //     printf("Error opening file in Serial.c\n");
    //     return 1;
    // }
    // fprintf(fp3, "workload: %d\tmin: %d\tmax: %d\n", size, MIN_EDGES_PARALLEL, MAX_EDGES_PARALLEL);
    // fprintf(fp3, "serial\n");
    // fprintf(fp3, "read graph: %f\n", read_time_spent);
    // fprintf(fp3, "tarjan result: %f\n", tarjan_time_spent);
    // fprintf(fp3, "write result: %f\n", write_time_spent);
    // fprintf(fp3, "total time: %f\n", total_time_spent);
    // fclose(fp3);

    // printf("Total excution time serial: %f\n", total_time_spent);

    SCCResultDestroy(result);

    return 0;
}