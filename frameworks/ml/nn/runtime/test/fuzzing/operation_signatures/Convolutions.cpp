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

static void conv2DExplicitConstructor(Type, uint32_t rank, HalVersion ver, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingLeft = op->inputs[3]->value<int32_t>();
    int32_t paddingRight = op->inputs[4]->value<int32_t>();
    int32_t paddingTop = op->inputs[5]->value<int32_t>();
    int32_t paddingBottom = op->inputs[6]->value<int32_t>();
    int32_t strideWidth = op->inputs[7]->value<int32_t>();
    int32_t strideHeight = op->inputs[8]->value<int32_t>();
    bool useNchw = false;
    int32_t dilationWidth = 1, dilationHeight = 1;
    if (op->inputs.size() > 10) {
        useNchw = op->inputs[10]->value<bool8>();
        if (op->inputs.size() > 11) {
            dilationWidth = op->inputs[11]->value<int32_t>();
            dilationHeight = op->inputs[12]->value<int32_t>();
        }
    }
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [channel_out, height_flt, width_flt, channel_in]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, op->inputs[0]->dimensions[channelIndex]};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height
    explicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, dilationHeight, paddingTop, paddingBottom,
                    &op->outputs[0]->dimensions[heightIndex]);

    // width
    explicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, dilationWidth, paddingLeft, paddingRight,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/ver == HalVersion::V1_0, op);
}

static void conv2DImplicitConstructor(Type, uint32_t rank, HalVersion ver, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingScheme = op->inputs[3]->value<int32_t>();
    int32_t strideWidth = op->inputs[4]->value<int32_t>();
    int32_t strideHeight = op->inputs[5]->value<int32_t>();
    bool useNchw = false;
    int32_t dilationWidth = 1, dilationHeight = 1;
    if (op->inputs.size() > 7) {
        useNchw = op->inputs[7]->value<bool8>();
        if (op->inputs.size() > 8) {
            dilationWidth = op->inputs[8]->value<int32_t>();
            dilationHeight = op->inputs[9]->value<int32_t>();
        }
    }
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [channel_out, height_flt, width_flt, channel_in]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, op->inputs[0]->dimensions[channelIndex]};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height and width
    implicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, dilationHeight, paddingScheme,
                    &op->outputs[0]->dimensions[heightIndex]);
    implicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, dilationWidth, paddingScheme,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/ver == HalVersion::V1_0, op);
}

#define DEFINE_CONV_2D_SIGNATURE(ver, ...)                                                     \
    DEFINE_OPERATION_SIGNATURE(CONV_2D_explicit_##ver){                                        \
            .opType = ANEURALNETWORKS_CONV_2D,                                                 \
            .supportedDataTypes = {__VA_ARGS__},                                               \
            .supportedRanks = {4},                                                             \
            .version = HalVersion::ver,                                                        \
            .inputs =                                                                          \
                    {                                                                          \
                            INPUT_DEFAULT,                                                     \
                            INPUT_DEFAULT,                                                     \
                            INPUT_BIAS,                                                        \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),                         \
                    },                                                                         \
            .outputs = {OUTPUT_DEFAULT},                                                       \
            .constructor = std::bind(conv2DExplicitConstructor, _1, _2, HalVersion::ver, _3)}; \
    DEFINE_OPERATION_SIGNATURE(CONV_2D_implicit_##ver){                                        \
            .opType = ANEURALNETWORKS_CONV_2D,                                                 \
            .supportedDataTypes = {__VA_ARGS__},                                               \
            .supportedRanks = {4},                                                             \
            .version = HalVersion::ver,                                                        \
            .inputs =                                                                          \
                    {                                                                          \
                            INPUT_DEFAULT,                                                     \
                            INPUT_DEFAULT,                                                     \
                            INPUT_BIAS,                                                        \
                            PARAMETER_CHOICE(Type::INT32, 1, 2),                               \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),                         \
                    },                                                                         \
            .outputs = {OUTPUT_DEFAULT},                                                       \
            .constructor = std::bind(conv2DImplicitConstructor, _1, _2, HalVersion::ver, _3)};

