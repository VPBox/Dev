/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

D             [0-9]
L             [a-zA-Z_]
AN            [a-zA-Z_0-9]
H             [a-fA-F_0-9]
E             [Ee][+-]?{D}+
FS            (f|F|l|L)
IS            (u|U|l|L)*
S             [ \t]
DOT           [.]
PATH          ({DOT}|{AN}|\/|-)+
ID            {L}{AN}*

%{

#include "AST.h"
#include "Declaration.h"
#include "Type.h"
#include "VarDeclaration.h"
#include "FunctionDeclaration.h"
#include "CompositeDeclaration.h"
#include "Define.h"
#include "Include.h"
#include "EnumVarDeclaration.h"
#include "Note.h"
#include "TypeDef.h"
#include "Expression.h"

#include <assert.h>
#include <utils/Errors.h>

#include "c2hal_y.h"

using namespace android;

int check_type(yyscan_t yyscanner, struct yyguts_t *yyg);

extern int start_token;

extern std::string last_comment;

// :(
extern int numB;
extern std::string functionText;

extern std::string defineText;
extern std::string otherText;

extern bool isOpenGl;

#define YY_USER_ACTION yylloc->first_line = yylineno;

#define ID_UNLESS_OPEN_GL(OPEN_GL_CODE)                                  \
        do {                                                             \
            if (isOpenGl) {                                              \
                OPEN_GL_CODE                                             \
            } else {                                                     \
                yylval->str = strdup(yytext);                            \
                return ID;                                               \
            }                                                            \
        } while(0)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang diagnostic ignored "-Wregister"

%}

%option yylineno
%option nounput
%option noinput
%option reentrant
%option bison-bridge
%option bison-locations
%option extra-type="android::AST *"

%x COMMENT_STATE
%x INCLUDE_STATE
%x COPY_DECL_STATE
%x FUNCTION_STATE
%x DEFINE_STATE
%x DEFINE_SLURP_STATE

%%
%{
    if (start_token) {
        int token = start_token;
        start_token = 0;
        return token;
    }
%}

"\n"                              { /* needed for yylineno to update */ }

\/\*([^*]|\*+[^*\/])*\*+\/        { last_comment = strdup(yytext); }

"//"[^\r\n]*                      { /* skip C++ style comment */ }

"__BEGIN_DECLS"                   { /* macro'd 'extern "C" {' if CPP or nothing */ }
"__END_DECLS"                     { /* '}' */ }

"__attribute__((__packed__))"     { /* ignore */ }
"__attribute__((packed))"         { /* ignore */ }
"__attribute__((__deprecated__))" { /* ignore */ }

"EGLAPIENTRYP"                    { ID_UNLESS_OPEN_GL(return '*';); }
"EGLAPIENTRY"                     { ID_UNLESS_OPEN_GL(/* actually is nothing on android */); }
"GL_APIENTRYP"                    { ID_UNLESS_OPEN_GL(return '*';); }
"GL_APIENTRY"                     { ID_UNLESS_OPEN_GL(/* actually is nothing on android */); }
"GL_APICALL"                      { ID_UNLESS_OPEN_GL(/* __attribute__((visibility("default"))) */); }

"#include"                        { BEGIN(INCLUDE_STATE); return INCLUDE; }
<INCLUDE_STATE>"<"                { return '<'; }
<INCLUDE_STATE>">"                { return '>'; }
<INCLUDE_STATE>"\""               { return '"'; }
<INCLUDE_STATE>"\n"               { BEGIN(INITIAL); }
<INCLUDE_STATE>{PATH}             { yylval->str = strdup(yytext); return INCLUDE_FILE; }
<INCLUDE_STATE>.                  { /* ignore other characters */ }

"static"|"inline"                 {
                                    BEGIN(FUNCTION_STATE);
                                    functionText = strdup(yytext);
                                    numB = 0;
                                  }
<FUNCTION_STATE>[^{}]+            { functionText += yytext; }
<FUNCTION_STATE>"{"               { functionText += yytext; numB += 1;}
<FUNCTION_STATE>"}"               {
                                    functionText += yytext;
                                    numB -= 1;

                                    // Will fail if unbalanced brackets in
                                    // strings or comments in the function.
                                    if (numB <= 0) {
                                        BEGIN(INITIAL);
                                        yylval->str = strdup(functionText.c_str());
                                        return FUNCTION;
                                    }
                                   }

"#"{S}*"define"                   { BEGIN(DEFINE_STATE); return DEFINE; }
<DEFINE_STATE>{ID}                {
                                    BEGIN(DEFINE_SLURP_STATE);
                                    defineText = "";
                                    yylval->str = strdup(yytext);
                                    return ID;
                                  }
<DEFINE_STATE>.                   { /* ignore other characters */ }

<DEFINE_SLURP_STATE>\/\*([^*]|\*+[^*\/])*\*+\/ {
                                    defineText += yytext;
                                  }
