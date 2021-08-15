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

#include "TestCompliance.h"

#include <gtest/gtest.h>

#include "ModelBuilder.h"
#include "TestNeuralNetworksWrapper.h"
#include "Utils.h"

namespace compliance_test {

using namespace ::android::nn;
using HidlModel = V1_2::Model;
using WrapperModel = test_wrapper::Model;
using WrapperOperandType = test_wrapper::OperandType;
using WrapperType = test_wrapper::Type;

// Creates a HIDL model from a creator of the wrapper model.
static HidlModel createHidlModel(std::function<void(WrapperModel*)> createModel) {
    HidlModel hidlModel;
    WrapperModel wrapperModel;
    createModel(&wrapperModel);
    EXPECT_EQ(wrapperModel.finish(), test_wrapper::Result::NO_ERROR);
    ModelBuilder* modelBuilder = reinterpret_cast<ModelBuilder*>(wrapperModel.getHandle());
    modelBuilder->setHidlModel(&hidlModel);
    return hidlModel;
}

void ComplianceTest::testAvailableSinceV1_2(std::function<void(WrapperModel*)> createModel) {
    HidlModel model = createHidlModel(createModel);
    ASSERT_FALSE(compliantWithV1_1(model));
    ASSERT_FALSE(compliantWithV1_0(model));
}

void ComplianceTest::testAvailableSinceV1_1(std::function<void(WrapperModel*)> createModel) {
    HidlModel model = createHidlModel(createModel);
    ASSERT_TRUE(compliantWithV1_1(model));
    ASSERT_FALSE(compliantWithV1_0(model));
}

void ComplianceTest::testAvailableSinceV1_0(std::function<void(WrapperModel*)> createModel) {
    HidlModel model = createHidlModel(createModel);
    ASSERT_TRUE(compliantWithV1_1(model));
    ASSERT_TRUE(compliantWithV1_0(model));
}

static const WrapperOperandType kTypeTensorFloat(WrapperType::TENSOR_FLOAT32, {1});
static const WrapperOperandType kTypeTensorFloatRank0(WrapperType::TENSOR_FLOAT32, {});
static const WrapperOperandType kTypeInt32(WrapperType::INT32, {});

TEST_F(ComplianceTest, Rank0TensorModelInput) {
    int32_t act_init = 0;
    // A simple ADD operation: op1 ADD op2 = op3, with op1 and op2 of rank 0.
    testAvailableSinceV1_2([&act_init](WrapperModel* model) {
        auto op1 = model->addOperand(&kTypeTensorFloatRank0);
        auto op2 = model->addOperand(&kTypeTensorFloatRank0);
        auto act = model->addOperand(&kTypeInt32);
        auto op3 = model->addOperand(&kTypeTensorFloat);
        model->setOperandValue(act, &act_init, sizeof(act_init));
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
        model->identifyInputsAndOutputs({op1, op2}, {op3});
        assert(model->isValid());
    });
}

TEST_F(ComplianceTest, Rank0TensorModelOutput) {
    int32_t act_init = 0;
    // A simple ADD operation: op1 ADD op2 = op3, with op3 of rank 0.
    testAvailableSinceV1_2([&act_init](WrapperModel* model) {
        auto op1 = model->addOperand(&kTypeTensorFloat);
        auto op2 = model->addOperand(&kTypeTensorFloat);
        auto act = model->addOperand(&kTypeInt32);
        auto op3 = model->addOperand(&kTypeTensorFloatRank0);
        model->setOperandValue(act, &act_init, sizeof(act_init));
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
        model->identifyInputsAndOutputs({op1, op2}, {op3});
        assert(model->isValid());
    });
}

TEST_F(ComplianceTest, Rank0TensorTemporaryVariable) {
    int32_t act_init = 0;
    // Two ADD operations: op1 ADD op2 = op3, op3 ADD op4 = op5, with op3 of rank 0.
    testAvailableSinceV1_2([&act_init](WrapperModel* model) {
        auto op1 = model->addOperand(&kTypeTensorFloat);
        auto op2 = model->addOperand(&kTypeTensorFloat);
        auto op3 = model->addOperand(&kTypeTensorFloatRank0);
        auto op4 = model->addOperand(&kTypeTensorFloat);
        auto op5 = model->addOperand(&kTypeTensorFloat);
        auto act = model->addOperand(&kTypeInt32);
        model->setOperandValue(act, &act_init, sizeof(act_init));
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
        model->addOperation(ANEURALNETWORKS_ADD, {op3, op4, act}, {op5});
        model->identifyInputsAndOutputs({op1, op2, op4}, {op5});
        assert(model->isValid());
    });
}

TEST_F(ComplianceTest, HardwareBuffer) {
    const size_t memorySize = 20;
    AHardwareBuffer_Desc desc{
            .width = memorySize,
            .height = 1,
            .layers = 1,
            .format = AHARDWAREBUFFER_FORMAT_BLOB,
            .usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
    };

    AHardwareBuffer* buffer = nullptr;
    ASSERT_EQ(AHardwareBuffer_allocate(&desc, &buffer), 0);
    test_wrapper::Memory memory(buffer);
    ASSERT_TRUE(memory.isValid());

    int32_t act_init = 0;

    // A simple ADD operation: op1 ADD op2 = op3, with op2 using a const hardware buffer.
    testAvailableSinceV1_2([&memory, &act_init](WrapperModel* model) {
        auto op1 = model->addOperand(&kTypeTensorFloat);
        auto op2 = model->addOperand(&kTypeTensorFloat);
        auto act = model->addOperand(&kTypeInt32);
        auto op3 = model->addOperand(&kTypeTensorFloat);
        model->setOperandValueFromMemory(op2, &memory, 0, sizeof(float));
        model->setOperandValue(act, &act_init, sizeof(act_init));
        model->addOperation(ANEURALNETWORKS_ADD, {op1, op2, act}, {op3});
        model->identifyInputsAndOutputs({op1}, {op3});
        assert(model->isValid());
    });

    AHardwareBuffer_release(buffer);
}

}  // namespace compliance_test
