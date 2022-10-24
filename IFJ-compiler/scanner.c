/*********************************************************************
 * \file   scanner.c
 * \brief  Main file for scanner (IFJ project 2022)
 *
 * \author Matej Smida xsmida06
 * \date   October 2022
 *********************************************************************/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"

#define KW_COUNT 10

int row = 1;

// All reserved keywords
const char* KeyWords[KW_COUNT] = {"else", "float",  "function", "if",   "int",
                                  "null", "return", "string",   "void", "while"};

TokenType isKeyword(char* s) {

    for (int i = 0; i < KW_COUNT; i++) {
        if (strcmp(s, KeyWords[i]) == 0) {
            return KEYWORD;
        }
    }

    return ID;
}

T_token* getToken() {
    T_token* token = tokenInit();

    int c;

    States state = STATE_START;

    while (true) {

        c = getchar();

        token->row = row;

        switch (state) {
            case STATE_START:

                if (isalpha(c) || c == '_') {
                    AppendChar(token->val, c);
                    state = STATE_ID;
                }

                else if (c == '\n') {
                    row++;
                    token->row = row;
                }

                else if (isdigit(c)) {
                    AppendChar(token->val, c);
                    state = STATE_NUM;
                }

                else if (c == '?') {
                    token->type = QUESTION_MARK;
                    return token;
                }

                else if (c == ':') {
                    token->type = COLON;
                    return token;
                }

                else if (c == '!') {
                    state = STATE_NEG;
                }

                else if (c == '=') {
                    state = STATE_ASSIGN;
                }

                else if (c == '+') {
                    token->type = PLUS;
                    return token;
                }

                else if (c == '-') {
                    token->type = MINUS;
                    return token;
                }

                else if (c == '/') {
                    state = STATE_SLASH;
                }

                else if (c == '*') {
                    token->type = ASTERISK;
                    return token;
                }

                else if (c == '{') {
                    token->type = LEFT_CUR_BRACK;
                    return token;
                }

                else if (c == '(') {
                    token->type = LEFT_PAR;
                    return token;
                }

                else if (c == '}') {
                    token->type = RIGHT_CUR_BRACK;
                    return token;
                }

                else if (c == ')') {
                    token->type = RIGHT_PAR;
                    return token;
                }

                else if (c == '$') {
                    AppendChar(token->val, c);
                    state = STATE_VARIABLE_START;
                }

                else if (c == '"') {
                    AppendChar(token->val, c);
                    state = STATE_STRING;
                }

                else if (c == ',') {
                    token->type = COMMA;
                    return token;
                }

                else if (c == ';') {
                    token->type = SEMICOLON;
                    return token;
                }

                else if (c == '<') {
                    state = STATE_LESS;
                }

                else if (c == '>') {
                    state = STATE_MORE;
                }

                else if (isspace(c)) {
                    state = STATE_START;
                }

                else if (c == EOF) {
                    state = STATE_EOF;
                }

                else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Charactes is a letter so we need to check if it is ID or keyword
            case STATE_ID:
                if (isalpha(c) || isdigit(c) || c == '_') {
                    AppendChar(token->val, c);
                } else {
                    ungetc(c, stdin);
                    token->type = isKeyword(token->val->chars);
                    return token;
                }
                break;

            // Charactes is a number
            case STATE_NUM:
                if (isdigit(c)) {
                    AppendChar(token->val, c);
                }
                // Decimal point, changing to float
                else if (c == '.') {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_DEC;
                }
                // Exponent, changing to float
                else if (c == 'E' || c == 'e') {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_EXP;
                }
                // stopped reading, returning integer token type
                else {
                    ungetc(c, stdin);
                    token->type = INT;
                    return token;
                }
                break;

            // Need to add at least one digit after decimal point
            case STATE_FLOAT_DEC:;
                if (isdigit(c)) {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_DEC_NUM;
                }
                // or error
                else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Continue reading another digits if any
            case STATE_FLOAT_DEC_NUM:
                if (isdigit(c)) {
                    AppendChar(token->val, c);
                }
                // Exponent after decimal part (12.34e)
                else if (c == 'E' || c == 'e') {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_DEC_EXP;
                }
                // Stopped reading digits, did not get exponent, returning float
                else {
                    ungetc(c, stdin);
                    token->type = FLOAT;
                    return token;
                }
                break;

            // Exponent after decimal part can have + or - after E/e but it is not mandatory
            case STATE_FLOAT_DEC_EXP:
                if (c == '+' || c == '-') {
                    AppendChar(token->val, c);
                }
                // Need to add at least one digit after exponent
                else if (isdigit(c)) {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_DEC_EXP_NUM;
                }
                // or error
                else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Continue reading another digits in to exponent part if any
            case STATE_FLOAT_DEC_EXP_NUM:
                if (isdigit(c)) {
                    AppendChar(token->val, c);
                } else {
                    // Stopped reading digits, returning float with decimal part and exponent
                    ungetc(c, stdin);
                    token->type = FLOAT_DEC_EXP;
                    return token;
                }
                break;

            // Float doesn't have decimal part but have exponent
            case STATE_FLOAT_EXP:
                if (c == '+' || c == '-') {
                    AppendChar(token->val, c);
                }
                // Need to add at least one digit after exponent
                else if (isdigit(c)) {
                    AppendChar(token->val, c);
                    state = STATE_FLOAT_EXP_NUM;
                }
                // or error
                else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Continue reading digits if any
            case STATE_FLOAT_EXP_NUM:
                if (isdigit(c)) {
                    AppendChar(token->val, c);
                }
                // Stopped reading digits, returning float with exponent
                else {
                    ungetc(c, stdin);
                    token->type = FLOAT_EXP;
                    return token;
                }
                break;

            // Case !=
            case STATE_NEG:
                if (c == '=') {
                    state = STATE_NEG_EQ;
                } else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Case !==
            case STATE_NEG_EQ:
                if (c == '=') {
                    token->type = NOT_EQ;
                    return token;
                } else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Case ==
            case STATE_ASSIGN:
                if (c == '=') {
                    state = STATE_EQ;
                } else {
                    ungetc(c, stdin);
                    token->type = ASSIGN;
                    return token;
                }
                break;

            // Case ===
            case STATE_EQ:
                if (c == '=') {
                    token->type = EQ;
                    return token;
                } else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Case // single line comment
            case STATE_SLASH:
                if (c == '/') {
                    state = STATE_SINGLELINE_COMMENT;
                }
                // Case /* multiline comment
                else if (c == '*') {
                    state = STATE_MULTILINE_COMMENT;
                }
                // Returning division token type
                else {
                    ungetc(c, stdin);
                    token->type = DIV;
                    return token;
                }
                break;

            // Skipping comments and incrementing row counter
            case STATE_SINGLELINE_COMMENT:
                if (c == '\n') {
                    row++;
                    token->row = row;
                    state = STATE_START;
                } else if (c == EOF) {
                    state = STATE_EOF;
                }
                break;

            // Skipping multiline comments
            case STATE_MULTILINE_COMMENT:
                if (c == '\n') {
                    row++;
                    token->row = row;
                }
                // File ended but comment has not been properly ended => error
                else if (c == EOF) {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                // Case /* ______ *
                else if (c == '*') {
                    state = STATE_END_MULTILINE_COMMENT;
                }
                break;

            // Checking for endlines becouse * can be part of comment
            case STATE_END_MULTILINE_COMMENT:
                if (c == '\n') {
                    row++;
                    token->row = row;
                }
                // File ended but comment has not been properly ended => error
                else if (c == EOF) {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                // Comment has been properly ended
                else if (c == '/') {
                    state = STATE_START;
                }
                break;

            // Variable is just ID with '$' prefix, so it must start with either a letter or an
            // underscore
            case STATE_VARIABLE_START:
                if (isalpha(c) || c == '_') {
                    AppendChar(token->val, c);
                    state = STATE_VARIABLE;
                } else {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                }
                break;

            // Continue rading rest of variable
            case STATE_VARIABLE:
                if (isalpha(c) || isdigit(c) || c == '_') {
                    AppendChar(token->val, c);
                } else {
                    ungetc(c, stdin);
                    token->type = VAR;
                    return token;
                }
                break;

            case STATE_STRING:
                if (c == '"') {
                    AppendChar(token->val, c);
                    token->type = STRING;
                    return token;
                }
                // String has not been properly ended => error
                else if (c == EOF) {
                    ungetc(c, stdin);
                    state = STATE_ERROR;
                } else {
                    AppendChar(token->val, c);
                }
                break;

            // Case <
            case STATE_LESS:
                // Case <=
                if (c == '=') {
                    token->type = LESS_EQ;
                    return token;
                }
                ungetc(c, stdin);
                token->type = LESS;
                return token;

                break;

            // Case >
            case STATE_MORE:
                // Case >=
                if (c == '=') {
                    token->type = MORE_EQ;
                    return token;
                }
                ungetc(c, stdin);
                token->type = MORE;
                return token;

                break;

            case STATE_EOF:
                token->type = TOKEN_EOF;
                return token;
                break;

            case STATE_ERROR:
                token->type = NOT_TOKEN;
                return token;
                break;
        }
    }
}

T_token* tokenInit() {

    T_token* token;

    token = (T_token*)malloc(sizeof(T_token));
    if (token == NULL) {
        WriteErrorMessage(INTERNAL_COMPILER_ERROR);
        return NULL;
    }

    int err;

    token->val = InitString(&err);
    if (err != 0) {
        WriteErrorMessage(INTERNAL_COMPILER_ERROR);
        return NULL;
    }

    return token;
}

int tokenDtor(T_token* token) {

    if (token == NULL) {
        return NULL_POINTER_EXCEPTION;
    }

    DisposeString(token->val);
    free(token);
    token = NULL;
    return 0;
}