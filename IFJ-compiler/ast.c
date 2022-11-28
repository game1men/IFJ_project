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
    ast_tmp->isFloatOperation = false;
    ast_tmp->isNull = false;
    ast_tmp->name = NULL;
    ast_tmp->valueInt = 0;
    ast_tmp->valueString = NULL;
    ast_tmp->valueFloat =0.0;

    ast_tmp->left = NULL;
    ast_tmp->right = NULL;
    ast_tmp->middle = NULL;
    ast_tmp->id = id; // unique id for each node

    id++;
    return ast_tmp;
}

/**
 * \param ast_tmp pointer to strucure
 * \brief Frees AST structure and all of its content
 * \return 0 if everything was succesfull or NULL_POINTER_EXCEPTION if pointer is null
 */
int ASTDtor(AST* ast_tmp) {

    if (ast_tmp == NULL) {
        return NULL_POINTER_EXCEPTION;
    }

    DisposeString(ast_tmp->name);
    DisposeString(ast_tmp->valueString);

    ASTDtor(ast_tmp->left);
    ASTDtor(ast_tmp->right);
    ASTDtor(ast_tmp->middle);

    free(ast_tmp);
    ast_tmp = NULL;
    return OK;
}
