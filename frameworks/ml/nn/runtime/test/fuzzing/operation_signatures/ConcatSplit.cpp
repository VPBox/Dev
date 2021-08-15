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

namespace android {
namespace nn {
namespace fuzzing_test {

// CONCATENATION with fixed number of input tensors.
static void concatConstructor(uint32_t numInputs, bool isV1_0, uint32_t rank, RandomOperation* op) {
    for (uint32_t i = 0; i < numInputs; i++) {
        op->inputs[i]->dimensions.resize(rank);
        if (isV1_0) setSameQuantization(op->inputs[i], op->inputs[0]);
    }
    op->outputs[0]->dimensions.resize(rank);

    int32_t axis = getUniform<int32_t>(0, rank - 1);
    op->inputs[numInputs]->setScalarValue<int32_t>(axis);
    for (uint32_t i = 0; i < rank; i++) {
        op->inputs[0]->dimensions[i] = RandomVariableType::FREE;
        op->outputs[0]->dimensions[i] = op->inputs[0]->dimensions[i];
        for (uint32_t j = 1; j < numInputs; j++) {
            if (axis == static_cast<int32_t>(i)) {
                op->inputs[j]->dimensions[i] = RandomVariableType::FREE;
                op->outputs[0]->dimensions[i] =
                        op->outputs[0]->dimensions[i] + op->inputs[j]->dimensions[i];
            } else {
                op->inputs[j]->dimensions[i] = op->inputs[0]->dimensions[i];
            }
        }
    }
}

DEFINE_OPERATION_SIGNATURE(CONCAT_2_V1_0){
        .opType = ANEURALNETWORKS_CONCATENATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            concatConstructor(/*numInputs=*/2, /*isV1_0=*/true, rank, op);
        }};

DEFINE_OPERATION_SIGNATURE(CONCAT_3_V1_0){
        .opType = ANEURALNETWORKS_CONCATENATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            concatConstructor(/*numInputs=*/3, /*isV1_0=*/true, rank, op);
        }};

DEFINE_OPERATION_SIGNATURE(CONCAT_2_V1_2){
        .opType = ANEURALNETWORKS_CONCATENATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            concatConstructor(/*numInputs=*/2, /*isV1_0=*/false, rank, op);
        }};

DEFINE_OPERATION_SIGNATURE(CONCAT_3_V1_2){
        .opType = ANEURALNETWORKS_CONCATENATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            concatConstructor(/*numInputs=*/3, /*isV1_0=*/false, rank, op);
        }};

// SPLIT with fixed number of splits.
static void splitConstructor(uint32_t numSplits, uint32_t rank, RandomOperation* op) {
    int32_t axis = getUniform<int32_t>(-rank, rank - 1);
    op->inputs[1]->setScalarValue<int32_t>(axis);
    if (axis < 0) axis += rank;

    op->inputs[0]->dimensions.resize(rank);
    for (uint32_t i = 0; i < numSplits; i++) {
        op->outputs[i]->dimensions.resize(rank);
        setSameQuantization(op->outputs[i], op->inputs[0]);
    }

    for (uint32_t i = 0; i < rank; i++) {
        op->inputs[0]->dimensions[i] = RandomVariableType::FREE;
        RandomVariable outDim;
        if (axis == static_cast<int32_t>(i)) {
            outDim = op->inputs[0]->dimensions[i].exactDiv(numSplits);
        } else {
            outDim = op->inputs[0]->dimensions[i];
        }
        for (uint32_t j = 0; j < numSplits; j++) op->outputs[j]->dimensions[i] = outDim;
    }
}

DEFINE_OPERATION_SIGNATURE(SPLIT_2_V1_2){
        .opType = ANEURALNETWORKS_SPLIT,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16, Type::TENSOR_INT32,
                               Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::INT32), PARAMETER_CHOICE(Type::INT32, 2)},
        .outputs = {OUTPUT_DEFAULT, OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            splitConstructor(/*numSplits=*/2, rank, op);
        }};

DEFINE_OPERATION_SIGNATURE(SPLIT_3_V1_2){
        .opType = ANEURALNETWORKS_SPLIT,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16, Type::TENSOR_INT32,
                               Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::INT32), PARAMETER_CHOICE(Type::INT32, 3)},
        .outputs = {OUTPUT_DEFAULT, OUTPUT_DEFAULT, OUTPUT_DEFAULT},
        .constructor = [](Type, uint32_t rank, RandomOperation* op) {
            splitConstructor(/*numSplits=*/3, rank, op);
        }};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
