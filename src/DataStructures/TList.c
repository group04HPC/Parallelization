#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TList.h"

/* Operations on int */

int valueEqual (int value1, int value2) {
    return value1 == value2;
}
int valueGreater (int value1, int value2) {
    return  value1 > value2;
}
int valueLess (int value1, int value2) {
    return  value1 < value2;
}
void valuePrint (int value) {
    printf ("%d ", value);
}

/* Operations on TNode */

TNode *nodeCreate(int value) {
    TNode *new = malloc (sizeof(TNode));
    if (new != NULL) {
        new->value = value;
        new->link = NULL;
    }
    return new;
}

void nodeDestroy(TNode *node) {
    free (node);
}

/* Operations on TList */

TList listCreate() {
    return NULL;
}

TList listDestroy(TList list) {
    TNode *node;
    while (list != NULL) {
        node = list;
        list = list->link;
        nodeDestroy(node);
    }
    return list;
}

void listPrint(TList list) {
    if (list != NULL) {
        TNode *node = list;
        while (node!=NULL)
        {
            valuePrint(node->value);
            node = node->link;
        }
        
    }
}

TNode *listSearch(TList list, int value) {
    TNode *node = list;
    while (node != NULL && !valueGreater(node->value, value)) {
        if (valueEqual(node->value, value))
            return node;
        node = node->link;
    }
    return NULL;
}

TList listInsertHead(TList l, int value) {
    TNode *new = nodeCreate(value);
    assert (new != NULL);
    new->link = l;
    return new;
}

TList listInsert(TList l, int value) {
    TNode *prec, *succ, *new;
    prec = NULL;
    succ = l;
    new = nodeCreate(value);
    assert (new != NULL);

    /* search the insert position */
    while (succ != NULL && valueGreater(value, succ->value)) {
        prec = succ;
        succ = succ->link;
    }

    /* insertion in the links' chain */
    if (prec == NULL) {
        new->link = l;
        return new;
    } else {
        new->link = succ;
        prec->link = new;
        return l;
    }
}

int listCount (TList list) {
    int conta = 0;
    while (list != NULL) {
        conta++;
        list = list->link;
    }
    return conta;
}