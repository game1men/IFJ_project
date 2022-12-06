/*********************************************************************
 * \file   ast.c
 *
 * \author Rene Ceska xceska06
 * \date   October 2022
 *********************************************************************/

#include "ast.h"

/**
 * \brief Allocates a memory for AST structure, also generates unique ids
 * \return A pointer to the initialized AST structure
 */
AST* ASTInit() {
    static int id = 0; // unique id generation
    AST* ast_tmp;

    ast_tmp = (AST*)malloc(sizeof(AST));
    if (ast_tmp == NULL) {
        WriteErrorMessage(INTERNAL_COMPILER_ERROR);
        return NULL;
    }

    ast_tmp->name = NULL;
    ast_tmp->valueString = NULL;

    ast_tmp->nodeT = n_undefined;
    ast_tmp->cmpT = undefined;
    ast_tmp->varT = undefined_type;
    ast_tmp->name = NULL;
    ast_tmp->valueInt = 0;
    ast_tmp->valueString = NULL;
    ast_tmp->valueFloat = 0.0;

    ast_tmp->left = NULL;
    ast_tmp->right = NULL;
    ast_tmp->middle = NULL;
    ast_tmp->id = id; // unique id for each node

    id++;
    return ast_tmp;
}

/// @brief Recursively frees AST structure and all of its content
/// @param ast_tmp pointer to strucure
/// @param disposeString True to dispose internal strings in node as well
/// @return OK if everything was successfully or NULL_POINTER_EXCEPTION if pointer is null
int ASTDtor(AST* ast_tmp, bool disposeString) {

    if (ast_tmp == NULL) {
        return NULL_POINTER_EXCEPTION;
    }

    if (disposeString) {
        DisposeString(ast_tmp->name);
        DisposeString(ast_tmp->valueString);
    }

    ASTDtor(ast_tmp->left, disposeString);
    ASTDtor(ast_tmp->right, disposeString);
    ASTDtor(ast_tmp->middle, disposeString);

    free(ast_tmp);
    ast_tmp = NULL;
    return OK;
}