DEFINE_CONV_2D_SIGNATURE(V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_CONV_2D_SIGNATURE(V1_2, Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM);

DEFINE_OPERATION_SIGNATURE(CONV_2D_explicit_layout_V1_2){
        .opType = ANEURALNETWORKS_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(conv2DExplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(CONV_2D_implicit_layout_V1_2){
        .opType = ANEURALNETWORKS_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(conv2DImplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(CONV_2D_explicit_dilation_V1_2){
        .opType = ANEURALNETWORKS_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(conv2DExplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(CONV_2D_implicit_dilation_V1_2){
        .opType = ANEURALNETWORKS_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(conv2DImplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

static void depthwiseConv2DExplicitConstructor(Type, uint32_t rank, HalVersion ver,
                                               RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingLeft = op->inputs[3]->value<int32_t>();
    int32_t paddingRight = op->inputs[4]->value<int32_t>();
    int32_t paddingTop = op->inputs[5]->value<int32_t>();
    int32_t paddingBottom = op->inputs[6]->value<int32_t>();
    int32_t strideWidth = op->inputs[7]->value<int32_t>();
    int32_t strideHeight = op->inputs[8]->value<int32_t>();
    bool useNchw = false;
    int32_t dilationWidth = 1, dilationHeight = 1;
    if (op->inputs.size() > 11) {
        useNchw = op->inputs[11]->value<bool8>();
        if (op->inputs.size() > 12) {
            dilationWidth = op->inputs[12]->value<int32_t>();
            dilationHeight = op->inputs[13]->value<int32_t>();
        }
    }
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [1, height_flt, width_flt, channel_out]
    RandomVariable channelOut =
            op->inputs[9]->value<RandomVariable>() * op->inputs[0]->dimensions[channelIndex];
    op->inputs[1]->dimensions = {1, RandomVariableType::FREE, RandomVariableType::FREE, channelOut};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {channelOut};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = channelOut;

    // height
    explicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, dilationHeight, paddingTop, paddingBottom,
                    &op->outputs[0]->dimensions[heightIndex]);

    // width
    explicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, dilationWidth, paddingLeft, paddingRight,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/ver == HalVersion::V1_0, op);
}

static void depthwiseConv2DImplicitConstructor(Type, uint32_t rank, HalVersion ver,
                                               RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingScheme = op->inputs[3]->value<int32_t>();
    int32_t strideWidth = op->inputs[4]->value<int32_t>();
    int32_t strideHeight = op->inputs[5]->value<int32_t>();
    bool useNchw = false;
    int32_t dilationWidth = 1, dilationHeight = 1;
    if (op->inputs.size() > 8) {
        useNchw = op->inputs[8]->value<bool8>();
        if (op->inputs.size() > 9) {
            dilationWidth = op->inputs[9]->value<int32_t>();
            dilationHeight = op->inputs[10]->value<int32_t>();
        }
    }
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [1, height_flt, width_flt, channel_out]
    RandomVariable channelOut =
            op->inputs[6]->value<RandomVariable>() * op->inputs[0]->dimensions[channelIndex];
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, channelOut};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {channelOut};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = channelOut;

    // height and width
    implicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, dilationHeight, paddingScheme,
                    &op->outputs[0]->dimensions[heightIndex]);
    implicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, dilationWidth, paddingScheme,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/ver == HalVersion::V1_0, op);
}

#define DEFINE_DEPTHWISE_CONV_2D_SIGNATURE(ver, ...)                                             \
    DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_explicit_##ver){                                \
            .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,                                         \
            .supportedDataTypes = {__VA_ARGS__},                                                 \
            .supportedRanks = {4},                                                               \
            .version = HalVersion::ver,                                                          \
            .inputs =                                                                            \
                    {                                                                            \
                            INPUT_DEFAULT,                                                       \
                            INPUT_DEFAULT,                                                       \
                            INPUT_BIAS,                                                          \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            RANDOM_INT_RANGE(1, 5),                                              \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),                           \
                    },                                                                           \
            .outputs = {OUTPUT_DEFAULT},                                                         \
            .constructor =                                                                       \
                    std::bind(depthwiseConv2DExplicitConstructor, _1, _2, HalVersion::ver, _3)}; \
    DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_implicit_##ver){                                \
            .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,                                         \
            .supportedDataTypes = {__VA_ARGS__},                                                 \
            .supportedRanks = {4},                                                               \
            .version = HalVersion::ver,                                                          \
            .inputs =                                                                            \
                    {                                                                            \
                            INPUT_DEFAULT,                                                       \
                            INPUT_DEFAULT,                                                       \
                            INPUT_BIAS,                                                          \
                            PARAMETER_CHOICE(Type::INT32, 1, 2),                                 \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            PARAMETER_RANGE(Type::INT32, 1, 3),                                  \
                            RANDOM_INT_RANGE(1, 5),                                              \
                            PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),                           \
                    },                                                                           \
            .outputs = {OUTPUT_DEFAULT},                                                         \
            .constructor =                                                                       \
                    std::bind(depthwiseConv2DImplicitConstructor, _1, _2, HalVersion::ver, _3)};

