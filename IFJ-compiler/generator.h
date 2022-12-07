/*********************************************************************
 * \file   generator.h
 * \brief  Header file for generator (IFJ project 2022)
 *
 * \author Rene Ceska xceska06
 * \date   October 2022
 *********************************************************************/
#include "ast.h"
#include "utils.h"

/// @brief converts string to right format for generating IFJCODE
char* _stringConvert(String* s);

/// @brief generates header with helper functions and variables
void header();

/// @brief generates footer
/// @param bodyVars variables defined inside main bodyF
void footer(String* bodyVars);

/// @brief generates variable definition according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GdefVar(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates variable assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GasVar(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function read assignment according to AST.md
/// @param tree
void Gread(AST* tree);

/// @brief generates comparison according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gcompare(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates if according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gif(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates while according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gwhile(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function write assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gwrite(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates constant according to AST.md
/// @param tree
void Gconstant(AST* tree);

/// @brief generates varriable according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
void Gvarriable(AST* tree, char* frame);

/// @brief generates addition according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GoAdd(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates multiplication according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GoMull(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates subtraction according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GoSubb(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates division according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GoDiv(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates function according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
void Gfun(AST* tree, char* frame, String* bodyVars, String* funVars);

/// @brief generates return according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Greturn(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates function definition argument according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Garg(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates function call argument according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gcall(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates list of call arguments according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GargLcall(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates list of function definition arguments according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void GargLfun(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function write assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gwrite(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function write assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gord(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function ord assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gchr(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function chr assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gstringval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function stringval assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gintval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function intval assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gfloatval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function floatval assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gstrlen(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates inbuilt function substr assignment according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gsubstr(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief generates concatenation according to AST.md
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void Gconcat(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief decides what to generate from AST tree
/// @param tree
/// @param frame in which frame should be code generated. In most cases use LF
/// @param bodyVars variables defined inside main body
/// @param funVars variables defined inside function
/// @param inFunction is this structure generated in function
void generate(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction);

/// @brief Generates code from AST tree
/// @param tree
void startGenerate(AST* tree);