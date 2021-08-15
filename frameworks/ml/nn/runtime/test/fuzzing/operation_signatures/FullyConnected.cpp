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

#include "fuzzing/operation_signatures/OperationSignatureUtils.h"

using namespace std::placeholders;

namespace android {
namespace nn {
namespace fuzzing_test {

static void fullyConnectedConstructor(Type, uint32_t rank, HalVersion ver, RandomOperation* op) {
    // Inputs, flattened to [batch_size, input_size]
    op->inputs[0]->dimensions.resize(rank);
    RandomVariable numElements = 1;
    for (uint32_t i = 0; i < rank; i++) {
        op->inputs[0]->dimensions[i] = RandomVariableType::FREE;
        numElements = numElements * op->inputs[0]->dimensions[i];
    }

    // Weights, [num_units, input_size]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE};

    // Bias, [num_units]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch_size, num_units]
    op->outputs[0]->dimensions = {numElements.exactDiv(op->inputs[1]->dimensions[1]),
                                  op->inputs[1]->dimensions[0]};

    setConvFCScale(/*applyOutputScaleBound=*/ver == HalVersion::V1_0, op);
}

DEFINE_OPERATION_SIGNATURE(signature_FULLY_CONNECTED_V1_0){
        .opType = ANEURALNETWORKS_FULLY_CONNECTED,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {2, 3, 4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, INPUT_BIAS,
                   PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(fullyConnectedConstructor, _1, _2, HalVersion::V1_0, _3)};

DEFINE_OPERATION_SIGNATURE(signature_FULLY_CONNECTED_V1_2){
        .opType = ANEURALNETWORKS_FULLY_CONNECTED,
        .supportedDataTypes = {Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_FLOAT16},
        .supportedRanks = {2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, INPUT_BIAS,
                   PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(fullyConnectedConstructor, _1, _2, HalVersion::V1_2, _3)};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
