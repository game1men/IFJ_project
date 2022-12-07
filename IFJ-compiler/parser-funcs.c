#include "parser-funcs.h"

AST* parseFunction(T_token* token, Stack* symtable, T_BTnode* funtable) {
    // check if fun name is ID
    token = getToken();

    if (token->type != ID) {
        tokenDtor(token);
        DisposeStack(symtable, freeStack);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // check if fun name exists in symtable F
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

    // check for (
    token = getToken();

    if (token->type != LEFT_PAR) {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // parse params
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
    // check for keyword
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
    } else if (strcmp(token->val->chars, "float") == 0) {
        tree->varT = float_type;
    } else if (strcmp(token->val->chars, "string") == 0) {
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
        // TODO fix symtable and token->type
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
    } else {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    tree->name = token->val;

    // check for , or )
    token = getToken();

    if (token->type == COMMA) {
        tree->right = parsFuncParams(token, symtable);
    } else if (token->type == RIGHT_PAR) {
        return tree;
    } else {
        freeAll(tree, token, symtable);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    return tree;
}

AST* parseFunCall(T_token* token, Stack* symtable, T_BTnode* funtable) {
    int ex = OK;
    bool firstCall = false;
    T_funParam* funInfo;

    // check fun in symtable
    if (!BTsearch(funtable, token->val)) {
        funInfo = (T_funParam*)malloc(sizeof(T_funParam));
        funInfo->funType = n_funDef;
        funInfo->waDefined = false;
        firstCall = true;
        ex = BTinsert(&funtable, token->val, funInfo);
        if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    } else {
        funInfo = BTgetData(funtable, token->val, &ex);
        if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    AST* funRoot = ASTInit();
    funRoot->name = token->val;
    if (funInfo->funType == n_funDef) {
        funRoot->varT = n_funCall;
    } else {
        funRoot->varT = funInfo->funType;
    }

    // check (
    token = getToken();
    if (token->type != LEFT_PAR) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    // Buffer init
    List* buffer = (List*)malloc(sizeof(List));
    ex = InitList(buffer);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // Check for func without params
    token = getToken();
    ex = ListAdd(buffer, token);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    if (token->type == RIGHT_PAR) {
        if (firstCall) {
            funInfo->argCount = 0;
        } else if (funInfo->argCount > 0) {
            exit(WriteErrorMessage(SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE));
        }
    }

    // Parse arguments
    if (funInfo->argCount != 0) {
        funRoot->left = parseCallParams(symtable, buffer, firstCall, funInfo);
    }

    // ; at the end
    token = getToken();
    if (token->type != SEMICOLON) {
        ASTDtor(funRoot, true);
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    return funRoot;
}

AST* parseCallParams(Stack* symtable, List* buffer, bool firstCall, T_funParam* funInfo) {
    // parse params
    T_token* lastToken;
    int count = 0;

    AST* argNode = ASTInit();
    argNode->nodeT = n_argLcall;
    AST* root = argNode;

    // Perform only for functions with arguments
    while (true) {
        AST* argVal = GetStringTree(&buffer, &lastToken, symtable);
        if (lastToken->type != COLON || lastToken->type != RIGHT_PAR) {
            if (argVal != NULL) ASTDtor(argVal, false);
            argVal = GetArithmeticTree(buffer, &lastToken, symtable);
        }

        // arg is empty or is conditional expression
        // would result in fun(,) as fun() is already handled
        if (argVal->nodeT == n_undefined || argVal->nodeT == n_comp) {
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }

        count++;
        if (firstCall) funInfo->argCount = count;

        // More arguments in function call, than should be
        if (!firstCall && count > funInfo->argCount && funInfo->argCount != -1) {
            exit(WriteErrorMessage(SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE));
        }

        // Join the param expression
        argNode->left = argVal;

        // was last argument
        if (lastToken->type == RIGHT_PAR) break;

        // Invalid expresion in parameter
        if (lastToken->type != COLON) {
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }

        // Next arg list
        argNode->right = ASTInit();
        argNode = argNode->right;
        argNode->nodeT = n_argLcall;
    }

    if (!firstCall && count != funInfo->argCount && funInfo->argCount != -1) {
        exit(WriteErrorMessage(SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE));
    }

    return root;
}