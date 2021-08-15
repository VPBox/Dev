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

static void broadcastOpConstructor(Type dataType, uint32_t rank, RandomOperation* op) {
    // TODO: All inputs of the broadcast op have the same rank 4 for now.
    op->inputs[0]->dimensions.resize(rank);
    op->inputs[1]->dimensions.resize(rank);
    op->outputs[0]->dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; i++) {
        if (getBernoulli(0.9f)) {
            op->inputs[0]->dimensions[i] = RandomVariableType::FREE;
        } else {
            op->inputs[0]->dimensions[i] = 1;
        }
        if (getBernoulli(0.9f)) {
            op->inputs[1]->dimensions[i] = op->inputs[0]->dimensions[i];
        } else {
            op->inputs[1]->dimensions[i] = 1;
        }
        op->outputs[0]->dimensions[i] =
                max(op->inputs[0]->dimensions[i], op->inputs[1]->dimensions[i]);
    }

    // MUL requires output.scale > input0.scale * input1.scale.
    if (dataType == Type::TENSOR_QUANT8_ASYMM && op->opType == ANEURALNETWORKS_MUL) {
        float minScale = op->inputs[0]->scale * op->inputs[1]->scale;
        op->outputs[0]->scale = getUniform(minScale, minScale * 5);
    }

    // DIV and POW may produce Inf output values. We should not connect this output tensor to the
    // input of another operation.
    if (op->opType == ANEURALNETWORKS_DIV || op->opType == ANEURALNETWORKS_POW) {
        op->outputs[0]->doNotConnect = true;
    }
}

// For broadcast operations with fused activation.
#define DEFINE_BROADCAST_WITH_ACT_SIGNATURE(op, ver, ...)                                        \
    DEFINE_OPERATION_SIGNATURE(op##_##ver){                                                      \
            .opType = ANEURALNETWORKS_##op,                                                      \
            .supportedDataTypes = {__VA_ARGS__},                                                 \
            .supportedRanks = {1, 2, 3, 4},                                                      \
            .version = HalVersion::ver,                                                          \
            .inputs = {INPUT_DEFAULT, INPUT_DEFAULT, PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3)}, \
            .outputs = {OUTPUT_DEFAULT},                                                         \
            .constructor = broadcastOpConstructor};

// Arithmetic with activation.
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(ADD, V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(MUL, V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(SUB, V1_1, Type::TENSOR_FLOAT32);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(DIV, V1_1, Type::TENSOR_FLOAT32);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(ADD, V1_2, Type::TENSOR_FLOAT16);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(MUL, V1_2, Type::TENSOR_FLOAT16);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(SUB, V1_2, Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM);
DEFINE_BROADCAST_WITH_ACT_SIGNATURE(DIV, V1_2, Type::TENSOR_FLOAT16);

// For broadcast ops with output of the same data type as inputs.
#define DEFINE_BROADCAST_SIGNATURE(op, ver, ...)                                     \
    DEFINE_OPERATION_SIGNATURE(op##_##ver){.opType = ANEURALNETWORKS_##op,           \
                                           .supportedDataTypes = {__VA_ARGS__},      \
                                           .supportedRanks = {1, 2, 3, 4, 5},        \
                                           .version = HalVersion::ver,               \
                                           .inputs = {INPUT_DEFAULT, INPUT_DEFAULT}, \
                                           .outputs = {OUTPUT_DEFAULT},              \
                                           .constructor = broadcastOpConstructor};

// Arithmetic without activation.
DEFINE_BROADCAST_SIGNATURE(POW, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16);
DEFINE_BROADCAST_SIGNATURE(PRELU, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                           Type::TENSOR_QUANT8_ASYMM);
DEFINE_BROADCAST_SIGNATURE(MAXIMUM, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                           Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_INT32);
DEFINE_BROADCAST_SIGNATURE(MINIMUM, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                           Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_INT32);

// Logical
DEFINE_BROADCAST_SIGNATURE(LOGICAL_AND, V1_2, Type::TENSOR_BOOL8);
DEFINE_BROADCAST_SIGNATURE(LOGICAL_OR, V1_2, Type::TENSOR_BOOL8);

// Comparisons
#define DEFINE_COMPARISON_SIGNATURE(op, ver, ...)                                         \
    DEFINE_OPERATION_SIGNATURE(op##_##ver){.opType = ANEURALNETWORKS_##op,                \
                                           .supportedDataTypes = {__VA_ARGS__},           \
                                           .supportedRanks = {1, 2, 3, 4},                \
                                           .version = HalVersion::ver,                    \
                                           .inputs = {INPUT_DEFAULT, INPUT_DEFAULT},      \
                                           .outputs = {OUTPUT_TYPED(Type::TENSOR_BOOL8)}, \
                                           .constructor = broadcastOpConstructor};

DEFINE_COMPARISON_SIGNATURE(EQUAL, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_BOOL8);
DEFINE_COMPARISON_SIGNATURE(GREATER, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_COMPARISON_SIGNATURE(GREATER_EQUAL, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_COMPARISON_SIGNATURE(LESS, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_COMPARISON_SIGNATURE(LESS_EQUAL, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_COMPARISON_SIGNATURE(NOT_EQUAL, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                            Type::TENSOR_INT32, Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_BOOL8);

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
