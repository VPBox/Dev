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

%{

#include "AST.h"
#include "Annotation.h"
#include "ArrayType.h"
#include "CompoundType.h"
#include "ConstantExpression.h"
#include "DocComment.h"
#include "EnumType.h"
#include "Interface.h"
#include "Location.h"
#include "Method.h"
#include "RefType.h"
#include "Scope.h"
#include "TypeDef.h"
#include "VectorType.h"

#include "hidl-gen_y.h"

#include <android-base/logging.h>
#include <hidl-util/FQName.h>
#include <hidl-util/StringHelper.h>
#include <stdio.h>

using namespace android;

extern int yylex(yy::parser::semantic_type*, yy::parser::location_type*, void*, Scope** const);

void enterScope(AST* /* ast */, Scope** scope, Scope* container) {
    CHECK(container->parent() == (*scope));
    *scope = container;
}

void leaveScope(AST* ast, Scope** scope) {
    CHECK((*scope) != ast->getRootScope());
    *scope = (*scope)->parent();
}

::android::Location convertYYLoc(const yy::parser::location_type &loc) {
    return ::android::Location(
            ::android::Position(*(loc.begin.filename), loc.begin.line, loc.begin.column),
            ::android::Position(*(loc.end.filename), loc.end.line, loc.end.column)
    );
}

bool isValidInterfaceField(const std::string& identifier, std::string *errorMsg) {
    static const std::vector<std::string> reserved({
        // Injected names to C++ interfaces by auto-generated code
        "isRemote", "descriptor", "hidlStaticBlock", "onTransact",
        "castFrom", "Proxy", "Stub", "getService",

        // Injected names to Java interfaces by auto-generated code
        "asInterface", "castFrom", "getService", "toString",

        // Inherited methods from IBase is detected in addMethod. Not added here
        // because we need hidl-gen to compile IBase.

        // Inherited names by interfaces from IInterface / IBinder
        "onAsBinder", "asBinder", "queryLocalInterface", "getInterfaceDescriptor", "isBinderAlive",
        "pingBinder", "dump", "transact", "checkSubclass", "attachObject", "findObject",
        "detachObject", "localBinder", "remoteBinder", "mImpl",

        // Inherited names from HidlInstrumentor
        "InstrumentationEvent", "configureInstrumentation", "registerInstrumentationCallbacks",
        "isInstrumentationLib", "mInstrumentationCal1lbacks", "mEnableInstrumentation",
        "mInstrumentationLibPackage", "mInterfaceName",

        // Collide with names in BsFoo
        "mImpl", "addOnewayTask", "mOnewayQueue",

        // Inherited names from Java IHwInterface
        "asBinder",
    });
    if (std::find(reserved.begin(), reserved.end(), identifier) != reserved.end()) {
        *errorMsg = identifier + " cannot be a name inside an interface";
        return false;
    }
    return true;
}

bool isValidStructField(const std::string& identifier, std::string *errorMsg) {
    static const std::vector<std::string> reserved({
        // Injected names to structs and unions by auto-generated code
        "readEmbeddedFromParcel", "writeEmbeddedToParcel", "readVectorFromParcel",
        "writeVectorToParcel", "writeEmbeddedToBlob",
    });
    if (std::find(reserved.begin(), reserved.end(), identifier) != reserved.end()) {
        *errorMsg = identifier + " cannot be a name inside an struct or union";
        return false;
    }
    return true;
}

bool isValidCompoundTypeField(CompoundType::Style style, const std::string& identifier,
                              std::string *errorMsg) {
    // Unions don't support fix-up types; as such, they can't
    // have name collisions with embedded read/write methods.
    if (style == CompoundType::STYLE_UNION) { return true; }

    return isValidStructField(identifier, errorMsg);;
}

bool isValidIdentifier(const std::string& identifier, std::string *errorMsg) {
    static const std::vector<std::string> keywords({
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "int8_t", "int16_t", "int32_t", "int64_t", "bool", "float", "double",
        "interface", "struct", "union", "string", "vec", "enum", "ref", "handle",
        "package", "import", "typedef", "generates", "oneway", "extends",
        "fmq_sync", "fmq_unsync", "safe_union",
    });
    static const std::vector<std::string> cppKeywords({
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit",
        "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch",
        "char", "char16_t", "char32_t", "class", "compl", "concept", "const", "constexpr",
        "const_cast", "continue", "decltype", "default", "delete", "do", "double",
        "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
        "for", "friend", "goto", "if", "inline", "int", "import", "long", "module", "mutable",
        "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq",
        "private", "protected", "public", "register", "reinterpret_cast", "requires", "return",
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
        "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try",
        "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void",
        "volatile", "wchar_t", "while", "xor", "xor_eq",
    });
    static const std::vector<std::string> javaKeywords({
        "abstract", "continue", "for", "new", "switch", "assert", "default", "goto", "package",
        "synchronized", "boolean", "do", "if", "private", "this", "break", "double",
        "implements", "protected", "throw", "byte", "else", "import", "public", "throws",
        "case", "enum", "instanceof", "return", "transient", "catch", "extends", "int",
        "short", "try", "char", "final", "interface", "static", "void", "class", "finally",
        "long", "strictfp", "volatile", "const", "float", "native", "super", "while",
    });
    static const std::vector<std::string> cppCollide({
        "size_t", "offsetof",
    });

    // errors
    if (std::find(keywords.begin(), keywords.end(), identifier) != keywords.end()) {
        *errorMsg = identifier + " is a HIDL keyword "
            "and is therefore not a valid identifier";
        return false;
    }
    if (std::find(cppKeywords.begin(), cppKeywords.end(), identifier) != cppKeywords.end()) {
        *errorMsg = identifier + " is a C++ keyword "
            "and is therefore not a valid identifier";
        return false;
    }
    if (std::find(javaKeywords.begin(), javaKeywords.end(), identifier) != javaKeywords.end()) {
        *errorMsg = identifier + " is a Java keyword "
            "and is therefore not a valid identifier";
        return false;
    }
    if (std::find(cppCollide.begin(), cppCollide.end(), identifier) != cppCollide.end()) {
        *errorMsg = identifier + " collides with reserved names in C++ code "
            "and is therefore not a valid identifier";
        return false;
    }
    if (StringHelper::StartsWith(identifier, "_hidl_")) {
        *errorMsg = identifier + " starts with _hidl_ "
            "and is therefore not a valid identifier";
        return false;
    }
    if (StringHelper::StartsWith(identifier, "hidl_")) {
        *errorMsg = identifier + " starts with hidl_ "
            "and is therefore not a valid identifier";
        return false;
    }
    if (StringHelper::EndsWith(identifier, "_cb")) {
        *errorMsg = identifier + " ends with _cb "
            "and is therefore not a valid identifier";
        return false;
    }

    return true;
}

// Return true if identifier is an acceptable name for an UDT.
bool isValidTypeName(const std::string& identifier, std::string *errorMsg) {
    if (!isValidIdentifier(identifier, errorMsg)) {
        return false;
    }

    if (identifier == "toString") {
        *errorMsg = identifier + " is not a valid type name";
        return false;
    }

    return true;
}

%}

