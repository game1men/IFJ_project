/*********************************************************************
 * \file   scanner.h
 * \brief  Header file for scanner (IFJ project 2022)
 *
 * \author Matej Smida xsmida06
 * \date   October 2022
 *********************************************************************/
#ifndef SCANNER_H
#define SCANNER_H

#include "utils.h"

/**
 * \brief Enum type for different types of tokens
 */
typedef enum {
    INT,           // 0
    FLOAT,         // 1
    FLOAT_DEC_EXP, // 2
    FLOAT_EXP,     // 3
    STRING,        // 4
    VAR,           // 5
    KEYWORD,       // 6
    ID,            // 7

    QUESTION_MARK, // 8
    COLON,         // 9

    NOT_EQ, // 10
    EQ,     // 11
    ASSIGN, // 12

    PLUS,     // 13
    MINUS,    // 14
    DIV,      // 15
    ASTERISK, // 16

    LEFT_CUR_BRACK,  // 17
    LEFT_PAR,        // 18
    RIGHT_CUR_BRACK, // 19
    RIGHT_PAR,       // 20

    COMMA,     // 21
    SEMICOLON, // 22

    LESS,    // 23
    LESS_EQ, // 24
    MORE,    // 25
    MORE_EQ, // 26

    TOKEN_EOF, // 27

    NOT_TOKEN, // 28

    DOT, // 29

    EPILOG // 30
} TokenType;

/**
 * \brief Enum type for all FSM states
 */
typedef enum {
    STATE_START,

    STATE_ID,
    STATE_NUM,

    STATE_FLOAT_DEC,
    STATE_FLOAT_DEC_NUM,
    STATE_FLOAT_DEC_EXP,
    STATE_FLOAT_DEC_EXP_NUM,

    STATE_FLOAT_EXP,
    STATE_FLOAT_EXP_NUM,

    STATE_NEG,
    STATE_NEG_EQ,

    STATE_ASSIGN,
    STATE_EQ,

    STATE_SLASH,
    STATE_SINGLELINE_COMMENT,
    STATE_MULTILINE_COMMENT,
    STATE_END_MULTILINE_COMMENT,

    STATE_VARIABLE_START,
    STATE_VARIABLE,

    STATE_STRING,

    STATE_LESS,
    STATE_MORE,

    STATE_EOF,

    STATE_ERROR,

    STATE_QUESTION_MARK,
    STATE_BACKSLASH,
    STATE_DOLLAR_STRING
} States;

/**
 * \brief Structure of a single token
 */
typedef struct token {
    String* val;
    TokenType type;
    int row;
} T_token;

/**
 * \brief Allocates a memory for token structure
 * \return A pointer to the initialized token structure
 */
T_token* tokenInit();

/*
 * \brief Reads characters from stdin and converts them to tokens based on lexical analysis
 *
 * Token has 3 parts:
 * token->val->chars = names of functions ID's, variables and strigns
 * token->row        = row on which token is located
 * token->type       = type of token based on enum TokenTypes (example 0 = INT)
 *
 * \return A pointer to the token structure initialized before with updated attributes
 */
T_token* getToken();

/*
 * \brief Checks if token is keyword or not
 * \return Token type, either keyword or id
 */
TokenType isKeyword(char* s);

/**
 * \param pointer to strucure
 * \brief Frees token structure and all of its content
 * \return 0 if everything was succesfull or NULL_POINTER_EXCEPTION if pointer is null
 */
int tokenDtor(T_token*);


/**
 * \brief skips chracters from stdin until searched character
 * \return 1 if everything was succesfull or EOF if searched character was not found
 */
int skipUntil(char c);

/**
 * \brief detects if file contains prolog in correct format
 * \return 1 if everything was succesfull or EOF if EOF was reached
 */
int skipComment();

/**
 * \brief detects if file contains prolog in correct format
 * \return 1 if everything was succesfull or 0 if prolog in correct format was not found
 */
int detectProlog();

T_token* GetNextBufferToken(List*);

#endif // !SCANNER_H
