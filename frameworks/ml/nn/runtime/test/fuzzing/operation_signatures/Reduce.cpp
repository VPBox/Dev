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

static void reduceOpConstructor(Type, uint32_t rank, RandomOperation* op) {
    setFreeDimensions(op->inputs[0], rank);

    // A boolean array indicating whether each dimension is selected to be reduced.
    bool reduce[4] = {false, false, false, false};

    // Generate values for the "axis" tensor.
    uint32_t numAxis = getUniform<int32_t>(1, 10);
    op->inputs[1]->dimensions = {numAxis};
    op->inputs[1]->resizeBuffer<int32_t>(numAxis);
    for (uint32_t i = 0; i < numAxis; i++) {
        int32_t dim = getUniform<int32_t>(-rank, rank - 1);
        op->inputs[1]->value<int32_t>(i) = dim;
        reduce[dim < 0 ? dim + rank : dim] = true;
    }

    // This scalar may have two types: in MEAN it is INT32, in REDUCE_* it is BOOL
    bool keepDims;
    if (op->inputs[2]->dataType == Type::BOOL) {
        keepDims = op->inputs[2]->value<bool8>();
    } else {
        keepDims = op->inputs[2]->value<int32_t>() > 0;
    }

    for (uint32_t i = 0; i < rank; i++) {
        if (!reduce[i]) {
            op->outputs[0]->dimensions.emplace_back(op->inputs[0]->dimensions[i]);
        } else if (keepDims) {
            op->outputs[0]->dimensions.emplace_back(1);
        }
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);

    // REDUCE_PROD may produce Inf output values. We should not connect the output tensor to the
    // input of another operation.
    if (op->opType == ANEURALNETWORKS_REDUCE_PROD) {
        op->outputs[0]->doNotConnect = true;
    }
}

#define DEFINE_MEAN_SIGNATURE(ver, ...)                                   \
    DEFINE_OPERATION_SIGNATURE(MEAN_##ver){                               \
            .opType = ANEURALNETWORKS_MEAN,                               \
            .supportedDataTypes = {__VA_ARGS__},                          \
            .supportedRanks = {1, 2, 3, 4},                               \
            .version = HalVersion::ver,                                   \
            .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32), \
                       PARAMETER_CHOICE(Type::INT32, -100, 100)},         \
            .outputs = {OUTPUT_DEFAULT},                                  \
            .constructor = reduceOpConstructor};

DEFINE_MEAN_SIGNATURE(V1_1, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_MEAN_SIGNATURE(V1_2, Type::TENSOR_FLOAT16);

#define DEFINE_REDUCE_SIGNATURE(op, ver, ...)                             \
    DEFINE_OPERATION_SIGNATURE(op##_##ver){                               \
            .opType = ANEURALNETWORKS_##op,                               \
            .supportedDataTypes = {__VA_ARGS__},                          \
            .supportedRanks = {1, 2, 3, 4},                               \
            .version = HalVersion::ver,                                   \
            .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32), \
                       PARAMETER_CHOICE(Type::BOOL, true, false)},        \
            .outputs = {OUTPUT_DEFAULT},                                  \
            .constructor = reduceOpConstructor};

DEFINE_REDUCE_SIGNATURE(REDUCE_ALL, V1_2, Type::TENSOR_BOOL8);
DEFINE_REDUCE_SIGNATURE(REDUCE_ANY, V1_2, Type::TENSOR_BOOL8);
DEFINE_REDUCE_SIGNATURE(REDUCE_PROD, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16);
DEFINE_REDUCE_SIGNATURE(REDUCE_SUM, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16);
DEFINE_REDUCE_SIGNATURE(REDUCE_MAX, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                        Type::TENSOR_QUANT8_ASYMM);
DEFINE_REDUCE_SIGNATURE(REDUCE_MIN, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                        Type::TENSOR_QUANT8_ASYMM);

static void singleAxisReduceOpConstructor(Type, uint32_t rank, RandomOperation* op) {
    setFreeDimensions(op->inputs[0], rank);
    // "axis" must be in the range [-rank, rank).
    // Negative "axis" is used to specify axis from the end.
    int32_t axis = getUniform<int32_t>(-rank, rank - 1);
    op->inputs[1]->setScalarValue<int32_t>(axis);
    for (uint32_t i = 0; i < rank; i++) {
        if (i != static_cast<uint32_t>(axis) && i != axis + rank) {
            op->outputs[0]->dimensions.emplace_back(op->inputs[0]->dimensions[i]);
        }
    }
}

#define DEFINE_ARGMIN_MAX_SIGNATURE(op, ver, ...)                                                  \
    DEFINE_OPERATION_SIGNATURE(op##_##ver){                                                        \
            .opType = ANEURALNETWORKS_##op,                                                        \
            .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16, Type::TENSOR_INT32, \
                                   Type::TENSOR_QUANT8_ASYMM},                                     \
            .supportedRanks = {1, 2, 3, 4, 5},                                                     \
            .version = HalVersion::ver,                                                            \
            .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},                                \
            .outputs = {OUTPUT_TYPED(Type::TENSOR_INT32)},                                         \
            .constructor = singleAxisReduceOpConstructor};

DEFINE_ARGMIN_MAX_SIGNATURE(ARGMAX, V1_2);
DEFINE_ARGMIN_MAX_SIGNATURE(ARGMIN, V1_2);

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