%initial-action {
    // Initialize the initial location.
    @$.begin.filename = @$.end.filename =
        const_cast<std::string *>(&ast->getFilename());
}

%parse-param { void* scanner }
%parse-param { android::AST* const ast }
%parse-param { android::Scope** const scope }
%lex-param { void* scanner }
%lex-param { android::Scope** const scope }
%pure-parser
%glr-parser
%skeleton "glr.cc"

%expect-rr 0
%error-verbose

%debug

%token<docComment> DOC_COMMENT "doc comment"

%token<void> ENUM "keyword `enum`"
%token<void> EXTENDS "keyword `extends`"
%token<str> FQNAME "fully-qualified name"
%token<void> GENERATES "keyword `generates`"
%token<str> IDENTIFIER "identifier"
%token<void> IMPORT "keyword `import`"
%token<str> INTEGER "integer value"
%token<str> FLOAT "float value"
%token<void> INTERFACE "keyword `interface`"
%token<str> PACKAGE "keyword `package`"
%token<type> TYPE "type"
%token<void> STRUCT "keyword `struct`"
%token<str> STRING_LITERAL "string literal"
%token<void> TYPEDEF "keyword `typedef`"
%token<void> UNION "keyword `union`"
%token<void> SAFE_UNION "keyword `safe_union`"
%token<templatedType> TEMPLATED "templated type"
%token<void> ONEWAY "keyword `oneway`"
%token<str> UNKNOWN "unknown character"

