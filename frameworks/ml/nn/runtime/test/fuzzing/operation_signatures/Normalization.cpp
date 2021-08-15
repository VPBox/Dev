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

static void softmaxConstructor(Type dataType, uint32_t rank, RandomOperation* op) {
    sameDimensionOpConstructor(dataType, rank, op);
    // Generate value for "axis" parameter.
    if (op->inputs.size() > 2) {
        op->inputs[2]->setScalarValue<int32_t>(getUniform<int32_t>(-rank, rank - 1));
    }
}

DEFINE_OPERATION_SIGNATURE(SOFTMAX_V1_0){
        .opType = ANEURALNETWORKS_SOFTMAX,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {2, 4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.1, 10.0)},
        .outputs = {OUTPUT_QUANT(1.f / 256, 0)},
        .constructor = softmaxConstructor};

DEFINE_OPERATION_SIGNATURE(SOFTMAX_V1_2){
        .opType = ANEURALNETWORKS_SOFTMAX,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 3},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.1, 10.0)},
        .outputs = {OUTPUT_QUANT(1.f / 256, 0)},
        .constructor = softmaxConstructor};

DEFINE_OPERATION_SIGNATURE(SOFTMAX_float16_V1_2){
        .opType = ANEURALNETWORKS_SOFTMAX,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.1, 10.0)},
        .outputs = {OUTPUT_QUANT(1.f / 256, 0)},
        .constructor = softmaxConstructor};

DEFINE_OPERATION_SIGNATURE(SOFTMAX_axis_V1_2){
        .opType = ANEURALNETWORKS_SOFTMAX,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.1, 10.0), PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_QUANT(1.f / 256, 0)},
        .constructor = softmaxConstructor};

static void l2normConstructor(Type dataType, uint32_t rank, RandomOperation* op) {
    sameDimensionOpConstructor(dataType, rank, op);
    // Generate value for "axis" parameter.
    if (op->inputs.size() > 1) {
        op->inputs[1]->setScalarValue<int32_t>(getUniform<int32_t>(-rank, rank - 1));
    }
    // L2_NORMALIZATION may produce NaN output values with all zero inputs. We should not connect
    // the output tensor to the input of another operation.
    op->outputs[0]->doNotConnect = true;
}

DEFINE_OPERATION_SIGNATURE(L2_NORMALIZATION_V1_0){.opType = ANEURALNETWORKS_L2_NORMALIZATION,
                                                  .supportedDataTypes = {Type::TENSOR_FLOAT32},
                                                  .supportedRanks = {4},
                                                  .version = HalVersion::V1_0,
                                                  .inputs = {INPUT_DEFAULT},
                                                  .outputs = {OUTPUT_DEFAULT},
                                                  .constructor = l2normConstructor};

DEFINE_OPERATION_SIGNATURE(L2_NORMALIZATION_V1_2){.opType = ANEURALNETWORKS_L2_NORMALIZATION,
                                                  .supportedDataTypes = {Type::TENSOR_FLOAT32},
                                                  .supportedRanks = {1, 2, 3},
                                                  .version = HalVersion::V1_2,
                                                  .inputs = {INPUT_DEFAULT},
                                                  .outputs = {OUTPUT_DEFAULT},
                                                  .constructor = l2normConstructor};

DEFINE_OPERATION_SIGNATURE(L2_NORMALIZATION_type_V1_2){
        .opType = ANEURALNETWORKS_L2_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT16, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT},
        .outputs = {OUTPUT_QUANT(1.f / 128, 128)},
        .constructor = l2normConstructor};

DEFINE_OPERATION_SIGNATURE(L2_NORMALIZATION_axis_V1_2){
        .opType = ANEURALNETWORKS_L2_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16,
                               Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_QUANT(1.f / 128, 128)},
        .constructor = l2normConstructor};

static void localResponseNormConstructor(Type dataType, uint32_t rank, RandomOperation* op) {
    sameDimensionOpConstructor(dataType, rank, op);
    // Generate value for "axis" parameter.
    if (op->inputs.size() > 5) {
        op->inputs[5]->setScalarValue<int32_t>(getUniform<int32_t>(-rank, rank - 1));
    }
}

DEFINE_OPERATION_SIGNATURE(LOCAL_RESPONSE_NORMALIZATION_V1_0){
        .opType = ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32},
        .supportedRanks = {4},
        .version = HalVersion::V1_0,
        .inputs =
                {
                        INPUT_DEFAULT,
                        PARAMETER_RANGE(Type::INT32, 1, 10),
                        PARAMETER_FLOAT_RANGE(0.0, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 1.0),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = localResponseNormConstructor};

DEFINE_OPERATION_SIGNATURE(LOCAL_RESPONSE_NORMALIZATION_V1_2){
        .opType = ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32},
        .supportedRanks = {1, 2, 3},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        PARAMETER_RANGE(Type::INT32, 1, 10),
                        PARAMETER_FLOAT_RANGE(0.0, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 1.0),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = localResponseNormConstructor};

DEFINE_OPERATION_SIGNATURE(LOCAL_RESPONSE_NORMALIZATION_float16_V1_2){
        .opType = ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        PARAMETER_RANGE(Type::INT32, 1, 10),
                        PARAMETER_FLOAT_RANGE(0.0, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 1.0),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = localResponseNormConstructor};

DEFINE_OPERATION_SIGNATURE(LOCAL_RESPONSE_NORMALIZATION_axis_V1_2){
        .opType = ANEURALNETWORKS_LOCAL_RESPONSE_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        PARAMETER_RANGE(Type::INT32, 1, 10),
                        PARAMETER_FLOAT_RANGE(0.0, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 10.0),
                        PARAMETER_FLOAT_RANGE(0.1, 1.0),
                        PARAMETER_NONE(Type::INT32),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = localResponseNormConstructor};

DEFINE_OPERATION_SIGNATURE(INSTANCE_NORMALIZATION_V1_2){
        .opType = ANEURALNETWORKS_INSTANCE_NORMALIZATION,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        PARAMETER_FLOAT_RANGE(0.1, 10.0),
                        PARAMETER_FLOAT_RANGE(-10.0, 10.0),
                        PARAMETER_FLOAT_RANGE(0.01, 1.0),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = sameShapeOpConstructor};

DEFINE_OPERATION_SIGNATURE(LOG_SOFTMAX_TENSOR_FLOAT32_V1_2){
        .opType = ANEURALNETWORKS_LOG_SOFTMAX,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.1, 10.0), PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = softmaxConstructor};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
