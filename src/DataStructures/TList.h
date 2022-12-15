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
int *listToArray(TList list);

#endif