/*********************************************************************
 * \file   generator.c
 * \brief  Main file for generator (IFJ project 2022)
 *
 * \author Rene Ceska xceska06
 * \date   October 2022
 *********************************************************************/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "generator.h"


char* _stringConvert(String* s) {
    int err;
    String* newS = InitString(&err);

    if (err != OK) return NULL;

    int x = 0;
    while (s->chars[x] != EOS) {
        if (s->chars[x] < 33) {
            if (s->chars[x] < 10) {
                if (AppendChar(newS, '\\') != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, '0') != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, '0') != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, s->chars[x]) != OK)
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            } else {
                char res[4];
                sprintf(res, "%02d", s->chars[x]);
                if (AppendChar(newS, '\\') != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, '0') != OK) exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, res[0]) != OK)
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                if (AppendChar(newS, res[1]) != OK)
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }
        } else {
            if (s->chars[x] == '\\' && x < s->length - 1) {

                switch (s->chars[x + 1]) {
                    case '"':
                        if (AppendChar(newS, '"') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        x++;
                        break;
                    case 'n':
                        if (AppendChar(newS, '\\') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '0') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '1') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '0') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        x++;
                        break;
                    case 't':
                        if (AppendChar(newS, '\\') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '0') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '0') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '9') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        x++;
                        break;
                    case '\\':
                        if (AppendChar(newS, '\\') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '0') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '9') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        if (AppendChar(newS, '2') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        x++;
                        break;
                    case 'x': // detect hexa

                        if (x < s->length - 3 &&
                            ((s->chars[x + 2] >= 48 && s->chars[x + 2] <= 57) ||
                             (s->chars[x + 2] >= 65 && s->chars[x + 2] <= 70) ||
                             (s->chars[x + 2] >= 97 && s->chars[x + 2] <= 102)) &&
                            ((s->chars[x + 3] >= 48 && s->chars[x + 3] <= 57) ||
                             (s->chars[x + 3] >= 65 && s->chars[x + 3] <= 70) ||
                             (s->chars[x + 3] >= 97 && s->chars[x + 3] <= 102))) {
                            // convert from hexa to dec
                            int first = 0;
                            if (s->chars[x + 2] >= 65) {
                                first = s->chars[x + 2] - 55;
                            } else if (s->chars[x + 2] >= 97) {
                                first = s->chars[x + 2] - 87;
                            } else {
                                first = s->chars[x + 2] - 48;
                            }
                            int second = 0;
                            if (s->chars[x + 3] >= 65) {
                                second = s->chars[x + 3] - 55;
                            } else if (s->chars[x + 3] >= 97) {
                                second = s->chars[x + 3] - 87;
                            } else {
                                second = s->chars[x + 3] - 48;
                            }
                            int result = first * 16 + second;
                            char res[4];
                            sprintf(res, "%02d", result);
                            if (AppendChar(newS, '\\') != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, '0') != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, res[0]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, res[1]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            x += 3;
                        }

                        break;
                    case '$':
                        if (AppendChar(newS, '$') != OK)
                            exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        x++;
                        break;
                    default:

                        if (x < s->length - 3 && s->chars[x + 1] >= 48 && s->chars[x + 1] <= 57 &&
                            s->chars[x + 2] >= 48 && s->chars[x + 2] <= 57 &&
                            s->chars[x + 3] >= 48 && s->chars[x + 3] <= 57) {
                            // convert from hexa to dec
                            int first = s->chars[x + 1] - 48;
                            int second = s->chars[x + 2] - 48;
                            int third = s->chars[x + 3] - 48;
                            int result = 64 * first + 8 * second + third;
                            char res[4];
                            sprintf(res, "%03d", result);
                            if (AppendChar(newS, '\\') != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, res[0]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, res[1]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            if (AppendChar(newS, res[2]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                            x += 3;
                        } else {
                            if (AppendChar(newS, s->chars[x]) != OK)
                                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
                        }
                        break;
                }
            } else {
                if (AppendChar(newS, s->chars[x]) != OK)
                    exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
            }
        }
        x++;
    }
    return newS->chars;
}

void convToFloat(bool isFloat) {
    if (isFloat) {
        printf("INT2FLOATS\n");
    }
}

