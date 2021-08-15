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

#include "ast_java.h"
#include "code_writer.h"
#include "type_java.h"

using std::string;

namespace android {
namespace aidl {
namespace java {
namespace {

const char kExpectedClassOutput[] =
R"(// class comment
final class TestClass extends SuperClass
{
}
)";

}  // namespace

TEST(AstJavaTests, GeneratesClass) {
  JavaTypeNamespace types;
  types.Init();
  Type class_type(&types, "TestClass", ValidatableType::KIND_GENERATED, false);
  Type extend_type(&types, "SuperClass", ValidatableType::KIND_BUILT_IN, false);
  Class a_class;
  a_class.comment = "// class comment";
  a_class.modifiers = FINAL;
  a_class.what = Class::CLASS;
  a_class.type = class_type.JavaType();
  a_class.extends = extend_type.JavaType();

  string actual_output;
  a_class.Write(CodeWriter::ForString(&actual_output).get());
  EXPECT_EQ(string(kExpectedClassOutput), actual_output);
}

TEST(AstJavaTests, ToString) {
  std::string literal = "public void foo() {}";
  LiteralClassElement ce(literal);
  std::string actual = ce.ToString();
  EXPECT_EQ(literal, actual);

  std::string written;
  ce.Write(CodeWriter::ForString(&written).get());
  EXPECT_EQ(literal, written);
}

}  // namespace java
}  // namespace aidl
}  // namespace android
