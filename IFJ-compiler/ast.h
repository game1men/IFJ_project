/*********************************************************************
 * \file   ast.h
 * \brief  Header file for scanner (IFJ project 2022)
 *
 * \author Rene Ceska xceska06
 * \date   October 2022
 *********************************************************************/

#include "utils.h"

// types of nodes
typedef enum {
    n_if,       // left is expression that has to return true/false
                // middle is body
                // right is else body
    n_while,    // left is expression that has to return true/false
                //  middle is body
    n_comp,     // cmpType ct has to be set for comparation type
                // pushes answer to stack
    n_constant, // any constant in code
    n_defvar,   // for first encounter of variable. name of variable in name
    n_asignVar, // assigns value from right node to variable
                // right - any node that returns value
                // name  - name of variable
    n_var,      // if first encounter of var, define variable first by defvar.
                // name of variable in name
    n_write,    // writes value of right child
    n_stList,   // list of nodes

    // opreations work with values of its childs.
    //  if any of operands of any child operation was float set isFloatOperation to true
    n_add,
    n_mull,
    n_div,
    n_sub,
    n_argLfun,  // List of arguments when defining function
                // name - name of argument
                // right side can be another n_argLfun
    n_argLcall, // list of call arguments, left child is argument(var,cons,func,etc), right side can
                // be another argLcall
    n_read,     // pushes to stack value from stdin, depends on vartype
    n_funCall,     // name of function is in name variable
    n_return,
    n_funDef,
    n_undefined,
    n_concat,
    n_chr,
    n_ord,
    n_strlen,
    n_stringval,
    n_intval,
    n_floatval,
    n_substring
} nodeType;

// comparation types
typedef enum {
    eq,
    gt,
    lt,
    leq,
    geq,
    neq,
    undefined
} cmpType;

typedef enum {
    float_type,
    string_type,
    int_type,
    void_type,
    undefined_type
} variableType;

typedef struct T_AST {
    struct T_AST* left;
    struct T_AST* right;
    struct T_AST* middle;
    nodeType nodeT;
    cmpType cmpT;
    variableType varT;
    bool isNullable;
    String* name;
    int valueInt;
    String* valueString;
    double valueFloat;
    int id; // unique id for each node
} AST;

AST* ASTInit();
int ASTDtor(AST* ast_tmp);