#include "stree.h"

/// @brief Creates abstract string expression tree. Performs syntactic check for expression. Found
/// errors writes to the output, then exits the application.
/// @param tokenList All loaded tokens needed to verify expression existence before calling this
/// function. This token list is populated with input tokens to make use of it in arithmetic checks.
/// @param lastToken The last token which is not part of valid string expression.
/// @param variableScope Scope stack of variables. The data part of symtable node is not checked in
/// the function, it could be null.
/// @return Returns created string expression tree upon the first token not part of the expression.
/// On incomplete expression returns NULL and populates back token list
AST* GetStringTree(List** tokenList, T_token** lastToken, Stack* variableScope) {
    List* postfixList = _ConvertToSPostfix(tokenList, lastToken);
    if (postfixList == NULL) return NULL;

    AST* tree = _CreateStringNode(postfixList, variableScope);
    return tree;
}

/// @brief Recursively creates the nodes for the AST tree
/// @param postfixList Inverted input postfix expression list
/// @param varScope Variable scope stack
/// @return Valid tree, empty tree on empty postfix or NULL on error
AST* _CreateStringNode(List* postfixList, Stack* varScope) {
    int ex = OK;

    AST* root = ASTInit();

    // List is empty
    if (postfixList->count == 0) return root;

    StringPFItem* rootItem = (StringPFItem*)ListGetFirst(postfixList, &ex);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    ex = ListDeleteFirst(postfixList);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // Create root
    _ConvertToSAstNode(root, rootItem, varScope);

    // For concatenation and conditional expressions like ===, <=, etc...
    if (rootItem->spftype != SPF_STRING) {
        // As postfix list is inverted, order must be also inverted
        root->right = _CreateStringNode(postfixList, varScope);
        root->left = _CreateStringNode(postfixList, varScope);
    }

    DisposeSPfItem(rootItem);

    return root;
}

/// @brief Converts postfix item to the AST node
/// @param tree target instance of AST node
/// @param item input postfix item to perform conversion
/// @param varScope Stack of variable scopes
void _ConvertToSAstNode(AST* tree, StringPFItem* item, Stack* varScope) {
    T_token* token = item->token;
    switch (token->type) {
        case STRING:
            tree->nodeT = n_constant;
            tree->varT = string_type;
            tree->valueString = item->token->val;
            break;
        case DOT:
            tree->nodeT = n_concat;
            break;
        case VAR: {
            tree->nodeT = n_var;
            tree->name = token->val;
            bool found = IsInScope(tree->name, varScope);
            if (!found) {
                exit(WriteErrorMessage(SEMANTIC_ERROR_USE_OF_UNDEFINED_VARIABLE));
            }
            break;
        }
        case KEYWORD:
            tree->nodeT = n_constant;
            tree->varT = void_type;
            break;
        case NOT_EQ:
            tree->nodeT = n_comp;
            tree->cmpT = neq;
            break;
        case EQ:
            tree->nodeT = n_comp;
            tree->cmpT = eq;
            break;
        case LESS:
            tree->nodeT = n_comp;
            tree->cmpT = lt;
            break;
        case LESS_EQ:
            tree->nodeT = n_comp;
            tree->cmpT = leq;
            break;
        case MORE:
            tree->nodeT = n_comp;
            tree->cmpT = gt;
            break;
        case MORE_EQ:
            tree->nodeT = n_comp;
            tree->cmpT = geq;
            break;

        default:
            break;
    }
}