/* Operator precedence and associativity, as per
 * http://en.cppreference.com/w/cpp/language/operator_precedence */
/* Precedence level 15 ternary operator */
%right '?' ':'
/* Precedence level 13 - 14, LTR, logical operators*/
%left LOGICAL_OR
%left LOGICAL_AND
/* Precedence level 10 - 12, LTR, bitwise operators*/
%left '|'
%left '^'
%left '&'
/* Precedence level 9, LTR */
%left EQUALITY NEQ
/* Precedence level 8, LTR */
%left '<' '>' LEQ GEQ
/* Precedence level 7, LTR */
%left LSHIFT RSHIFT
/* Precedence level 6, LTR */
%left '+' '-'
/* Precedence level 5, LTR */
%left '*' '/' '%'
/* Precedence level 3, RTL; but we have to use %left here */
%left UNARY_MINUS UNARY_PLUS '!' '~'

%token '#'

%type<docComment> doc_comments

%type<str> error_stmt error
%type<str> package
%type<fqName> fqname
%type<referenceToType> fqtype
%type<str> valid_identifier valid_type_name

%type<referenceToType> type enum_storage_type type_or_inplace_compound_declaration
%type<referenceToType> array_type_base
%type<arrayType> array_type
%type<referenceToType> opt_extends
%type<type> type_declaration commentable_type_declaration type_declaration_body
%type<type> interface_declaration typedef_declaration
%type<type> named_struct_or_union_declaration named_enum_declaration
%type<type> compound_declaration annotated_compound_declaration

%type<field> field_declaration commentable_field_declaration
%type<fields> field_declarations struct_or_union_body
%type<constantExpression> const_expr
%type<enumValue> enum_value commentable_enum_value
%type<enumValues> enum_values enum_declaration_body
%type<typedVars> typed_vars non_empty_typed_vars
%type<typedVar> typed_var
%type<method> method_declaration commentable_method_declaration
%type<compoundStyle> struct_or_union_keyword
%type<stringVec> annotation_string_values annotation_string_value
%type<constExprVec> annotation_const_expr_values annotation_const_expr_value
%type<annotationParam> annotation_param
%type<annotationParams> opt_annotation_params annotation_params
%type<annotation> annotation
%type<annotations> opt_annotations

%start program

%union {
    const char *str;
    android::Type* type;
    android::Reference<android::Type>* referenceToType;
    android::ArrayType *arrayType;
    android::TemplatedType *templatedType;
    android::FQName *fqName;
    android::CompoundType *compoundType;
    android::NamedReference<android::Type>* field;
    std::vector<android::NamedReference<android::Type>*>* fields;
    android::EnumValue *enumValue;
    android::ConstantExpression *constantExpression;
    std::vector<android::EnumValue *> *enumValues;
    android::NamedReference<android::Type>* typedVar;
    android::TypedVarVector *typedVars;
    android::Method *method;
    android::CompoundType::Style compoundStyle;
    std::vector<std::string> *stringVec;
    std::vector<android::ConstantExpression *> *constExprVec;
    android::AnnotationParam *annotationParam;
    android::AnnotationParamVector *annotationParams;
    android::Annotation *annotation;
    std::vector<android::Annotation *> *annotations;
    android::DocComment* docComment;
}

%%

program
    // Don't care if license header is a doc comment or not
    : DOC_COMMENT package imports type_declarations
    | package imports type_declarations
    ;

doc_comments
    : DOC_COMMENT { $$ = $1; }
    | doc_comments DOC_COMMENT
      {
        $1->merge($2);
        $$ = $1;
      }
    | doc_comments '}'
      {
        std::cerr << "ERROR: Doc comments must preceed what they describe at " << @1 << "\n";
        YYERROR;
      }
    ;

valid_identifier
    : IDENTIFIER
      {
        std::string errorMsg;
        if (!isValidIdentifier($1, &errorMsg)) {
            std::cerr << "ERROR: " << errorMsg << " at " << @1 << "\n";
            YYERROR;
        }
        $$ = $1;
      }
    ;

valid_type_name
    : IDENTIFIER
      {
        std::string errorMsg;
        if (!isValidTypeName($1, &errorMsg)) {
            std::cerr << "ERROR: " << errorMsg << " at " << @1 << "\n";
            YYERROR;
        }
        $$ = $1;
      }
    ;

