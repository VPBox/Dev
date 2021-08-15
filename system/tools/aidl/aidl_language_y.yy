%{
#include "aidl_language.h"
#include "aidl_language_y.h"
#include "logging.h"
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(yy::parser::semantic_type *, yy::parser::location_type *, void *);

AidlLocation loc(const yy::parser::location_type& l) {
  CHECK(l.begin.filename == l.end.filename);
  AidlLocation::Point begin {
    .line = l.begin.line,
    .column = l.begin.column,
  };
  AidlLocation::Point end {
    .line = l.end.line,
    .column = l.end.column,
  };
  return AidlLocation(*l.begin.filename, begin, end);
}

#define lex_scanner ps->Scanner()

%}

%initial-action {
    @$.begin.filename = @$.end.filename =
        const_cast<std::string *>(&ps->FileName());
}

%parse-param { Parser* ps }
%lex-param { void *lex_scanner }

%pure-parser
%glr-parser
%skeleton "glr.cc"

%expect-rr 0

%error-verbose

%union {
    AidlToken* token;
    char character;
    std::string *str;
    AidlAnnotation* annotation;
    std::vector<AidlAnnotation>* annotation_list;
    AidlTypeSpecifier* type;
    AidlArgument* arg;
    AidlArgument::Direction direction;
    AidlConstantValue* constant_value;
    std::vector<std::unique_ptr<AidlConstantValue>>* constant_value_list;
    std::vector<std::unique_ptr<AidlArgument>>* arg_list;
    AidlVariableDeclaration* variable;
    std::vector<std::unique_ptr<AidlVariableDeclaration>>* variable_list;
    AidlMethod* method;
    AidlMember* constant;
    std::vector<std::unique_ptr<AidlMember>>* interface_members;
    AidlQualifiedName* qname;
    AidlInterface* interface;
    AidlParcelable* parcelable;
    AidlDefinedType* declaration;
    std::vector<std::unique_ptr<AidlTypeSpecifier>>* type_args;
}

%token<token> ANNOTATION "annotation"
%token<token> C_STR "string literal"
%token<token> IDENTIFIER "identifier"
%token<token> INTERFACE "interface"
%token<token> PARCELABLE "parcelable"
%token<token> ONEWAY "oneway"

%token<character> CHARVALUE "char literal"
%token<token> FLOATVALUE "float literal"
%token<token> HEXVALUE "hex literal"
%token<token> INTVALUE "int literal"

%token '(' ')' ',' '=' '[' ']' '<' '>' '.' '{' '}' ';'
%token CONST "const"
%token UNKNOWN "unrecognized character"
%token CPP_HEADER "cpp_header"
%token IMPORT "import"
%token IN "in"
%token INOUT "inout"
%token OUT "out"
%token PACKAGE "package"
%token TRUE_LITERAL "true"
%token FALSE_LITERAL "false"

%type<declaration> decl
%type<variable_list> variable_decls
%type<variable> variable_decl
%type<interface_members> interface_members
%type<declaration> unannotated_decl
%type<interface> interface_decl
%type<parcelable> parcelable_decl
%type<method> method_decl
%type<constant> constant_decl
%type<annotation> annotation
%type<annotation_list>annotation_list
%type<type> type
%type<type> unannotated_type
%type<arg_list> arg_list
%type<arg> arg
%type<direction> direction
%type<type_args> type_args
%type<qname> qualified_name
%type<constant_value> constant_value
%type<constant_value_list> constant_value_list
%type<constant_value_list> constant_value_non_empty_list

%type<token> identifier error
%%
document
 : package imports decls {};

/* A couple of tokens that are keywords elsewhere are identifiers when
 * occurring in the identifier position. Therefore identifier is a
 * non-terminal, which is either an IDENTIFIER token, or one of the
 * aforementioned keyword tokens.
 */
identifier
 : IDENTIFIER
  { $$ = $1; }
 | CPP_HEADER
  { $$ = new AidlToken("cpp_header", ""); }
 ;

package
 : {}
 | PACKAGE qualified_name ';'
  { ps->SetPackage(unique_ptr<AidlQualifiedName>($2)); };

imports
 : {}
 | import imports {};

import
 : IMPORT qualified_name ';'
  { ps->AddImport(new AidlImport(loc(@2), $2->GetDotName()));
    delete $2;
  };

