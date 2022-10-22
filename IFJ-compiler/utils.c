#include "utils.h"

const int DEFAULT_STRING_ALLOCATION = 20;
const int EOS = '\0';

String* InitString(int* exception) {
    String *s = (String*)malloc(sizeof(String));
    if(s == NULL) {
        *exception = INVALID_ALLOCATION_EXCEPTION;
        return NULL;
    }
    
    s->length = 0;
    s->allocated = DEFAULT_STRING_ALLOCATION;
    s->chars = (char*)malloc(sizeof(char) * s->allocated);

    if(s->chars == NULL) {
        *exception = INVALID_ALLOCATION_EXCEPTION;
        return NULL;
    }

    s->chars[0] = EOS;

    *exception = OK;
    return s;
}

int AppendChar(String* s, const char c) {
    if(s == NULL) return NULL_POINTER_EXCEPTION;

    // Reallocation
    if(s->allocated <= s->length + 1) {
        char* ptr = realloc(s->chars, s->allocated * 2);

        if (ptr == NULL) return INVALID_ALLOCATION_EXCEPTION;

        s->chars = ptr;
    }

    s->chars[s->length] = c;
    s->length++;
    s->chars[s->length] = EOS;

    return OK;
}

int RemoveLastChar(String* s) {
    if(s == NULL) return NULL_POINTER_EXCEPTION;

    if(s->chars[0] == EOS || s->length == 0) return STRING_EMPTY_EXCEPTION;

    s->length--;
    s->chars[s->length] = EOS;

    return OK;
}

int DisposeString(String* s) {
    if(s == NULL) return NULL_POINTER_EXCEPTION;

    free(s->chars);
    free(s);
    s = NULL;
    return OK;
}
