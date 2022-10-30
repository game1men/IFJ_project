#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// This file contains general helper functions used across the program

extern const int EOS;
extern const int DEFAULT_STRING_ALLOCATION;


typedef enum {
    OK = 0,
    NULL_POINTER_EXCEPTION,
    INVALID_ALLOCATION_EXCEPTION,
    STRING_EMPTY_EXCEPTION,
    STRUCT_INITIALIZED_EXCEPTION
} ERROR_TYPE;

typedef enum {
    LEXICAL_ANALYSIS_ERROR = 1,
    SYNTACTIC_ANALYSIS_ERROR = 2,
    SEMANTIC_ERROR_FUNCTION_UNDEFINED_OR_REDEFINED = 3,
    SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE = 4,
    SEMANTIC_ERROR_USE_OF_UNDEFINED_VARIABLE = 5,
    SEMANTIC_ERROR_WRONG_NUMBER_OF_EXPRESSIONS = 6,
    SEMANTIC_ERROR_OF_TYPE_COMPATABILITY = 7,
    OTHER_SEMANTIC_ERRORS = 8,
    INTERNAL_COMPILER_ERROR = 99
} COMPILATION_ERROR_TYPE;

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

// Writes error message to stderr
// Returns error code
int WriteErrorMessage(COMPILATION_ERROR_TYPE);