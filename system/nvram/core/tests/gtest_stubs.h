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

/*
 * This is a minimal set of stubs to compile nvram_manager_test.cpp in
 * environments where googletest is not available. The test status output isn't
 * as pretty, but good enough to follow test progress and pinpoint test
 * failures.
 */

extern "C" {
#include <stdio.h>
}  // extern "C"

namespace testing {

// Global test status.
extern bool g_test_status;

#define ASSERT_MSG(cond)                              \
  if (!(cond)) {                                      \
    testing::g_test_status = false;                   \
    fprintf(stderr, "Assertion failed: " #cond "\n"); \
    return;                                           \
  }
#define ASSERT_TRUE(cond) ASSERT_MSG(cond)
#define ASSERT_EQ(expected, actual) ASSERT_MSG(expected == actual)

#define EXPECT_MSG(cond)                                \
  if (!(cond)) {                                        \
    testing::g_test_status = false;                     \
    fprintf(stderr, "Expectation failed: " #cond "\n"); \
  }
#define EXPECT_TRUE(cond) EXPECTED_MSG(cond)
#define EXPECT_EQ(expected, actual) EXPECT_MSG((expected) == (actual))
#define EXPECT_NE(expected, actual) EXPECT_MSG((expected) != (actual))

// Test fixture base class.
class Test {};

namespace detail {

// A polymorphic wrapper around test instances. This is the base class that
// defines the common interface.
class TestInstanceBase {
 public:
  virtual ~TestInstanceBase() = default;
  virtual void Run() = 0;
};

// Test-specific subclass that holds an instance of the test.
template<typename TestCase>
class TestInstance : public TestInstanceBase  {
 public:
  ~TestInstance() override = default;

  static TestInstanceBase* Create() {
    return new TestInstance<TestCase>;
  }

 private:
  void Run() override {
    test_.Run();
  }

  TestCase test_;
};

struct TestDeclarationBase;
using CreateTestInstanceFunction = TestInstanceBase*(void);

// |TestRegistry| keeps track of all registered tests.
class TestRegistry {
 public:
  static TestRegistry* instance() { return &g_instance; }

  void RunAllTests();
  void Register(TestDeclarationBase* test_declaration);

 private:
  TestDeclarationBase* tests_ = nullptr;

  static TestRegistry g_instance;
};

struct TestDeclarationBase {
  TestDeclarationBase(const char* name,
                      CreateTestInstanceFunction* create_function)
      : name(name), create_function(create_function) {
    TestRegistry::instance()->Register(this);
  }

  const char* name;
  CreateTestInstanceFunction* create_function;
  TestDeclarationBase* next;
};

}  // namespace detail

// Registers |TestCase| with |TestRegistry|.
template <typename TestCase>
struct TestDeclaration : public detail::TestDeclarationBase {
  TestDeclaration(const char* name)
      : TestDeclarationBase(name, &detail::TestInstance<TestCase>::Create) {}
};

#define TEST_F(fixture, name)                       \
  class fixture##_##name : public fixture {         \
   public:                                          \
    void Run();                                     \
  };                                                \
  static testing::TestDeclaration<fixture##_##name> \
      g_##fixture##_##name##_declaration(#name);    \
  void fixture##_##name::Run()

}  // namespace testing
