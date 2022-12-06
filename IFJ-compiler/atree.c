/*********************************************************************
 * \file   atree.c
 * \brief  Code file for generating arithmetic expression tree (IFJ project 2022)
 *
 * \author Petr Vecera (xvecer29)
 * \date   November 2022
 *********************************************************************/

#include "atree.h"

/// @brief Creates abstract syntactic tree from input arithmetic expression. Performs syntactic and
/// semantic check for expression. Found errors in expression writes to output, then exits the
/// application
/// @param tokenList All loaded tokens needed to verify expression existence before calling this
/// function, the rest of the tokens will be loaded by the function from input
/// @param lastToken The last token which is not part of valid arithmetic expression.
/// @param variableScope Scope stack of variables. The data part of symtable node is not checked in
/// the function, it could be null.
/// @return Returns created abstract syntactic tree upon to the first token not part of the valid
/// expression. Only error which is not handled by this function is greater count of closing
/// brackets, the caller is required to check whether after the bracket continues valid code syntax.
/// If expression is not present upon closing bracket, the functions returns initialized empty tree.
AST* GetArithmeticTree(List* tokenList, T_token** lastToken, Stack* variableScope) {
    List* postfixList = _ConvertToPostfix(tokenList, lastToken);
    if (postfixList == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    AST* tree = _CreateNode(postfixList, variableScope);
    return tree;
}

/// @brief Recursively creates the nodes for the AST tree
/// @param postfixList Inverted input postfix expression list
/// @param varScope Variable scope stack
/// @return Valid tree, empty tree on empty postfix or NULL on error
AST* _CreateNode(List* postfixList, Stack* varScope) {
    int ex = OK;

    AST* root = ASTInit();

    // List is empty
    if (postfixList->count == 0) return root;

    PostFixItem* rootItem = (PostFixItem*)ListGetFirst(postfixList, &ex);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    ex = ListDeleteFirst(postfixList);
    if (ex != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // Create root
    _ConvertToAstNode(root, rootItem, varScope);

    // For operations like plus, minus, multiplication, etc... make left and right numbers
    if (rootItem->pftype != PF_NUMBER) {
        // As postfix list is inverted, order must be also inverted
        root->right = _CreateNode(postfixList, varScope);
        root->left = _CreateNode(postfixList, varScope);
    }

    DisposePfItem(rootItem);

    return root;
}

/// @brief Converts postfix item to the AST node
/// @param tree target instance of AST node
/// @param item input postfix item to perform conversion
/// @param varScope Stack of variable scopes
void _ConvertToAstNode(AST* tree, PostFixItem* item, Stack* varScope) {
    T_token* token = item->token;
    switch (token->type) {
        case INT: {
            tree->nodeT = n_constant;
            tree->varT = int_type;
            long long convNum = strtoll(token->val->chars, NULL, 10);
            if (convNum > INT_MAX) {
                exit(WriteErrorMessage(SEMANTIC_ERROR_OF_TYPE_COMPATABILITY));
            }
            tree->valueInt = (int)convNum;
            break;
        }
        case FLOAT:
        case FLOAT_DEC_EXP:
        case FLOAT_EXP:
            tree->nodeT = n_constant;
            tree->varT = float_type;
            tree->valueFloat = strtod(token->val->chars, NULL);
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
        case PLUS:
            tree->nodeT = n_add;
            break;
        case MINUS:
            tree->nodeT = n_sub;
            break;
        case DIV:
            tree->nodeT = n_div;
            break;
        case ASTERISK:
            tree->nodeT = n_mull;
            break;
        default:
            break;
    }
}

/// @brief Converts infix notation from tokens to postfix
/// @param tokenList A token buffer loaded before calling this function
/// @param lastToken The last loaded token that ended postfix conversion
/// @return Inverted list order of PostFixType items in postfix notation
List* _ConvertToPostfix(List* tokenList, T_token** lastToken) {
    // Exception handling
    if (tokenList == NULL || lastToken == NULL) return NULL;

    bool errorOccurred = false;

    // List init
    List* resultList = (List*)malloc(sizeof(List));
    if (resultList == NULL) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    int exception = InitList(resultList);

    // Stack init
    Stack s;
    Stack* stack = &s;
    exception += InitStack(stack);

    // Exception handling
    if (exception != OK) {
        free(resultList);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    // Get first token
    T_token* currentToken = GetNextBufferToken(tokenList);

    *lastToken = currentToken;

    // Create pf item from token
    PostFixItem* currentPfItem = _CreatePFItem(currentToken);
    if (currentPfItem == NULL) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    // Semantic values init
    int bracketCount = 0;
    bool hasConditional = false;
    bool shouldBeNumber = true;

    // Iterate tokens until error or invalid token
    while (!errorOccurred && currentPfItem->pftype != PF_INVALID) {

        if (currentPfItem->pftype == PF_LBRACKET) bracketCount++;

        if (currentPfItem->pftype == PF_RBRACKET) {
            bracketCount--;

            // No error here - It is up to the caller to check whether the reaming brackets are
            // correct after the ar. expression
            if (bracketCount < 0) break;
        }

        // Every operator excluding brackets must be between two numbers
        if (currentPfItem->pftype != PF_RBRACKET && currentPfItem->pftype != PF_LBRACKET) {
            if (currentPfItem->pftype != PF_NUMBER && shouldBeNumber) errorOccurred = true;
            if (currentPfItem->pftype == PF_NUMBER && !shouldBeNumber) errorOccurred = true;

            if (errorOccurred) {
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
            shouldBeNumber = !shouldBeNumber;
        }

        // One conditional check
        if (currentPfItem->pftype == PF_CONDITIONAL_OPERATOR) {
            if (hasConditional) {
                exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
            }
            hasConditional = true;
        }

        // Ensure correct operation precedence before pushing
        // Left bracket does not need to check precedence on stack
        if (currentPfItem->stackWeight != -1 && currentPfItem->pftype != PF_LBRACKET) {
            exception = _EmptyTo(currentPfItem->stackWeight, currentPfItem->pftype == PF_RBRACKET,
                                 stack, resultList);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        if (currentPfItem->belongToStack) {
            exception = Push(stack, currentPfItem);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        // Numbers go always to list
        if (currentPfItem->pftype == PF_NUMBER) {
            exception = ListInsertFirst(resultList, currentPfItem);
            if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        // Internal error in data types
        if (exception != OK) {
            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

        currentPfItem = _CreatePFItem(GetNextBufferToken(tokenList));

        *lastToken = currentPfItem->token;
    } // end of while

    // Expression is not complete, but allows empty expressions
    if (shouldBeNumber && currentPfItem->pftype == PF_INVALID && resultList->count != 0) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    if (bracketCount > 0) {
        exit(WriteErrorMessage(SYNTACTIC_ANALYSIS_ERROR));
    }

    // Empty rest from the stack at the end
    exception = _EmptyTo(currentPfItem->stackWeight, false, stack, resultList);
    if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    // No PF item is disposed here as stack is already empty
    exception = DisposeStack(stack, DisposePfItem);
    if (exception != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));

    if (exception != OK || errorOccurred) {
        DisposeList(resultList, DisposePfItem);
        free(resultList);
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }

    exception = OK;
    return resultList;
}

/// @brief Empties stack to lower desired precedence
/// @param desiredPrecedence Desired precedence to empty stack
/// @param toSamePrecedenceInclusive true for empty to at most same (inclusive) precedence and
/// remove first satisfied precedence
/// @param stack input stack to empty from
/// @param list output list to pop items from stack
/// @return Exception from stack or list
int _EmptyTo(int desiredPrecedence, bool toSamePrecedenceInclusive, Stack* stack, List* list) {
    int ex = OK;

    // Empty stack
    if (IsEmpty(stack, &ex)) return OK;

    while (!IsEmpty(stack, &ex)) {
        if (ex != OK) break;

        PostFixItem* stackItem = (PostFixItem*)Top(stack, &ex);

        // Breaks either on first lower precedence or same precedence
        if (toSamePrecedenceInclusive) {
            if (stackItem->stackWeight <= desiredPrecedence) break;
        } else {
            if (stackItem->stackWeight < desiredPrecedence) break;
        }

        ex = ListInsertFirst(list, Pop(stack, &ex));
    }

    // First satisfied occurrence will pop as well, if there is any item still left
    if (toSamePrecedenceInclusive && !IsEmpty(stack, &ex)) Pop(stack, &ex);

    return ex;
}

/// @brief Creates post fix item from input token
/// @param token Input token
/// @return Postfix item, NULL when token is NULL or on malloc fail
PostFixItem* _CreatePFItem(T_token* token) {
    if (token == NULL) return NULL;

    PostFixItem* item = (PostFixItem*)malloc(sizeof(PostFixItem));
    if (item == NULL) return NULL;

    item->token = token;
    item->belongToStack = false;
    item->pftype = PF_INVALID;
    item->stackWeight = -1;

    switch (token->type) {
        case INT:
        case FLOAT:
        case FLOAT_EXP:
        case FLOAT_DEC_EXP:
        case VAR:
            item->pftype = PF_NUMBER;
            break;
        case KEYWORD:
            if(strcmp(token->val->chars, "null") == 0) item->pftype = PF_NUMBER;
            break;
        case NOT_EQ:
        case EQ:
        case LESS:
        case LESS_EQ:
        case MORE:
        case MORE_EQ:
            item->pftype = PF_CONDITIONAL_OPERATOR;
            item->belongToStack = true;
            item->stackWeight = 1;
            break;
        case PLUS:
        case MINUS:
            item->pftype = PF_OPERATOR;
            item->belongToStack = true;
            item->stackWeight = 2;
            break;
        case DIV:
        case ASTERISK:
            item->pftype = PF_OPERATOR;
            item->belongToStack = true;
            item->stackWeight = 3;
            break;
        case LEFT_PAR:
            item->pftype = PF_LBRACKET;
            item->belongToStack = true;
            item->stackWeight = 0;
            break;
        case RIGHT_PAR:
            item->pftype = PF_RBRACKET;
            item->stackWeight = 0;
            break;
        default:
            break;
    }

    return item;
}

/// @brief Custom disposer for postfix item used in stack and list
/// @param item Postfix item pointer
void DisposePfItem(void* item) {
    if (item == NULL) return;

    PostFixItem* pf = (PostFixItem*)item;
    free(pf->token);
    free(pf);
}