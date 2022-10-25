#include "utils.h"

const int DEFAULT_STRING_ALLOCATION = 20;
const int EOS = '\0';

String *InitString(int *exception) {
  String *s = (String *)malloc(sizeof(String));
  if (s == NULL) {
    *exception = INVALID_ALLOCATION_EXCEPTION;
    return NULL;
  }

  s->length = 0;
  s->allocated = DEFAULT_STRING_ALLOCATION;
  s->chars = (char *)malloc(sizeof(char) * s->allocated);

  if (s->chars == NULL) {
    *exception = INVALID_ALLOCATION_EXCEPTION;
    return NULL;
  }

  s->chars[0] = EOS;

  *exception = OK;
  return s;
}

int AppendChar(String *s, const char c) {
  if (s == NULL)
    return NULL_POINTER_EXCEPTION;

  // Reallocation
  if (s->allocated <= s->length + 1) {
    char *ptr = realloc(s->chars, s->allocated * 2);

    if (ptr == NULL)
      return INVALID_ALLOCATION_EXCEPTION;

    s->chars = ptr;
  }

  s->chars[s->length] = c;
  s->length++;
  s->chars[s->length] = EOS;

  return OK;
}

int RemoveLastChar(String *s) {
  if (s == NULL)
    return NULL_POINTER_EXCEPTION;

  if (s->chars[0] == EOS || s->length == 0)
    return STRING_EMPTY_EXCEPTION;

  s->length--;
  s->chars[s->length] = EOS;

  return OK;
}

int DisposeString(String *s) {
  if (s == NULL)
    return NULL_POINTER_EXCEPTION;

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
  case SYNTAKTIC_ANALYSIS_ERROR:
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
    fprintf(stderr,
            "Semantic error: wrong number of expressions in return command\n");
    break;
  case SEMANTIC_ERROR_OF_TYPE_COMPATABILITY:
    fprintf(stderr, "Semantic error: operation between uncompatible types\n");
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
