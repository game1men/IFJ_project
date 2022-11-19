#include "list.h"
#include "utils.h"

/// @brief Initializes linked list struct
/// @param l Input list to work with
/// @return exception for NULL list
int InitList(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;
    l->firstItem = l->activeItem = l->lastItem = NULL;
    l->count = 0;
    return OK;
}

/// @brief Frees allocations for each item and it's contents.
/// As the contents of each item are freed too. It is necessary to allocate the data on
/// the heap, otherwise this function ends with segfault
/// @param l Input list to work with
/// @return exception for NULL list
int DisposeList(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    ListItem currentItem = l->firstItem;

    while (currentItem != NULL) {
        ListItem nextItem = currentItem->nextItem;

        // frees the item contents, MUST BE ON HEAP otherwise seg fault will occur here
        if (currentItem->data != NULL) free(currentItem->data);

        free(currentItem);
        currentItem = nextItem;
    }

    l->firstItem = NULL;
    l->activeItem = NULL;
    l->lastItem = NULL;
    l->count = 0;

    return OK;
}

/// @brief Makes the first item in list active
/// @param l Input list to work with
/// @return exception for NULL list
int ListFirst(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    l->activeItem = l->firstItem;

    return OK;
}

/// @brief Internal function for list item allocation
ListItem _ItemAllocation(void* data) {
    ListItem item = (ListItem)malloc(sizeof(struct litem_t));
    if (item != NULL) item->data = data;
    return item;
}

/// @brief Inserts new item at the beginning of the list
/// @param l Input list to work with
/// @param data Data for the new item
/// @return exception for NULL list, invalid allocation
int ListInsertFirst(List* l, void* data) {
    // Exception check
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    ListItem newItem = _ItemAllocation(data);
    if (newItem == NULL) {
        return INVALID_ALLOCATION_EXCEPTION;
    }
    l->count++;
    newItem->nextItem = l->firstItem;
    l->firstItem = newItem;

    // First item in list is also last item
    if (l->count == 1) {
        l->lastItem = newItem;
    }

    return OK;
}

/// @brief Adds a new item at the end of list
/// @param l Input list to work with Input list
/// @param data data for the new item
/// @return Return exception for NULL list, invalid allocation or invalid activity
int ListAdd(List* l, void* data) {

    if (l == NULL) return NULL_POINTER_EXCEPTION;
    ListItem tmp = l->activeItem;

    if (l->count == 0) return ListInsertFirst(l, data);

    l->activeItem = l->lastItem;
    int result = ListInsertAfter(l, data);
    l->activeItem = tmp;
    return result;
}

/// @brief Deletes the first occurence item with specified data
/// @param l Input list to work with
/// @param data Data for which item will be deleted
/// @return Exception for NULL list, non existent data or empty / not initialized list
int ListDelete(List* l, void* data) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    ListItem tmp = l->activeItem;
    ListFirst(l);

    int ex = OK;

    // list is empty
    if (!ListIsActive(l, &ex)) {
        return STRUCT_NOT_ACTIVE_EXCEPTION;
    }

    // Special case for deleting first item in list
    if (l->firstItem->data == data) {
        ex = ListDeleteFirst(l);
        l->activeItem = tmp;
        return ex;
    }

    // Make active the item before the item, which will be deleted
    while (l->activeItem->nextItem != NULL) {
        if (ex != OK) {
            l->activeItem = tmp;
            return ex;
        }

        if (l->activeItem->nextItem->data == data) break;
        ex = ListNext(l);
    }

    if (l->activeItem->nextItem == NULL) {
        ex = NULL_POINTER_EXCEPTION; // Item was not found
    } else {
        ex = ListDeleteAfter(l); // Item to delete is after the active item
    }

    l->activeItem = tmp;
    return ex;
}

