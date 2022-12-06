/*****************************************************************//**
 * \file   parser.c
 * \brief  
 * 
 * \author Matej Smida xsmida06
 * \date   December 2022
 *********************************************************************/

#include "parser.h"

AST* PROLOG() {
    if (detectProlog() != 1) exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));

    AST* tree = PROGRAM();

    return tree;
}

AST* PROGRAM() {
    AST* tree = ASTInit();

    if (tree == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    tree->nodeT = n_stList;

    T_BTnode* sym;
    BTinit(&sym);

    T_BTnode* funtable;
    BTinit(&funtable);


    Stack s;
    Stack* stack = &s;
    int err = InitStack(stack);

    if (err != OK) {
        ASTDtor(tree, true);
        BTdispose(sym, freeBtree);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    Push(stack, &sym);

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
        } 
        else if (token->type == EPILOG) {
            int err = parseEpilog(token);

            if (err != OK) {
                freeAll(tree, token, stack);
                BTdispose(sym, freeBtree);
                exit(WriteErrorMessage(err));
            }
        }
        else {
            tree->left = BODY(token, stack, funtable);
        }

        token = getToken();

        tree->right = ASTInit();
        tree->right->nodeT = n_stList;
        tree = tree->right;
    }
    return tree;
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

    AST* tmptree = tree;

    while (token->type != TOKEN_EOF) {

        switch (token->type) {
            case KEYWORD:
                if (strcmp(token->val->chars, "if") == 0) {
                    tree = parseIf(token, symtable, funtable);
                    break;
                } else if (strcmp(token->val->chars, "while") == 0) {
                    tree = parseWhile(token, symtable, funtable);
                    break;
                } else {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;

            case VAR: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = parseVAR(token, &lastToken, symtable, funtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }

            case ID:
                parseFunCall(token, symtable, funtable);
                break;

            case INT: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = sendToAtree(token, &lastToken, symtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }

            case FLOAT: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = sendToAtree(token, &lastToken, symtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }

            case FLOAT_DEC_EXP: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = sendToAtree(token, &lastToken, symtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }

            case FLOAT_EXP: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = sendToAtree(token, &lastToken, symtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }

            case NOT_TOKEN:
                freeAll(tree, token, symtable);
                exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                break;

            default: {
                T_token* lastToken = tokenInit();
                if (lastToken == NULL) {
                    freeAll(tree, token, symtable);
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                }

                tree = sendToAtree(token, &lastToken, symtable);

                if (lastToken->type != SEMICOLON) {
                    freeAll(tree, token, symtable);
                    tokenDtor(lastToken);
                    exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
                }
                break;
            }
        }

        token = getToken();

        tree->right = ASTInit();
        tree->right->nodeT = n_stList;
        tree = tree->right;
    }

    return tmptree;
}

AST* parseFunction(T_token* token, Stack* symtable, T_BTnode* funtable) {
    //check if fun name is ID
    token = getToken();

    if (token->type != ID) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // check if fun name exists in symtable
    if (IsInScope(token->val, symtable)) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(SEMANTIC_ERROR_FUNCTION_UNDEFINED_OR_REDEFINED));
    }

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    tree->nodeT = n_funDef;
    tree->name = token->val;

    //check for (
    token = getToken();

    if (token->type != LEFT_PAR) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    //parse params
    tree->left = parsFuncParams(token, symtable);

    // check for :
    // check for ret type
    // set ret type
    // check for {
    // FUN BODY
    // check for return
    // set return type
    // do till }

    return tree;
}

