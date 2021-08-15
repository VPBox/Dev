/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <string>

#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "CodeWriter.h"

namespace {

constexpr const char* kIndent = "    ";

constexpr const char* kHelloWorld =
    R"(#include <stdio.h>

int main() {
    printf("Hello World\n");
}
)";

}  // namespace

TEST(SyspropTest, CodeWriterIndentOutputTest) {
  CodeWriter writer(kIndent);
  writer.Write("test1\ntest2\n");
  writer.Indent();
  writer.Write("test3\ntest4\n");
  writer.Indent();
  writer.Write("test5\ntest6\n");
  writer.Dedent();
  writer.Dedent();
  writer.Write("test7\ntest8\n");

  ASSERT_EQ(writer.Code(),
            "test1\n"
            "test2\n"
            "    test3\n"
            "    test4\n"
            "        test5\n"
            "        test6\n"
            "test7\n"
            "test8\n");
}

TEST(SyspropTest, CodeWriterCodeGenerationTest) {
  CodeWriter writer(kIndent);
  writer.Write(kHelloWorld);
  ASSERT_EQ(writer.Code(), kHelloWorld);
}
