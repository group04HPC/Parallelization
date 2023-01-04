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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/TArray.h"

/**
 * Function: stackPop
 * ------------------
 * Removes the top element from the stack and returns it.
 *
 * Parameters:
 * a: the stack from which the element has to be removed.
 *
 */
int stackPop(TArray *a)
{
    return a->arr[--a->size];
}

/**
 * Function: stackCreate
 * ---------------------
 * Creates a new stack of integers.
 *
 * Parameters:
 * size: the size of the stack.
 *
 * Returns:
 * a pointer to the newly created stack.
 */
TArray *stackCreate(int size)
{
    TArray *result = malloc(sizeof(TArray));
    result->size = 0;
    result->arr = malloc(sizeof(int) * size);
    return result;
}

/**
 * Function: stackDestroy
 * ----------------------
 * Frees the memory allocated for the stack.
 *
 * Parameters:
 * a: the stack to be destroyed.
 *
 */
void stackDestroy(TArray *a)
{
    free(a->arr);
    free(a);
}

/**
 * Function: arrayGet
 * ------------------
 * Returns the element at the specified index.
 *
 * Parameters:
 * a: the stack from which the element has to be retrieved.
 * index: the index of the element to be retrieved.
 *
 * Returns:
 * the element at the specified index.
 *
 */
int arrayGet(TArray *a, int index)
{
    return a->arr[index];
}

/**
 * Function: stackTop
 * ------------------
 * Returns the top element of the stack.
 *
 * Parameters:
 * a: the stack from which the element has to be retrieved.
 *
 * Returns:
 * the top element of the stack.
 */
int stackTop(TArray *a)
{
    return a->arr[a->size - 1];
}

/**
 * Function: stackPush
 * -------------------
 * Adds an element to the top of the stack.
 *
 * Parameters:
 * a: the stack to which the element has to be added.
 * value: the value of the element to be added.
 *
 */
void stackPush(TArray *a, int value)
{
    a->arr[a->size] = value;
    a->size++;
}

/**
 * Function: stackIsEmpty
 * ----------------------
 * Checks if the stack is empty.
 *
 * Parameters:
 * a: the stack to be checked.
 *
 * Returns:
 * true if the stack is empty, false otherwise.
 */
bool stackIsEmpty(TArray *a)
{
    return a->size == 0;
}