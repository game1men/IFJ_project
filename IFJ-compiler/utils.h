/*********************************************************************
 * \file   utils.h
 * \brief  Shared functions across program (IFJ project 2022)
 *
 * \author Petr Vecera (xvecer29)
 * \author Rene Ceska (xceska06)
 * \date   October 2022
 *********************************************************************/



#ifndef UTILS_H
#define UTILS_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "stack.h"

// This file contains general helper functions used across the program

extern const int EOS;
extern const int DEFAULT_STRING_ALLOCATION;


typedef enum {
    OK = 0,
    NULL_POINTER_EXCEPTION,
    INVALID_ALLOCATION_EXCEPTION,
    STRING_EMPTY_EXCEPTION,
    STRUCT_NOT_INITIALIZED_EXCEPTION,
    STRUCT_NOT_ACTIVE_EXCEPTION,
    STRUCT_EMPTY_EXCEPTION
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

String* InitString(int*);

int AppendChar(String*, const char);

int RemoveLastChar(String*);

int AppendCharacters(String*, char*);

int AppendString(String*, String*);

int DisposeString(String*);

// Writes error message to stderr
// Returns error code
int WriteErrorMessage(COMPILATION_ERROR_TYPE);

int CompareVarNames(String*, String*);

int CompareChars(char, char);

int min(int a, int b);

bool IsInScope(String*, Stack*);

#endif