AST* parsFuncParams(T_token* token, Stack* symtable) {
    //check for keyword
    token = getToken();

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    // Case ()
    if (token->type == RIGHT_PAR) return tree;

    if (token->type != KEYWORD) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    if (strcmp(token->val->chars, "int") == 0) {
        tree->varT = int_type;
    } 
    else if (strcmp(token->val->chars, "float") == 0) {
        tree->varT = float_type;
    } 
    else if (strcmp(token->val->chars, "string") == 0) {
        tree->varT = string_type;
    } else {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    token = getToken();

    if (token->type != VAR) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    if (!IsInScope(token->val, symtable)) {
        //TODO fix symtable and token->type
        int e;
        T_BTnode* sym = Pop(symtable, &e);
        if (sym == NULL) {
            freeAll(tree, token, symtable);
        }

        int err = BTinsert(&sym, token->val, NULL);
        if (err != OK) {
            freeAll(tree, token, symtable);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }
    } 
    else {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    tree->name = token->val;

    // check for , or )
    token = getToken();

    if (token->type == COMMA) {
        tree->right = parsFuncParams(token, symtable);
    } 
    else if (token->type == RIGHT_PAR) {
        return tree;
    } 
    else {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    return tree;
}

AST* parseFunCall(T_token* token, Stack* symtable, T_BTnode* funtable) {
    return NULL;
}

AST* parseIf(T_token* token, Stack* symtable, T_BTnode* funtable) {
    return NULL;
}

AST* parseWhile(T_token* token, Stack* symtable, T_BTnode* funtable) {
    return NULL;
}

AST* parseVAR(T_token* token, T_token** lastToken, Stack* symtable, T_BTnode* funtable) {
    T_token* prevToken = token;
    token = getToken();

    // Case $VAR = 
    if (token->type == ASSIGN) {

        AST* varTree = ASTInit();
        if (varTree == NULL) {
            tokenDtor(token);
            tokenDtor(*lastToken);
            DisposeStack(symtable, freeStack);
        }

        if (IsInScope(prevToken->val, symtable)) {
            varTree->nodeT = n_asignVar;
        } 
        else varTree->nodeT = n_defvar;

        varTree->name = prevToken->val;

        token = getToken();

        List* tokenList = malloc(sizeof(List));
        if (tokenList == NULL) {
            freeAll(varTree, token, symtable);
            tokenDtor(*lastToken);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        int err = InitList(tokenList);
        if (err != OK) {
            freeAll(varTree, token, symtable);
            tokenDtor(*lastToken);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        ListAdd(tokenList, token);

        AST* tree = ASTInit();
        if (tree == NULL) {
            freeAll(varTree, token, symtable);
            tokenDtor(*lastToken);
            DisposeList(tokenList, freeList);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        tree = GetStringTree(&tokenList, lastToken, symtable);

        // check if its not a function $x = foo()
        if ((*lastToken)->type == ID) {
            varTree->right = parseFunCall(token, symtable, funtable);
            return varTree;
        }

        int er = 0;

        // checks that tree is not empty and doesnt end with semicolon $VAR = ;
        if ((*lastToken)->type != SEMICOLON)
            er = 1;
        else if (tree == NULL)
            er = 1;
        else if (tree->nodeT == n_comp)
            er = 1;

        if (er == 1) {
            tree = GetArithmeticTree(tokenList, lastToken, symtable);

            int e = 0;

            if ((*lastToken)->type != SEMICOLON)
                e = 1;
            else if (tree->nodeT == n_comp)
                e = 1;
            if (e == 1) {
                freeAll(varTree, token, symtable);
                tokenDtor(*lastToken);
                DisposeList(tokenList, freeList);
                ASTDtor(tree, true);
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
        }

        varTree->right = tree;

        return varTree;
    } 
    //Case $VAR ...
    else {
        AST* tree = ASTInit();
        if (tree == NULL) {
            tokenDtor(token);
            DisposeStack(symtable, freeStack);
            tokenDtor(*lastToken);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        List* tokenList = malloc(sizeof(List));
        if (tokenList == NULL) {
            freeAll(tree, token, symtable);
            tokenDtor(*lastToken);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        int err = InitList(tokenList);

        if (err != OK) {
            freeAll(tree, token, symtable);
            tokenDtor(*lastToken);
            DisposeList(tokenList, freeList);
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        ListAdd(tokenList, prevToken);
        ListAdd(tokenList, token);


        tree = GetStringTree(&tokenList, lastToken, symtable);

        int er = 0;

        // checks that tree is not empty and doesnt end with semicolon $VAR = ;
        if ((*lastToken)->type != SEMICOLON)
            er = 1;
        else if (tree == NULL)
            er = 1;
        else if (tree->nodeT == n_comp)
            er = 1;

        if (er == 1) {
            tree = GetArithmeticTree(tokenList, lastToken, symtable);

            int e = 0;

            if ((*lastToken)->type != SEMICOLON)
                e = 1;
            else if (tree->nodeT == n_comp)
                e = 1;
            if (e == 1) {
                freeAll(tree, token, symtable);
                tokenDtor(*lastToken);
                DisposeList(tokenList, freeList);
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
        }

        return tree;
    }
}


AST* sendToAtree(T_token* token, T_token** lastToken, Stack* symtable) {

    List* tokenList = malloc(sizeof(List));
    if (tokenList == NULL) {
        tokenDtor(token);
        tokenDtor(*lastToken);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    int err = InitList(tokenList);
    if (err != OK) {
        tokenDtor(token);
        tokenDtor(*lastToken);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    ListAdd(tokenList, token);

    AST* tree = ASTInit();
    if (tree == NULL) {
        tokenDtor(token);
        tokenDtor(*lastToken);
        DisposeList(tokenList, freeList);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }


    tree = GetStringTree(&tokenList, lastToken, symtable);

    int er = 0;

    // checks that tree is not empty and doesnt end with semicolon $VAR = ;
    if ((*lastToken)->type != SEMICOLON)
        er = 1;
    else if (tree == NULL)
        er = 1;
    else if (tree->nodeT == n_comp)
        er = 1;

    if (er == 1) {
        tree = GetArithmeticTree(tokenList, lastToken, symtable);

        int e = 0;

        if ((*lastToken)->type != SEMICOLON)
            e = 1;
        else if (tree->nodeT == n_comp)
            e = 1;
        if (e == 1) {
            tokenDtor(token);
            tokenDtor(*lastToken);
            DisposeList(tokenList, freeList);
            DisposeStack(symtable, freeStack);
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }
    }

    return tree;
}

int parseEpilog(T_token* token){
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
//TODO free stack and its content
void freeStack() {
    return;
}

void freeList() {
    return;
}

void freeBtree() {
    return;
}