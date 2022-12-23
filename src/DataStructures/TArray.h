#ifndef TARRAY_H
#define TARRAY_H

typedef struct TArray{
    int *arr;
    int size;
} TArray;

TArray *stackCreate(int size);
void stackDestroy(TArray *a);
int stackPop(TArray *t);
void stackPush(TArray *a, int value);
int stackTop(TArray *a);
int arrayGet(TArray *a, int index);
int stackTop(TArray *a);
bool stackIsEmpty(TArray *a);

#endif