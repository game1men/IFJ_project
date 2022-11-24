#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct litem_t {
    void* data;
    struct litem_t* nextItem;
};

// Pointer of item for List type
// ListItem is allocated on heap, data must also be allocated on heap as dispose will free them
typedef struct litem_t* ListItem;

// Linked list struct
// data of each item in list must be allocated on heap as dispose will free them
typedef struct list_t {
    ListItem firstItem;
    ListItem activeItem;
    ListItem lastItem;
    int count;
} List;

int InitList(List*);

int DisposeList(List*, void (*itemDisposer)(void*));

int ListAdd(List*, void*);

int ListInsertAfter(List*, void*);

int ListInsertFirst(List*, void*);

ListItem _ItemAllocation(void*);

int ListFirst(List*);

void* ListGetFirst(List*, int*);

int ListDeleteFirst(List*);

int ListDelete(List*, void*);

int ListDeleteAfter(List*);

int ListNext(List*);

void* ListGetValue(List*, int*);

int ListSetValue(List*, void*);

bool ListIsActive(List*, int*);

void DisposePrimitive(void*);