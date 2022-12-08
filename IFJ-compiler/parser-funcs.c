#include "parser-funcs.h"

AST* parseFunction(T_token* token, Stack* symtable, T_BTnode* funtable) {
    // check if fun name is ID
    token = getToken();

    if (token->type != ID) {

        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    // parameters of function
    T_funParam* params = malloc(sizeof(T_funParam));

    bool wasCalled = false;
    int paramCount;
    int err;
    T_token* funName = token;
    // check if function has been called but not declared
    if (BTsearch(funtable, token->val)) {
        params = BTgetData(funtable, token->val, &err);

        if (err != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        // check if funtions has been declared
        if (params->waDefined == true) {
            exit(WriteErrorMessage(SEMANTIC_ERROR_FUNCTION_UNDEFINED_OR_REDEFINED));
        }
        // need to check count of params later
        paramCount = params->argCount;
        wasCalled = true;
    }

    AST* tree = ASTInit();
    if (tree == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    tree->nodeT = n_funDef;
    tree->name = token->val;

    // check for (
    token = getToken();

    if (token->type != LEFT_PAR) {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // parse params
    int count = 0;
    tree->left = parsFuncParams(token, symtable, &count);

    // invalid character count
    if (wasCalled) {
        if (paramCount != count) {
            exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
        }
    } else {
        params->argCount = count;
    }

    // check for :
    token = getToken();

    if (token->type != COLON) {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // check for ret type
    token = getToken();

    if (token->type != KEYWORD) {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }
    if (strcmp(token->val->chars, "int") == 0) {
        tree->varT = int_type;
        params->retType = int_type;
        params->funType = n_funDef;
    } else if (strcmp(token->val->chars, "float") == 0) {
        tree->varT = float_type;
        params->retType = float_type;
        params->funType = n_funDef;
    } else if (strcmp(token->val->chars, "string") == 0) {
        tree->varT = string_type;
        params->retType = string_type;
        params->funType = n_funDef;
    } else if (strcmp(token->val->chars, "void") == 0) {
        tree->varT = void_type;
        params->retType = void_type;
        params->funType = n_funDef;
    } else {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    // check for {
    token = getToken();

    if (token->type != LEFT_CUR_BRACK) {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    params->waDefined = true;

    BTinsert(&funtable, funName->val, params);

    // FUN BODY
    // token = getToken();
    tree->right = BODY(token, symtable, funtable, params->retType);


    return tree;
}

AST* parseRet(Stack* symtable, T_BTnode* funtable, variableType returnType) {
    AST* tree = ASTInit();

    tree->varT = returnType;
    tree->nodeT = n_return;

    List* list = (List*)malloc(sizeof(List));
    int ex = InitList(list);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    T_token* lastToken;

    AST* expressionTree = GetStringTree(&list, &lastToken, symtable);

    if (lastToken->type != SEMICOLON) {
        expressionTree = GetArithmeticTree(list, &lastToken, symtable);
    }

    if (lastToken->type == ID) {
        tree->right = parseFunCall(lastToken, symtable, funtable);
        return tree;
    }
    tree->right = expressionTree;
    if (lastToken->type != SEMICOLON) exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));

    if (expressionTree->nodeT == n_comp)
    exit(WriteErrorMessage(SEMANTIC_ERROR_WRONG_ARGS_OR_RETURN_VALUE));

        if (expressionTree->nodeT != n_undefined) tree->right = expressionTree;

    return tree;
}

AST* parsFuncParams(T_token* token, Stack* symtable, int* count) {
    // check for keyword
    token = getToken();

    // Case ()
    if (token->type == RIGHT_PAR && *count == 0) return NULL;

    AST* tree = ASTInit();
    tree->nodeT = n_argLfun;
    if (tree == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }



    if (token->type != KEYWORD) {
        exit(WriteErrorMessage(LEXICAL_ANALYSIS_ERROR));
    }

    if (strcmp(token->val->chars, "int") == 0) {
        tree->varT = int_type;
    } else if (strcmp(token->val->chars, "float") == 0) {
        tree->varT = float_type;
    } else if (strcmp(token->val->chars, "string") == 0) {
        tree->varT = string_type;
    } else {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    token = getToken();

    if (token->type != VAR) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    if (!IsInScope(token->val, symtable)) {
        int e;
        T_BTnode** sym = Top(symtable, &e);
        if (sym == NULL) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        int err = BTinsert(sym, token->val, NULL);
        if (err != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }
    } else {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }
    tree->name = token->val;
    // successfully loaded one parameter => increase count
    (*count)++;

    // check for , or )
    token = getToken();

    if (token->type == COMMA) {
        tree->right = parsFuncParams(token, symtable, count);
    } else if (token->type == RIGHT_PAR) {
        return tree;
    } else {
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
        funRoot->nodeT = n_funCall;
    } else {
        funRoot->nodeT = funInfo->funType;
        funRoot->varT = funInfo->retType;
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
    if (funInfo->argCount != 0 || firstCall) {
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
        if (lastToken->type != COMMA && lastToken->type != RIGHT_PAR) {
            if (argVal != NULL) ASTDtor(argVal, false);
            argVal = GetArithmeticTree(buffer, &lastToken, symtable);
        } else {
            InitList(buffer);
        }

        // arg is empty or is conditional expression
        // would result in fun(,) as fun() is already handled
        if ((argVal->nodeT == n_undefined || argVal->nodeT == n_comp)) {
            if(funInfo->argCount != -1 || count != 0) {
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
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
        if (lastToken->type != COMMA) {
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