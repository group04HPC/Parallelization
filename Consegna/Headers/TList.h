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
int valueEqual(int value1, int value2);
int valueGreater(int value1, int value2);
int valueLess(int value1, int value2);
void valuePrint(int value);

/* Operations on TNode */
TNode *nodeCreate(int value);
void nodeDestroy(TNode *node);

/* Operations on TList */
TList listCreate();
TList listDestroy(TList list);
void listPrint(TList list);
TNode *listSearch(TList list, int value);
TList listInsertHead(TList l, int value);
TList listInsert(TList l, int value);
int listCount(TList list);
void listCopy(TList source, TList *dest);
TList listRemove(TList list, int value);
int listGet(TList list, int pos);
int *listToArray(TList list);

#endif