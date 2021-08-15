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

static void spaceToDepthConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    bool useNchw = false;
    if (op->inputs.size() > 2) useNchw = op->inputs[2]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int depthIndex = useNchw ? 1 : 3;

    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};
    int32_t blockSize = op->inputs[1]->value<int32_t>();
    auto outHeight = op->inputs[0]->dimensions[heightIndex].exactDiv(blockSize);
    auto outWidth = op->inputs[0]->dimensions[widthIndex].exactDiv(blockSize);
    auto outDepth = op->inputs[0]->dimensions[depthIndex] * (blockSize * blockSize);

    if (useNchw) {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], outDepth, outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], outHeight, outWidth, outDepth};
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(SPACE_TO_DEPTH_V1_0){
        .opType = ANEURALNETWORKS_SPACE_TO_DEPTH,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 5)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToDepthConstructor};

DEFINE_OPERATION_SIGNATURE(SPACE_TO_DEPTH_V1_2){
        .opType = ANEURALNETWORKS_SPACE_TO_DEPTH,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 5)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToDepthConstructor};

DEFINE_OPERATION_SIGNATURE(SPACE_TO_DEPTH_layout_V1_2){
        .opType = ANEURALNETWORKS_SPACE_TO_DEPTH,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 5),
                   PARAMETER_CHOICE(Type::BOOL, true, false)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToDepthConstructor};

static void depthToSpaceConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    bool useNchw = false;
    if (op->inputs.size() > 2) useNchw = op->inputs[2]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;
    int depthIndex = useNchw ? 1 : 3;

    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};
    int32_t blockSize = op->inputs[1]->value<int32_t>();
    auto outHeight = op->inputs[0]->dimensions[heightIndex] * blockSize;
    auto outWidth = op->inputs[0]->dimensions[widthIndex] * blockSize;
    auto outDepth = op->inputs[0]->dimensions[depthIndex].exactDiv(blockSize * blockSize);

    if (useNchw) {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], outDepth, outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {op->inputs[0]->dimensions[0], outHeight, outWidth, outDepth};
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(DEPTH_TO_SPACE_V1_0){
        .opType = ANEURALNETWORKS_DEPTH_TO_SPACE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = depthToSpaceConstructor};

DEFINE_OPERATION_SIGNATURE(DEPTH_TO_SPACE_V1_2){
        .opType = ANEURALNETWORKS_DEPTH_TO_SPACE,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = depthToSpaceConstructor};

DEFINE_OPERATION_SIGNATURE(DEPTH_TO_SPACE_layout_V1_2){
        .opType = ANEURALNETWORKS_DEPTH_TO_SPACE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 3),
                   PARAMETER_CHOICE(Type::BOOL, true, false)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = depthToSpaceConstructor};

static void reshapeConstructor(Type, uint32_t rank, RandomOperation* op) {
    setFreeDimensions(op->inputs[0], rank);
    op->inputs[1]->dimensions = {rank};
    op->inputs[1]->randomBuffer.resize(rank);
    RandomVariable numInputElements = 1;
    RandomVariable numOutputElements = 1;
    for (uint32_t i = 0; i < rank; i++) {
        op->inputs[1]->randomBuffer[i] = RandomVariableType::FREE;
        numInputElements = numInputElements * op->inputs[0]->dimensions[i];
        numOutputElements = numOutputElements * op->inputs[1]->randomBuffer[i];
    }
    numInputElements.setEqual(numOutputElements);
    op->outputs[0]->dimensions = op->inputs[1]->randomBuffer;
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(RESHAPE_V1_0){
        .opType = ANEURALNETWORKS_RESHAPE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_0,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = reshapeConstructor};

DEFINE_OPERATION_SIGNATURE(RESHAPE_V1_2){
        .opType = ANEURALNETWORKS_RESHAPE,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = reshapeConstructor};

static void batchToSpaceConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    bool useNchw = false;
    if (op->inputs.size() > 2) useNchw = op->inputs[2]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;

    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};
    int32_t blockHeight = op->inputs[1]->value<int32_t>(0);
    int32_t blockWidth = op->inputs[1]->value<int32_t>(1);
    auto outBatch = op->inputs[0]->dimensions[0].exactDiv(blockHeight * blockWidth);
    auto outHeight = op->inputs[0]->dimensions[heightIndex] * blockHeight;
    auto outWidth = op->inputs[0]->dimensions[widthIndex] * blockWidth;

    if (useNchw) {
        op->outputs[0]->dimensions = {outBatch, op->inputs[0]->dimensions[1], outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {outBatch, outHeight, outWidth, op->inputs[0]->dimensions[3]};
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(BATCH_TO_SPACE_ND_V1_1){
        .opType = ANEURALNETWORKS_BATCH_TO_SPACE_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_1,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = batchToSpaceConstructor};

DEFINE_OPERATION_SIGNATURE(BATCH_TO_SPACE_ND_V1_2){
        .opType = ANEURALNETWORKS_BATCH_TO_SPACE_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 3)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = batchToSpaceConstructor};

DEFINE_OPERATION_SIGNATURE(BATCH_TO_SPACE_ND_layout_V1_2){
        .opType = ANEURALNETWORKS_BATCH_TO_SPACE_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 3),
                   PARAMETER_CHOICE(Type::BOOL, true, false)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = batchToSpaceConstructor};