<DEFINE_SLURP_STATE>[^\\\n]       { defineText += yytext; }
<DEFINE_SLURP_STATE>"\\\n"        { defineText += yytext; }
<DEFINE_SLURP_STATE>"\n"          {
                                    BEGIN(INITIAL);
                                    yylval->str = strdup(defineText.c_str());
                                    return DEFINE_SLURP;
                                  }

"using"                           { BEGIN(COPY_DECL_STATE); otherText = strdup(yytext); }
"#"{S}*{L}+                       { BEGIN(COPY_DECL_STATE); otherText = strdup(yytext); }
<COPY_DECL_STATE>\/\*([^*]|\*+[^*\/])*\*+\/ {
                                    otherText += yytext;
                                  }
<COPY_DECL_STATE>[^\\\n]          { otherText += yytext; }
<COPY_DECL_STATE>"\\\n"           { otherText += yytext; }
<COPY_DECL_STATE>"\n"             {
                                    BEGIN(INITIAL);
                                    yylval->str = strdup(otherText.c_str());
                                    // decls/macros we want to preserve
                                    // in the output, but there is nothing
                                    // special to do about them yet
                                    return OTHER_STATEMENT;
                                  }

"struct"                          { return STRUCT; }
"union"                           { return UNION; }
"enum"                            { return ENUM; }
"class"                           { return CLASS; }
"const"                           { return CONST; }
"typedef"                         { return TYPEDEF; }
"void"                            { return VOID; }
"unsigned"                        { return UNSIGNED; }
"signed"                          { return SIGNED; }
"namespace"                       { return NAMESPACE; }
"extern"                          { return EXTERN; }
"\"C\""                           { return C_STRING; }

{ID}                              { yylval->str = strdup(yytext); return ID; }
0[xX]{H}+{IS}?                    { yylval->str = strdup(yytext); return INTEGRAL_VALUE; }
0{D}+{IS}?                        { yylval->str = strdup(yytext); return INTEGRAL_VALUE; }
{D}+{IS}?                         { yylval->str = strdup(yytext); return INTEGRAL_VALUE; }

{D}+{E}{FS}?                      { yylval->str = strdup(yytext); return VALUE; }
{D}+\.{E}?{FS}?                   { yylval->str = strdup(yytext); return VALUE; }
{D}*\.{D}+{E}?{FS}?               { yylval->str = strdup(yytext); return VALUE; }
L?\"(\\.|[^\\"])*\"               { yylval->str = strdup(yytext); return VALUE; }

"("                               { return '('; }
")"                               { return ')'; }
"<"                               { return '<'; }
">"                               { return '>'; }
"{"                               { return '{'; }
"}"                               { return '}'; }
"["                               { return '['; }
"]"                               { return ']'; }
"?"                               { return '?'; }
":"                               { return ':'; }
"*"                               { return '*'; }
";"                               { return ';'; }
","                               { return ','; }
"="                               { return '='; }
"+"                               { return '+'; }
"-"                               { return '-'; }
"/"                               { return '/'; }
"%"                               { return '%'; }
"&"                               { return '&'; }
"|"                               { return '|'; }
"^"                               { return '^'; }
"~"                               { return '~'; }
"<<"                              { return LSHIFT; }
">>"                              { return RSHIFT; }

"..."                             { return VARARGS; }

.                                 { /* ignore other characters */ }

%%

#pragma clang diagnostic pop

// allows us to specify what start symbol will be used in the grammar
int start_token;
bool should_report_errors;

std::string last_comment;

// this is so frowned upon on so many levels, but here vars are so that we can
// slurp up function text as a string and don't have to implement
// the *entire* grammar of C (and C++ in some files) just to parse headers
int numB;
std::string functionText;

std::string defineText;
std::string otherText;

bool isOpenGl;

int yywrap(yyscan_t) {
    return 1;
}

status_t parseFile(AST *ast) {
    FILE *file = fopen(ast->getFilename().c_str(), "rb");

    if (file == NULL) {
        return -errno;
    }

    start_token = START_HEADER;
    isOpenGl = ast->isOpenGl();
    should_report_errors = true;

    yyscan_t scanner;
    yylex_init_extra(ast, &scanner);
    ast->setScanner(scanner);

    yyset_in(file, scanner);
    int res = yyparse(ast);

    yylex_destroy(scanner);
    ast->setScanner(NULL);

    fclose(file);
    file = NULL;

    return res;
}

status_t parseExpression(AST *ast, std::string str) {
    start_token = START_EXPR;
    isOpenGl = ast->isOpenGl();
    should_report_errors = false;

    yyscan_t scanner;
    yylex_init_extra(ast, &scanner);
    ast->setScanner(scanner);

    YY_BUFFER_STATE buf = yy_scan_string(str.c_str(), scanner);

    int res = yyparse(ast);

    yy_delete_buffer(buf, scanner);

    yylex_destroy(scanner);
    ast->setScanner(NULL);

    return res;
}
