/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ast_cpp.h"

#include <algorithm>

#include "code_writer.h"
#include "logging.h"

using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace aidl {
namespace cpp {

std::string AstNode::ToString() {
  std::string str;
  Write(CodeWriter::ForString(&str).get());
  return str;
}

LiteralDecl::LiteralDecl(const std::string& expression) : expression_(expression) {}

void LiteralDecl::Write(CodeWriter* to) const {
  to->Write("%s", expression_.c_str());
}

ClassDecl::ClassDecl(const std::string& name, const std::string& parent)
    : name_(name),
      parent_(parent) {}

ClassDecl::ClassDecl(const std::string& name, const std::string& parent,
                     std::vector<unique_ptr<Declaration>> public_members,
                     std::vector<unique_ptr<Declaration>> private_members)
    : name_(name),
      parent_(parent),
      public_members_(std::move(public_members)),
      private_members_(std::move(private_members)) {}

void ClassDecl::Write(CodeWriter* to) const {
  to->Write("class %s ", name_.c_str());

  if (parent_.length() > 0)
      to->Write(": public %s ", parent_.c_str());

  to->Write("{\n");

  if (!public_members_.empty())
      to->Write("public:\n");

  to->Indent();
  for (const auto& dec : public_members_)
    dec->Write(to);
  to->Dedent();

  if (!private_members_.empty())
      to->Write("private:\n");

  to->Indent();
  for (const auto& dec : private_members_)
    dec->Write(to);
  to->Dedent();

  to->Write("};  // class %s\n", name_.c_str());
}

void ClassDecl::AddPublic(std::unique_ptr<Declaration> member) {
  public_members_.push_back(std::move(member));
}

void ClassDecl::AddPrivate(std::unique_ptr<Declaration> member) {
  private_members_.push_back(std::move(member));
}

Enum::EnumField::EnumField(const string& k, const string& v)
    : key(k),
      value(v) {}

Enum::Enum(const string& name, const string& base_type)
    : enum_name_(name), underlying_type_(base_type) {}

Enum::Enum(const string& name) : Enum(name, "") {}

void Enum::Write(CodeWriter* to) const {
  if (underlying_type_.empty()) {
    to->Write("enum %s {\n", enum_name_.c_str());
  } else {
    to->Write("enum %s : %s {\n", enum_name_.c_str(), underlying_type_.c_str());
  }
  to->Indent();
  for (const auto& field : fields_) {
    if (field.value.empty()) {
      to->Write("%s,\n", field.key.c_str());
    } else {
      to->Write("%s = %s,\n", field.key.c_str(), field.value.c_str());
    }
  }
  to->Dedent();
  to->Write("};\n");
}

void Enum::AddValue(const string& key, const string& value) {
  fields_.emplace_back(key, value);
}

ArgList::ArgList(const std::string& single_argument)
    : ArgList(vector<string>{single_argument}) {}

ArgList::ArgList(const std::vector<std::string>& arg_list) {
  for (const auto& s : arg_list) {
    arguments_.emplace_back(new LiteralExpression(s));
  }
}

ArgList::ArgList(std::vector<std::unique_ptr<AstNode>> arg_list)
    : arguments_(std::move(arg_list)) {}

ArgList::ArgList(ArgList&& arg_list) noexcept : arguments_(std::move(arg_list.arguments_)) {}

void ArgList::Write(CodeWriter* to) const {
  to->Write("(");
  bool is_first = true;
  for (const auto& s : arguments_) {
    if (!is_first) { to->Write(", "); }
    is_first = false;
    s->Write(to);
  }
  to->Write(")");
}

ConstructorDecl::ConstructorDecl(
    const std::string& name,
    ArgList&& arg_list)
    : ConstructorDecl(name, std::move(arg_list), 0u) {}

ConstructorDecl::ConstructorDecl(
    const std::string& name,
    ArgList&& arg_list,
    uint32_t modifiers)
    : name_(name),
      arguments_(std::move(arg_list)),
      modifiers_(modifiers) {}

void ConstructorDecl::Write(CodeWriter* to) const {
  if (modifiers_ & Modifiers::IS_VIRTUAL)
    to->Write("virtual ");

  if (modifiers_ & Modifiers::IS_EXPLICIT)
    to->Write("explicit ");

  to->Write("%s", name_.c_str());

  arguments_.Write(to);

  if (modifiers_ & Modifiers::IS_DEFAULT)
    to->Write(" = default");

  to->Write(";\n");
}

MacroDecl::MacroDecl(const std::string& name, ArgList&& arg_list)
    : name_(name),
      arguments_(std::move(arg_list)) {}

void MacroDecl::Write(CodeWriter* to) const {
  to->Write("%s", name_.c_str());
  arguments_.Write(to);
  to->Write("\n");
}

MethodDecl::MethodDecl(const std::string& return_type,
                       const std::string& name,
                       ArgList&& arg_list)
    : MethodDecl(return_type, name, std::move(arg_list), 0u) {}

MethodDecl::MethodDecl(const std::string& return_type, const std::string& name, ArgList&& arg_list,
                       uint32_t modifiers)
    : return_type_(return_type),
      name_(name),
      arguments_(std::move(arg_list)),
      is_const_(modifiers & IS_CONST),
      is_virtual_(modifiers & IS_VIRTUAL),
      is_override_(modifiers & IS_OVERRIDE),
      is_pure_virtual_(modifiers & IS_PURE_VIRTUAL),
      is_static_(modifiers & IS_STATIC),
      is_final_(modifiers & IS_FINAL) {}

void MethodDecl::Write(CodeWriter* to) const {
  if (is_virtual_)
    to->Write("virtual ");

  if (is_static_)
    to->Write("static ");

  to->Write("%s %s", return_type_.c_str(), name_.c_str());

  arguments_.Write(to);

  if (is_const_)
    to->Write(" const");

  if (is_override_)
    to->Write(" override");

  if (is_final_) to->Write(" final");

  if (is_pure_virtual_)
    to->Write(" = 0");

  to->Write(";\n");
}

void StatementBlock::AddStatement(unique_ptr<AstNode> statement) {
  statements_.push_back(std::move(statement));
}

void StatementBlock::AddStatement(AstNode* statement) {
  statements_.emplace_back(statement);
}

void StatementBlock::AddLiteral(const std::string& expression_str,
                                bool add_semicolon) {
  if (add_semicolon) {
    statements_.push_back(unique_ptr<AstNode>(new Statement(expression_str)));
  } else {
    statements_.push_back(unique_ptr<AstNode>(
        new LiteralExpression(expression_str)));
  }
}

void StatementBlock::Write(CodeWriter* to) const {
  to->Write("{\n");
  to->Indent();
  for (const auto& statement : statements_) {
    statement->Write(to);
  }
  to->Dedent();
  to->Write("}\n");
}

ConstructorImpl::ConstructorImpl(const string& class_name,
                                 ArgList&& arg_list,
                                 const vector<string>& initializer_list)
      : class_name_(class_name),
        arguments_(std::move(arg_list)),
        initializer_list_(initializer_list) {}

void ConstructorImpl::Write(CodeWriter* to) const {
  to->Write("%s::%s", class_name_.c_str(), class_name_.c_str());
  arguments_.Write(to);
  to->Write("\n");

  bool is_first = true;
  for (const string& i : initializer_list_) {
    if (is_first) {
      to->Write("    : %s", i.c_str());
    } else {
      to->Write(",\n      %s", i.c_str());
    }
    is_first = false;
  }

  body_.Write(to);
}

MethodImpl::MethodImpl(const string& return_type,
                       const string& class_name,
                       const string& method_name,
                       ArgList&& arg_list,
                       bool is_const_method)
    : return_type_(return_type),
      method_name_(method_name),
      arguments_(std::move(arg_list)),
      is_const_method_(is_const_method) {
  if (!class_name.empty()) {
    method_name_ = class_name + "::" + method_name;
  }
}

StatementBlock* MethodImpl::GetStatementBlock() {
  return &statements_;
}

void MethodImpl::Write(CodeWriter* to) const {
  to->Write("%s %s", return_type_.c_str(), method_name_.c_str());
  arguments_.Write(to);
  to->Write("%s ", (is_const_method_) ? " const" : "");
  statements_.Write(to);
}

SwitchStatement::SwitchStatement(const std::string& expression)
    : switch_expression_(expression) {}

StatementBlock* SwitchStatement::AddCase(const string& value_expression) {
  auto it = std::find(case_values_.begin(), case_values_.end(), value_expression);
  if (it != case_values_.end()) {
    LOG(ERROR) << "internal error: duplicate switch case labels";
    return nullptr;
  }
  StatementBlock* ret = new StatementBlock();
  case_values_.push_back(value_expression);
  case_logic_.push_back(unique_ptr<StatementBlock>{ret});
  return ret;
}

void SwitchStatement::Write(CodeWriter* to) const {
  to->Write("switch (%s) {\n", switch_expression_.c_str());
  for (size_t i = 0; i < case_values_.size(); ++i) {
    const string& case_value = case_values_[i];
    const unique_ptr<StatementBlock>& statements = case_logic_[i];
    if (case_value.empty()) {
      to->Write("default:\n");
    } else {
      to->Write("case %s:\n", case_value.c_str());
    }
    statements->Write(to);
    to->Write("break;\n");
  }
  to->Write("}\n");
}


Assignment::Assignment(const std::string& left, const std::string& right)
    : Assignment(left, new LiteralExpression{right}) {}

Assignment::Assignment(const std::string& left, AstNode* right)
    : lhs_(left),
      rhs_(right) {}

void Assignment::Write(CodeWriter* to) const {
  to->Write("%s = ", lhs_.c_str());
  rhs_->Write(to);
  to->Write(";\n");
}

MethodCall::MethodCall(const std::string& method_name,
                       const std::string& single_argument)
    : MethodCall(method_name, ArgList{single_argument}) {}

MethodCall::MethodCall(const std::string& method_name,
                       ArgList&& arg_list)
    : method_name_(method_name),
      arguments_{std::move(arg_list)} {}

void MethodCall::Write(CodeWriter* to) const {
  to->Write("%s", method_name_.c_str());
  arguments_.Write(to);
}

IfStatement::IfStatement(AstNode* expression, bool invert_expression)
    : expression_(expression),
      invert_expression_(invert_expression) {}

void IfStatement::Write(CodeWriter* to) const {
  to->Write("if (%s", (invert_expression_) ? "!(" : "");
  expression_->Write(to);
  to->Write(")%s ", (invert_expression_) ? ")" : "");
  on_true_.Write(to);

  if (!on_false_.Empty()) {
    to->Write("else ");
    on_false_.Write(to);
  }
}

Statement::Statement(unique_ptr<AstNode> expression)
    : expression_(std::move(expression)) {}

Statement::Statement(AstNode* expression) : expression_(expression) {}

Statement::Statement(const string& expression)
    : expression_(new LiteralExpression(expression)) {}

void Statement::Write(CodeWriter* to) const {
  expression_->Write(to);
  to->Write(";\n");
}

Comparison::Comparison(AstNode* lhs, const string& comparison, AstNode* rhs)
    : left_(lhs),
      right_(rhs),
      operator_(comparison) {}

void Comparison::Write(CodeWriter* to) const {
  to->Write("((");
  left_->Write(to);
  to->Write(") %s (", operator_.c_str());
  right_->Write(to);
  to->Write("))");
}

LiteralExpression::LiteralExpression(const std::string& expression)
    : expression_(expression) {}

void LiteralExpression::Write(CodeWriter* to) const {
  to->Write("%s", expression_.c_str());
}

CppNamespace::CppNamespace(const std::string& name,
                           std::vector<unique_ptr<Declaration>> declarations)
    : declarations_(std::move(declarations)),
      name_(name) {}

CppNamespace::CppNamespace(const std::string& name,
                           unique_ptr<Declaration> declaration)
    : name_(name) {
  declarations_.push_back(std::move(declaration));
}
CppNamespace::CppNamespace(const std::string& name)
    : name_(name) {}

void CppNamespace::Write(CodeWriter* to) const {
  to->Write("namespace %s {\n\n", name_.c_str());

  for (const auto& dec : declarations_) {
    dec->Write(to);
    to->Write("\n");
  }

  to->Write("}  // namespace %s\n", name_.c_str());
}

Document::Document(const std::vector<std::string>& include_list,
                   std::vector<unique_ptr<Declaration>> declarations)
    : include_list_(include_list), declarations_(std::move(declarations)) {}

void Document::Write(CodeWriter* to) const {
  for (const auto& include : include_list_) {
    to->Write("#include <%s>\n", include.c_str());
  }
  to->Write("\n");

  for (const auto& declaration : declarations_) {
    declaration->Write(to);
  }
}

CppHeader::CppHeader(const std::string& include_guard, const std::vector<std::string>& include_list,
                     std::vector<std::unique_ptr<Declaration>> declarations)
    : Document(include_list, std::move(declarations)), include_guard_(include_guard) {}

void CppHeader::Write(CodeWriter* to) const {
  to->Write("#ifndef %s\n", include_guard_.c_str());
  to->Write("#define %s\n\n", include_guard_.c_str());

  Document::Write(to);
  to->Write("\n");

  to->Write("#endif  // %s\n", include_guard_.c_str());
}

CppSource::CppSource(const std::vector<std::string>& include_list,
                     std::vector<std::unique_ptr<Declaration>> declarations)
    : Document(include_list, std::move(declarations)) {}

}  // namespace cpp
}  // namespace aidl
}  // namespace android