qualified_name
 : identifier {
    $$ = new AidlQualifiedName(loc(@1), $1->GetText(), $1->GetComments());
    delete $1;
  }
 | qualified_name '.' identifier
  { $$ = $1;
    $$->AddTerm($3->GetText());
    delete $3;
  };

decls
 : decl {
    ps->AddDefinedType(unique_ptr<AidlDefinedType>($1));
  }
 | decls decl {
    ps->AddDefinedType(unique_ptr<AidlDefinedType>($2));
  };

decl
 : annotation_list unannotated_decl
   {
    $$ = $2;

    if ($1->size() > 0) {
      // copy comments from annotation to decl
      $2->SetComments($1->begin()->GetComments());
    }

    $$->Annotate(std::move(*$1));
    delete $1;
   }
 ;

unannotated_decl
 : parcelable_decl
  { $$ = $1; }
 | interface_decl
  { $$ = $1; }
 ;

parcelable_decl
 : PARCELABLE qualified_name ';' {
    $$ = new AidlParcelable(loc(@2), $2, ps->Package(), $1->GetComments());
  }
 | PARCELABLE qualified_name CPP_HEADER C_STR ';' {
    $$ = new AidlParcelable(loc(@2), $2, ps->Package(), $1->GetComments(), $4->GetText());
  }
 | PARCELABLE identifier '{' variable_decls '}' {
    AidlQualifiedName* name = new AidlQualifiedName(loc(@2), $2->GetText(), $2->GetComments());
    $$ = new AidlStructuredParcelable(loc(@2), name, ps->Package(), $1->GetComments(), $4);
 }
 | PARCELABLE error ';' {
    ps->AddError();
    $$ = NULL;
  };

variable_decls
 : /* empty */ {
    $$ = new std::vector<std::unique_ptr<AidlVariableDeclaration>>;
 }
 | variable_decls variable_decl {
    $$ = $1;
    if ($2 != nullptr) {
      $$->push_back(std::unique_ptr<AidlVariableDeclaration>($2));
    }
 };

variable_decl
 : type identifier ';' {
   $$ = new AidlVariableDeclaration(loc(@2), $1, $2->GetText());
 }
 | type identifier '=' constant_value ';' {
   $$ = new AidlVariableDeclaration(loc(@2), $1, $2->GetText(),  $4);
 }
 | error ';' {
   ps->AddError();
   $$ = nullptr;
 }

interface_decl
 : INTERFACE identifier '{' interface_members '}' {
    $$ = new AidlInterface(loc(@1), $2->GetText(), $1->GetComments(), false, $4, ps->Package());
    delete $1;
    delete $2;
  }
 | ONEWAY INTERFACE identifier '{' interface_members '}' {
    $$ = new AidlInterface(loc(@2), $3->GetText(),  $1->GetComments(), true, $5, ps->Package());
    delete $1;
    delete $2;
    delete $3;
  }
 | INTERFACE error '{' interface_members '}' {
    ps->AddError();
    $$ = nullptr;
    delete $1;
    delete $2;
    delete $4;
  };

interface_members
 :
  { $$ = new std::vector<std::unique_ptr<AidlMember>>(); }
 | interface_members method_decl
  { $1->push_back(std::unique_ptr<AidlMember>($2)); }
 | interface_members constant_decl
  { $1->push_back(std::unique_ptr<AidlMember>($2)); }
 | interface_members error ';' {
    ps->AddError();
    $$ = $1;
  };

constant_value
 : TRUE_LITERAL { $$ = AidlConstantValue::Boolean(loc(@1), true); }
 | FALSE_LITERAL { $$ = AidlConstantValue::Boolean(loc(@1), false); }
 | CHARVALUE { $$ = AidlConstantValue::Character(loc(@1), $1); }
 | INTVALUE {
    $$ = AidlConstantValue::Integral(loc(@1), $1->GetText());
    delete $1;
  }
 | FLOATVALUE {
    $$ = AidlConstantValue::Floating(loc(@1), $1->GetText());
    delete $1;
  }
 | HEXVALUE {
    $$ = AidlConstantValue::Hex(loc(@1), $1->GetText());
    delete $1;
  }
 | C_STR {
    $$ = AidlConstantValue::String(loc(@1), $1->GetText());
    delete $1;
  }
 | '{' constant_value_list '}' {
    $$ = AidlConstantValue::Array(loc(@1), $2);
    delete $2;
  }
 ;

constant_value_list
 : /* empty */ {
    $$ = new std::vector<std::unique_ptr<AidlConstantValue>>;
 }
 | constant_value_non_empty_list {
    $$ = $1;
 }
 ;