opt_annotations
    : /* empty */
      {
          $$ = new std::vector<Annotation *>;
      }
    | opt_annotations annotation
      {
          $$ = $1;
          $$->push_back($2);
      }
    ;

annotation
    : '@' IDENTIFIER opt_annotation_params
      {
          $$ = new Annotation($2, $3);
      }
    ;

opt_annotation_params
    : /* empty */
      {
          $$ = new AnnotationParamVector;
      }
    | '(' annotation_params ')'
      {
          $$ = $2;
      }
    ;

annotation_params
    : annotation_param
      {
          $$ = new AnnotationParamVector;
          $$->push_back($1);
      }
    | annotation_params ',' annotation_param
      {
          $$ = $1;
          $$->push_back($3);
      }
    ;

annotation_param
    : IDENTIFIER '=' annotation_string_value
      {
          $$ = new StringAnnotationParam($1, $3);
      }
    | IDENTIFIER '=' annotation_const_expr_value
      {
          $$ = new ConstantExpressionAnnotationParam($1, $3);
      }
    ;

annotation_string_value
    : STRING_LITERAL
      {
          $$ = new std::vector<std::string>;
          $$->push_back($1);
      }
    | '{' annotation_string_values '}' { $$ = $2; }
    ;

annotation_string_values
    : STRING_LITERAL
      {
          $$ = new std::vector<std::string>;
          $$->push_back($1);
      }
    | annotation_string_values ',' STRING_LITERAL
      {
          $$ = $1;
          $$->push_back($3);
      }
    ;

annotation_const_expr_value
    : const_expr
      {
          $$ = new std::vector<ConstantExpression *>;
          $$->push_back($1);
      }
    | '{' annotation_const_expr_values '}' { $$ = $2; }
    ;

annotation_const_expr_values
    : const_expr
      {
          $$ = new std::vector<ConstantExpression *>;
          $$->push_back($1);
      }
    | annotation_const_expr_values ',' const_expr
      {
          $$ = $1;
          $$->push_back($3);
      }
    ;

error_stmt
  : error ';'
    {
      $$ = $1;
      ast->addSyntaxError();
    }
  ;

require_semicolon
    : ';'
    | /* empty */
      {
          std::cerr << "ERROR: missing ; at " << @$ << "\n";
          ast->addSyntaxError();
      }
    ;

fqname
    : FQNAME
      {
          $$ = new FQName();
          if(!FQName::parse($1, $$)) {
              std::cerr << "ERROR: FQName '" << $1 << "' is not valid at "
                        << @1
                        << ".\n";
              YYERROR;
          }
      }
    | valid_type_name
      {
          $$ = new FQName();
          if(!FQName::parse($1, $$)) {
              std::cerr << "ERROR: FQName '" << $1 << "' is not valid at "
                        << @1
                        << ".\n";
              YYERROR;
          }
      }
    ;

fqtype
    : fqname
      {
          $$ = new Reference<Type>(*$1, convertYYLoc(@1));
      }
    | TYPE
      {
          $$ = new Reference<Type>($1, convertYYLoc(@1));
      }
    ;

package
    : PACKAGE FQNAME require_semicolon
      {
          if (!ast->setPackage($2)) {
              std::cerr << "ERROR: Malformed package identifier '"
                        << $2
                        << "' at "
                        << @2
                        << "\n";

              YYERROR;
          }
      }
    | error
    {
      std::cerr << "ERROR: Package statement must be at the beginning of the file (" << @1 << ")\n";
      $$ = $1;
      ast->addSyntaxError();
    }
    ;

import_stmt
    : IMPORT FQNAME require_semicolon
      {
          if (!ast->addImport($2)) {
              std::cerr << "ERROR: Unable to import '" << $2 << "' at " << @2
                        << "\n";
              ast->addSyntaxError();
          }
      }
    | IMPORT valid_type_name require_semicolon
      {
          if (!ast->addImport($2)) {
              std::cerr << "ERROR: Unable to import '" << $2 << "' at " << @2
                        << "\n";
              ast->addSyntaxError();
          }
      }
    | IMPORT error_stmt
    ;


imports
    : /* empty */
    | imports import_stmt
    ;

opt_extends
    : /* empty */ { $$ = nullptr; }
    | EXTENDS fqtype { $$ = $2; }
    ;

