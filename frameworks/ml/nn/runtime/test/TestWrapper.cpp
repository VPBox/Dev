/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "NeuralNetworksWrapper.h"

#include <gtest/gtest.h>

using namespace ::android::nn::wrapper;

// This file tests certain aspects of the interfaces from NeuralNetworksWrapper.h.

class WrapperTestModelFinish : public ::testing::Test {
protected:
    void SetUp() override {
        OperandType type(Type::TENSOR_FLOAT32, {1});
        mIn = mModel.addOperand(&type);
        mOut = mModel.addOperand(&type);
        mModel.addOperation(ANEURALNETWORKS_TANH, {mIn}, {mOut});
    }

    Model mModel;
    uint32_t mIn, mOut;
};

TEST_F(WrapperTestModelFinish, Good) {
    ASSERT_TRUE(mModel.isValid());
    mModel.identifyInputsAndOutputs({mIn}, {mOut});
    ASSERT_TRUE(mModel.isValid());
    ASSERT_EQ(mModel.finish(), Result::NO_ERROR);
    ASSERT_TRUE(mModel.isValid());
}

TEST_F(WrapperTestModelFinish, BadIdentify) {
    ASSERT_TRUE(mModel.isValid());
    // A Model must have at least one input and one output. Verify
    // that if it doesn't, then isValid() goes false and finish()
    // fails.
    mModel.identifyInputsAndOutputs({}, {});
    ASSERT_FALSE(mModel.isValid());
    ASSERT_EQ(mModel.finish(), Result::BAD_STATE);
    ASSERT_FALSE(mModel.isValid());
}

TEST_F(WrapperTestModelFinish, BadFinish) {
    ASSERT_TRUE(mModel.isValid());
    mModel.identifyInputsAndOutputs({mIn}, {mOut});
    ASSERT_TRUE(mModel.isValid());
    ASSERT_EQ(mModel.finish(), Result::NO_ERROR);
    ASSERT_TRUE(mModel.isValid());
    // The easiest way to get a failed finish() is to call it a second
    // time. Verify that this fails and that isValid() goes false.
    ASSERT_EQ(mModel.finish(), Result::BAD_STATE);
    ASSERT_FALSE(mModel.isValid());
}
