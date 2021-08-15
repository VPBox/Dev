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

#include <string>

#include <gtest/gtest.h>

#include "ast_cpp.h"
#include "code_writer.h"

using std::string;
using std::vector;
using std::unique_ptr;

namespace android {
namespace aidl {
namespace cpp {
namespace {

const char kExpectedHeaderOutput[] =
R"(#ifndef HEADER_INCLUDE_GUARD_H_
#define HEADER_INCLUDE_GUARD_H_

#include <string>
#include <memory>

namespace android {

namespace test {

class TestClass {
public:
  void NormalMethod(int normalarg, float normal2);
  virtual void SubMethod(int subarg) const;
};  // class TestClass

class TestSubClass : public TestClass {
public:
  virtual void SubMethod(int subarg) const;
};  // class TestSubClass

}  // namespace test

}  // namespace android

#endif  // HEADER_INCLUDE_GUARD_H_
)";

const char kExpectedEnumOutput[] =
R"(enum Foo {
  BAR = 42,
  BAZ,
};
)";

const char kExpectedSwitchOutput[] =
R"(switch (var) {
case 2:
{
  baz;
}
break;
case 1:
{
  foo;
  bar;
}
break;
}
)";

const char kExpectedMethodImplOutput[] =
R"(return_type ClassName::MethodName(arg 1, arg 2, arg 3) const {
  foo;
  bar;
}
)";
}  // namespace

class AstCppTests : public ::testing::Test {
 protected:
  void CompareGeneratedCode(const AstNode& node,
                            const string& expected_output) {
    string actual_output;
    node.Write(CodeWriter::ForString(&actual_output).get());
    EXPECT_EQ(expected_output, actual_output);
  }
};  // class AstCppTests


TEST_F(AstCppTests, GeneratesHeader) {
  unique_ptr<MethodDecl> norm{new MethodDecl(
      "void", "NormalMethod",
      ArgList{vector<string>{"int normalarg", "float normal2"}})};
  unique_ptr<MethodDecl> sub{
      new MethodDecl("void", "SubMethod",
                     ArgList{ "int subarg" },
                     MethodDecl::IS_CONST | MethodDecl::IS_VIRTUAL)};
  unique_ptr<MethodDecl> sub2{
      new MethodDecl("void", "SubMethod",
                     ArgList{ "int subarg" },
                     MethodDecl::IS_CONST | MethodDecl::IS_VIRTUAL)};
  vector<unique_ptr<Declaration>> test_methods;
  test_methods.push_back(std::move(norm));
  test_methods.push_back(std::move(sub));

  vector<unique_ptr<Declaration>> test_sub_methods;
  test_sub_methods.push_back(std::move(sub2));

  unique_ptr<Declaration> test{new ClassDecl { "TestClass", "",
      std::move(test_methods), {} }};

  unique_ptr<Declaration> test_sub{new ClassDecl { "TestSubClass",
      "TestClass", std::move(test_sub_methods), {} }};

  vector<unique_ptr<Declaration>> classes;
  classes.push_back(std::move(test));
  classes.push_back(std::move(test_sub));

  unique_ptr<CppNamespace> test_ns{new CppNamespace {"test",
      std::move(classes)}};

  vector<unique_ptr<Declaration>> test_ns_vec;
  test_ns_vec.push_back(std::move(test_ns));

  unique_ptr<CppNamespace> android_ns{new CppNamespace {"android",
      std::move(test_ns_vec) }};

  vector<unique_ptr<Declaration>> test_ns_globals;
  test_ns_globals.push_back(std::move(android_ns));

  CppHeader cpp_header{"HEADER_INCLUDE_GUARD_H_", {"string", "memory"}, std::move(test_ns_globals)};
  CompareGeneratedCode(cpp_header, kExpectedHeaderOutput);
}

TEST_F(AstCppTests, GeneratesEnum) {
  Enum e("Foo");
  e.AddValue("BAR", "42");
  e.AddValue("BAZ", "");
  CompareGeneratedCode(e, kExpectedEnumOutput);
}

