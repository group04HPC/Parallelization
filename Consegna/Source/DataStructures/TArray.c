#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../Headers/TArray.h"

int stackPop(TArray *a){
    return a->arr[--a->size];
}

TArray *stackCreate(int size){
    TArray *result = malloc(sizeof(TArray));
    result->size = 0;
    result->arr = malloc(sizeof(int) * size);
    return result;
}

void stackDestroy(TArray *a){
    free(a->arr);
    free(a);
}

int arrayGet(TArray *a, int index){
    return a->arr[index];
}

int stackTop(TArray *a){
    return a->arr[a->size - 1];
}

void stackPush(TArray *a, int value){
    a->arr[a->size] = value;
    a->size++;
}

bool stackIsEmpty(TArray *a){
    return a->size == 0;
}