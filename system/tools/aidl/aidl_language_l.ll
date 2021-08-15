%{
#include <string.h>
#include <stdlib.h>

#include "aidl_language.h"
#include "aidl_language_y.h"

#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option yylineno
%option noyywrap
%option nounput
%option noinput
%option reentrant
%option bison-bridge
%option bison-locations

%x COPYING LONG_COMMENT

identifier  [_a-zA-Z][_a-zA-Z0-9]*
whitespace  ([ \t\r]+)
intvalue    [-+]?(0|[1-9][0-9]*)
hexvalue    0[x|X][0-9a-fA-F]+
floatvalue  [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?f?

%%
%{
  /* This happens at every call to yylex (every time we receive one token) */
  std::string extra_text;
  yylloc->step();
%}


\%\%\{                { extra_text += "/**"; BEGIN(COPYING); }
<COPYING>\}\%\%       { extra_text += "**/"; yylloc->step(); BEGIN(INITIAL); }
<COPYING>.*           { extra_text += yytext; }
<COPYING>\n+          { extra_text += yytext; yylloc->lines(yyleng); }

\/\*                  { extra_text += yytext; BEGIN(LONG_COMMENT); }
<LONG_COMMENT>\*+\/   { extra_text += yytext; yylloc->step(); BEGIN(INITIAL);  }
<LONG_COMMENT>\*+     { extra_text += yytext; }
<LONG_COMMENT>\n+     { extra_text += yytext; yylloc->lines(yyleng); }
<LONG_COMMENT>[^*\n]+ { extra_text += yytext; }

\"[^\"]*\"            { yylval->token = new AidlToken(yytext, extra_text);
                        return yy::parser::token::C_STR; }

\/\/.*\n              { extra_text += yytext; yylloc->lines(1); yylloc->step(); }

\n+                   { yylloc->lines(yyleng); yylloc->step(); }
{whitespace}          {}
<<EOF>>               { yyterminate(); }

    /* symbols */
;                     { return ';'; }
\{                    { return '{'; }
\}                    { return '}'; }
=                     { return '='; }
,                     { return ','; }
\.                    { return '.'; }
\(                    { return '('; }
\)                    { return ')'; }
\[                    { return '['; }
\]                    { return ']'; }
\<                    { return '<'; }
\>                    { return '>'; }

    /* annotations */
@{identifier}         { yylval->token = new AidlToken(yytext + 1, extra_text);
                        return yy::parser::token::ANNOTATION;
                      }

    /* keywords */
parcelable            { yylval->token = new AidlToken("parcelable", extra_text);
                        return yy::parser::token::PARCELABLE;
                      }
import                { return yy::parser::token::IMPORT; }
package               { return yy::parser::token::PACKAGE; }
in                    { return yy::parser::token::IN; }
out                   { return yy::parser::token::OUT; }
inout                 { return yy::parser::token::INOUT; }
cpp_header            { return yy::parser::token::CPP_HEADER; }
const                 { return yy::parser::token::CONST; }
true                  { return yy::parser::token::TRUE_LITERAL; }
false                 { return yy::parser::token::FALSE_LITERAL; }

interface             { yylval->token = new AidlToken("interface", extra_text);
                        return yy::parser::token::INTERFACE;
                      }
oneway                { yylval->token = new AidlToken("oneway", extra_text);
                        return yy::parser::token::ONEWAY;
                      }

    /* scalars */
{identifier}          { yylval->token = new AidlToken(yytext, extra_text);
                        return yy::parser::token::IDENTIFIER;
                      }
'.'                   { yylval->character = yytext[1];
                        return yy::parser::token::CHARVALUE;
                      }
{intvalue}            { yylval->token = new AidlToken(yytext, extra_text);
                        return yy::parser::token::INTVALUE; }
{floatvalue}          { yylval->token = new AidlToken(yytext, extra_text);
                        return yy::parser::token::FLOATVALUE; }
{hexvalue}            { yylval->token = new AidlToken(yytext, extra_text);
                        return yy::parser::token::HEXVALUE; }

  /* lexical error! */
.                     { return yy::parser::token::UNKNOWN; }

%%

// comment and whitespace handling
// ================================================
