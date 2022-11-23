/*********************************************************************
 * \file   utils.c
 * \brief  Shared functions across program (IFJ project 2022)
 *
 * \author Petr Vecera (xvecer29)
 * \author Rene Ceska (xceska06)
 * \date   October 2022
 *********************************************************************/

#include "utils.h"
#include "symtable.h"

const int DEFAULT_STRING_ALLOCATION = 20;
const int EOS = '\0';

/// @brief Initializes a new instance of string struct
/// @param exception Exception on failed allocation
/// @return A new string instance
String* InitString(int* exception) {
    String* s = (String*)malloc(sizeof(String));
    if (s == NULL) {
        *exception = INVALID_ALLOCATION_EXCEPTION;
        return NULL;
    }

    s->length = 0;
    s->allocated = DEFAULT_STRING_ALLOCATION;
    s->chars = (char*)malloc(sizeof(char) * s->allocated);

    if (s->chars == NULL) {
        *exception = INVALID_ALLOCATION_EXCEPTION;
        return NULL;
    }

    s->chars[0] = EOS;

    *exception = OK;
    return s;
}

/// @brief Appends character at the end of string
/// @param s input String struct
/// @param c Character which will be appended at the end of String
/// @return Exception on failed reallocation
int AppendChar(String* s, const char c) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    // Reallocation
    if (s->allocated <= s->length + 1) {
        char* ptr = realloc(s->chars, s->allocated * 2);

        if (ptr == NULL) return INVALID_ALLOCATION_EXCEPTION;

        s->chars = ptr;
        s->allocated *= 2;
    }

    s->chars[s->length] = c;
    s->length++;
    s->chars[s->length] = EOS;

    return OK;
}

/// @brief Removes last character from the string
/// @param s Input string
/// @return Exception on empty string or NULL string
int RemoveLastChar(String* s) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    if (s->chars[0] == EOS || s->length == 0) return STRING_EMPTY_EXCEPTION;

    s->length--;
    s->chars[s->length] = EOS;

    return OK;
}

/// @brief Appends string at the end of input string
/// @param inputString Input string to append new string to
/// @param s Characters which will be appended at the end of input string. At the end must be EOS
/// @return Exception on NULL strings or from failed reallocation
int AppendCharacters(String* inputString, char* s) {
    if (inputString == NULL || s == NULL) return NULL_POINTER_EXCEPTION;
    int ex = OK;

    for (int i = 0; s[i] != EOS; i++) {
        ex = AppendChar(inputString, s[i]);
        if (ex != OK) break;
    }

    return ex;
}

/// @brief Appends String instance at the end of another string instance
/// @param inputString String to append another string at it's end
/// @param appendString String which will be appended at the end of string
/// @return Exception on NULL strings or from failed reallocation
int AppendString(String* inputString, String* appendString) {
    if (appendString == NULL) return NULL_POINTER_EXCEPTION;
    return AppendCharacters(inputString, appendString->chars);
}

/// @brief Frees resources of string struct
/// @param s Input string
/// @return Exception on NULL pointer
int DisposeString(String* s) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    free(s->chars);
    free(s);
    s = NULL;
    return OK;
}

int WriteErrorMessage(COMPILATION_ERROR_TYPE ce_t) {
    switch (ce_t) {
        case LEXICAL_ANALYSIS_ERROR:
            fprintf(stderr, "Error in lexical analysis\n");
            break;
        case SYNTACTIC_ANALYSIS_ERROR:
            fprintf(stderr, "Error in syntactic analysis\n");
            break;
        case SEMANTIC_ERROR_FUNCTION_UNDEFINED_OR_REDEFINED:
            fprintf(stderr, "Semantic error: undefined or redefined function\n");
            break;
        case SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE:
            fprintf(stderr, "Semantic error: wrong amount of arguments or wrong type "
                            "of return value\n");
            break;
        case SEMANTIC_ERROR_USE_OF_UNDEFINED_VARIABLE:
            fprintf(stderr, "Semantic error: use of undefined variable\n");
            break;
        case SEMANTIC_ERROR_WRONG_NUMBER_OF_EXPRESSIONS:
            fprintf(stderr, "Semantic error: wrong number of expressions in return command\n");
            break;
        case SEMANTIC_ERROR_OF_TYPE_COMPATABILITY:
            fprintf(stderr, "Semantic error: operation between incompatible types\n");
            break;
        case OTHER_SEMANTIC_ERRORS:
            fprintf(stderr, "Semantic error: other\n");
            break;
            break;
        case INTERNAL_COMPILER_ERROR:
            fprintf(stderr, "Internal compiler error\n");
            break;
    }

    return ce_t;
}

