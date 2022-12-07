#include "parser.h"

AST* parseIf(T_token*, Stack*, T_BTnode*);

void parseCondConstruct(AST*, T_token*, Stack*, T_BTnode*);

AST* parseWhile(T_token*, Stack*, T_BTnode*);