interface_declarations
    : /* empty */
    | interface_declarations commentable_type_declaration
      {
          CHECK((*scope)->isInterface());

          std::string errorMsg;
          if ($2 != nullptr && $2->isNamedType() &&
              !isValidInterfaceField(static_cast<NamedType*>($2)->localName().c_str(),
                    &errorMsg)) {
              std::cerr << "ERROR: " << errorMsg << " at "
                        << @2 << "\n";
              YYERROR;
          }
      }
    | interface_declarations commentable_method_declaration
      {
          CHECK((*scope)->isInterface());

          std::string errorMsg;
          if ($2 != nullptr &&
              !isValidInterfaceField($2->name().c_str(), &errorMsg)) {
              std::cerr << "ERROR: " << errorMsg << " at "
                        << @2 << "\n";
              YYERROR;
          }

          if ($2 != nullptr) {
            Interface *iface = static_cast<Interface*>(*scope);
            if (!iface->addMethod($2)) {
                std::cerr << "ERROR: Unable to add method '" << $2->name()
                          << "' at " << @2 << "\n";

                YYERROR;
            }
          }
          // ignore if $2 is nullptr (from error recovery)
      }
    ;

type_declarations
    : /* empty */
    | error_stmt
    | type_declarations commentable_type_declaration
    ;

commentable_type_declaration
    : doc_comments type_declaration
      {
        $2->setDocComment($1);
        $$ = $2;
      }
    | type_declaration { $$ = $1; }
    ;

type_declaration
    : opt_annotations type_declaration_body
      {
          if (!$2->isTypeDef()) {
              CHECK($2->isScope());
              static_cast<Scope*>($2)->setAnnotations($1);
          } else if (!$1->empty()) {
              // Since typedefs are always resolved to their target it makes
              // little sense to annotate them and have their annotations
              // impose semantics other than their target type.
              std::cerr << "ERROR: typedefs cannot be annotated at " << @2
                        << "\n";

              YYERROR;
          }
          $$ = $2;
      }
    ;

type_declaration_body
    : named_struct_or_union_declaration require_semicolon
    | named_enum_declaration require_semicolon
    | typedef_declaration require_semicolon
    | interface_declaration require_semicolon
    ;

interface_declaration
    : INTERFACE valid_type_name opt_extends
      {
          Reference<Type>* superType = $3;
          bool isIBase = ast->package().package() == gIBaseFqName.package();

          if (isIBase) {
              if (superType != nullptr) {
                  std::cerr << "ERROR: IBase must not extend any interface at " << @3
                        << "\n";

                  YYERROR;
              }
              superType = new Reference<Type>();
          } else {
              if (!ast->addImport(gIBaseFqName.string().c_str())) {
                  std::cerr << "ERROR: Unable to automatically import '"
                            << gIBaseFqName.string()
                            << "' at " << @$
                            << "\n";
                  YYERROR;
              }

              if (superType == nullptr) {
                  superType = new Reference<Type>(gIBaseFqName, convertYYLoc(@$));
              }
          }

          if ($2[0] != 'I') {
              std::cerr << "ERROR: All interface names must start with an 'I' "
                        << "prefix at " << @2 << "\n";

              YYERROR;
          }

          if (*scope != ast->getRootScope()) {
              std::cerr << "ERROR: All interface must declared in "
                        << "global scope at " << @2 << "\n";

              YYERROR;
          }

          Interface* iface = new Interface(
              $2, ast->makeFullName($2, *scope), convertYYLoc(@2),
              *scope, *superType, ast->getFileHash());

          enterScope(ast, scope, iface);
      }
      '{' interface_declarations '}'
      {
          CHECK((*scope)->isInterface());

          Interface *iface = static_cast<Interface *>(*scope);
          CHECK(iface->addAllReservedMethods());

          leaveScope(ast, scope);
          ast->addScopedType(iface, *scope);
          $$ = iface;
      }
    ;

typedef_declaration
    : TYPEDEF type valid_type_name
      {
          // The reason we wrap the given type in a TypeDef is simply to suppress
          // emitting any type definitions later on, since this is just an alias
          // to a type defined elsewhere.
          TypeDef* typeDef = new TypeDef(
              $3, ast->makeFullName($3, *scope), convertYYLoc(@2), *scope, *$2);
          ast->addScopedType(typeDef, *scope);
          $$ = typeDef;
      }
    ;