/// @brief Performs comparaison between two variable names
/// @param inputVarName Incoming variable name for comparision with node's variable name
/// @param nodeVarName Node's variable name to compare to
/// @return One when first occurrence ASCII value of input variable character is greater than in
/// node. Zero, when variables are the same and minus one, when first occurrence ASCII value of
/// input variable character is lower than in node. For different input's size as the node is result
/// determined either in case of shorter input by last input's char compared with
/// node's char on same index or in case of longer input by input's first character after
/// the removal of node prefix. This char is  compared with node's last character. In both cases
/// function returns one for input's greater value, otherwise minus one.
int CompareVarNames(String* inputVarName, String* nodeVarName) {
    if (nodeVarName == NULL || inputVarName == NULL) return NULL_POINTER_EXCEPTION;

    // For inputs longer than node, take only one char more of node's length
    int maxSufficentInputLength = min(inputVarName->length, nodeVarName->length + 1);

    // Explicit difference check on shared indexes
    for (int i = 0; i < maxSufficentInputLength; i++) {
        int nodeIndex = min(i, nodeVarName->length - 1); // Prevent overflow
        char nodeChar = nodeVarName->chars[nodeIndex];

        char inputChar = inputVarName->chars[i];

        int result = CompareChars(inputChar, nodeChar);
        if (result != 0) return result;
    }

    // All chars were same on shared indexes, strings are same with same length
    if (nodeVarName->length == inputVarName->length) return 0;

    // Take last char from node for long input and same index char for short input
    int nodeCompareIndex = min(nodeVarName->length - 1, maxSufficentInputLength - 1);

    int result = CompareChars(inputVarName->chars[maxSufficentInputLength - 1],
                              nodeVarName->chars[nodeCompareIndex]);

    // Implicit to leftside with eqvivalence
    if (result == 0) return -1;
    return result;
}

/// @brief Compares ASCII value of two characters
/// @param a first character
/// @param b second character
/// @return Minus one when first char has lower value than second one. Zero when are values the
/// same. Otherwise one.
int CompareChars(char a, char b) {
    if (a == b) return 0;

    if (a < b) {
        return -1;
    }

    return 1;
}

int min(int a, int b) {
    return a < b ? a : b;
}

/// @brief Checks whether the symbol is in scope stack of symbolic trees. Function writes errors to
/// the output and exits upon error.
/// @param symbol Name of the symbol to be found in scope stack.
/// @param symScopeStack Stack of symbolic tree scopes.
/// @return True when the symbol is present in scope stack and no error occurred, otherwise false.
bool IsInScope(String* symbol, Stack* symScopeStack) {
    bool result = false;
    int e = OK;
    int* ex = &e;

    if (IsEmpty(symScopeStack, ex)) return false;
    if (e != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    List sl = symScopeStack->list;
    List* scopeList = &sl;
    *ex = ListFirst(scopeList);
    if (e != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    while (e == OK && ListIsActive(scopeList, ex)) {
        if (e != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        T_BTnode* scope = ListGetValue(scopeList, ex);
        result = BTsearch(scope, symbol);

        if (result) break;

        ListNext(scopeList);
    }

    return result;
}