void header() {
    // helper functions
    printf(".IFJcode22\n"
           "\n"
           "jump %%%%%%owerErrorHandler\n"
           "label %%%%%%err1\n"
           "exit int@1\n"
           "label %%%%%%err2\n"
           "exit int@2\n"
           "label %%%%%%err3\n"
           "exit int@3\n"
           "label %%%%%%err4\n"
           "exit int@4\n"
           "label %%%%%%err5\n"
           "exit int@5\n"
           "label %%%%%%err6\n"
           "exit int@6\n"
           "label %%%%%%err7\n"
           "exit int@7\n"
           "label %%%%%%err8\n"
           "exit int@8\n"
           "label %%%%%%owerErrorHandler\n"
           "\n"
           "defvar GF@%%read \n"
           "defvar GF@%%%%%%tmp1\n"
           "defvar GF@%%%%%%tmp2\n"
           "defvar GF@%%%%%%tmp3\n"
           "defvar GF@%%%%%%tmp4\n"
           "defvar GF@%%%%%%strlen\n"
           "defvar GF@%%%%%%strlenvar\n"
           "defvar GF@%%%%%%int\n"
           "move GF@%%%%%%int string@int\n"
           "defvar GF@%%%%%%string\n"
           "move GF@%%%%%%string string@string\n"
           "defvar GF@%%%%%%bool\n"
           "move GF@%%%%%%bool string@bool\n"
           "defvar GF@%%%%%%nil\n"
           "move GF@%%%%%%nil string@nil\n"
           "defvar GF@%%%%%%float\n"
           "move GF@%%%%%%float string@float\n"
           "createframe\n"
           "pushframe\n"
           "\n"
           "jump %%%%%%defVarsBody\n"
           "label %%%%%%bodyStart\n"
           "######substr\n"
           "# string\n"
           "# from\n"
           "# to\n"
           "##start------------------------------------------------over%%%%%%substr\n"
           "jump over%%%%%%substr\n"
           "label %%%%%%substr\n"
           "createframe\n");
    printf("pushframe\n"
           "    defvar LF@%%%%%%to\n"
           "    defvar LF@%%%%%%from\n"
           "    defvar LF@%%%%%%string\n"
           "\n"
           "\n"
           "    pops LF@%%%%%%to\n"
           "    pops LF@%%%%%%from\n"
           "    pops LF@%%%%%%string\n"
           "\n"
           "\n"
           "    defvar LF@%%%%%%strLen\n"
           "    STRLEN LF@%%%%%%strLen LF@%%%%%%string\n"
           "\n"
           "    #conditions\n"
           "    defvar LF@%%%%condition\n"
           "    defvar LF@%%%%condition2\n"
           "    LT LF@%%%%condition LF@%%%%%%to int@0\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "    LT LF@%%%%condition LF@%%%%%%from int@0\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "    LT LF@%%%%condition LF@%%%%%%to LF@%%%%%%from\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "    LT LF@%%%%condition LF@%%%%%%to LF@%%%%%%from\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "    GT LF@%%%%condition LF@%%%%%%to LF@%%%%%%strLen\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "    GT LF@%%%%condition LF@%%%%%%from LF@%%%%%%strLen\n"
           "    JUMPIFEQ retNull%%%%%%substr LF@%%%%condition bool@true\n"
           "\n"
           "    defvar LF@%%%%%%tmpChar\n"
           "    defvar LF@%%%%%%iter\n"
           "      defvar LF@%%%%%%tmpString\n"
           "      move LF@%%%%%%tmpString string@\n"
           "    move LF@%%%%%%iter int@0\n"
           "    label lop%%%%%%substr\n"
           "    JUMPIFEQ endLoop%%%%%%substr LF@%%%%%%from LF@%%%%%%to\n"
           "\n"
           "    getchar  LF@%%%%%%tmpChar LF@%%%%%%string LF@%%%%%%from\n"
           "    concat LF@%%%%%%tmpString LF@%%%%%%tmpString LF@%%%%%%tmpChar\n"
           "\n"
           "    add LF@%%%%%%iter  LF@%%%%%%iter int@1\n"
           "    add LF@%%%%%%from  LF@%%%%%%from int@1\n"
           "\n"
           "    jump lop%%%%%%substr\n"
           "\n"
           "    label endLoop%%%%%%substr\n"
           "        pushs LF@%%%%%%tmpString\n"
           "    jump ret%%%%%%substr\n"
           "\n"
           "label retNull%%%%%%substr\n"
           "pushs nil@nil\n"
           "\n"
           "label ret%%%%%%substr\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%substr\n"
           "##end------------------------------------------------over%%%%%%substr\n"
           "\n"
           "#converts from int/float/nil to float. anything else is err7\n"
           "##start------------------------------------------------over%%%%%%convertToInt\n"
           "jump over%%%%%%convertToInt\n"
           "label %%%%%%convertToInt\n"
           "createframe\n"
           "pushframe\n"
           "defvar LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var1\n");
    printf("    defvar LF@%%%%%%var1type\n"
           "\n"
           "    #get type\n"
           "    type LF@%%%%%%var1type LF@%%%%%%var1\n"
           "\n"
           "    #jump where is type\n"
           "    jumpifeq int%%%%%%convertToInt  LF@%%%%%%var1type string@int\n"
           "    jumpifeq float%%%%%%convertToInt LF@%%%%%%var1type string@float\n"
           "    jumpifeq nil%%%%%%convertToInt  LF@%%%%%%var1type string@nil\n"
           "\n"
           "    #if neither its err7\n"
           "    jump  %%%%%%err7\n"
           "\n"
           "    label int%%%%%%convertToInt\n"
           "        pushs LF@%%%%%%var1\n"
           "         #just return alredy right type\n"
           "\n"
           "\n"
           "    jump ret%%%%%%convertToInt\n"
           "\n"
           "    label float%%%%%%convertToInt\n"
           "        pushs LF@%%%%%%var1\n"
           "        FLOAT2INTs #convert to int\n"
           "    jump ret%%%%%%convertToInt\n"
           "\n"
           "    label nil%%%%%%convertToInt # nill is converted to zero\n"
           "        pushs int@0\n"
           "        jump ret%%%%%%convertToInt\n"
           "    label ret%%%%%%convertToInt\n"
           "\n"
           "\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%convertToInt\n"
           "##end------------------------------------------------over%%%%%%convertToInt\n"
           "##testts\n"
           "\n"
           "##start------------------------------------------------over%%%%%%typeCheck\n"
           "jump over%%%%%%typeCheck\n"
           "label %%%%%%typeCheck\n"
           "createframe\n"
           "pushframe\n"
           "    defvar LF@%%%%%%var\n"
           "    defvar LF@%%%%%%varTypeCheck\n"
           "    defvar LF@%%%%%%err\n"
           "    pops LF@%%%%%%err\n"
           "    pops LF@%%%%%%varTypeCheck\n"
           "    pops LF@%%%%%%var\n"
           "\n"
           "        defvar LF@%%%%%%varType\n"
           "        type LF@%%%%%%varType LF@%%%%%%var\n"
           "\n"
           "\n"
           "        JUMPIFEQ   ret%%%%%%typeCheck  LF@%%%%%%varType   LF@%%%%%%varTypeCheck\n"
           "\n"
           "        jumpifeq %%%%%%err1  LF@%%%%%%err int@1\n"
           "        jumpifeq %%%%%%err2  LF@%%%%%%err int@2\n"
           "        jumpifeq %%%%%%err3  LF@%%%%%%err int@3\n"
           "        jumpifeq %%%%%%err4  LF@%%%%%%err int@4\n"
           "        jumpifeq %%%%%%err5  LF@%%%%%%err int@5\n"
           "        jumpifeq %%%%%%err6  LF@%%%%%%err int@6\n"
           "        jumpifeq %%%%%%err7  LF@%%%%%%err int@7\n"
           "\n"
           "label ret%%%%%%typeCheck\n"
           "pushs LF@%%%%%%var\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%typeCheck\n"
           "##end------------------------------------------------over%%%%%%typeCheck\n");
    printf(
        "\n"
        "##start------------------------------------------------over%%%%%%typeCheck\n"
        "jump over%%%%%%typeCheckIgnoreNil\n"
        "label %%%%%%typeCheckIgnoreNil\n"
        "createframe\n"
        "pushframe\n"
        "    defvar LF@%%%%%%var\n"
        "    defvar LF@%%%%%%varTypeCheck\n"
        "    defvar LF@%%%%%%err\n"
        "    pops LF@%%%%%%err\n"
        "    pops LF@%%%%%%varTypeCheck\n"
        "    pops LF@%%%%%%var\n"
        "\n"
        "        defvar LF@%%%%%%varType\n"
        "        type LF@%%%%%%varType LF@%%%%%%var\n"
        "\n"
        "\n"
        "        JUMPIFEQ   ret%%%%%%typeCheckIgnoreNil  LF@%%%%%%varType   LF@%%%%%%varTypeCheck\n"
        "        JUMPIFEQ   ret%%%%%%typeCheckIgnoreNil  LF@%%%%%%varType   string@nil\n"
        "\n"
        "        jumpifeq %%%%%%err1  LF@%%%%%%err int@1\n"
        "        jumpifeq %%%%%%err2  LF@%%%%%%err int@2\n"
        "        jumpifeq %%%%%%err3  LF@%%%%%%err int@3\n"
        "        jumpifeq %%%%%%err4  LF@%%%%%%err int@4\n"
        "        jumpifeq %%%%%%err5  LF@%%%%%%err int@5\n"
        "        jumpifeq %%%%%%err6  LF@%%%%%%err int@6\n"
        "        jumpifeq %%%%%%err7  LF@%%%%%%err int@7\n"
        "\n"
        "label ret%%%%%%typeCheckIgnoreNil\n"
        "pushs LF@%%%%%%var\n"
        "popframe\n"
        "return\n"
        "label over%%%%%%typeCheckIgnoreNil\n"
        "##end------------------------------------------------over%%%%%%typeCheckIgnoreNil\n"
        "\n"
        "#converts from int/float/nil to float. anything else is err7\n"
        "##start------------------------------------------------over%%%%%%convertToFloat\n"
        "jump over%%%%%%convertToFloat\n"
        "label %%%%%%convertToFloat\n"
        "createframe\n"
        "pushframe\n"
        "defvar LF@%%%%%%var1\n"
        "    pops LF@%%%%%%var1\n"
        "    defvar LF@%%%%%%var1type\n"
        "\n"
        "    #get type\n"
        "    type LF@%%%%%%var1type LF@%%%%%%var1\n"
        "\n"
        "    #jump where is type\n"
        "    jumpifeq int%%%%%%convertToFloat  LF@%%%%%%var1type string@int\n"
        "    jumpifeq float%%%%%%convertToFloat LF@%%%%%%var1type string@float\n"
        "    jumpifeq nil%%%%%%convertToFloat  LF@%%%%%%var1type string@nil\n"
        "\n"
        "    #if neither its err7\n"
        "    jump  %%%%%%err7\n"
        "\n"
        "    label int%%%%%%convertToFloat\n"
        "        pushs LF@%%%%%%var1\n"
        "        INT2FLOATs #convert to float\n"
        "\n"
        "    jump ret%%%%%%convertToFloat\n"
        "\n"
        "    label float%%%%%%convertToFloat\n"
        "        pushs LF@%%%%%%var1\n"
        "        #just return alredy right type\n"
        "    jump ret%%%%%%convertToFloat\n"
        "\n"
        "    label nil%%%%%%convertToFloat # nill is converted to zero\n"
        "        pushs float@0x0p+0\n"
        "        jump ret%%%%%%convertToFloat\n"
        "    label ret%%%%%%convertToFloat\n"
        "\n"
        "\n"
        "popframe\n"
        "return\n"
        "label over%%%%%%convertToFloat\n"
        "##end------------------------------------------------over%%%%%%convertToFloat\n"
        "\n"
        "#converts int/float/nil according to assignment to same type. anything else is err7\n"
        "##start------------------------------------------------over%%%%%%convertOperation\n"
        "jump over%%%%%%convertOperation\n"
        "label %%%%%%convertOperation\n"
        "createframe\n"
        "pushframe\n"
        "    defvar LF@%%%%%%var2\n"
        "    pops LF@%%%%%%var2\n");
    printf("    defvar LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var1\n"
           "    defvar LF@%%%%%%var1type\n"
           "    defvar LF@%%%%%%var2type\n"
           "\n"
           "    #get type\n"
           "    type LF@%%%%%%var1type LF@%%%%%%var1\n"
           "    type LF@%%%%%%var2type LF@%%%%%%var2\n"
           "\n"
           "    #jump where is type\n"
           "    jumpifeq float1%%%%%%convertOperation LF@%%%%%%var1type string@float\n"
           "    jumpifeq float2%%%%%%convertOperation LF@%%%%%%var2type string@float\n"
           "    jumpifeq int1%%%%%%convertOperation  LF@%%%%%%var1type string@int\n"
           "    jumpifeq int2%%%%%%convertOperation  LF@%%%%%%var1type string@int\n"
           "\n"
           "        #if neither try converting both to int\n"
           "        pushs LF@%%%%%%var2\n"
           "        call %%%%%%convertToInt\n"
           "        pushs LF@%%%%%%var1\n"
           "        call %%%%%%convertToInt\n"
           "        jump ret%%%%%%convertOperation\n"
           "\n"
           "\n"
           "    #----------------------------------------\n"
           "    label float1%%%%%%convertOperation\n"
           "    #--------------body---------------------\n"
           "        #convert var2 to float\n"
           "        pushs LF@%%%%%%var2\n"
           "        call %%%%%%convertToFloat\n"
           "        pushs LF@%%%%%%var1\n"
           "\n"
           "\n"
           "       #---------------------------------------\n"
           "    jump ret%%%%%%convertOperation\n"
           "    #---------------------------------------\n"
           "\n"
           "     #----------------------------------------\n"
           "    label float2%%%%%%convertOperation\n"
           "    #--------------body---------------------\n"
           "        #convert var1 to float\n"
           "        pushs LF@%%%%%%var2\n"
           "        pushs LF@%%%%%%var1\n"
           "        call %%%%%%convertToFloat\n"
           "\n"
           "       #---------------------------------------\n"
           "    jump ret%%%%%%convertOperation\n"
           "    #---------------------------------------\n"
           "\n"
           "     #----------------------------------------\n"
           "    label int1%%%%%%convertOperation\n"
           "    #--------------body---------------------\n"
           "\n"
           "        #convert var2 to int\n"
           "        pushs LF@%%%%%%var2\n"
           "        call %%%%%%convertToInt\n"
           "        pushs LF@%%%%%%var1\n"
           "\n"
           "       #---------------------------------------\n");
    printf("    jump ret%%%%%%convertOperation\n"
           "    #---------------------------------------\n"
           "\n"
           "     #----------------------------------------\n"
           "    label int2%%%%%%convertOperation\n"
           "    #--------------body---------------------\n"
           "\n"
           "         #convert var1 to int\n"
           "        pushs LF@%%%%%%var2\n"
           "            pushs LF@%%%%%%var1\n"
           "        call %%%%%%convertToInt\n"
           "\n"
           "\n"
           "       #---------------------------------------\n"
           "    jump ret%%%%%%convertOperation\n"
           "    #---------------------------------------\n"
           "\n"
           "label ret%%%%%%convertOperation\n"
           "\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%convertOperation\n"
           "##end------------------------------------------------over%%%%%%convertOperation\n"
           "##testts\n"
           "\n"
           "#converts nil/string to string\n"
           "##start------------------------------------------------over%%%%%%convertToString\n"
           "jump over%%%%%%convertToString\n"
           "label %%%%%%convertToString\n"
           "createframe\n"
           "pushframe\n"
           "    defvar LF@%%%%%%var\n"
           "    pops LF@%%%%%%var\n"
           "    defvar LF@%%%%%%varType\n"
           "\n"
           "    #get type\n"
           "    type LF@%%%%%%varType LF@%%%%%%var\n"
           "\n"
           "    #jump where is type\n"
           "    jumpifeq nil%%%%%%convertToString  LF@%%%%%%varType  string@nil\n"
           "    jumpifeq string%%%%%%convertToString LF@%%%%%%varType  string@string\n"
           "\n"
           "    #if neither its err7\n"
           "    jump  %%%%%%err7\n"
           "\n"
           "    label string%%%%%%convertToString\n"
           "        pushs LF@%%%%%%var\n"
           "         #just return alredy right type\n"
           "\n"
           "    jump ret%%%%%%convertToString\n"
           "\n"
           "    label nil%%%%%%convertToString\n"
           "        pushs string@ #push empty string\n"
           "    jump ret%%%%%%convertToString\n"
           "\n"
           "    label ret%%%%%%convertToString\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%convertToString\n"
           "##end------------------------------------------------over%%%%%%convertToString\n"
           "##testts\n"
           "\n"
           "#converts from int/float/nil/bool/string to bool. anything else is err7\n"
           "##start------------------------------------------------over%%%%%%convertToBool\n"
           "jump over%%%%%%convertToBool\n"
           "label %%%%%%convertToBool\n"
           "createframe\n"
           "pushframe\n"
           "defvar LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var1\n"
           "    defvar LF@%%%%%%var1type\n"
           "\n"
           "    #get type\n"
           "    type LF@%%%%%%var1type LF@%%%%%%var1\n"
           "\n"
           "    #jump where is type\n"
           "    jumpifeq int%%%%%%convertToBool  LF@%%%%%%var1type string@int\n"
           "    jumpifeq float%%%%%%convertToBool LF@%%%%%%var1type string@float\n"
           "    jumpifeq nil%%%%%%convertToBool  LF@%%%%%%var1type string@nil\n"
           "    jumpifeq string%%%%%%convertToBool  LF@%%%%%%var1type string@string\n"
           "    jumpifeq bool%%%%%%convertToBool  LF@%%%%%%var1type string@bool\n"
           "\n"
           "    #if neither its err7\n"
           "    jump  %%%%%%err7\n"
           "\n"
           "    label int%%%%%%convertToBool\n");
    printf("        jumpifeq false%%%%%%convertToBool LF@%%%%%%var1 int@0\n"
           "        pushs bool@true\n"
           "\n"
           "    jump ret%%%%%%convertToBool\n"
           "\n"
           "    label float%%%%%%convertToBool\n"
           "        jumpifeq false%%%%%%convertToBool LF@%%%%%%var1  float@0x0p+0\n"
           "        pushs bool@true\n"
           "\n"
           "    jump ret%%%%%%convertToBool\n"
           "\n"
           "    label nil%%%%%%convertToBool\n"
           "\n"
           "    jump false%%%%%%convertToBool\n"
           "\n"
           "    label string%%%%%%convertToBool\n"
           "     jumpifeq false%%%%%%convertToBool LF@%%%%%%var1 string@0\n"
           "     jumpifeq false%%%%%%convertToBool LF@%%%%%%var1 string@\n"
           "  pushs bool@true\n"
           "    jump ret%%%%%%convertToBool\n"
           "\n"
           "    label bool%%%%%%convertToBool\n"
           "        pushs LF@%%%%%%var1\n"
           "        jump ret%%%%%%convertToBool\n"
           "\n"
           "    label false%%%%%%convertToBool\n"
           "    pushs bool@false\n"
           "    label ret%%%%%%convertToBool\n"
           "\n"
           "\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%convertToBool\n"
           "##end------------------------------------------------over%%%%%%convertToBool\n"
           "#compares strings/nils (helper function for compare)\n"
           "#var1 string/nil\n"
           "#var2 string/nil\n"
           "#va3 string containing (LT,GT,EQ, LEQ, GEQ)\n"
           "##start------------------------------------------------over%%%%%%strcmp\n"
           "jump over%%%%%%strcmp\n"
           "label %%%%%%strcmp\n"
           "createframe\n"
           "pushframe\n"
           "    defvar LF@%%%%%%var2\n"
           "    pops LF@%%%%%%var2\n"
           "    defvar LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var1\n"
           "    defvar LF@%%%%%%comp\n"
           "    pops LF@%%%%%%comp\n"
           "\n"
           "\n"
           "    #get types\n"
           "    defvar  LF@%%%%%%var1type\n"
           "    type    LF@%%%%%%var1type   LF@%%%%%%var1\n"
           "    defvar  LF@%%%%%%var2type\n"
           "    type    LF@%%%%%%var2type   LF@%%%%%%var2\n"
           "\n"
           "\n"
           "    #convert both varriables to strings\n"
           "    pushs LF@%%%%%%var1\n"
           "    call %%%%%%convertToString\n"
           "    pops LF@%%%%%%var1\n"
           "\n"
           "    pushs LF@%%%%%%var2\n"
           "    call %%%%%%convertToString\n"
           "    pops LF@%%%%%%var2\n"
           "\n"
           "\n"
           "\n"
           "    JUMPIFEQ LT%%%%%%strcmp    LF@%%%%%%comp  string@LT\n"
           "    JUMPIFEQ GT%%%%%%strcmp    LF@%%%%%%comp  string@GT\n"
           "    JUMPIFEQ EQ%%%%%%strcmp    LF@%%%%%%comp  string@EQ\n"
           "    JUMPIFEQ LEQ%%%%%%strcmp   LF@%%%%%%comp  string@LEQ\n"
           "    JUMPIFEQ GEQ%%%%%%strcmp   LF@%%%%%%comp  string@GEQ\n"
           "    JUMPIFEQ NEQ%%%%%%strcmp   LF@%%%%%%comp  string@NEQ\n"
           "\n"
           "    label LT%%%%%%strcmp\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         LTS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "    label GT%%%%%%strcmp\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         GTS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "    label EQ%%%%%%strcmp\n"
           "        pushs LF@%%%%%%var1\n"
           "        pushs LF@%%%%%%var2\n"
           "        EQS\n"
           "        pushs LF@%%%%%%var1type\n"
           "        pushs LF@%%%%%%var2type\n");
    printf("        EQS\n"
           "        ANDS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "    label NEQ%%%%%%strcmp\n"
           "        pushs LF@%%%%%%var1\n"
           "        pushs LF@%%%%%%var2\n"
           "        EQS\n"
           "        pushs LF@%%%%%%var1type\n"
           "        pushs LF@%%%%%%var2type\n"
           "        EQS\n"
           "        ANDS\n"
           "        NOTS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "    label LEQ%%%%%%strcmp\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         LTS\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         EQS\n"
           "         ORS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "    label GEQ%%%%%%strcmp\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         GTS\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         EQS\n"
           "         ORS\n"
           "    jump ret%%%%%%strcmp\n"
           "\n"
           "\n"
           "\n"
           "\n"
           "label ret%%%%%%strcmp\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%strcmp\n"
           "##end------------------------------------------------over%%%%%%strcmp\n"
           "\n"
           "\n"
           "\n"
           "#compares values\n"
           "#var1 value\n"
           "#var2 value\n"
           "#va3 string containing (LT,GT,EQ, LEQ, GEQ, NEQ)\n"
           "##start------------------------------------------------over%%%%%%compare\n"
           "jump over%%%%%%compare\n"
           "label %%%%%%compare\n"
           "createframe\n"
           "pushframe\n"
           "    defvar LF@%%%%%%var2\n"
           "    pops LF@%%%%%%var2\n"
           "    defvar LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var1\n"
           "    defvar LF@%%%%%%comp\n"
           "    pops LF@%%%%%%comp\n"
           "\n"
           "    #get types\n"
           "    defvar  LF@%%%%%%var1type\n"
           "    type    LF@%%%%%%var1type   LF@%%%%%%var1\n"
           "    defvar  LF@%%%%%%var2type\n"
           "    type    LF@%%%%%%var2type   LF@%%%%%%var2\n"
           "\n"
           "    #if any var is string go to string compare\n");
    printf("    JUMPIFEQ c%%%%%%compare    LF@%%%%%%var1type  string@string\n"
           "    JUMPIFEQ c%%%%%%compare    LF@%%%%%%var2type  string@string\n"
           "    jump ower1%%%%%%compare\n"
           "    label c%%%%%%compare\n"
           "    pushs LF@%%%%%%comp\n"
           "    pushs LF@%%%%%%var1\n"
           "    pushs LF@%%%%%%var2\n"
           "    call  %%%%%%strcmp\n"
           "    jump ret%%%%%%compare\n"
           "    label ower1%%%%%%compare\n"
           "\n"
           "    #convert to same type\n"
           "    pushs LF@%%%%%%var1\n"
           "    pushs LF@%%%%%%var2\n"
           "    call %%%%%%convertOperation\n"
           "    pops LF@%%%%%%var1\n"
           "    pops LF@%%%%%%var2\n"
           "\n"
           "\n"
           "    JUMPIFEQ LT%%%%%%compare    LF@%%%%%%comp  string@LT\n"
           "    JUMPIFEQ GT%%%%%%compare    LF@%%%%%%comp  string@GT\n"
           "    JUMPIFEQ EQ%%%%%%compare    LF@%%%%%%comp  string@EQ\n"
           "    JUMPIFEQ LEQ%%%%%%compare   LF@%%%%%%comp  string@LEQ\n"
           "    JUMPIFEQ GEQ%%%%%%compare   LF@%%%%%%comp  string@GEQ\n"
           "    JUMPIFEQ NEQ%%%%%%compare   LF@%%%%%%comp  string@NEQ\n"
           "\n"
           "    label LT%%%%%%compare\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         LTS\n"
           "    jump ret%%%%%%compare\n"
           "\n"
           "    label GT%%%%%%compare\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         GTS\n"
           "    jump ret%%%%%%compare\n"
           "\n"
           "    label EQ%%%%%%compare\n"
           "        pushs LF@%%%%%%var1\n"
           "        pushs LF@%%%%%%var2\n"
           "        EQS\n"
           "        pushs LF@%%%%%%var1type\n"
           "        pushs LF@%%%%%%var2type\n"
           "        EQS\n"
           "        ANDS\n"
           "    jump ret%%%%%%compare\n"
           "\n"
           "    label NEQ%%%%%%compare\n"
           "        pushs LF@%%%%%%var1\n"
           "        pushs LF@%%%%%%var2\n"
           "        EQS\n"
           "        pushs LF@%%%%%%var1type\n"
           "        pushs LF@%%%%%%var2type\n"
           "        EQS\n"
           "        ANDS\n"
           "        NOTS\n"
           "    jump ret%%%%%%compare\n"
           "\n"
           "    label LEQ%%%%%%compare\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         LTS\n"
           "         pushs LF@%%%%%%var1\n"
           "         pushs LF@%%%%%%var2\n"
           "         EQS\n"
           "         ORS\n"
           "    jump ret%%%%%%compare\n"
           "\n"
           "    label GEQ%%%%%%compare\n"
           "         pushs LF@%%%%%%var2\n"
           "         pushs LF@%%%%%%var1\n"
           "         GTS\n"
           "         pushs LF@%%%%%%var1\n"
           "         pushs LF@%%%%%%var2\n"
           "         EQS\n"
           "         ORS\n"
           "    jump ret%%%%%%compare\n");
    printf("\n"
           "\n"
           "\n"
           "\n"
           "label ret%%%%%%compare\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%compare\n"
           "##end------------------------------------------------over%%%%%%compare\n"
           "######prints to stdout\n"
           "# arg0\n"
           "# ...\n"
           "# argn\n"
           "#int argument count (without this argument)\n"
           "##start------------------------------------------------over%%%%%%printf\n"
           "jump over%%%%%%printf\n"
           "label %%%%%%printf\n"
           "createframe\n"
           "pushframe\n"
           "    defvar LF@%%%%%%argcount\n"
           "    pops LF@%%%%%%argcount\n"
           "      defvar LF@%%%%%%iter\n"
           "      move LF@%%%%%%iter int@0\n"
           "\n"
           "        JUMPIFEQ   ret%%%%%%printf int@0  LF@%%%%%%argcount\n"
           "\n"
           "        defvar LF@%%%%%%write\n"
           "        pops   LF@%%%%%%write\n"
           "        sub  LF@%%%%%%argcount  LF@%%%%%%argcount  int@1\n"
           "        pushs LF@%%%%%%argcount\n"
           "        call %%%%%%printf\n"
           "        write  LF@%%%%%%write\n"
           "\n"
           "\n"
           "label ret%%%%%%printf\n"
           "popframe\n"
           "return\n"
           "label over%%%%%%printf\n"
           "##end------------------------------------------------over%%%%%%printf\n");
}

