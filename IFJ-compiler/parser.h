/*****************************************************************//**
 * \file   parser.h
 * \brief  
 * 
 * \author Matej Smida xsmida06
 * \date   December 2022
 *********************************************************************/

#include "utils.h"
#include "scanner.h"
#include "symtable.h"
#include "stack.h"
#include "atree.h"
#include "stree.h"

typedef struct funParam {
    int argCount;
    variableType retType;
    nodeType funType;
    bool wasCalled;
    bool waDefined;
}T_funParam;

AST* PROLOG();

AST* PROGRAM();

AST* BODY(T_token*, Stack*, T_BTnode*);

AST* parseFunction(T_token*, Stack*, T_BTnode*);

AST* parseIf(T_token*, Stack*, T_BTnode*);

AST* parseWhile(T_token*, Stack*, T_BTnode*);

AST* parseFunCall(T_token*, Stack*, T_BTnode*);

AST* parsFuncParams(T_token*, Stack*);

AST* sendToAtree(T_token*, T_token**, Stack*);

AST* parseVAR(T_token*, T_token**, Stack*, T_BTnode*);

int parseEpilog(T_token*);

void freeStack();

void freeList();

void freeBtree();

void freeAll(AST*, T_token*, Stack*);