static void spaceToBatchConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);

    bool useNchw = false;
    if (op->inputs.size() > 3) useNchw = op->inputs[3]->value<bool8>();
    int heightIndex = useNchw ? 2 : 1;
    int widthIndex = useNchw ? 3 : 2;

    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};

    // Compute padded height and width.
    auto paddedHeight = op->inputs[0]->dimensions[heightIndex] +
                        (op->inputs[2]->value<int32_t>(0) + op->inputs[2]->value<int32_t>(1));
    auto paddedWidth = op->inputs[0]->dimensions[widthIndex] +
                       (op->inputs[2]->value<int32_t>(2) + op->inputs[2]->value<int32_t>(3));

    // blockHeight/blockWidth must be a divisor of padded height/width
    int32_t blockHeight = op->inputs[1]->value<int32_t>(0);
    int32_t blockWidth = op->inputs[1]->value<int32_t>(1);
    auto outBatch = op->inputs[0]->dimensions[0] * (blockHeight * blockWidth);
    auto outHeight = paddedHeight.exactDiv(blockHeight);
    auto outWidth = paddedWidth.exactDiv(blockWidth);

    if (useNchw) {
        op->outputs[0]->dimensions = {outBatch, op->inputs[0]->dimensions[1], outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {outBatch, outHeight, outWidth, op->inputs[0]->dimensions[3]};
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

// The paddings tensor in SPACE_TOBATCH_ND, a [2, 2] tensor with value selected from [0, 10].
static const OperandSignature paddingTensor_SPACE_TO_BATCH_ND = {
        .type = RandomOperandType::CONST, .constructor = [](Type, uint32_t, RandomOperand* op) {
            op->dataType = Type::TENSOR_INT32;
            op->dimensions = {2, 2};
            op->resizeBuffer<int32_t>(4);
            for (int i = 0; i < 4; i++) op->value<int32_t>(i) = getUniform<int32_t>(0, 10);
        }};

DEFINE_OPERATION_SIGNATURE(SPACE_TO_BATCH_ND_V1_1){
        .opType = ANEURALNETWORKS_SPACE_TO_BATCH_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_1,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 5),
                   paddingTensor_SPACE_TO_BATCH_ND},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToBatchConstructor};

DEFINE_OPERATION_SIGNATURE(SPACE_TO_BATCH_ND_V1_2){
        .opType = ANEURALNETWORKS_SPACE_TO_BATCH_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 5),
                   paddingTensor_SPACE_TO_BATCH_ND},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToBatchConstructor};

DEFINE_OPERATION_SIGNATURE(SPACE_TO_BATCH_ND_layout_V1_2){
        .opType = ANEURALNETWORKS_SPACE_TO_BATCH_ND,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT,
                   PARAMETER_VEC_RANGE(Type::TENSOR_INT32, /*len=*/2, /*range=*/1, 5),
                   paddingTensor_SPACE_TO_BATCH_ND, PARAMETER_CHOICE(Type::BOOL, true, false)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = spaceToBatchConstructor};