void footer(String* bodyVars) {
    printf("jump %%%%%%owerdefVarsBody\n");
    printf("label %%%%%%defVarsBody\n");
    printf("%s", bodyVars->chars); // if any vars were in main body print them
    DisposeString(bodyVars);
    printf("jump %%%%%%bodyStart\n");
    printf("label %%%%%%owerdefVarsBody\n");
    printf("POPFRAME\n");
}

void GdefVar(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    if (inFunction) {
        int sizeNeeded = snprintf(NULL, 0, "DEFVAR %s@%s\n", frame, tree->name->chars);
        char tmp[sizeNeeded];
        sprintf(tmp, "DEFVAR %s@%s\n", frame, tree->name->chars);
        if (strstr(funVars->chars, tmp) == NULL) {
            if (AppendCharacters(funVars, tmp) != OK)
                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }

    } else {
        int sizeNeeded = snprintf(NULL, 0, "DEFVAR %s@%s\n", frame, tree->name->chars);
        char tmp[sizeNeeded];
        sprintf(tmp, "DEFVAR %s@%s\n", frame, tree->name->chars);
        if (strstr(bodyVars->chars, tmp) == NULL) {
            if (AppendCharacters(bodyVars, tmp) != OK)
                exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
        }
    }
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("POPS %s@%s\n", frame, tree->name->chars);
}

