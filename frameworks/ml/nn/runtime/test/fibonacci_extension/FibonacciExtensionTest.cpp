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

#include "HalInterfaces.h"
#include "Manager.h"
#include "NeuralNetworks.h"
#include "NeuralNetworksExtensions.h"
#include "NeuralNetworksWrapperExtensions.h"
#include "TestNeuralNetworksWrapper.h"
#include "TypeManager.h"
#include "Utils.h"
#include "ValidateHal.h"

#include <gtest/gtest.h>

#include "FibonacciDriver.h"
#include "FibonacciExtension.h"

namespace android {
namespace nn {
namespace {

using ::android::nn::test_wrapper::ExtensionModel;
using ::android::nn::test_wrapper::ExtensionOperandParams;
using ::android::nn::test_wrapper::ExtensionOperandType;
using ::android::nn::test_wrapper::Type;

class FibonacciExtensionTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
        if (DeviceManager::get()->getUseCpuOnly()) {
            // This test requires the use a custom driver.
            GTEST_SKIP();
        }

        // Real world extension tests should run against actual hardware
        // implementations, but there is no hardware supporting the test
        // extension. Hence the sample software driver.
        DeviceManager::get()->forTest_registerDevice(sample_driver::FibonacciDriver::kDriverName,
                                                     new sample_driver::FibonacciDriver());
        // Discover extensions provided by registered devices.
        TypeManager::get()->forTest_reset();

        uint32_t numDevices = 0;
        ASSERT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
        ANeuralNetworksDevice* fibonacciDevice = nullptr;
        ANeuralNetworksDevice* cpuDevice = nullptr;
        for (uint32_t i = 0; i < numDevices; i++) {
            ANeuralNetworksDevice* device = nullptr;
            EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
            bool supportsFibonacciExtension;
            ASSERT_EQ(ANeuralNetworksDevice_getExtensionSupport(
                              device, TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                              &supportsFibonacciExtension),
                      ANEURALNETWORKS_NO_ERROR);
            if (supportsFibonacciExtension) {
                ASSERT_EQ(fibonacciDevice, nullptr) << "Found multiple Fibonacci drivers";
                fibonacciDevice = device;
            } else if (DeviceManager::get()->forTest_isCpuDevice(device)) {
                ASSERT_EQ(cpuDevice, nullptr) << "Found multiple CPU drivers";
                cpuDevice = device;
            }
        }
        ASSERT_NE(fibonacciDevice, nullptr) << "Expecting Fibonacci driver to be available";
        ASSERT_NE(cpuDevice, nullptr) << "Expecting CPU driver to be available";
        mDevices = {fibonacciDevice, cpuDevice};
    }

    virtual void TearDown() {
        if (mExecution) {
            ANeuralNetworksExecution_free(mExecution);
        }
        if (mCompilation) {
            ANeuralNetworksCompilation_free(mCompilation);
        }
        DeviceManager::get()->forTest_reInitializeDeviceList();
        TypeManager::get()->forTest_reset();
    }

    void checkSupportedOperations(const std::vector<bool>& expected) {
        const uint32_t kMaxNumberOperations = 256;
        EXPECT_LE(expected.size(), kMaxNumberOperations);
        bool supported[kMaxNumberOperations] = {false};
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(
                          mModel.getHandle(), mDevices.data(), mDevices.size(), supported),
                  ANEURALNETWORKS_NO_ERROR);
        for (size_t i = 0; i < expected.size(); ++i) {
            SCOPED_TRACE(::testing::Message() << "i = " << i);
            EXPECT_EQ(supported[i], expected[i]);
        }
    }

    void prepareForExecution() {
        ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                              mDevices.size(), &mCompilation),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &mExecution),
                  ANEURALNETWORKS_NO_ERROR);
    }

    std::vector<ANeuralNetworksDevice*> mDevices;
    ANeuralNetworksExecution* mExecution = nullptr;
    ANeuralNetworksCompilation* mCompilation = nullptr;
    ExtensionModel mModel;
};