const_expr
    : INTEGER
      {
          $$ = LiteralConstantExpression::tryParse($1);

          if ($$ == nullptr) {
              std::cerr << "ERROR: Could not parse literal: "
                        << $1 << " at " << @1 << ".\n";
              YYERROR;
          }
      }
    | fqname
      {
          if(!$1->isValidValueName()) {
              std::cerr << "ERROR: '" << $1->string()
                        << "' does not refer to an enum value at "
                        << @1 << ".\n";
              YYERROR;
          }

          $$ = new ReferenceConstantExpression(
              Reference<LocalIdentifier>(*$1, convertYYLoc(@1)), $1->string());
      }
    | fqname '#' IDENTIFIER
      {
          $$ = new AttributeConstantExpression(
              Reference<Type>(*$1, convertYYLoc(@1)), $1->string(), $3);
      }
    | const_expr '?' const_expr ':' const_expr
      {
          $$ = new TernaryConstantExpression($1, $3, $5);
      }
    | const_expr LOGICAL_OR const_expr  { $$ = new BinaryConstantExpression($1, "||", $3); }
    | const_expr LOGICAL_AND const_expr { $$ = new BinaryConstantExpression($1, "&&", $3); }
    | const_expr '|' const_expr { $$ = new BinaryConstantExpression($1, "|" , $3); }
    | const_expr '^' const_expr { $$ = new BinaryConstantExpression($1, "^" , $3); }
    | const_expr '&' const_expr { $$ = new BinaryConstantExpression($1, "&" , $3); }
    | const_expr EQUALITY const_expr { $$ = new BinaryConstantExpression($1, "==", $3); }
    | const_expr NEQ const_expr { $$ = new BinaryConstantExpression($1, "!=", $3); }
    | const_expr '<' const_expr { $$ = new BinaryConstantExpression($1, "<" , $3); }
    | const_expr '>' const_expr { $$ = new BinaryConstantExpression($1, ">" , $3); }
    | const_expr LEQ const_expr { $$ = new BinaryConstantExpression($1, "<=", $3); }
    | const_expr GEQ const_expr { $$ = new BinaryConstantExpression($1, ">=", $3); }
    | const_expr LSHIFT const_expr { $$ = new BinaryConstantExpression($1, "<<", $3); }
    | const_expr RSHIFT const_expr { $$ = new BinaryConstantExpression($1, ">>", $3); }
    | const_expr '+' const_expr { $$ = new BinaryConstantExpression($1, "+" , $3); }
    | const_expr '-' const_expr { $$ = new BinaryConstantExpression($1, "-" , $3); }
    | const_expr '*' const_expr { $$ = new BinaryConstantExpression($1, "*" , $3); }
    | const_expr '/' const_expr { $$ = new BinaryConstantExpression($1, "/" , $3); }
    | const_expr '%' const_expr { $$ = new BinaryConstantExpression($1, "%" , $3); }
    | '+' const_expr %prec UNARY_PLUS  { $$ = new UnaryConstantExpression("+", $2); }
    | '-' const_expr %prec UNARY_MINUS { $$ = new UnaryConstantExpression("-", $2); }
    | '!' const_expr { $$ = new UnaryConstantExpression("!", $2); }
    | '~' const_expr { $$ = new UnaryConstantExpression("~", $2); }
    | '(' const_expr ')' { $$ = $2; }
    | '(' error ')'
      {
        ast->addSyntaxError();
        // to avoid segfaults
        $$ = ConstantExpression::Zero(ScalarType::KIND_INT32).release();
      }
    ;

commentable_method_declaration
    : doc_comments method_declaration
      {
        if ($2 != nullptr) $2->setDocComment($1);
        $$ = $2;
      }
    | method_declaration
      {
        $$ = $1;
      }

method_declaration
    : error_stmt { $$ = nullptr; }
    | opt_annotations valid_identifier '(' typed_vars ')' require_semicolon
      {
          $$ = new Method($2 /* name */,
                          $4 /* args */,
                          new std::vector<NamedReference<Type>*> /* results */,
                          false /* oneway */,
                          $1 /* annotations */,
                          convertYYLoc(@$));
      }
    | opt_annotations ONEWAY valid_identifier '(' typed_vars ')' require_semicolon
      {
          $$ = new Method($3 /* name */,
                          $5 /* args */,
                          new std::vector<NamedReference<Type>*> /* results */,
                          true /* oneway */,
                          $1 /* annotations */,
                          convertYYLoc(@$));
      }
    | opt_annotations valid_identifier '(' typed_vars ')' GENERATES '(' typed_vars ')' require_semicolon
      {
          if ($8->empty()) {
              std::cerr << "ERROR: generates clause used without result at " << @1 << "\n";
              ast->addSyntaxError();
          }

          $$ = new Method($2 /* name */,
                          $4 /* args */,
                          $8 /* results */,
                          false /* oneway */,
                          $1 /* annotations */,
                          convertYYLoc(@$));
      }
    ;