void GasVar(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("POPS %s@%s\n", frame, tree->name->chars);
}

void Gfloatval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call  %%%%%%convertToFloat\n");
}

void Gintval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call  %%%%%%convertToInt\n");
}

void Gstringval(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call  %%%%%%convertToString\n");
}

void Gconcat(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("call  %%%%%%convertToString\n");
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call  %%%%%%convertToString\n");
    printf("pops GF@%%%%%%tmp1\n");
    printf("pops GF@%%%%%%tmp2\n");
    printf("concat GF@%%%%%%tmp1 GF@%%%%%%tmp1 GF@%%%%%%tmp2\n");
    printf("pushs GF@%%%%%%tmp1\n");
}

void Gstrlen(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("pushs string@string\n");
    printf("pushs int@4\n");
    printf("call %%%%%%typeCheck\n");
    printf("pops GF@%%%%%%strlenvar\n");
    printf("STRLEN GF@%%%%%%strlen GF@%%%%%%strlenvar\n");
    printf("pushs GF@%%%%%%strlen\n");
}

void Gord(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("pushs string@string\n");
    printf("pushs int@4\n");
    printf("call %%%%%%typeCheckIgnoreNil\n");
    printf("pops GF@%%%%%%strlenvar\n");
    printf("type GF@%%%%%%tmp4 GF@%%%%%%strlenvar\n");
    printf("jumpifeq owerord%%%%%%%d GF@%%%%%%tmp4 string@nil\n", tree->id);
    printf("jumpifeq owerord%%%%%%%d GF@%%%%%%strlenvar string@\n", tree->id);
    printf("STRI2INT GF@%%%%%%strlen GF@%%%%%%strlenvar int@0\n");
    printf("pushs GF@%%%%%%strlen\n");
    printf("jump owerowerord%%%%%%%d\n", tree->id);
    printf("label owerord%%%%%%%d\n", tree->id);
    printf("pushs int@0\n");
    printf("label owerowerord%%%%%%%d\n", tree->id);
}

