/*********************************************************************
 * \file   atree.h
 * \brief  Header file for generating string expression tree (IFJ project 2022)
 *
 * \author Petr Vecera (xvecer29)
 * \date   November 2022
 *********************************************************************/

#ifndef STREE_H
#define STREE_H


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

typedef enum spft {
    SPF_INVALID = 0,
    SPF_STRING,
    SPF_CONCAT,
    SPF_CONDITIONAL
} StringPFType;

typedef struct stringpf_t {
    T_token* token;
    StringPFType spftype;
    bool belongToStack;
    int stackWeight;
} StringPFItem;

AST* GetStringTree(List**, T_token**, Stack*);

AST* _CreateStringNode(List*, Stack*);

void _ConvertToSAstNode(AST*, StringPFItem*, Stack*);

List* _ConvertToSPostfix(List**, T_token**);

void _EmptyInputBuffer(List*, List*);

void _SConvertToAstNode(AST*, StringPFItem*, Stack*);

int _SEmptyTo(int, Stack*, List*);

StringPFItem* _CreateSPFItem(T_token*);

void DisposeSPfItem(void*);

#endif