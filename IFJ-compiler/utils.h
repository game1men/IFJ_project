#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <limits.h>

//This file contains general helper functions used across the program

typedef enum {
    OK = 0,
    NULL_POINTER_EXCEPTION,
    INVALID_ALLOCATION_EXCEPTION,
    STRING_EMPTY_EXCEPTION,
    STRUCT_INITIALIZED_EXCEPTION
} ERROR_TYPE;

typedef struct {
    int length;
    int allocated;
    char* chars;
} String;

// Initializes a new instance of string struct
// Upon failed allocation it will return NULL and exception will be set
String* InitString(int*);

// Appends a character to the end of the string
// Upon failed allocation it will return an error
int AppendChar(String*, const char);

// Removes last character from the string
// If the string is emtpy it will return an error
int RemoveLastChar(String*);

// Frees the string struct and it's contents
// Null pointer returns exception
int DisposeString(String*);