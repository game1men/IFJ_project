/*****************************************************************
 * \file   parser.c
 * \brief
 *
 * \author Matej Smida xsmida06
 * \date   December 2022
 *********************************************************************/

#include "parser.h"
#include "parser-cond.h"
#include "parser-funcs.h"
#include "parser-vars.h"

AST* PROLOG() {
    if (detectProlog() != 1) exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));

    AST* tree = PROGRAM();

    return tree;
}

T_BTnode* PrepareFuntable() {
    T_BTnode* funs;
    BTinit(&funs);

    BTinsert(&funs, CreateFunName("write"), SetupFunParams(n_write, void_type, -1));
    BTinsert(&funs, CreateFunName("floatval"), SetupFunParams(n_floatval, float_type, 1));
    BTinsert(&funs, CreateFunName("intval"), SetupFunParams(n_intval, int_type, 1));
    BTinsert(&funs, CreateFunName("strval"), SetupFunParams(n_stringval, string_type, 1));
    BTinsert(&funs, CreateFunName("strlen"), SetupFunParams(n_strlen, int_type, 1));
    BTinsert(&funs, CreateFunName("substring"), SetupFunParams(n_substring, string_type, 3));
    BTinsert(&funs, CreateFunName("ord"), SetupFunParams(n_ord, int_type, 1));
    BTinsert(&funs, CreateFunName("chr"), SetupFunParams(n_chr, string_type, 1));
    BTinsert(&funs, CreateFunName("reads"), SetupFunParams(n_read, string_type, 0));
    BTinsert(&funs, CreateFunName("readi"), SetupFunParams(n_read, int_type, 0));
    BTinsert(&funs, CreateFunName("readf"), SetupFunParams(n_read, float_type, 0));

    return funs;
}

String* CreateFunName(char* name) {
    int er = OK;
    String* str = InitString(&er);
    if (er != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    er = AppendCharacters(str, name);
    if (er != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    return str;
}

T_funParam* SetupFunParams(nodeType type, variableType vartype, int argCount) {
    T_funParam* funInfo = (T_funParam*)malloc(sizeof(T_funParam));
    funInfo->retType = vartype;
    funInfo->funType = type;
    funInfo->waDefined = true;
    funInfo->wasCalled = false;
    funInfo->argCount = argCount;
    return funInfo;
}

AST* PROGRAM() {
    AST* tree = ASTInit();

    if (tree == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    tree->nodeT = n_stList;

    T_BTnode* sym;
    BTinit(&sym);

    T_BTnode* funtable = PrepareFuntable();

    Stack s;
    Stack* stack = &s;
    int err = InitStack(stack);

    if (err != OK) {
        ASTDtor(tree, true);
        BTdispose(sym, freeBtree);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    Push(stack, &sym);

    AST* root = tree;

    T_token* token;
    token = getToken();

    while (token->type != TOKEN_EOF) {

        if (strcmp(token->val->chars, "function") == 0) {
            T_BTnode* funSym;
            BTinit(&funSym);

            Stack fs;
            Stack* funStack = &fs;
            int e = InitStack(funStack);

            if (e != OK) {
                freeAll(tree, token, stack);
                BTdispose(sym, freeBtree);
                exit(WriteErrorMessage(err));
            }

            Push(funStack, &funSym);

            tree->left = parseFunction(token, funStack, funtable);
        } else if (token->type == EPILOG) {
            int err = parseEpilog(token);

            if (err != OK) {
                freeAll(tree, token, stack);
                BTdispose(sym, freeBtree);
                exit(WriteErrorMessage(err));
            }
        } else {
            tree->left = BODYCASE(token, stack, funtable);
        }

        token = getToken();

        if (token->type == TOKEN_EOF) break;

        tree->right = ASTInit();
        tree->right->nodeT = n_stList;
        tree = tree->right;
    }
    return root;
}

AST* BODY(T_token* token, Stack* symtable, T_BTnode* funtable) {

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    if (token->type == NOT_TOKEN) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    AST* root = tree;
    tree->nodeT = n_stList;

    while (token->type != TOKEN_EOF) {

        token = getToken();
        tree->left = BODYCASE(token, symtable, funtable);
            if(tree->left == NULL) break;
        tree->right = ASTInit();
        tree = tree->right;
        tree->nodeT = n_stList;
    }

    return root;
}

AST* BODYCASE(T_token* token, Stack* symtable, T_BTnode* funtable) {

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        BTdispose(funtable, freeBtree);
    }

    switch (token->type) {
        case KEYWORD:
            if (strcmp(token->val->chars, "if") == 0) {
                tree = parseIf(token, symtable, funtable);
                break;
            } else if (strcmp(token->val->chars, "while") == 0) {
                tree = parseWhile(token, symtable, funtable);
                break;
            } else {

                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;

        case VAR: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = parseVAR(token, &lastToken, symtable, funtable);

            // if (lastToken->type != SEMICOLON) {

            //     tokenDtor(lastToken);
            //     exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            // }

            break;
        }

        case ID:
            tree = parseFunCall(token, symtable, funtable);
            break;

        case INT: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case FLOAT: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }

            break;
        }

        case FLOAT_DEC_EXP: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case FLOAT_EXP: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }

        case NOT_TOKEN:

            exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            break;
        case RIGHT_CUR_BRACK:
            return NULL;

        default: {
            T_token* lastToken = tokenInit();
            if (lastToken == NULL) {

                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }

            tree = sendToAtree(token, &lastToken, symtable);

            if (lastToken->type != SEMICOLON) {

                tokenDtor(lastToken);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
            }
            break;
        }
    }

    return tree;
}

int parseEpilog(T_token* token) {
    token = getToken();

    if (token->type != TOKEN_EOF) return SYNTACTIC_ANALYSIS_ERROR;

    return OK;
}

void freeAll(AST* tree, T_token* token, Stack* stack) {
    ASTDtor(tree, true);
    tokenDtor(token);
    DisposeStack(stack, freeStack);

    return;
}
// TODO free stack and its content
void freeStack() {
    return;
}

void freeList() {
    return;
}

void freeBtree() {
    return;
}