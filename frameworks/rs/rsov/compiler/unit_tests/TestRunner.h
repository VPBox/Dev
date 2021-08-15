/*
 * Copyright 2016, The Android Open Source Project
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

#ifndef RS2SPIRV_TEST_RUNNER
#define RS2SPIRV_TEST_RUNNER

#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <cassert>
#include <vector>

namespace rs2spirv {

struct TestCase {
  void (*testPtr)(void);
  const char *const description;
};

class TestRunnerContext {
public:
  static TestRunnerContext &getInstance() {
    static TestRunnerContext ctx;
    return ctx;
  }

  static void addTest(TestCase TC) { getInstance().tests.push_back(TC); }
  static size_t &getCheckSuccessNum() { return getInstance().checkSuccessNum; }
  static size_t &getTotalCheckNum() { return getInstance().totalCheckNum; }

  static int runTests() {
    bool Failed = false;
    for (auto &TC : getInstance().tests) {
      getCheckSuccessNum() = getTotalCheckNum() = 0;
      llvm::outs() << "Test(" << TC.description << ") {\n";
      TC.testPtr();
      llvm::outs() << "\n} (" << TC.description << ") [" << getCheckSuccessNum()
                   << "/" << getTotalCheckNum() << "]\n\n";
      Failed |= getCheckSuccessNum() != getTotalCheckNum();
    }

    return static_cast<int>(Failed);
  }

private:
  TestRunnerContext() = default;
  std::vector<TestCase> tests;
  size_t checkSuccessNum;
  size_t totalCheckNum;
};

struct TestAdder {
  TestAdder(TestCase TC) { TestRunnerContext::addTest(TC); }
};

#define RS2SPIRV_CONCAT_IMPL(S1, S2) S1##S2
#define RS2SPIRV_CONCAT(S1, S2) RS2SPIRV_CONCAT_IMPL(S1, S2)
#define RS2SPIRV_ANONYMOUS(X) RS2SPIRV_CONCAT(X, __COUNTER__)

#if RS2SPIRV_DEBUG
#define RS2SPIRV_TEST_CASE_ADD_IMPL(FNAME, VNAME, DESCRIPTION)                 \
  static void FNAME();                                                         \
  static rs2spirv::TestAdder VNAME({FNAME, DESCRIPTION});                      \
  inline void FNAME()
#elif defined(__GNUC__) || defined(__clang__)
#define RS2SPIRV_TEST_CASE_ADD_IMPL(FNAME, VNAME, DESCRIPTION)                 \
  static inline void __attribute__((unused)) FNAME()
#else
#define RS2SPIRV_TEST_CASE_ADD_IMPL(FNAME, VNAME, DESCRIPTION)                 \
  static inline void FNAME()
#endif

#define RS2SPIRV_TEST_CASE_ADD(NAME, DESCRIPTION)                              \
  RS2SPIRV_TEST_CASE_ADD_IMPL(RS2SPIRV_ANONYMOUS(NAME),                        \
                              RS2SPIRV_ANONYMOUS(NAME), DESCRIPTION)

#define TEST_CASE(DESCRIPTION) RS2SPIRV_TEST_CASE_ADD(TC, DESCRIPTION)

#define CHECK(CONDITION)                                                       \
  ++rs2spirv::TestRunnerContext::getTotalCheckNum();                           \
  if (!(CONDITION))                                                            \
    llvm::errs() << "\nCHECK <(  " #CONDITION "  )> failed!\n";                \
  else                                                                         \
    ++rs2spirv::TestRunnerContext::getCheckSuccessNum();                       \
  (void)0

} // namespace rs2spirv

#endif
