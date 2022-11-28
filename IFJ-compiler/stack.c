#include "stack.h"
#include "utils.h"

/// @brief Initializes stack and it's internal linked list
/// @param s Input stack to work with
/// @return Exception for null stack or exceptions for internal list
int InitStack(Stack* s) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    int ex = InitList(&s->list);
    s->top = NULL;
    return ex;
}

/// @brief Pushes a new item at the top of the stack with specified data
/// @param s Input stack to work with
/// @param data Data for the new item
/// @return Exception for null stack or exceptions for internal list
int Push(Stack* s, void* data) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;
    if (&s->list == NULL) return STRUCT_NOT_INITIALIZED_EXCEPTION;

    int ex = ListInsertFirst(&(s->list), data);
    s->top = s->list.firstItem;
    return ex;
}

/// @brief Removes one item from the top of the stack
/// @param s Input stack to work with
/// @param ex Exception for null stack or exception for internal list
/// @return Data from the item which is removed
void* Pop(Stack* s, int* ex) {
    if (s == NULL) {
        *ex = NULL_POINTER_EXCEPTION;
        return NULL;
    }

    if (&s->list == NULL) {
        *ex = STRUCT_NOT_INITIALIZED_EXCEPTION;
        return NULL;
    }

    if (IsEmtpy(s, ex)) {
        *ex = STRUCT_EMPTY_EXCEPTION;
        return NULL;
    }

    void* tmp = s->top->data;
    *ex = ListDeleteFirst(&s->list);
    s->top = s->list.firstItem;

    return tmp;
}

/// @brief Gets the data from the top item in stack
/// @param s Input stack to work with
/// @param ex Exception for the NULL stack or empty stack
/// @return Data from the top item in the stack
void* Top(Stack* s, int* ex) {
    if (s == NULL) {
        *ex = NULL_POINTER_EXCEPTION;
        return NULL;
    }

    if (IsEmtpy(s, ex)) {
        *ex = STRUCT_EMPTY_EXCEPTION;
        return NULL;
    }

    return s->top->data;
}

/// @brief Checks if the stack is empty
/// @param s Input stack to work with
/// @param ex Exception for NULL stack
/// @return True if the stack is empty or stack is NULL, otherwise false
bool IsEmtpy(Stack* s, int* ex) {
    if (s == NULL) {
        *ex = NULL_POINTER_EXCEPTION;
        return true;
    }

    return s->top == NULL;
}

/// @brief Disposes stack struct and the internal list
/// @param s Input stack to work with
/// @param itemDisposer A pointer to the function ensuring correct stack item disposal
/// @return Exception for the null stack or exception on internal list
int DisposeStack(Stack* s, void (*itemDisposer)(void*)) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    return DisposeList(&s->list, itemDisposer);
    s->top = NULL;
}