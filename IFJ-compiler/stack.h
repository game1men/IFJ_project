#include "list.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef STACK_H
#define STACK_H
// Stack type with internal linked list
typedef struct stack_t {
    List list;
    ListItem top;
} Stack;

int InitStack(Stack*);

int Push(Stack*, void*);

void* Pop(Stack*, int*);

void* Top(Stack*, int*);

bool IsEmtpy(Stack*, int*);

int DisposeStack(Stack*, void (*itemDisposer)(void*));

#endif