/// @brief Gets the first item data in list
/// @param l Input list to work with
/// @param ex Exception for NULL list or emtpy / not initialized list
/// @return data from the first item or NULL for exception
void* ListGetFirst(List* l, int* ex) {
    // Exception handling
    if (l == NULL) {
        *ex = NULL_POINTER_EXCEPTION;
        return NULL;
    }
    *ex = OK;
    return l->firstItem != NULL ? l->firstItem->data : NULL;
}

/// @brief Deletees the first item in the list
/// @param l Input list to work with
/// @return Exception for NULL list or emtpy / not initialized list
int ListDeleteFirst(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    if (l->firstItem == NULL) return STRUCT_NOT_INITIALIZED_EXCEPTION;

    ListItem secondItem = l->firstItem->nextItem;
    // Item is active, list is losing activity
    if (l->activeItem == l->firstItem) l->activeItem = NULL;

    // List has one item, dereferencing last item
    if (l->firstItem == l->lastItem) l->lastItem = NULL;

    free(l->firstItem);
    l->firstItem = secondItem;

    l->count--;
    return OK;
}

/// @brief Deletes the first item after the active item
/// @param l Input list to work with
/// @return Exception for NULL list or invalid activity
int ListDeleteAfter(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    if (l->activeItem == NULL) return STRUCT_NOT_ACTIVE_EXCEPTION;
    if (l->activeItem->nextItem == NULL) return NULL_POINTER_EXCEPTION;

    ListItem itemToDelete = l->activeItem->nextItem;
    l->activeItem->nextItem = itemToDelete->nextItem;

    // Deleted item was last, last item is active item
    if (itemToDelete == l->lastItem) l->lastItem = l->activeItem;

    free(itemToDelete);

    l->count--;
    return OK;
}

/// @brief Inserts new item with specified data after the active item
/// @param l Input list to work with
/// @param data Data associated with the new item
/// @return Exception for NULL list, invalid allocation or invalid activity
int ListInsertAfter(List* l, void* data) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;
    if (l->activeItem == NULL) return STRUCT_NOT_ACTIVE_EXCEPTION;

    // New item allocation
    ListItem newItem = _ItemAllocation(data);
    if (newItem == NULL) {
        return INVALID_ALLOCATION_EXCEPTION;
    }

    l->count++;
    newItem->nextItem = l->activeItem->nextItem;
    l->activeItem->nextItem = newItem;

    // Update last item
    if (l->activeItem == l->lastItem) {
        l->lastItem = newItem;
    }

    return OK;
}

/// @brief Gets the data on the active item in list
/// @param l Input list to work with
/// @param error Exception for NULL list or invalid activity
/// @return Data from the current active item or NULL on exception
void* ListGetValue(List* l, int* error) {
    if (l->activeItem == NULL) {
        *error = STRUCT_NOT_ACTIVE_EXCEPTION;
        return NULL;
    } else {
        *error = OK;
        return l->activeItem->data;
    }
}

/// @brief Moves the activity to the next item from the current one.
/// If the activity is on the last item, the list became inactive
/// @param l Input list to work with
/// @return Exception for the NULL list
int ListNext(List* l) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;

    // Avoid null reference, list became inactive
    if (l->activeItem == NULL) return OK;

    l->activeItem = l->activeItem->nextItem;

    return OK;
}

/// @brief Return whether the list is active
/// @param l Input list to work with
/// @param ex Exception for NULL list
/// @return True if list is active, otherwise false
bool ListIsActive(List* l, int* ex) {
    if (l == NULL) {
        *ex = NULL_POINTER_EXCEPTION;
        return false;
    }
    return l->activeItem != NULL;
}

/// @brief Sets the specified value on current active item in list
/// @param l Input list to work with
/// @param data New data which will be set on active item
/// @return Exception for the NULL list or invalid activity
int ListSetValue(List* l, void* data) {
    if (l == NULL) return NULL_POINTER_EXCEPTION;
    if (l->activeItem == NULL) return STRUCT_NOT_ACTIVE_EXCEPTION;

    l->activeItem->data = data;
    return OK;
}