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

#ifndef CHRE_PLATFORM_LINUX_ASSERT_H_
#define CHRE_PLATFORM_LINUX_ASSERT_H_

#include <cassert>

#define CHRE_ASSERT_USES_STDLIB_ASSERT

#ifdef GTEST

#include "chre/platform/log.h"
#include "gmock/gmock.h"

class MockAssert;
extern MockAssert *gMockAssert;

class AssertInterface {
 public:
  virtual void doAssert() = 0;
};

class MockAssert : public AssertInterface {
 public:
  MockAssert() {
    gMockAssert = this;
  }
  ~MockAssert() {
    gMockAssert = nullptr;
  }

  MOCK_METHOD0(doAssert, void());
};

/**
 * Helper macro that wraps a statement in a block that sets up the mock for
 * CHRE_ASSERT and expects it to be called at least once. This allows for
 * verification that the code to be tested throws an expected assertion failure,
 * and also handles the failure gracefully when assertions are compiled out.
 * Triggered assertions are logged using LOGI, so they can be manually checked
 * in the test output.
 *
 * Example:
 * @code{.cpp}
 *   TEST(DynamicVector, InsertToSparseIndexFails) {
 *     DynamicVector<int> vector;
 *     EXPECT_CHRE_ASSERT(EXPECT_FALSE(vector.insert(5));
 *   }
 * @endcode
 */
#define EXPECT_CHRE_ASSERT(statement)                                     \
  do {                                                                    \
    ASSERT_EQ(gMockAssert, nullptr);                                      \
    MockAssert chreMockAssert;                                            \
    EXPECT_CALL(chreMockAssert, doAssert()).Times(::testing::AtLeast(1)); \
    statement;                                                            \
  } while (0)

#define CHRE_ASSERT(condition)                           \
  do {                                                   \
    if (gMockAssert != nullptr && !(condition)) {        \
      LOGI("Mocked assertion " #condition " triggered"); \
      gMockAssert->doAssert();                           \
    } else {                                             \
      assert(condition);                                 \
    }                                                    \
  } while (0)

#else  // if !defined(GTEST)

#define CHRE_ASSERT(condition) assert(condition)

#endif  // GTEST

#endif  // CHRE_PLATFORM_LINUX_ASSERT_H_