void Gchr(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("pushs string@int\n");
    printf("pushs int@4\n");
    printf("call %%%%%%typeCheck\n");
    printf("pops GF@%%%%%%strlenvar\n");
    printf("INT2CHAR GF@%%%%%%strlen GF@%%%%%%strlenvar\n");
    printf("pushs GF@%%%%%%strlen\n");
}

void Gsubstr(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%substr\n");
}

void Gread(AST* tree) {
    switch (tree->varT) {
        case int_type:
            printf("READ GF@%%read int\n");
            printf("PUSHS GF@%%read\n");
            break;
        case float_type:
            printf("READ GF@%%read float\n");
            printf("PUSHS GF@%%read\n");
            break;
        case string_type:
            printf("READ GF@%%read string\n");
            printf("PUSHS GF@%%read\n");
            break;
        default:
            break;
    }
}

void Gcompare(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {

    switch (tree->cmpT) {
        case lt:
            printf("pushs string@LT\n");
            break;
        case gt:
            printf("pushs string@GT\n");
            break;
        case eq:
            printf("pushs string@EQ\n");
            break;
        case geq:
            printf("pushs string@GEQ\n");
            break;
        case leq:
            printf("pushs string@LEQ\n");
            break;
        case neq:
            printf("pushs string@NEQ\n");
            break;

        default:
            break;
    }
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%compare\n");
}

void Gif(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    printf("#condition-----------------\n");
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertToBool\n");
    printf("PUSHs bool@true\n");
    printf("JUMPIFNEQS ower1%%%d # if\n", tree->id);
    generate(tree->middle, frame, bodyVars, funVars, inFunction);
    printf("JUMP ower2%%%d# jump ower else\n", tree->id);
    printf("LABEL ower1%%%d # else\n", tree->id);
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("LABEL ower2%%%d\n", tree->id);
}

void Gwhile(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    printf("#-----------while---------------\n");
    printf("LABEL start%%%%%d #while\n", tree->id);
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertToBool\n");
    printf("PUSHs bool@true\n");
    printf("JUMPIFNEQS ower%%%d #-----------body\n", tree->id);
    generate(tree->middle, frame, bodyVars, funVars, inFunction);
    printf("JUMP start%%%%%d #-----------bodyower\n", tree->id);
    printf("LABEL ower%%%d\n", tree->id);
    printf("#-----------while---------------end\n");
}

void Gwrite(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {

    int argCounter = 0;
    AST* ast_tmp = tree->left;
    while (ast_tmp != NULL) {
        if (ast_tmp->left != NULL && ast_tmp->left->nodeT != n_undefined) {
            argCounter++;
            generate(ast_tmp->left, frame, bodyVars, funVars, inFunction);
        }
        ast_tmp = ast_tmp->right;
    }
    printf("pushs int@%d\n", argCounter);
    printf("call %%%%%%printf\n");
}

void Gconstant(AST* tree) {

    switch (tree->varT) {
        case float_type:
            printf("PUSHS float@%a\n", tree->valueFloat);
            break;
        case string_type:

            printf("PUSHS string@%s\n", _stringConvert(tree->valueString));
            break;
        case int_type:
            printf("PUSHS int@%d\n", tree->valueInt);
            break;
        case void_type:
            printf("PUSHS nil@nil\n");
            break;
        default:
            break;
    }
}
void Gvarriable(AST* tree, char* frame) {
    printf("type GF@%%%%%%tmp1 %s@%s\n", frame, tree->name->chars);
    printf("jumpifeq %%%%%%err5 GF@%%%%%%tmp1 string@\n");

    printf("PUSHS %s@%s\n", frame, tree->name->chars);
}

void GoAdd(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertOperation\n");
    printf("ADDS\n");
}

void GoMull(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertOperation\n");
    printf("MULS\n");
}

void GoSubb(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertOperation\n");
    printf("SUBS\n");
}

void GoDiv(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertToFloat\n");
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("call %%%%%%convertToFloat\n");
    printf("DIVS\n");
}

void Gfun(AST* tree, char* frame, String* bodyVars, String* funVars) {

    printf("##start------------------------------------------------%%%d%s\n", tree->id,
           tree->name->chars);
    printf("JUMP funOwer%%%d\n", tree->id);
    printf("LABEL fun%%%s\n", tree->name->chars);
    printf("jump funDefVars%%%s\n", tree->name->chars); // define all varriables before using them
    printf("LABEL funDefVarsBack%%%s\n", tree->name->chars);
    printf("createframe\n"
           "pushframe\n");
    generate(tree->left, frame, bodyVars, funVars, true);
    generate(tree->right, frame, bodyVars, funVars, true);

    if (tree->varT == void_type) {
        printf("pushs nil@nil\n");
        printf("popframe\n");
        printf("return\n"); // for void functions
    } else {
        printf("jump %%%%%%err4\n");
    }
    printf("label funDefVars%%%s\n", tree->name->chars); // define all varriables before using them

    printf("%s", funVars->chars); // print vars and clear string
    printf("jump funDefVarsBack%%%s\n", tree->name->chars);
    DisposeString(funVars);
    int err;
    funVars = InitString(&err);
    printf("jump fun%%%s\n", tree->name->chars);

    printf("LABEL funOwer%%%d\n", tree->id);
    printf("##end------------------------------------------------%%%d%s\n", tree->id,
           tree->name->chars);
}

void Greturn(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);

    if (tree->varT == void_type) {
        printf("pushs nil@nil\n");
        printf("popframe\n");
        printf("return\n");
    }

    switch (tree->varT) {
        case float_type:
            printf("pushs string@float\n");
            break;
        case string_type:
            printf("pushs string@string\n");
            break;
        case int_type:
            printf("pushs string@int\n");
            break;
        default:
            break;
    }

    printf("pushs int@4\n");
    printf("call %%%%%%typeCheck\n"); // checking for right err code
    printf("popframe\n");
    printf("return\n");
}

void Gcall(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->left, frame, bodyVars, funVars, inFunction);
    printf("CALL fun%%%s\n", tree->name->chars);
}