DEFINE_DEPTHWISE_CONV_2D_SIGNATURE(V1_0, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM);
DEFINE_DEPTHWISE_CONV_2D_SIGNATURE(V1_2, Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM);

DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_explicit_layout_V1_2){
        .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(depthwiseConv2DExplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_implicit_layout_V1_2){
        .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(depthwiseConv2DImplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_explicit_dilation_V1_2){
        .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(depthwiseConv2DExplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

DEFINE_OPERATION_SIGNATURE(DEPTHWISE_CONV_2D_implicit_dilation_V1_2){
        .opType = ANEURALNETWORKS_DEPTHWISE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = std::bind(depthwiseConv2DImplicitConstructor, _1, _2, HalVersion::V1_2, _3)};

static void groupedConv2DExplicitConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingLeft = op->inputs[3]->value<int32_t>();
    int32_t paddingRight = op->inputs[4]->value<int32_t>();
    int32_t paddingTop = op->inputs[5]->value<int32_t>();
    int32_t paddingBottom = op->inputs[6]->value<int32_t>();
    int32_t strideWidth = op->inputs[7]->value<int32_t>();
    int32_t strideHeight = op->inputs[8]->value<int32_t>();
    bool useNchw = op->inputs[11]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    RandomVariable numGroups = op->inputs[9]->value<RandomVariable>();
    RandomVariable channelGroup = RandomVariableType::FREE;
    if (useNchw) {
        op->inputs[0]->dimensions = {RandomVariableType::FREE, numGroups * channelGroup,
                                     RandomVariableType::FREE, RandomVariableType::FREE};
    } else {
        op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                     RandomVariableType::FREE, numGroups * channelGroup};
    }

    // Filter, [channel_out, height_flt, width_flt, channel_group]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, channelGroup};
    // channel_out must be divisible by num_groups.
    (op->inputs[1]->dimensions[0] % numGroups).setEqual(0);

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height
    explicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, /*dilation=*/1, paddingTop, paddingBottom,
                    &op->outputs[0]->dimensions[heightIndex]);

    // width
    explicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, /*dilation=*/1, paddingLeft, paddingRight,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/false, op);
}

static void groupedConv2DImplicitConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingScheme = op->inputs[3]->value<int32_t>();
    int32_t strideWidth = op->inputs[4]->value<int32_t>();
    int32_t strideHeight = op->inputs[5]->value<int32_t>();
    bool useNchw = op->inputs[8]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    RandomVariable numGroups = op->inputs[6]->value<RandomVariable>();
    RandomVariable channelGroup = RandomVariableType::FREE;
    if (useNchw) {
        op->inputs[0]->dimensions = {RandomVariableType::FREE, numGroups * channelGroup,
                                     RandomVariableType::FREE, RandomVariableType::FREE};
    } else {
        op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                     RandomVariableType::FREE, numGroups * channelGroup};
    }

    // Filter, [channel_out, height_flt, width_flt, channel_group]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, channelGroup};
    // channel_out must be divisible by num_groups.
    (op->inputs[1]->dimensions[0] % numGroups).setEqual(0);

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height and width
    implicitPadding(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                    strideHeight, /*dilation=*/1, paddingScheme,
                    &op->outputs[0]->dimensions[heightIndex]);
    implicitPadding(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                    strideWidth, /*dilation=*/1, paddingScheme,
                    &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/false, op);
}

