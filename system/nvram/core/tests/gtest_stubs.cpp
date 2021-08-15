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

#include "gtest_stubs.h"

namespace testing {

bool g_test_status = false;

namespace detail {

void TestRegistry::RunAllTests() {
  int test_count = 0;
  int test_failures = 0;
  for (TestDeclarationBase* decl = tests_; decl; decl = decl->next) {
    TestInstanceBase* instance = decl->create_function();
    fprintf(stderr, "[ %s ] Starting...\n", decl->name);
    g_test_status = true;
    instance->Run();
    test_failures += g_test_status ? 0 : 1;
    ++test_count;
    fprintf(stderr, "[ %s ] %s\n", decl->name, g_test_status ? "PASS" : "FAIL");
    delete instance;
  }

  fprintf(stderr, "Ran %d tests, %d failures.\n", test_count, test_failures);
}

void TestRegistry::Register(TestDeclarationBase* test_declaration) {
  test_declaration->next = tests_;
  tests_ = test_declaration;
}

TestRegistry TestRegistry::g_instance;

}  // namespace detail
}  // namespace testing

int main() {
  testing::detail::TestRegistry::instance()->RunAllTests();
}
