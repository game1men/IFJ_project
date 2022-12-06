/*********************************************************************
 * \file   ast.h
 * \brief  Header file for scanner (IFJ project 2022)
 *
 * \author Rene Ceska xceska06
 * \date   October 2022
 *********************************************************************/
#include "ast.h"
#include "utils.h"

void header();

void footer();

void GdefVar(AST* tree, char* frame);

void GasVar(AST* tree, char* frame);

void Gread();

void Gcompare(AST* tree, char* frame);

void Gif(AST* tree, char* frame);

void Gwhile(AST* tree, char* frame);

void Gwrite(AST* tree, char* frame);

void Gconstant(AST* tree);

void Gvarriable(AST* tree, char* frame);

void GoAdd(AST* tree, char* frame);

void GoMull(AST* tree, char* frame);

void GoSubb(AST* tree, char* frame);

void GoDiv(AST* tree, char* frame);

void Gfun(AST* tree, char* frame);

void Greturn(AST* tree, char* frame);

void Garg(AST* tree, char* frame);

void Gcall(AST* tree, char* frame);

void GargLcall(AST* tree, char* frame);

void GargLfun(AST* tree, char* frame);

void Gwrite(AST* tree, char* frame);

void generate(AST* tree, char* frame);

/// @brief Generates code from AST tree
/// @param tree
void startGenerate(AST* tree);