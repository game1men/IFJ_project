#include "parser-cond.h"

AST* parseIf(T_token* token, Stack* symtable, T_BTnode* funtable) {
    AST* ifRoot = ASTInit();
    ifRoot->nodeT = n_if;
    //int ex = OK;

    // if() { BODY }
    parseCondConstruct(ifRoot, token, symtable, funtable);

    // else
    token = getToken();
    if (token->type == KEYWORD) {
        if (strcmp(token->val->chars, "else") != 0) {
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }
    } else {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    // {
    token = getToken();
    if (token->type != LEFT_CUR_BRACK) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    T_BTnode* scope;
    BTinit(&scope);

    // push new scope
    //ex = Push(symtable, &scope);
    //if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // BODY
    //token = getToken();
    ifRoot->right = BODY(token, symtable, funtable);

    // pop scope
    //BTdispose(Pop(symtable, &ex), NULL);
    //if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    return ifRoot;
}

void parseCondConstruct(AST* root, T_token* token, Stack* symtable, T_BTnode* funtable) {
    token = getToken();
    // (
    if (token->type != LEFT_PAR) exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));

    // parse expression
    List* buffer = (List*)malloc(sizeof(List));
    int ex = InitList(buffer);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    T_token* lastToken;
    AST* expressionTree = GetStringTree(&buffer, &lastToken, symtable);
    if (lastToken->type != RIGHT_PAR) {
        expressionTree = GetArithmeticTree(buffer, &lastToken, symtable);
    }

    if (lastToken->type != RIGHT_PAR) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    // Empty expression
    if (expressionTree->nodeT == n_undefined) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    root->left = expressionTree;

    // {
    token = getToken();
    if (token->type != LEFT_CUR_BRACK) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    // BODY
    //token = getToken();

    //T_BTnode* scope;
    //BTinit(&scope);
    // push new scope
    //ex = Push(symtable, &scope);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    root->middle = BODY(token, symtable, funtable);

    // pop scope
    //BTdispose(Pop(symtable, &ex), NULL);
    //if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
}

AST* parseWhile(T_token* token, Stack* symtable, T_BTnode* funtable) {
    AST* whileRoot = ASTInit();
    whileRoot->nodeT = n_while;

    // while() { BODY }
    parseCondConstruct(whileRoot, token, symtable, funtable);

    return whileRoot;
}