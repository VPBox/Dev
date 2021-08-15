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

static void resizeOpConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);
    setFreeDimensions(op->inputs[0], rank);

    bool useNchw = op->inputs.size() > 3 ? static_cast<bool>(op->inputs[3]->value<bool8>()) : false;
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;

    // Compute output height and width.
    RandomVariable outHeight, outWidth;
    switch (op->inputs[1]->dataType) {
        // Resize by shape.
        case Type::INT32:
            outWidth = op->inputs[1]->value<RandomVariable>();
            outHeight = op->inputs[2]->value<RandomVariable>();
            break;
        // Resize by scale.
        case Type::FLOAT32:
            outWidth = op->inputs[0]->dimensions[widthIndex] * op->inputs[1]->value<float>();
            outHeight = op->inputs[0]->dimensions[heightIndex] * op->inputs[2]->value<float>();
            break;
        case Type::FLOAT16:
            outWidth = op->inputs[0]->dimensions[widthIndex] *
                       static_cast<float>(op->inputs[1]->value<_Float16>());
            outHeight = op->inputs[0]->dimensions[heightIndex] *
                        static_cast<float>(op->inputs[2]->value<_Float16>());
            break;
        default:
            NN_FUZZER_CHECK(false) << "Unsupported data type.";
    }

    if (useNchw) {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], op->inputs[0]->dimensions[1],
                                      outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], outHeight, outWidth,
                                      op->inputs[0]->dimensions[3]};
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

#define DEFINE_RESIZE_WITHOUT_LAYOUT_SIGNATURE(op, ver, ...)             \
    DEFINE_OPERATION_SIGNATURE(op##_no_layout_##ver){                    \
            .opType = ANEURALNETWORKS_##op,                              \
            .supportedDataTypes = {__VA_ARGS__},                         \
            .supportedRanks = {4},                                       \
            .version = HalVersion::ver,                                  \
            .inputs = {INPUT_DEFAULT, RANDOM_INT_FREE, RANDOM_INT_FREE}, \
            .outputs = {OUTPUT_DEFAULT},                                 \
            .constructor = resizeOpConstructor};

DEFINE_RESIZE_WITHOUT_LAYOUT_SIGNATURE(RESIZE_BILINEAR, V1_0, Type::TENSOR_FLOAT32);
DEFINE_RESIZE_WITHOUT_LAYOUT_SIGNATURE(RESIZE_BILINEAR, V1_2, Type::TENSOR_QUANT8_ASYMM,
                                       Type::TENSOR_FLOAT16);

#define DEFINE_RESIZE_OP_SIGNATURE(op, ver, ...)                        \
    DEFINE_OPERATION_SIGNATURE(op##_shape_##ver){                       \
            .opType = ANEURALNETWORKS_##op,                             \
            .supportedDataTypes = {__VA_ARGS__},                        \
            .supportedRanks = {4},                                      \
            .version = HalVersion::ver,                                 \
            .inputs = {INPUT_DEFAULT, RANDOM_INT_FREE, RANDOM_INT_FREE, \
                       PARAMETER_CHOICE(Type::BOOL, false, true)},      \
            .outputs = {OUTPUT_DEFAULT},                                \
            .constructor = resizeOpConstructor};                        \
    DEFINE_OPERATION_SIGNATURE(op##_scale_##ver){                       \
            .opType = ANEURALNETWORKS_##op,                             \
            .supportedDataTypes = {__VA_ARGS__},                        \
            .supportedRanks = {4},                                      \
            .version = HalVersion::ver,                                 \
            .inputs = {INPUT_DEFAULT, PARAMETER_FLOAT_RANGE(0.2, 4.0),  \
                       PARAMETER_FLOAT_RANGE(0.2, 4.0),                 \
                       PARAMETER_CHOICE(Type::BOOL, false, true)},      \
            .outputs = {OUTPUT_DEFAULT},                                \
            .constructor = resizeOpConstructor};

DEFINE_RESIZE_OP_SIGNATURE(RESIZE_BILINEAR, V1_2, Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                           Type::TENSOR_FLOAT16);
DEFINE_RESIZE_OP_SIGNATURE(RESIZE_NEAREST_NEIGHBOR, V1_2, Type::TENSOR_FLOAT32,
                           Type::TENSOR_QUANT8_ASYMM, Type::TENSOR_FLOAT16);

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