/// @brief Converts infix notation from tokens to postfix
/// @param tokenList A token buffer loaded before calling this function
/// @param lastToken The last loaded token that ended postfix conversion
/// @return Inverted list order of PostFixType items in postfix notation
List* _ConvertToSPostfix(List** tokenListPtr, T_token** lastToken) {

    // Exception handling
    if (tokenListPtr == NULL || lastToken == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    List* tokenList = *tokenListPtr;
    if (tokenList == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    bool errorOccurred = false;

    // List init
    List* resultList = (List*)malloc(sizeof(List));
    if (resultList == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    int exception = InitList(resultList);

    // Output buffer init
    List* outputBuffer = (List*)malloc(sizeof(List));
    if (outputBuffer == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    exception = InitList(resultList);

    // Stack init
    Stack s;
    Stack* stack = &s;
    exception += InitStack(stack);

    // Exception handling
    if (exception != OK) {
        free(outputBuffer);
        free(resultList);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    // Get first token
    T_token* currentToken = GetNextBufferToken(tokenList);
    exception = ListAdd(outputBuffer, currentToken);
    if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    *lastToken = currentToken;

    // Create pf item from token
    StringPFItem* currentSpfItem = _CreateSPFItem(currentToken);
    if (currentSpfItem == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    // Semantic values init
    bool hasConditional = false;
    bool shouldBeString = true;

    while (!errorOccurred && currentSpfItem->spftype != SPF_INVALID) {
        // Every concat must be between two strings
        if (currentSpfItem->spftype != SPF_STRING && shouldBeString) errorOccurred = true;
        if (currentSpfItem->spftype == SPF_STRING && !shouldBeString) errorOccurred = true;

        if (errorOccurred) {
            exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
        }
        shouldBeString = !shouldBeString;

        // One conditional check
        if (currentSpfItem->spftype == SPF_CONDITIONAL) {
            if (hasConditional) {
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
            hasConditional = true;
        }

        // Ensure correct operation precedence before pushing
        if (currentSpfItem->stackWeight != -1) {
            exception = _SEmptyTo(currentSpfItem->stackWeight, stack, resultList);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        if (currentSpfItem->belongToStack) {
            exception = Push(stack, currentSpfItem);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        // Strings go always to list
        if (currentSpfItem->spftype == SPF_STRING) {
            exception = ListInsertFirst(resultList, currentSpfItem);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        // Internal error in data types
        if (exception != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        currentToken = GetNextBufferToken(tokenList);
        exception = ListAdd(outputBuffer, currentToken);
        if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

        currentSpfItem = _CreateSPFItem(currentToken);
        *lastToken = currentToken;
    } // end of while

    // Expression is not complete, but allows empty expressions
    if (shouldBeString && currentSpfItem->spftype == SPF_INVALID && resultList->count != 0) {
        // must not fail to enable atree to test it
        errorOccurred = true;
    }

    // Override token list to output buffer
    _EmptyInputBuffer(tokenList, outputBuffer);
    free(tokenList);
    *tokenListPtr = outputBuffer;

    // Empty rest from the stack at the end
    exception = _SEmptyTo(currentSpfItem->stackWeight, stack, resultList);
    if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // No PF item is disposed here as stack is already empty
    exception = DisposeStack(stack, DisposeSPfItem);
    if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    if (exception != OK || errorOccurred) {
        DisposeList(resultList, DisposeSPfItem);
        free(resultList);
        return NULL;
    }

    exception = OK;
    return resultList;
}

/// @brief Moves items from input buffer to the output buffer
/// @param input Input buffer list
/// @param output Output buffer list
void _EmptyInputBuffer(List* input, List* output) {
    int ex = OK;
    while (input->count != 0) {
        T_token* item = ListGetFirst(input, &ex);
        if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

        ex = ListAdd(output, item);
        if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

        ex = ListDeleteFirst(input);
        if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }
}

/// @brief Creates post fix item from input token
/// @param token Input token
/// @return Postfix item, NULL when token is NULL or on malloc fail
StringPFItem* _CreateSPFItem(T_token* token) {
    if (token == NULL) return NULL;

    StringPFItem* item = (StringPFItem*)malloc(sizeof(StringPFItem));
    if (item == NULL) return NULL;

    item->token = token;
    item->belongToStack = false;
    item->spftype = SPF_INVALID;
    item->stackWeight = -1;

    switch (token->type) {
        case VAR:
        case STRING:
            item->spftype = SPF_STRING;
            break;
        case KEYWORD:
            if (strcmp(token->val->chars, "null") == 0) item->spftype = SPF_STRING;
            break;
        case NOT_EQ:
        case EQ:
        case LESS:
        case LESS_EQ:
        case MORE:
        case MORE_EQ:
            item->spftype = SPF_CONDITIONAL;
            item->belongToStack = true;
            item->stackWeight = 1;
            break;
        case DOT:
            item->spftype = SPF_CONCAT;
            item->belongToStack = true;
            item->stackWeight = 2;
            break;
        default:
            break;
    }

    return item;
}

/// @brief Empties stack to lower desired precedence
/// @param desiredPrecedence Desired precedence to empty stack
/// @param stack input stack to empty from
/// @param list output list to pop items from stack
/// @return Exception from stack or list
int _SEmptyTo(int desiredPrecedence, Stack* stack, List* list) {
    int ex = OK;

    // Empty stack
    if (IsEmpty(stack, &ex)) return OK;

    while (!IsEmpty(stack, &ex)) {
        if (ex != OK) break;

        StringPFItem* stackItem = (StringPFItem*)Top(stack, &ex);

        // Breaks on first lower precedence
        if (stackItem->stackWeight < desiredPrecedence) break;

        ex = ListInsertFirst(list, Pop(stack, &ex));
    }

    return ex;
}

/// @brief Custom disposer for string postfix item used in stack and list
/// @param item Postfix item pointer
void DisposeSPfItem(void* item) {
    if (item == NULL) return;

    StringPFItem* spf = (StringPFItem*)item;
    free(spf);
}