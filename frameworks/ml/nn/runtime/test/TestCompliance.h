/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_TESTCOMPLIANCE_H
#define ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_TESTCOMPLIANCE_H

#include <gtest/gtest.h>

#include "TestNeuralNetworksWrapper.h"

#if defined(NNTEST_ONLY_PUBLIC_API) || defined(NNTEST_MULTITHREADED)
#define TEST_AVAILABLE_SINCE(version, testName, createModel)
#else

namespace compliance_test {
using namespace ::android::nn;

// Tag for the compilance tests
class ComplianceTest : public ::testing::Test {
   protected:
    void testAvailableSinceV1_2(std::function<void(test_wrapper::Model*)> createModel);
    void testAvailableSinceV1_1(std::function<void(test_wrapper::Model*)> createModel);
    void testAvailableSinceV1_0(std::function<void(test_wrapper::Model*)> createModel);
};

}  // namespace compliance_test

#define TEST_AVAILABLE_SINCE(version, testName, createModel)                       \
    namespace compliance_test {                                                    \
    TEST_F(ComplianceTest, testName) { testAvailableSince##version(createModel); } \
    }

#endif

#endif  // ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_TESTCOMPLIANCE_H
