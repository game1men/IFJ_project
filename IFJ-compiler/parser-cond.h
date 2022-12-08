#include "parser.h"

AST* parseIf(T_token*, Stack*, T_BTnode*, variableType);

void parseCondConstruct(AST*, T_token*, Stack*, T_BTnode*, variableType);

AST* parseWhile(T_token*, Stack*, T_BTnode*, variableType);