constant_value_non_empty_list
 : constant_value {
    $$ = new std::vector<std::unique_ptr<AidlConstantValue>>;
    $$->push_back(std::unique_ptr<AidlConstantValue>($1));
 }
 | constant_value_non_empty_list ',' constant_value {
    $$ = $1;
    $$->push_back(std::unique_ptr<AidlConstantValue>($3));
 }
 ;

constant_decl
 : CONST type identifier '=' constant_value ';' {
    $$ = new AidlConstantDeclaration(loc(@3), $2, $3->GetText(), $5);
    delete $3;
   }
 ;

method_decl
 : type identifier '(' arg_list ')' ';' {
    $$ = new AidlMethod(loc(@2), false, $1, $2->GetText(), $4, $1->GetComments());
    delete $2;
  }
 | ONEWAY type identifier '(' arg_list ')' ';' {
    $$ = new AidlMethod(loc(@3), true, $2, $3->GetText(), $5, $1->GetComments());
    delete $1;
    delete $3;
  }
 | type identifier '(' arg_list ')' '=' INTVALUE ';' {
    $$ = new AidlMethod(loc(@2), false, $1, $2->GetText(), $4, $1->GetComments(), std::stoi($7->GetText()));
    delete $2;
  }
 | ONEWAY type identifier '(' arg_list ')' '=' INTVALUE ';' {
    $$ = new AidlMethod(loc(@3), true, $2, $3->GetText(), $5, $1->GetComments(), std::stoi($8->GetText()));
    delete $1;
    delete $3;
  };

arg_list
 :
  { $$ = new std::vector<std::unique_ptr<AidlArgument>>(); }
 | arg {
    $$ = new std::vector<std::unique_ptr<AidlArgument>>();
    $$->push_back(std::unique_ptr<AidlArgument>($1));
  }
 | arg_list ',' arg {
    $$ = $1;
    $$->push_back(std::unique_ptr<AidlArgument>($3));
  };

arg
 : direction type identifier {
    $$ = new AidlArgument(loc(@3), $1, $2, $3->GetText());
    delete $3;
  }
 | type identifier {
    $$ = new AidlArgument(loc(@2), $1, $2->GetText());
    delete $2;
  }
 | error {
    ps->AddError();
  };

unannotated_type
 : qualified_name {
    $$ = new AidlTypeSpecifier(loc(@1), $1->GetDotName(), false, nullptr, $1->GetComments());
    ps->DeferResolution($$);
    delete $1;
  }
 | qualified_name '[' ']' {
    $$ = new AidlTypeSpecifier(loc(@1), $1->GetDotName(), true, nullptr, $1->GetComments());
    ps->DeferResolution($$);
    delete $1;
  }
 | qualified_name '<' type_args '>' {
    $$ = new AidlTypeSpecifier(loc(@1), $1->GetDotName(), false, $3, $1->GetComments());
    ps->DeferResolution($$);
    delete $1;
  };

type
 : annotation_list unannotated_type {
    $$ = $2;
    if ($1->size() > 0) {
      // copy comments from annotation to type
      $2->SetComments($1->begin()->GetComments());
    }
    $2->Annotate(std::move(*$1));
    delete $1;
  };

type_args
 : unannotated_type {
    $$ = new std::vector<std::unique_ptr<AidlTypeSpecifier>>();
    $$->emplace_back($1);
  }
 | type_args ',' unannotated_type {
    $1->emplace_back($3);
  };

annotation_list
 :
  { $$ = new std::vector<AidlAnnotation>(); }
 | annotation_list annotation
  {
    if ($2 != nullptr) {
      $1->emplace_back(std::move(*$2));
      delete $2;
    }
  };

annotation
 : ANNOTATION
  {
    $$ = AidlAnnotation::Parse(loc(@1), $1->GetText());
    if ($$ == nullptr) {
      ps->AddError();
    }
    $$->SetComments($1->GetComments());
  };

direction
 : IN
  { $$ = AidlArgument::IN_DIR; }
 | OUT
  { $$ = AidlArgument::OUT_DIR; }
 | INOUT
  { $$ = AidlArgument::INOUT_DIR; };

%%

#include <ctype.h>
#include <stdio.h>

void yy::parser::error(const yy::parser::location_type& l, const std::string& errstr) {
  AIDL_ERROR(loc(l)) << errstr;
  // parser will return error value
}
