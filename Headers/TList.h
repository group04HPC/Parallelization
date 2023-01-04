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

#ifndef TLIST_H
#define TLIST_H

/* TNode type definition */
typedef struct SNode
{
    int value;
    struct SNode *link;
} TNode;

/* TList type definition */
typedef TNode *TList;

/* Operations on int */
int valueEqual(int value1, int value2);   /* Compares two values and returns true if they are equal. */
int valueGreater(int value1, int value2); /* Compares two values and returns true if the first value is greater than the second. */
int valueLess(int value1, int value2);    /* Compares two values and returns true if the first value is less than the second. */
void valuePrint(int value);               /* Prints a value. */

/* Operations on TNode */
TNode *nodeCreate(int value);  /* Creates a new node. */
void nodeDestroy(TNode *node); /* Destroys a node. */

/* Operations on TList */
TList listCreate();                       /* Creates a new list. */
TList listDestroy(TList list);            /* Destroys a list. */
void listPrint(TList list);               /* Prints a list. */
TNode *listSearch(TList list, int value); /* Searches a value in a list. */
TList listInsertHead(TList l, int value); /* Inserts a value at the head of a list. */
TList listInsert(TList l, int value);     /* Inserts a value in a list. */
int listCount(TList list);                /* Counts the number of nodes in a list. */
void listCopy(TList source, TList *dest); /* Copies a list. */
TList listRemove(TList list, int value);  /* Removes a value from a list. */
int listGet(TList list, int pos);         /* Gets the value at a given position in a list. */
int *listToArray(TList list);             /* Converts a list to an array. */

#endif