void GargLcall(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    AST* ast_tmp = tree;
    while (ast_tmp != NULL) {
        generate(ast_tmp->left, frame, bodyVars, funVars, inFunction);
        ast_tmp = ast_tmp->right;
    }
}

void GargLfun(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    generate(tree->right, frame, bodyVars, funVars, inFunction);
    printf("DEFVAR %s@%s\n", frame, tree->name->chars);
    switch (tree->varT) {
        case float_type:
            printf("pushs string@float\n");
            break;
        case string_type:
            printf("pushs string@string\n");
            break;
        case int_type:
            printf("pushs string@int\n");
            break;
        default:
            break;
    }

    printf("pushs int@4\n"); // pushses error
    if (tree->isNullable) {
        printf("call %%%%%%typeCheckIgnoreNil\n");
    } else {
        printf("call %%%%%%typeCheck\n");
    }

    printf("POPS %s@%s\n", frame, tree->name->chars);
}

void startGenerate(AST* tree) {
    String* bodyVars;
    String* funVars;

    int err;
    funVars = InitString(&err);
    if (err) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }
    bodyVars = InitString(&err);
    if (err) {
        exit(WriteErrorMessage(INTERNAL_COMPILER_ERROR));
    }
    header();
    generate(tree, "LF", bodyVars, funVars, false);
    footer(bodyVars);
    DisposeString(funVars);
}