typed_vars
    : /* empty */
      {
          $$ = new TypedVarVector();
      }
    | non_empty_typed_vars
      {
          $$ = $1;
      }
    ;

non_empty_typed_vars
    : typed_var
      {
          $$ = new TypedVarVector();
          if (!$$->add($1)) {
              std::cerr << "ERROR: duplicated argument or result name "
                  << $1->name() << " at " << @1 << "\n";
              ast->addSyntaxError();
          }
      }
    | non_empty_typed_vars ',' typed_var
      {
          $$ = $1;
          if (!$$->add($3)) {
              std::cerr << "ERROR: duplicated argument or result name "
                  << $3->name() << " at " << @3 << "\n";
              ast->addSyntaxError();
          }
      }
    ;

typed_var
    : type valid_identifier
      {
          $$ = new NamedReference<Type>($2, *$1, convertYYLoc(@2));
      }
    | type
      {
          $$ = new NamedReference<Type>("", *$1, convertYYLoc(@1));

          const std::string typeName = $$->isResolved()
              ? $$->get()->typeName() : $$->getLookupFqName().string();

          std::cerr << "ERROR: variable of type " << typeName
              << " is missing a variable name at " << @1 << "\n";
          ast->addSyntaxError();
      }
    ;


struct_or_union_keyword
    : STRUCT { $$ = CompoundType::STYLE_STRUCT; }
    | UNION { $$ = CompoundType::STYLE_UNION; }
    | SAFE_UNION { $$ = CompoundType::STYLE_SAFE_UNION; }
    ;

named_struct_or_union_declaration
    : struct_or_union_keyword valid_type_name
      {
          CompoundType *container = new CompoundType(
              $1, $2, ast->makeFullName($2, *scope), convertYYLoc(@2), *scope);
          enterScope(ast, scope, container);
      }
      struct_or_union_body
      {
          CHECK((*scope)->isCompoundType());
          CompoundType *container = static_cast<CompoundType *>(*scope);
          container->setFields($4);

          leaveScope(ast, scope);
          ast->addScopedType(container, *scope);
          $$ = container;
      }
    ;

struct_or_union_body
    : '{' field_declarations '}' { $$ = $2; }
    ;

field_declarations
    : /* empty */ { $$ = new std::vector<NamedReference<Type>*>; }
    | field_declarations commentable_field_declaration
      {
          $$ = $1;

          // Compound declaration or error
          if ($2 != nullptr) {
              $$->push_back($2);
          }
      }
    ;

commentable_field_declaration
    : doc_comments field_declaration
    {
      if ($2 != nullptr) $2->setDocComment($1);
      $$ = $2;
    }
    | field_declaration { $$ = $1; }

field_declaration
    : error_stmt { $$ = nullptr; }
    | type_or_inplace_compound_declaration valid_identifier require_semicolon
      {
          CHECK((*scope)->isCompoundType());

          std::string errorMsg;
          auto style = static_cast<CompoundType *>(*scope)->style();

          if (!isValidCompoundTypeField(style, $2, &errorMsg)) {
              std::cerr << "ERROR: " << errorMsg << " at "
                        << @2 << "\n";
              YYERROR;
          }
          $$ = new NamedReference<Type>($2, *$1, convertYYLoc(@2));
      }
    | annotated_compound_declaration ';'
      {
          CHECK((*scope)->isCompoundType());

          std::string errorMsg;
          auto style = static_cast<CompoundType *>(*scope)->style();

          if ($1 != nullptr && $1->isNamedType() &&
              !isValidCompoundTypeField(style, static_cast<NamedType*>(
                        $1)->localName().c_str(), &errorMsg)) {
              std::cerr << "ERROR: " << errorMsg << " at "
                        << @2 << "\n";
              YYERROR;
          }
          // Returns fields only
          $$ = nullptr;
      }
    ;

