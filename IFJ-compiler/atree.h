/*********************************************************************
 * \file   atree.h
 * \brief  Header file for generating arithmetic expression tree (IFJ project 2022)
 *
 * \author Petr Vecera (xvecer29)
 * \date   November 2022
 *********************************************************************/

#include "ast.h"
#include "list.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef enum {
    PF_INVALID = 0,
    PF_NUMBER,
    PF_OPERATOR,
    PF_CONDITIONAL_OPERATOR,
    PF_LBRACKET,
    PF_RBRACKET
} PostFixType;

typedef struct postfixitem_t {
    T_token* token;
    PostFixType pftype;
    bool belongToStack;
    int stackWeight;
} PostFixItem;

AST* GetArithmeticTree(List*, T_token**, Stack*);

AST* _CreateNode(List*, Stack*);

List* _ConvertToPostfix(List*, T_token**);

void _ConvertToAstNode(AST*, PostFixItem*, Stack*);

int _EmptyTo(int, bool, Stack*, List*);

T_token* _GetNextToken(List*);

PostFixItem* _CreatePFItem(T_token*);

void DisposePfItem(void*);