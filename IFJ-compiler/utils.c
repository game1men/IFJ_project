#include "utils.h"

const int DEFAULT_STRING_ALLOCATION = 20;
const int EOS = '\0';

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

int AppendChar(String* s, const char c) {
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    // Reallocation
    if (s->allocated <= s->length + 1) {
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
    if (s == NULL) return NULL_POINTER_EXCEPTION;

    if (s->chars[0] == EOS || s->length == 0) return STRING_EMPTY_EXCEPTION;

    s->length--;
    s->chars[s->length] = EOS;

    return OK;
}

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

/// @brief Performs comparasion between two variable names
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
/// @return Minus one when first char has lower value than second one. Zero when are values the same. Otherwise one.
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