void addNopOperation(ExtensionModel* model, ExtensionOperandType inputType, uint32_t input,
                     uint32_t output) {
    // Our NOP operation is ADD, which has no extension type support.
    ASSERT_EQ(inputType.operandType.type, ANEURALNETWORKS_TENSOR_FLOAT32);
    ASSERT_EQ(inputType.dimensions.size(), 1u);

    uint32_t inputZeros = model->addOperand(&inputType);
    uint32_t inputSize = inputType.dimensions[0];
    uint32_t inputLength = sizeof(float) * inputSize;
    const float kZeros[100] = {};
    ASSERT_GE(sizeof(kZeros), inputLength);
    model->setOperandValue(inputZeros, &kZeros, inputLength);

    ExtensionOperandType scalarType(Type::INT32, {});
    uint32_t activation = model->addOperand(&scalarType);
    int32_t kNoActivation = ANEURALNETWORKS_FUSED_NONE;
    model->setOperandValue(activation, &kNoActivation, sizeof(kNoActivation));

    model->addOperation(ANEURALNETWORKS_ADD, {input, inputZeros, activation}, {output});
}

void createModel(ExtensionModel* model, ExtensionOperandType inputType,
                 ExtensionOperandType outputType, bool addNopOperations) {
    uint32_t fibonacciInput = model->addOperand(&inputType);
    uint32_t fibonacciOutput = model->addOperand(&outputType);

    uint32_t modelInput = addNopOperations ? model->addOperand(&inputType) : fibonacciInput;
    uint32_t modelOutput = addNopOperations ? model->addOperand(&outputType) : fibonacciOutput;

    if (addNopOperations) {
        addNopOperation(model, inputType, modelInput, fibonacciInput);
    }
    model->addOperation(model->getExtensionOperationType(TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                                         TEST_VENDOR_FIBONACCI),
                        {fibonacciInput}, {fibonacciOutput});
    if (addNopOperations) {
        addNopOperation(model, outputType, fibonacciOutput, modelOutput);
    }

    model->identifyInputsAndOutputs({modelInput}, {modelOutput});
    model->finish();
    ASSERT_TRUE(model->isValid());
}

TEST_F(FibonacciExtensionTest, ModelWithExtensionOperandTypes) {
    constexpr uint32_t N = 10;
    constexpr double scale = 0.5;
    constexpr int64_t zeroPoint = 10;

    ExtensionOperandType inputType(
            static_cast<Type>(mModel.getExtensionOperandType(TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                                             TEST_VENDOR_INT64)),
            {});
    ExtensionOperandType outputType(
            static_cast<Type>(mModel.getExtensionOperandType(TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                                             TEST_VENDOR_TENSOR_QUANT64_ASYMM)),
            {N},
            ExtensionOperandParams(TestVendorQuant64AsymmParams{
                    .scale = scale,
                    .zeroPoint = zeroPoint,
            }));
    createModel(&mModel, inputType, outputType, /*addNopOperations=*/false);
    checkSupportedOperations({true});
    prepareForExecution();

    int64_t input = N;
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, &input, sizeof(input)),
              ANEURALNETWORKS_NO_ERROR);

    int64_t output[N] = {};
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, &output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);

    ASSERT_EQ(ANeuralNetworksExecution_compute(mExecution), ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(output[0], 1 / scale + zeroPoint);
    EXPECT_EQ(output[1], 1 / scale + zeroPoint);
    EXPECT_EQ(output[2], 2 / scale + zeroPoint);
    EXPECT_EQ(output[3], 3 / scale + zeroPoint);
    EXPECT_EQ(output[4], 5 / scale + zeroPoint);
    EXPECT_EQ(output[5], 8 / scale + zeroPoint);
    EXPECT_EQ(output[6], 13 / scale + zeroPoint);
    EXPECT_EQ(output[7], 21 / scale + zeroPoint);
    EXPECT_EQ(output[8], 34 / scale + zeroPoint);
    EXPECT_EQ(output[9], 55 / scale + zeroPoint);
}

