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

// For pooling ops with explicit padding.
static void poolingExplicitOpConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingLeft = op->inputs[1]->value<int32_t>();
    int32_t paddingRight = op->inputs[2]->value<int32_t>();
    int32_t paddingTop = op->inputs[3]->value<int32_t>();
    int32_t paddingBottom = op->inputs[4]->value<int32_t>();
    int32_t strideWidth = op->inputs[5]->value<int32_t>();
    int32_t strideHeight = op->inputs[6]->value<int32_t>();
    auto filterWidth = op->inputs[7]->value<RandomVariable>();
    auto filterHeight = op->inputs[8]->value<RandomVariable>();
    bool useNchw = false;
    if (op->inputs.size() > 10) useNchw = op->inputs[10]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Output, [batch, height_out, width_out, channel]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[0]->dimensions[channelIndex];

    // height
    explicitPadding(op->inputs[0]->dimensions[heightIndex], filterHeight, strideHeight,
                    /*dilation=*/1, paddingTop, paddingBottom,
                    &op->outputs[0]->dimensions[heightIndex]);

    // width
    explicitPadding(op->inputs[0]->dimensions[widthIndex], filterWidth, strideWidth, /*dilation=*/1,
                    paddingLeft, paddingRight, &op->outputs[0]->dimensions[widthIndex]);

    setSameQuantization(op->outputs[0], op->inputs[0]);
}

// For pooling ops with implicit padding.
static void poolingImplicitOpConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingScheme = op->inputs[1]->value<int32_t>();
    int32_t strideWidth = op->inputs[2]->value<int32_t>();
    int32_t strideHeight = op->inputs[3]->value<int32_t>();
    auto filterWidth = op->inputs[4]->value<RandomVariable>();
    auto filterHeight = op->inputs[5]->value<RandomVariable>();
    bool useNchw = false;
    if (op->inputs.size() > 7) useNchw = op->inputs[7]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Output, [batch, height_out, width_out, channel]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[0]->dimensions[channelIndex];

    // height and width
    implicitPadding(op->inputs[0]->dimensions[heightIndex], filterHeight, strideHeight,
                    /*dilation=*/1, paddingScheme, &op->outputs[0]->dimensions[heightIndex]);
    implicitPadding(op->inputs[0]->dimensions[widthIndex], filterWidth, strideWidth,
                    /*dilation=*/1, paddingScheme, &op->outputs[0]->dimensions[widthIndex]);

    setSameQuantization(op->outputs[0], op->inputs[0]);
}

#define DEFINE_POOLING_SIGNATURE(op, ver, ...)                         \
    DEFINE_OPERATION_SIGNATURE(op##_explicit_##ver){                   \
            .opType = ANEURALNETWORKS_##op,                            \
            .supportedDataTypes = {__VA_ARGS__},                       \
            .supportedRanks = {4},                                     \
            .version = HalVersion::ver,                                \
            .inputs =                                                  \
                    {                                                  \
                            INPUT_DEFAULT,                             \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            RANDOM_INT_RANGE(1, 4),                    \
                            RANDOM_INT_RANGE(1, 4),                    \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3), \
                    },                                                 \
            .outputs = {OUTPUT_DEFAULT},                               \
            .constructor = poolingExplicitOpConstructor};              \
    DEFINE_OPERATION_SIGNATURE(op##_implicit_##ver){                   \
            .opType = ANEURALNETWORKS_##op,                            \
            .supportedDataTypes = {__VA_ARGS__},                       \
            .supportedRanks = {4},                                     \
            .version = HalVersion::ver,                                \
            .inputs =                                                  \
                    {                                                  \
                            INPUT_DEFAULT,                             \
                            PARAMETER_CHOICE(Type::INT32, 1, 2),       \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            RANDOM_INT_RANGE(1, 4),                    \
                            RANDOM_INT_RANGE(1, 4),                    \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3), \
                    },                                                 \
            .outputs = {OUTPUT_DEFAULT},                               \
            .constructor = poolingImplicitOpConstructor};

DEFINE_POOLING_SIGNATURE(AVERAGE_POOL_2D, V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_POOLING_SIGNATURE(L2_POOL_2D, V1_0, Type::TENSOR_FLOAT32);
DEFINE_POOLING_SIGNATURE(MAX_POOL_2D, V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);

DEFINE_POOLING_SIGNATURE(AVERAGE_POOL_2D, V1_2, Type::TENSOR_FLOAT16);
DEFINE_POOLING_SIGNATURE(L2_POOL_2D, V1_2, Type::TENSOR_FLOAT16);
DEFINE_POOLING_SIGNATURE(MAX_POOL_2D, V1_2, Type::TENSOR_FLOAT16);

#define DEFINE_POOLING_WITH_LAYOUT_SIGNATURE(op, ver, ...)             \
    DEFINE_OPERATION_SIGNATURE(op##_explicit_layout_##ver){            \
            .opType = ANEURALNETWORKS_##op,                            \
            .supportedDataTypes = {__VA_ARGS__},                       \
            .supportedRanks = {4},                                     \
            .version = HalVersion::ver,                                \
            .inputs =                                                  \
                    {                                                  \
                            INPUT_DEFAULT,                             \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            RANDOM_INT_RANGE(1, 4),                    \
                            RANDOM_INT_RANGE(1, 4),                    \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3), \
                            PARAMETER_CHOICE(Type::BOOL, true, false), \
                    },                                                 \
            .outputs = {OUTPUT_DEFAULT},                               \
            .constructor = poolingExplicitOpConstructor};              \
    DEFINE_OPERATION_SIGNATURE(op##_implicit_layout_##ver){            \
            .opType = ANEURALNETWORKS_##op,                            \
            .supportedDataTypes = {__VA_ARGS__},                       \
            .supportedRanks = {4},                                     \
            .version = HalVersion::ver,                                \
            .inputs =                                                  \
                    {                                                  \
                            INPUT_DEFAULT,                             \
                            PARAMETER_CHOICE(Type::INT32, 1, 2),       \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),        \
                            RANDOM_INT_RANGE(1, 4),                    \
                            RANDOM_INT_RANGE(1, 4),                    \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3), \
                            PARAMETER_CHOICE(Type::BOOL, true, false), \
                    },                                                 \
            .outputs = {OUTPUT_DEFAULT},                               \
            .constructor = poolingImplicitOpConstructor};

DEFINE_POOLING_WITH_LAYOUT_SIGNATURE(AVERAGE_POOL_2D, V1_2, Type::TENSOR_FLOAT32,
                                     Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM);
DEFINE_POOLING_WITH_LAYOUT_SIGNATURE(L2_POOL_2D, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16);
DEFINE_POOLING_WITH_LAYOUT_SIGNATURE(MAX_POOL_2D, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                                     Type::TENSOR_QUANT8_ASYMM);

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