annotated_compound_declaration
    : opt_annotations compound_declaration
      {
          CHECK($2->isScope());
          static_cast<Scope*>($2)->setAnnotations($1);
          $$ = $2;
      }
    ;

compound_declaration
    : named_struct_or_union_declaration { $$ = $1; }
    | named_enum_declaration { $$ = $1; }
    ;

enum_storage_type
    : ':' fqtype { $$ = $2; }
    | /* empty */ { $$ = nullptr; }
    ;

opt_comma
    : /* empty */
    | ','
    ;

named_enum_declaration
    : ENUM valid_type_name enum_storage_type
      {
          auto storageType = $3;

          if (storageType == nullptr) {
              std::cerr << "ERROR: Must explicitly specify enum storage type for "
                        << $2 << " at " << @2 << "\n";
              ast->addSyntaxError();
              storageType = new Reference<Type>(
                  new ScalarType(ScalarType::KIND_INT64, *scope), convertYYLoc(@2));
          }

          EnumType* enumType = new EnumType(
              $2, ast->makeFullName($2, *scope), convertYYLoc(@2), *storageType, *scope);
          enterScope(ast, scope, enumType);
      }
      enum_declaration_body
      {
          CHECK((*scope)->isEnum());
          EnumType* enumType = static_cast<EnumType*>(*scope);

          leaveScope(ast, scope);
          ast->addScopedType(enumType, *scope);
          $$ = enumType;
      }
    ;

enum_declaration_body
    : '{' enum_values opt_comma '}' { $$ = $2; }
    ;

commentable_enum_value
    : doc_comments enum_value
      {
        $2->setDocComment($1);
        $$ = $2;
      }
    | enum_value { $$ = $1; }
    ;

enum_value
    : valid_identifier
      {
          $$ = new EnumValue($1 /* name */, nullptr /* value */, convertYYLoc(@$));
      }
    | valid_identifier '=' const_expr
      {
          $$ = new EnumValue($1 /* name */, $3 /* value */, convertYYLoc(@$));
      }
    ;

enum_values
    : /* empty */
      { /* do nothing */ }
    | commentable_enum_value
      {
          CHECK((*scope)->isEnum());
          static_cast<EnumType *>(*scope)->addValue($1);
      }
    | enum_values ',' commentable_enum_value
      {
          CHECK((*scope)->isEnum());
          static_cast<EnumType *>(*scope)->addValue($3);
      }
    | error ',' commentable_enum_value
      {
          ast->addSyntaxError();

          CHECK((*scope)->isEnum());
          static_cast<EnumType *>(*scope)->addValue($3);
      }
    | enum_values ',' error ',' commentable_enum_value
      {
          ast->addSyntaxError();

          CHECK((*scope)->isEnum());
          static_cast<EnumType *>(*scope)->addValue($5);
      }
    ;

array_type_base
    : fqtype { $$ = $1; }
    | TEMPLATED '<' type '>'
      {
          $1->setElementType(*$3);
          $$ = new Reference<Type>($1, convertYYLoc(@1));
      }
    | TEMPLATED '<' TEMPLATED '<' type RSHIFT
      {
          $3->setElementType(*$5);
          $1->setElementType(Reference<Type>($3, convertYYLoc(@3)));
          $$ = new Reference<Type>($1, convertYYLoc(@1));
      }
    ;

array_type
    : array_type_base '[' const_expr ']'
      {
          $$ = new ArrayType(*$1, $3, *scope);
      }
    | array_type '[' const_expr ']'
      {
          $$ = $1;
          $$->appendDimension($3);
      }
    ;

type
    : array_type_base { $$ = $1; }
    | array_type { $$ = new Reference<Type>($1, convertYYLoc(@1)); }
    | INTERFACE
      {
          // "interface" is a synonym of android.hidl.base@1.0::IBase
          $$ = new Reference<Type>(gIBaseFqName, convertYYLoc(@1));
      }
    ;

type_or_inplace_compound_declaration
    : type { $$ = $1; }
    | annotated_compound_declaration
      {
          $$ = new Reference<Type>($1, convertYYLoc(@1));
      }
    ;

%%

void yy::parser::error(
        const yy::parser::location_type &where,
        const std::string &errstr) {
    std::cerr << "ERROR: " << errstr << " at " << where << "\n";
}

