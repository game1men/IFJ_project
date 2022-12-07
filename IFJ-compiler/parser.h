/*******************************************************************
 * \file   parser.h
 * \brief
 *
 * \author Matej Smida xsmida06
 * \date   December 2022
 *********************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include "atree.h"
#include "scanner.h"
#include "stack.h"
#include "stree.h"
#include "symtable.h"
#include "utils.h"

typedef struct funParam {
    int argCount;
    variableType retType;
    nodeType funType;
    bool wasCalled;
    bool waDefined;
} T_funParam;

AST* PROLOG();

AST* PROGRAM();

AST* BODY(T_token*, Stack*, T_BTnode*);

AST* BODYCASE(T_token*, Stack*, T_BTnode*);

T_BTnode* PrepareFuntable();

T_funParam* SetupFunParams(nodeType, variableType, int);

String* CreateFunName(char*);

int parseEpilog(T_token*);

void freeStack();

void freeList();

void freeBtree();

void freeAll(AST*, T_token*, Stack*);

#endif