TEST_F(AstCppTests, GeneratesArgList) {
  ArgList simple("foo");
  CompareGeneratedCode(simple, "(foo)");
  ArgList compound({"foo", "bar", "baz"});
  CompareGeneratedCode(compound, "(foo, bar, baz)");
  std::vector<unique_ptr<AstNode>> args;
  args.emplace_back(new LiteralExpression("foo()"));
  ArgList nested(std::move(args));
  CompareGeneratedCode(nested, "(foo())");
}

TEST_F(AstCppTests, GeneratesStatement) {
  Statement s(new LiteralExpression("foo"));
  CompareGeneratedCode(s, "foo;\n");
}

TEST_F(AstCppTests, GeneratesComparison) {
  Comparison c(
      new LiteralExpression("lhs"), "&&", new LiteralExpression("rhs"));
  CompareGeneratedCode(c, "((lhs) && (rhs))");
}

TEST_F(AstCppTests, GeneratesStatementBlock) {
  StatementBlock block;
  block.AddStatement(unique_ptr<AstNode>(new Statement("foo")));
  block.AddStatement(unique_ptr<AstNode>(new Statement("bar")));
  CompareGeneratedCode(block, "{\n  foo;\n  bar;\n}\n");
}

TEST_F(AstCppTests, GeneratesConstructorImpl) {
  ConstructorImpl c("ClassName", ArgList({"a", "b", "c"}),
                    {"baz_(foo)", "bar_(blah)"});
  string expected = R"(ClassName::ClassName(a, b, c)
    : baz_(foo),
      bar_(blah){
}
)";
  CompareGeneratedCode(c, expected);
}

TEST_F(AstCppTests, GeneratesAssignment) {
  Assignment simple("foo", "8");
  CompareGeneratedCode(simple, "foo = 8;\n");
  Assignment less_simple("foo", new MethodCall("f", "8"));
  CompareGeneratedCode(less_simple, "foo = f(8);\n");
}

TEST_F(AstCppTests, GeneratesMethodCall) {
  MethodCall single("single", "arg");
  CompareGeneratedCode(single, "single(arg)");
  MethodCall multi(
      "multi",
      ArgList({"has", "some", "args"}));
  CompareGeneratedCode(multi, "multi(has, some, args)");
}

TEST_F(AstCppTests, GeneratesIfStatement) {
  IfStatement s(new LiteralExpression("foo"));
  s.OnTrue()->AddLiteral("on true1");
  s.OnFalse()->AddLiteral("on false");
  CompareGeneratedCode(s, "if (foo) {\n  on true1;\n}\nelse {\n  on false;\n}\n");

  IfStatement s2(new LiteralExpression("bar"));
  s2.OnTrue()->AddLiteral("on true1");
  CompareGeneratedCode(s2, "if (bar) {\n  on true1;\n}\n");
}

TEST_F(AstCppTests, GeneratesSwitchStatement) {
  SwitchStatement s("var");
  // These are intentionally out of alphanumeric order.  We're testing
  // that switch respects case addition order.
  auto case2 = s.AddCase("2");
  case2->AddStatement(unique_ptr<AstNode>{new Statement{"baz"}});
  auto case1 = s.AddCase("1");
  case1->AddStatement(unique_ptr<AstNode>{new Statement{"foo"}});
  case1->AddStatement(unique_ptr<AstNode>{new Statement{"bar"}});
  CompareGeneratedCode(s, kExpectedSwitchOutput);
}

TEST_F(AstCppTests, GeneratesMethodImpl) {
  MethodImpl m{"return_type", "ClassName", "MethodName",
               ArgList{{"arg 1", "arg 2", "arg 3"}},
               true};
  auto b = m.GetStatementBlock();
  b->AddLiteral("foo");
  b->AddLiteral("bar");
  CompareGeneratedCode(m, kExpectedMethodImplOutput);
}

TEST_F(AstCppTests, ToString) {
  std::string literal = "void foo() {}";
  LiteralDecl decl(literal);
  std::string actual = decl.ToString();
  EXPECT_EQ(literal, actual);
  std::string written;
  decl.Write(CodeWriter::ForString(&written).get());
  EXPECT_EQ(literal, written);
}

}  // namespace cpp
}  // namespace aidl
}  // namespace android
