#include "parser.h"

AST* parseFunction(T_token*, Stack*, T_BTnode*);

AST* parseCallParams(Stack*, List*, bool, T_funParam*);

AST* parseFunCall(T_token*, Stack*, T_BTnode*);

AST* parsFuncParams(T_token*, Stack*, int*);

AST* parseRet(Stack*, T_BTnode*, variableType);