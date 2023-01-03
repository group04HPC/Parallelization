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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../Headers/TList.h"

/* Operations on int */

int valueEqual(int value1, int value2)
{
    return value1 == value2;
}
int valueGreater(int value1, int value2)
{
    return value1 > value2;
}
int valueLess(int value1, int value2)
{
    return value1 < value2;
}
void valuePrint(int value)
{
    printf("%d ", value);
}

/* Operations on TNode */

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

void nodeDestroy(TNode *node)
{
    free(node);
}

/* Operations on TList */

TList listCreate()
{
    return NULL;
}

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

TNode *listSearch(TList list, int value)
{
    TNode *node = list;
    while (node != NULL && !valueGreater(node->value, value))
    {
        if (valueEqual(node->value, value))
            return node;
        node = node->link;
    }
    return NULL;
}

TList listInsertHead(TList l, int value)
{
    TNode *new = nodeCreate(value);
    assert(new != NULL);
    new->link = l;
    return new;
}

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

    if (succ!=NULL && succ->value == value)
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

void listCopy(TList source, TList* dest){
    TNode *node = source;
    while (node != NULL)
    {
        *dest = listInsert(*dest, node->value);
        node = node->link;
    }
}

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

TList listRemove(TList list, int value){
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

int listGet(TList list, int pos){
    assert(list!=NULL);

    if(pos==0)
        return list->value;

    return listGet(list->link,pos-1);
}