DEFINE_OPERATION_SIGNATURE(GROUPED_CONV_2D_explicit_V1_2){
        .opType = ANEURALNETWORKS_GROUPED_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = groupedConv2DExplicitConstructor};

DEFINE_OPERATION_SIGNATURE(GROUPED_CONV_2D_implicit_V1_2){
        .opType = ANEURALNETWORKS_GROUPED_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        RANDOM_INT_RANGE(1, 5),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = groupedConv2DImplicitConstructor};

static void transposeConv2DExplicitConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingLeft = op->inputs[3]->value<int32_t>();
    int32_t paddingRight = op->inputs[4]->value<int32_t>();
    int32_t paddingTop = op->inputs[5]->value<int32_t>();
    int32_t paddingBottom = op->inputs[6]->value<int32_t>();
    int32_t strideWidth = op->inputs[7]->value<int32_t>();
    int32_t strideHeight = op->inputs[8]->value<int32_t>();
    bool useNchw = op->inputs[10]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [channel_out, height_flt, width_flt, channel_in]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, op->inputs[0]->dimensions[channelIndex]};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height
    explicitPaddingTranspose(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                             strideHeight, paddingTop, paddingBottom,
                             &op->outputs[0]->dimensions[heightIndex]);

    // width
    explicitPaddingTranspose(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                             strideWidth, paddingLeft, paddingRight,
                             &op->outputs[0]->dimensions[widthIndex]);

    setConvFCScale(/*applyOutputScaleBound=*/false, op);
}

static void transposeConv2DImplicitConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    // Parameters
    int32_t paddingScheme = op->inputs[4]->value<int32_t>();
    int32_t strideWidth = op->inputs[5]->value<int32_t>();
    int32_t strideHeight = op->inputs[6]->value<int32_t>();
    bool useNchw = op->inputs[8]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int channelIndex = useNchw ? 1 : 3;

    // Input, [batch, height_in, width_in, channel_in]
    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Filter, [channel_out, height_flt, width_flt, channel_in]
    op->inputs[1]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, op->inputs[0]->dimensions[channelIndex]};

    // Bias, [channel_out]
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};

    // Output, [batch, height_out, width_out, channel_out]
    op->outputs[0]->dimensions.resize(4);

    // batch and channel
    op->outputs[0]->dimensions[0] = op->inputs[0]->dimensions[0];
    op->outputs[0]->dimensions[channelIndex] = op->inputs[1]->dimensions[0];

    // height and width
    implicitPaddingTranspose(op->inputs[0]->dimensions[heightIndex], op->inputs[1]->dimensions[1],
                             strideHeight, paddingScheme, &op->outputs[0]->dimensions[heightIndex]);
    implicitPaddingTranspose(op->inputs[0]->dimensions[widthIndex], op->inputs[1]->dimensions[2],
                             strideWidth, paddingScheme, &op->outputs[0]->dimensions[widthIndex]);
    op->inputs[3]->dimensions = {4};
    op->inputs[3]->randomBuffer = op->outputs[0]->dimensions;

    setConvFCScale(/*applyOutputScaleBound=*/false, op);
}

DEFINE_OPERATION_SIGNATURE(TRANSPOSE_CONV_2D_explicit_V1_2){
        .opType = ANEURALNETWORKS_TRANSPOSE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = transposeConv2DExplicitConstructor};

DEFINE_OPERATION_SIGNATURE(TRANSPOSE_CONV_2D_implicit_V1_2){
        .opType = ANEURALNETWORKS_TRANSPOSE_CONV_2D,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        INPUT_DEFAULT,
                        INPUT_BIAS,
                        PARAMETER_NONE(Type::TENSOR_INT32),
                        PARAMETER_CHOICE(Type::INT32, 1, 2),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_RANGE(Type::INT32, 1, 3),
                        PARAMETER_CHOICE(Type::INT32, 0, 1, 2, 3),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = transposeConv2DImplicitConstructor};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
