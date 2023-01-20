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
#include <assert.h>
#include "../../Headers/TList.h"

/* Operations on int */

/**
 * Function: valueEqual
 * --------------------
 * Compares two values and returns true if they are equal.
 *
 * Parameters:
 * value1: the first value to compare.
 * value2: the second value to compare.
 *
 * Returns:
 * true if the two values are equal, false otherwise.
 */
int valueEqual(int value1, int value2)
{
    return value1 == value2;
}

/**
 * Function: valueGreater
 * ----------------------
 * Compares two values and returns true if the first value is greater than the second.
 *
 * Parameters:
 * value1: the first value to compare.
 * value2: the second value to compare.
 *
 * Returns:
 * true if the first value is greater than the second, false otherwise.
 */
int valueGreater(int value1, int value2)
{
    return value1 > value2;
}

/**
 * Function: valueLess
 * -------------------
 * Compares two values and returns true if the first value is less than the second.
 *
 * Parameters:
 * value1: the first value to compare.
 * value2: the second value to compare.
 *
 * Returns:
 * true if the first value is less than the second, false otherwise.
 *
 */
int valueLess(int value1, int value2)
{
    return value1 < value2;
}

/**
 * Function: valuePrint
 * --------------------
 * Prints a value.
 *
 * Parameters:
 * value: the value to print.
 *
 */
void valuePrint(int value)
{
    printf("%d ", value);
}

/* Operations on TNode */

/**
 * Function: nodeCreate
 * --------------------
 * Creates a new node.
 *
 * Parameters:
 * value: the value to store in the node.
 *
 * Returns:
 * a pointer to the new node.
 *
 */
TNode *nodeCreate(int value)
{
    TNode *new = malloc(sizeof(TNode));
    if (new != NULL)
    {
        new->value = value;
        new->link = NULL;
    }
    return new;
}

/**
 * Function: nodeDestroy
 * ---------------------
 * Destroys a node.
 *
 * Parameters:
 * node: the node to destroy.
 *
 */
void nodeDestroy(TNode *node)
{
    free(node);
}

/* Operations on TList */

/**
 * Function: listCreate
 * --------------------
 * Creates a new list.
 *
 * Returns:
 * a pointer to the new list.
 *
 */
TList listCreate()
{
    return NULL;
}

/**
 * Function: listDestroy
 * ---------------------
 * Destroys a list.
 *
 * Parameters:
 * list: the list to destroy.
 *
 * Returns:
 * a pointer to the destroyed list.
 *
 */
TList listDestroy(TList list)
{
    TNode *node;
    while (list != NULL)
    {
        node = list;
        list = list->link;
        nodeDestroy(node);
    }
    return list;
}

/**
 * Function: listPrint
 * -------------------
 * Prints a list.
 *
 * Parameters:
 * list: the list to print.
 *
 */
void listPrint(TList list)
{
    if (list != NULL)
    {
        TNode *node = list;
        while (node != NULL)
        {
            valuePrint(node->value);
            node = node->link;
        }
    }
}

/**
 * Function: listSearch
 * --------------------
 * Searches a value in a list.
 *
 * Parameters:
 * list: the list to search.
 * value: the value to search.
 *
 * Returns:
 * a pointer to the node containing the value, or NULL if the value is not found.
 *
 */
TNode *listSearch(TList list, int value)
{
    TNode *node = list;
    while (node != NULL)
    {
        if (valueEqual(node->value, value))
            return node;
        node = node->link;
    }
    return NULL;
}

/**
 * Function: listInsertHead
 * ------------------------
 * Inserts a value at the head of a list.
 *
 * Parameters:
 * list: the list to insert the value in.
 * value: the value to insert.
 *
 * Returns:
 * a pointer to the new list.
 *
 */
TList listInsertHead(TList l, int value)
{
    TNode *new = nodeCreate(value);
    assert(new != NULL);
    new->link = l;
    return new;
}

/**
 * Function: listInsert
 * --------------------
 * Inserts a value in a list.
 *
 * Parameters:
 * list: the list to insert the value in.
 * value: the value to insert.
 *
 * Returns:
 * a pointer to the new list.
 *
 */
TList listInsert(TList l, int value)
{
    TNode *prec, *succ, *new;
    prec = NULL;
    succ = l;

    /* search the insert position */
    while (succ != NULL && valueGreater(value, succ->value))
    {
        prec = succ;
        succ = succ->link;
    }

    if (succ != NULL && succ->value == value)
        return l;

    new = nodeCreate(value);
    assert(new != NULL);

    /* insertion in the links' chain */
    if (prec == NULL)
    {
        new->link = l;
        return new;
    }
    else
    {
        new->link = succ;
        prec->link = new;
        return l;
    }

    return l;
}

/**
 * Function: listCount
 * -------------------
 * Counts the number of nodes in a list.
 *
 * Parameters:
 * list: the list to count.
 *
 * Returns:
 * the number of nodes in the list.
 *
 */
int listCount(TList list)
{
    int conta = 0;
    while (list != NULL)
    {
        conta++;
        list = list->link;
    }
    return conta;
}

/**
 * Function: listCopy
 * ------------------
 * Copies a list.
 *
 * Parameters:
 * source: the list to copy.
 * dest: the pointer to the list to copy to.
 *
 * Returns:
 * a pointer to the new list.
 */
void listCopy(TList source, TList *dest)
{
    TNode *node = source;
    while (node != NULL)
    {
        *dest = listInsert(*dest, node->value);
        node = node->link;
    }
}

/**
 * Function: listToArray
 * ---------------------
 * Converts a list to an array.
 *
 * Parameters:
 * list: the list to convert.
 *
 * Returns:
 * a pointer to the array.
 */
int *listToArray(TList list)
{
    int dim = listCount(list), i = 0;
    int *res = malloc(dim * sizeof(int));

    TNode *node = list;
    while (node != NULL)
    {
        res[i] = node->value;
        node = node->link;
        i++;
    }
    return res;
}

/**
 * Function: listRemove
 * --------------------
 * Removes a value from a list.
 *
 * Parameters:
 * list: the list to remove the value from.
 * value: the value to remove.
 *
 * Returns:
 * a pointer to the new list.
 *
 */
TList listRemove(TList list, int value)
{
    TNode *prec, *succ;
    prec = NULL;
    succ = list;

    while (succ != NULL && !valueGreater(succ->value, value))
    {
        if (valueEqual(succ->value, value))
        {
            if (prec == NULL)
            {
                list = list->link;
                nodeDestroy(succ);
                return list;
            }
            else
            {
                prec->link = succ->link;
                nodeDestroy(succ);
                return list;
            }
        }
        prec = succ;
        succ = succ->link;
    }

    return list;
}

/**
 * Function: listGet
 * -----------------
 * Gets the value at a given position in a list.
 *
 * Parameters:
 * list: the list to get the value from.
 * pos: the position of the value.
 *
 * Returns:
 * the value at the given position.
 */
int listGet(TList list, int pos)
{
    assert(list != NULL);

    if (pos == 0)
        return list->value;

    return listGet(list->link, pos - 1);
}
