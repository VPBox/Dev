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

#define LOG_TAG "FibonacciDriver"

#include "FibonacciDriver.h"

#include "HalInterfaces.h"
#include "NeuralNetworksExtensions.h"
#include "OperationResolver.h"
#include "OperationsUtils.h"
#include "Utils.h"
#include "ValidateHal.h"

#include "FibonacciExtension.h"

namespace android {
namespace nn {
namespace sample_driver {
namespace {

const uint8_t kLowBitsType = static_cast<uint8_t>(Model::ExtensionTypeEncoding::LOW_BITS_TYPE);
const uint32_t kTypeWithinExtensionMask = (1 << kLowBitsType) - 1;

namespace fibonacci_op {

constexpr char kOperationName[] = "TEST_VENDOR_FIBONACCI";

constexpr uint32_t kNumInputs = 1;
constexpr uint32_t kInputN = 0;

constexpr uint32_t kNumOutputs = 1;
constexpr uint32_t kOutputTensor = 0;

bool getFibonacciExtensionPrefix(const Model& model, uint16_t* prefix) {
    NN_RET_CHECK_EQ(model.extensionNameToPrefix.size(), 1u);  // Assumes no other extensions in use.
    NN_RET_CHECK_EQ(model.extensionNameToPrefix[0].name, TEST_VENDOR_FIBONACCI_EXTENSION_NAME);
    *prefix = model.extensionNameToPrefix[0].prefix;
    return true;
}

bool isFibonacciOperation(const Operation& operation, const Model& model) {
    int32_t operationType = static_cast<int32_t>(operation.type);
    uint16_t prefix;
    NN_RET_CHECK(getFibonacciExtensionPrefix(model, &prefix));
    NN_RET_CHECK_EQ(operationType, (prefix << kLowBitsType) | TEST_VENDOR_FIBONACCI);
    return true;
}

bool validate(const Operation& operation, const Model& model) {
    NN_RET_CHECK(isFibonacciOperation(operation, model));
    NN_RET_CHECK_EQ(operation.inputs.size(), kNumInputs);
    NN_RET_CHECK_EQ(operation.outputs.size(), kNumOutputs);
    int32_t inputType = static_cast<int32_t>(model.operands[operation.inputs[0]].type);
    int32_t outputType = static_cast<int32_t>(model.operands[operation.outputs[0]].type);
    uint16_t prefix;
    NN_RET_CHECK(getFibonacciExtensionPrefix(model, &prefix));
    NN_RET_CHECK(inputType == ((prefix << kLowBitsType) | TEST_VENDOR_INT64) ||
                 inputType == ANEURALNETWORKS_TENSOR_FLOAT32);
    NN_RET_CHECK(outputType == ((prefix << kLowBitsType) | TEST_VENDOR_TENSOR_QUANT64_ASYMM) ||
                 outputType == ANEURALNETWORKS_TENSOR_FLOAT32);
    return true;
}

bool prepare(IOperationExecutionContext* context) {
    int64_t n;
    if (context->getInputType(kInputN) == OperandType::TENSOR_FLOAT32) {
        n = static_cast<int64_t>(context->getInputValue<float>(kInputN));
    } else {
        n = context->getInputValue<int64_t>(kInputN);
    }
    NN_RET_CHECK_GE(n, 1);
    Shape output = context->getOutputShape(kOutputTensor);
    output.dimensions = {static_cast<uint32_t>(n)};
    return context->setOutputShape(kOutputTensor, output);
}

template <typename ScaleT, typename ZeroPointT, typename OutputT>
bool compute(int32_t n, ScaleT outputScale, ZeroPointT outputZeroPoint, OutputT* output) {
    // Compute the Fibonacci numbers.
    if (n >= 1) {
        output[0] = 1;
    }
    if (n >= 2) {
        output[1] = 1;
    }
    if (n >= 3) {
        for (int32_t i = 2; i < n; ++i) {
            output[i] = output[i - 1] + output[i - 2];
        }
    }

    // Quantize output.
    for (int32_t i = 0; i < n; ++i) {
        output[i] = output[i] / outputScale + outputZeroPoint;
    }

    return true;
}

bool execute(IOperationExecutionContext* context) {
    int64_t n;
    if (context->getInputType(kInputN) == OperandType::TENSOR_FLOAT32) {
        n = static_cast<int64_t>(context->getInputValue<float>(kInputN));
    } else {
        n = context->getInputValue<int64_t>(kInputN);
    }
    if (context->getOutputType(kOutputTensor) == OperandType::TENSOR_FLOAT32) {
        float* output = context->getOutputBuffer<float>(kOutputTensor);
        return compute(n, /*scale=*/1.0, /*zeroPoint=*/0, output);
    } else {
        uint64_t* output = context->getOutputBuffer<uint64_t>(kOutputTensor);
        Shape outputShape = context->getOutputShape(kOutputTensor);
        auto outputQuant = reinterpret_cast<const TestVendorQuant64AsymmParams*>(
                outputShape.extraParams.extension().data());
        return compute(n, outputQuant->scale, outputQuant->zeroPoint, output);
    }
}

}  // namespace fibonacci_op
}  // namespace

const OperationRegistration* FibonacciOperationResolver::findOperation(
        OperationType operationType) const {
    // .validate is omitted because it's not used by the extension driver.
    static OperationRegistration operationRegistration(operationType, fibonacci_op::kOperationName,
                                                       nullptr, fibonacci_op::prepare,
                                                       fibonacci_op::execute, {});
    uint16_t prefix = static_cast<int32_t>(operationType) >> kLowBitsType;
    uint16_t typeWithinExtension = static_cast<int32_t>(operationType) & kTypeWithinExtensionMask;
    // Assumes no other extensions in use.
    return prefix != 0 && typeWithinExtension == TEST_VENDOR_FIBONACCI ? &operationRegistration
                                                                       : nullptr;
}

Return<void> FibonacciDriver::getSupportedExtensions(getSupportedExtensions_cb cb) {
    cb(ErrorStatus::NONE,
       {
               {
                       .name = TEST_VENDOR_FIBONACCI_EXTENSION_NAME,
                       .operandTypes =
                               {
                                       {
                                               .type = TEST_VENDOR_INT64,
                                               .isTensor = false,
                                               .byteSize = 8,
                                       },
                                       {
                                               .type = TEST_VENDOR_TENSOR_QUANT64_ASYMM,
                                               .isTensor = true,
                                               .byteSize = 8,
                                       },
                               },
               },
       });
    return Void();
}

Return<void> FibonacciDriver::getCapabilities_1_2(getCapabilities_1_2_cb cb) {
    android::nn::initVLogMask();
    VLOG(DRIVER) << "getCapabilities()";
    static const PerformanceInfo kPerf = {.execTime = 1.0f, .powerUsage = 1.0f};
    Capabilities capabilities = {.relaxedFloat32toFloat16PerformanceScalar = kPerf,
                                 .relaxedFloat32toFloat16PerformanceTensor = kPerf,
                                 .operandPerformance = nonExtensionOperandPerformance(kPerf)};
    cb(ErrorStatus::NONE, capabilities);
    return Void();
}

Return<void> FibonacciDriver::getSupportedOperations_1_2(const V1_2::Model& model,
                                                         getSupportedOperations_1_2_cb cb) {
    VLOG(DRIVER) << "getSupportedOperations()";
    if (!validateModel(model)) {
        cb(ErrorStatus::INVALID_ARGUMENT, {});
        return Void();
    }
    const size_t count = model.operations.size();
    std::vector<bool> supported(count);
    for (size_t i = 0; i < count; ++i) {
        const Operation& operation = model.operations[i];
        if (fibonacci_op::isFibonacciOperation(operation, model)) {
            if (!fibonacci_op::validate(operation, model)) {
                cb(ErrorStatus::INVALID_ARGUMENT, {});
                return Void();
            }
            supported[i] = true;
        }
    }
    cb(ErrorStatus::NONE, supported);
    return Void();
}

}  // namespace sample_driver
}  // namespace nn
}  // namespace android