static void padConstructor(Type, uint32_t rank, RandomOperation* op) {
    setFreeDimensions(op->inputs[0], rank);
    op->inputs[1]->dimensions = {rank, 2};
    op->inputs[1]->resizeBuffer<int32_t>(rank * 2);
    op->outputs[0]->dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; i++) {
        int32_t left = getUniform<int32_t>(0, 5), right = getUniform<int32_t>(0, 5);
        op->inputs[1]->value<int32_t>(i * 2) = left;
        op->inputs[1]->value<int32_t>(i * 2 + 1) = right;
        op->outputs[0]->dimensions[i] = op->inputs[0]->dimensions[i] + (left + right);
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

static const OperandSignature paddingScalar_PAD_V2 = {
        .type = RandomOperandType::CONST,
        .constructor = [](Type dataType, uint32_t, RandomOperand* op) {
            switch (dataType) {
                case Type::TENSOR_FLOAT32:
                    op->dataType = Type::FLOAT32;
                    op->setScalarValue<float>(getUniform<float>(-10.0f, 10.0f));
                    break;
                case Type::TENSOR_FLOAT16:
                    op->dataType = Type::FLOAT16;
                    op->setScalarValue<_Float16>(getUniform<_Float16>(-10.0f, 10.0f));
                    break;
                case Type::TENSOR_QUANT8_ASYMM:
                    op->dataType = Type::INT32;
                    op->setScalarValue<int32_t>(getUniform<int32_t>(0, 255));
                    break;
                default:
                    NN_FUZZER_CHECK(false) << "Unsupported data type for PAD_V2";
            }
        }};

DEFINE_OPERATION_SIGNATURE(PAD_V1_1){
        .opType = ANEURALNETWORKS_PAD,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_1,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = padConstructor};

DEFINE_OPERATION_SIGNATURE(PAD_V1_2){.opType = ANEURALNETWORKS_PAD,
                                     .supportedDataTypes = {Type::TENSOR_FLOAT16},
                                     .supportedRanks = {1, 2, 3, 4},
                                     .version = HalVersion::V1_2,
                                     .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
                                     .outputs = {OUTPUT_DEFAULT},
                                     .constructor = padConstructor};

DEFINE_OPERATION_SIGNATURE(PAD_V2_V1_2){
        .opType = ANEURALNETWORKS_PAD_V2,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32), paddingScalar_PAD_V2},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = padConstructor};