TEST_F(FibonacciExtensionTest, ModelWithTemporaries) {
    constexpr uint32_t N = 10;

    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {N});
    createModel(&mModel, inputType, outputType, /*addNopOperations=*/true);
    checkSupportedOperations({true, true, true});
    prepareForExecution();

    float input[] = {N};
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, &input, sizeof(input)),
              ANEURALNETWORKS_NO_ERROR);

    float output[N] = {};
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, &output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);

    ASSERT_EQ(ANeuralNetworksExecution_compute(mExecution), ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(output[0], 1);
    EXPECT_EQ(output[1], 1);
    EXPECT_EQ(output[2], 2);
    EXPECT_EQ(output[3], 3);
    EXPECT_EQ(output[4], 5);
    EXPECT_EQ(output[5], 8);
    EXPECT_EQ(output[6], 13);
    EXPECT_EQ(output[7], 21);
    EXPECT_EQ(output[8], 34);
    EXPECT_EQ(output[9], 55);
}

TEST_F(FibonacciExtensionTest, InvalidInputType) {
    ExtensionOperandType inputType(Type::TENSOR_INT32, {1});  // Unsupported type.
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {1});
    createModel(&mModel, inputType, outputType, /*addNopOperations=*/false);
    checkSupportedOperations({false});  // The driver reports that it doesn't support the operation.
    ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(FibonacciExtensionTest, InvalidOutputType) {
    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_INT32, {1});  // Unsupported type.
    createModel(&mModel, inputType, outputType, /*addNopOperations=*/false);
    checkSupportedOperations({false});  // The driver reports that it doesn't support the operation.
    ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(FibonacciExtensionTest, InvalidInputValue) {
    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {1});
    createModel(&mModel, inputType, outputType, /*addNopOperations=*/false);
    checkSupportedOperations({true});
    prepareForExecution();

    float input[] = {-1};  // Invalid input value.
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, &input, sizeof(input)),
              ANEURALNETWORKS_NO_ERROR);

    float output[1] = {};
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, &output, sizeof(output)),
              ANEURALNETWORKS_NO_ERROR);

    ASSERT_EQ(ANeuralNetworksExecution_compute(mExecution), ANEURALNETWORKS_OP_FAILED);
}

TEST_F(FibonacciExtensionTest, InvalidNumInputs) {
    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {1});
    uint32_t input1 = mModel.addOperand(&inputType);
    uint32_t input2 = mModel.addOperand(&inputType);  // Extra input.
    uint32_t output = mModel.addOperand(&outputType);
    mModel.addOperation(mModel.getExtensionOperationType(TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                                         TEST_VENDOR_FIBONACCI),
                        {input1, input2}, {output});
    mModel.identifyInputsAndOutputs({input1, input2}, {output});
    mModel.finish();
    ASSERT_TRUE(mModel.isValid());
    checkSupportedOperations({false});
    ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(FibonacciExtensionTest, InvalidNumOutputs) {
    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {1});
    uint32_t input = mModel.addOperand(&inputType);
    uint32_t output1 = mModel.addOperand(&outputType);
    uint32_t output2 = mModel.addOperand(&outputType);  // Extra output.
    mModel.addOperation(mModel.getExtensionOperationType(TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                                         TEST_VENDOR_FIBONACCI),
                        {input}, {output1, output2});
    mModel.identifyInputsAndOutputs({input}, {output1, output2});
    mModel.finish();
    ASSERT_TRUE(mModel.isValid());
    checkSupportedOperations({false});
    ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(FibonacciExtensionTest, InvalidOperation) {
    ExtensionOperandType inputType(Type::TENSOR_FLOAT32, {1});
    ExtensionOperandType outputType(Type::TENSOR_FLOAT32, {1});
    uint32_t input = mModel.addOperand(&inputType);
    uint32_t output = mModel.addOperand(&outputType);
    mModel.addOperation(mModel.getExtensionOperationType(
                                TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                                TEST_VENDOR_FIBONACCI + 1),  // This operation should not exist.
                        {input}, {output});
    mModel.identifyInputsAndOutputs({input}, {output});
    mModel.finish();
    ASSERT_TRUE(mModel.isValid());
    checkSupportedOperations({false});
    ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel.getHandle(), mDevices.data(),
                                                          mDevices.size(), &mCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_DATA);
}

}  // namespace
}  // namespace nn
}  // namespace android
