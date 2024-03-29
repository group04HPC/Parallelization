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
 * This file contains the definition of the TArray data structure.
 * TArray is a data structure that represents a stack of integers.
 *
 */

#ifndef TARRAY_H
#define TARRAY_H

typedef struct TArray
{
    int *arr;
    int size;
} TArray;

TArray *stackCreate(int size);        /* Creates the TArray structure, given a size. */
void stackDestroy(TArray *a);         /* Destroys the TArray structure. */
int stackPop(TArray *t);              /* Pops the last element from the stack. */
void stackPush(TArray *a, int value); /* Pushes a value on the stack. */
int stackTop(TArray *a);              /* Returns the last element of the stack. */
int arrayGet(TArray *a, int index);   /* Returns the element at the given index. */
int stackTop(TArray *a);              /* Returns the last element of the stack. */
bool stackIsEmpty(TArray *a);         /* Returns true if the stack is empty, false otherwise. */

#endif