static void transposeConstructor(Type, uint32_t rank, RandomOperation* op) {
    // Create the permutation value by randomly shuffling a sequential array.
    std::vector<int32_t> permutation(rank);
    std::iota(permutation.begin(), permutation.end(), 0);
    randomShuffle(&permutation);
    op->inputs[1]->resizeBuffer<int32_t>(rank);
    std::copy(permutation.begin(), permutation.end(),
              reinterpret_cast<int32_t*>(op->inputs[1]->buffer.data()));

    setFreeDimensions(op->inputs[0], rank);
    op->inputs[1]->dimensions = {rank};
    op->outputs[0]->dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; i++) {
        op->outputs[0]->dimensions[i] = op->inputs[0]->dimensions[permutation[i]];
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

// TODO: Test the case when the second input is omitted.
DEFINE_OPERATION_SIGNATURE(TRANSPOSE_V1_1){
        .opType = ANEURALNETWORKS_TRANSPOSE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_1,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = transposeConstructor};

DEFINE_OPERATION_SIGNATURE(TRANSPOSE_V1_2){
        .opType = ANEURALNETWORKS_TRANSPOSE,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = transposeConstructor};

static void channelShuffleConstructor(Type dataType, uint32_t rank, RandomOperation* op) {
    sameShapeOpConstructor(dataType, rank, op);
    // The number of groups must be a divisor of the target axis size.
    int32_t axis = getUniform<int32_t>(-rank, rank - 1);
    op->inputs[2]->setScalarValue<int32_t>(axis);
    int32_t numGroups = op->inputs[1]->value<int32_t>();
    if (axis < 0) axis += rank;
    (op->inputs[0]->dimensions[axis] % numGroups).setEqual(0);
}

DEFINE_OPERATION_SIGNATURE(CHANNEL_SHUFFLE_V1_2){
        .opType = ANEURALNETWORKS_CHANNEL_SHUFFLE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM,
                               Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_RANGE(Type::INT32, 1, 5), PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = channelShuffleConstructor};

static void squeezeConstructor(Type, uint32_t rank, RandomOperation* op) {
    // A boolean array indicating whether each dimension is selected to be squeezed.
    bool squeeze[4] = {false, false, false, false};
    uint32_t numAxis = getUniform<int32_t>(1, 10);
    op->inputs[1]->dimensions = {numAxis};
    op->inputs[1]->resizeBuffer<int32_t>(numAxis);
    for (uint32_t i = 0; i < numAxis; i++) {
        // Generate values for the "axis" tensor.
        int32_t dim = getUniform<int32_t>(0, rank - 1);
        op->inputs[1]->value<int32_t>(i) = dim;
        squeeze[dim] = true;
    }

    op->inputs[0]->dimensions.resize(rank);
    for (uint32_t i = 0; i < rank; i++) {
        if (squeeze[i]) {
            op->inputs[0]->dimensions[i] = 1;
        } else {
            op->inputs[0]->dimensions[i] = RandomVariableType::FREE;
            op->outputs[0]->dimensions.emplace_back(op->inputs[0]->dimensions[i]);
        }
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

// TODO: Test the case when the second input is omitted.
DEFINE_OPERATION_SIGNATURE(SQUEEZE_V1_1){
        .opType = ANEURALNETWORKS_SQUEEZE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_1,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = squeezeConstructor};

DEFINE_OPERATION_SIGNATURE(SQUEEZE_V1_2){
        .opType = ANEURALNETWORKS_SQUEEZE,
        .supportedDataTypes = {Type::TENSOR_FLOAT16},
        .supportedRanks = {1, 2, 3, 4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = squeezeConstructor};

static void expandDimsConstructor(Type, uint32_t rank, RandomOperation* op) {
    // Generate values for the "axis" tensor.
    int32_t axis = getUniform<int32_t>(-rank - 1, rank);
    op->inputs[1]->setScalarValue<int32_t>(axis);
    if (axis < 0) axis += rank + 1;

    setFreeDimensions(op->inputs[0], rank);
    for (uint32_t i = 0; i < rank; i++) {
        if (i == static_cast<uint32_t>(axis)) {
            op->outputs[0]->dimensions.push_back(1);
        }
        op->outputs[0]->dimensions.push_back(op->inputs[0]->dimensions[i]);
    }
    if (rank == static_cast<uint32_t>(axis)) op->outputs[0]->dimensions.push_back(1);
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(EXPAND_DIMS_V1_2){
        .opType = ANEURALNETWORKS_EXPAND_DIMS,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16, Type::TENSOR_INT32,
                               Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4, 5},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = expandDimsConstructor};

static void tileConstructor(Type, uint32_t rank, RandomOperation* op) {
    setFreeDimensions(op->inputs[0], rank);
    op->outputs[0]->dimensions.resize(rank);
    op->inputs[1]->dimensions = {rank};
    op->inputs[1]->resizeBuffer<int32_t>(rank);
    for (uint32_t i = 0; i < rank; i++) {
        int32_t multiple = getUniform<int32_t>(1, 5);
        op->inputs[1]->value<int32_t>(i) = multiple;
        op->outputs[0]->dimensions[i] = op->inputs[0]->dimensions[i] * multiple;
    }
    setSameQuantization(op->outputs[0], op->inputs[0]);
}

DEFINE_OPERATION_SIGNATURE(TILE_V1_2){
        .opType = ANEURALNETWORKS_TILE,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_FLOAT16, Type::TENSOR_INT32,
                               Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {1, 2, 3, 4, 5},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, PARAMETER_NONE(Type::TENSOR_INT32)},
        .outputs = {OUTPUT_DEFAULT},
        .constructor = tileConstructor};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