void generate(AST* tree, char* frame, String* bodyVars, String* funVars, bool inFunction) {
    if (tree == NULL) {
        return;
    }

    switch (tree->nodeT) {
        case n_if:
            Gif(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_while:
            Gwhile(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_defvar:
            GdefVar(tree, frame, bodyVars, funVars, inFunction);
            break;
        case n_write:
            Gwrite(tree, frame, bodyVars, funVars, inFunction);
            break;
        case n_constant:
            Gconstant(tree);
            break;
        case n_var:
            Gvarriable(tree, "LF");
            break;
        case n_add:
            GoAdd(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_mull:
            GoMull(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_div:
            GoDiv(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_sub:
            GoSubb(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_stList: {
            AST* ast_tmp = tree;
           do {
                generate(ast_tmp->left, "LF", bodyVars, funVars, inFunction);
                ast_tmp = ast_tmp->right;
            } while (ast_tmp != NULL);
        } break;
        case n_comp:
            Gcompare(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_funCall:
            Gcall(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_funDef:
            Gfun(tree, "LF", bodyVars, funVars);
            break;
        case n_return:
            Greturn(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_asignVar:
            GasVar(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_argLcall:
            GargLcall(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_argLfun:
            GargLfun(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_read:
            Gread(tree);
            break;
        case n_ord:
            Gord(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_chr:
            Gchr(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_stringval:
            Gstringval(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_intval:
            Gintval(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_floatval:
            Gfloatval(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_strlen:
            Gstrlen(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_substring:
            Gsubstr(tree, "LF", bodyVars, funVars, inFunction);
            break;
        case n_concat:
            Gconcat(tree, "LF", bodyVars, funVars, inFunction);
            break;

        default:
            break;
    }
}
