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
 * This file contains the main function needed to compare two SCCResult structures after the serial and parallel execution.
 * The provided functions are:
 *      int main(int argc, char* argv[])
 */

#include <stdio.h>
#include <stdlib.h>
#include "../Headers/SCCResult.h"

/*
 * Function:  main
 * --------------------
 *  The main function used to compare two SCCResult structures after the serial and parallel execution
 *
 *  Parameters:
 *  argc: the number of arguments
 *  argv: the arguments
 *
 *  Returns: the exit code of the main function
 */
int main(int argc, char *argv[])
{

    FILE *fp = fopen("Data/result.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file -- compare\n");
    }

    int nMacroNodes, value, len;
    fscanf(fp, "%d", &nMacroNodes);
    SCCResult *result = SCCResultCreate(nMacroNodes);
    for (int i = 0; i < nMacroNodes; i++)
    {
        fscanf(fp, "%d", &len);
        for (int j = 0; j < len; j++)
        {
            fscanf(fp, "%d", &value);
            SCCResultInsert(result, i, value);
        }
    }

    fscanf(fp, "%d", &nMacroNodes);

    SCCResult *result2 = SCCResultCreate(nMacroNodes);
    for (int i = 0; i < nMacroNodes; i++)
    {
        fscanf(fp, "%d", &len);
        for (int j = 0; j < len; j++)
        {
            fscanf(fp, "%d", &value);
            SCCResultInsert(result2, i, value);
        }
    }

    fclose(fp);

    printf("=============RESULTS=============\n");

    if (result->nMacroNodes != result2->nMacroNodes)
    {
        printf("Bad result, %d, %d\n", result->nMacroNodes, result2->nMacroNodes);
        return 1;
    }

    for (int i = 0; i < result->nMacroNodes; i++)
    {
        TList list = *result->vertices[i];
        TList list2 = *result2->vertices[i];
        while (list != NULL)
        {
            if (list->value != list2->value)
            {
                printf("Bad result, %d, %d\n", list->value, list2->value);
                return 1;
            }
            list = list->link;
            list2 = list2->link;
        }
    }

    printf("Completed Successfully!\n");

    return 0;
}
