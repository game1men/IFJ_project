#include "parser.h"

AST* sendToAtree(T_token*, T_token**, Stack*);

AST* parseVAR(T_token*, T_token**, Stack*, T_BTnode*);
