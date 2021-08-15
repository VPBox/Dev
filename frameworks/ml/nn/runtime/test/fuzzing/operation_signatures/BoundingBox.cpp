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

static void roiTensorConstructor(Type dataType, uint32_t, RandomOperand* op) {
    op->dataType = dataType;
    if (dataType == Type::TENSOR_QUANT8_ASYMM) {
        op->dataType = Type::TENSOR_QUANT16_ASYMM;
        op->scale = 0.125f;
        op->zeroPoint = 0;
    }
}

// TODO: Have a version that makes roi tensor an input.
static const OperandSignature kInputRoiTensor = {.type = RandomOperandType::CONST,
                                                 .constructor = roiTensorConstructor};
static const OperandSignature kOutputRoiTensor = {.type = RandomOperandType::OUTPUT,
                                                  .constructor = roiTensorConstructor};

static void roiConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);
    bool useNchw;
    if (op->opType == ANEURALNETWORKS_ROI_ALIGN) {
        useNchw = op->inputs[9]->value<bool8>();
    } else {
        useNchw = op->inputs[7]->value<bool8>();
    }

    op->inputs[0]->dimensions = {RandomVariableType::FREE, RandomVariableType::FREE,
                                 RandomVariableType::FREE, RandomVariableType::FREE};
    op->inputs[1]->dimensions = {RandomVariableType::FREE, 4};
    op->inputs[2]->dimensions = {op->inputs[1]->dimensions[0]};
    auto outBatch = op->inputs[1]->dimensions[0];
    auto outDepth = op->inputs[0]->dimensions[useNchw ? 1 : 3];
    auto outHeight = op->inputs[3]->value<RandomVariable>();
    auto outWidth = op->inputs[4]->value<RandomVariable>();
    if (useNchw) {
        op->outputs[0]->dimensions = {outBatch, outDepth, outHeight, outWidth};
    } else {
        op->outputs[0]->dimensions = {outBatch, outHeight, outWidth, outDepth};
    }

    if (op->opType == ANEURALNETWORKS_ROI_POOLING) {
        setSameQuantization(op->outputs[0], op->inputs[0]);
    }
}

template <typename T>
inline void fillRoiTensor(uint32_t numRois, T maxH, T maxW, RandomOperand* op) {
    for (uint32_t i = 0; i < numRois; i++) {
        T low = getUniform<T>(0, maxW);
        op->value<T>(i * 4) = low;
        op->value<T>(i * 4 + 2) = getUniform<T>(low, maxW);
        low = getUniform<T>(0, maxH);
        op->value<T>(i * 4 + 1) = low;
        op->value<T>(i * 4 + 3) = getUniform<T>(low, maxH);
    }
}

static void roiFinalizer(RandomOperation* op) {
    bool useNchw;
    if (op->opType == ANEURALNETWORKS_ROI_ALIGN) {
        useNchw = op->inputs[9]->value<bool8>();
    } else {
        useNchw = op->inputs[7]->value<bool8>();
    }

    uint32_t batch = op->inputs[0]->dimensions[0].getValue();
    uint32_t height = op->inputs[0]->dimensions[useNchw ? 2 : 1].getValue();
    uint32_t width = op->inputs[0]->dimensions[useNchw ? 3 : 2].getValue();
    uint32_t numRois = op->inputs[1]->dimensions[0].getValue();
    // Fill values to the roi tensor with format [x1, y1, x2, y2].
    switch (op->inputs[1]->dataType) {
        case Type::TENSOR_FLOAT32: {
            float maxH = static_cast<float>(height) * op->inputs[5]->value<float>();
            float maxW = static_cast<float>(width) * op->inputs[6]->value<float>();
            fillRoiTensor<float>(numRois, maxH, maxW, op->inputs[1].get());
        } break;
        case Type::TENSOR_QUANT16_ASYMM: {
            uint16_t maxH = static_cast<float>(height) * op->inputs[5]->value<float>();
            uint16_t maxW = static_cast<float>(width) * op->inputs[6]->value<float>();
            fillRoiTensor<uint16_t>(numRois, maxH, maxW, op->inputs[1].get());

        } break;
        default:
            NN_FUZZER_CHECK(false) << "Unsupported data type.";
    }

    // Fill values to the batch index tensor.
    std::vector<int32_t> batchIndex(numRois);
    for (uint32_t i = 0; i < numRois; i++) batchIndex[i] = getUniform<int32_t>(0, batch - 1);
    // Same batches are grouped together.
    std::sort(batchIndex.begin(), batchIndex.end());
    for (uint32_t i = 0; i < numRois; i++) op->inputs[2]->value<int32_t>(i) = batchIndex[i];
}

// Type::TENSOR_FLOAT16 is intentionally excluded for all bounding box ops because
// 1. It has limited precision for compuation on bounding box indices, which will lead to poor
//    accuracy evaluation.
// 2. There is no actual graph that uses this data type on bounding boxes.

DEFINE_OPERATION_SIGNATURE(ROI_ALIGN_V1_2){
        .opType = ANEURALNETWORKS_ROI_ALIGN,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        kInputRoiTensor,
                        PARAMETER_NONE(Type::TENSOR_INT32),
                        RANDOM_INT_FREE,
                        RANDOM_INT_FREE,
                        PARAMETER_FLOAT_RANGE(0.1f, 10.0f),
                        PARAMETER_FLOAT_RANGE(0.1f, 10.0f),
                        PARAMETER_RANGE(Type::INT32, 0, 10),
                        PARAMETER_RANGE(Type::INT32, 0, 10),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = roiConstructor,
        .finalizer = roiFinalizer};

DEFINE_OPERATION_SIGNATURE(ROI_POOLING_V1_2){
        .opType = ANEURALNETWORKS_ROI_POOLING,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs =
                {
                        INPUT_DEFAULT,
                        kInputRoiTensor,
                        PARAMETER_NONE(Type::TENSOR_INT32),
                        RANDOM_INT_FREE,
                        RANDOM_INT_FREE,
                        PARAMETER_FLOAT_RANGE(0.1f, 10.0f),
                        PARAMETER_FLOAT_RANGE(0.1f, 10.0f),
                        PARAMETER_CHOICE(Type::BOOL, true, false),
                },
        .outputs = {OUTPUT_DEFAULT},
        .constructor = roiConstructor,
        .finalizer = roiFinalizer};

static void heatmapMaxKeypointConstructor(Type, uint32_t rank, RandomOperation* op) {
    NN_FUZZER_CHECK(rank == 4);
    bool useNchw = op->inputs[2]->value<bool8>();
    RandomVariable heatmapSize = RandomVariableType::FREE;
    RandomVariable numRois = RandomVariableType::FREE;
    RandomVariable numKeypoints = RandomVariableType::FREE;
    heatmapSize.setRange(2, kInvalidValue);

    if (useNchw) {
        op->inputs[0]->dimensions = {numRois, numKeypoints, heatmapSize, heatmapSize};
    } else {
        op->inputs[0]->dimensions = {numRois, heatmapSize, heatmapSize, numKeypoints};
    }
    op->inputs[1]->dimensions = {numRois, 4};
    op->outputs[0]->dimensions = {numRois, numKeypoints};
    op->outputs[1]->dimensions = {numRois, numKeypoints, 2};

    // TODO: This is an ugly fix due to the limitation of the current generator that can not handle
    // the dimension dependency within an input. Without the following line, most of the generated
    // HEATMAP_MAX_KEYPOINT graphs will be invalid and triggers retry.
    RandomVariableNetwork::get()->addDimensionProd(
            {numRois, numKeypoints, heatmapSize * heatmapSize});
}

static void heatmapMaxKeypointFinalizer(RandomOperation* op) {
    uint32_t numRois = op->inputs[0]->dimensions[0].getValue();
    uint32_t heatmapSize = op->inputs[0]->dimensions[2].getValue();
    // Fill values to the roi tensor with format [x1, y1, x2, y2].
    switch (op->inputs[1]->dataType) {
        case Type::TENSOR_FLOAT32: {
            float maxSize = heatmapSize;
            fillRoiTensor<float>(numRois, maxSize, maxSize, op->inputs[1].get());
        } break;
        case Type::TENSOR_QUANT16_ASYMM: {
            uint16_t maxSize = static_cast<uint16_t>(heatmapSize * 8);
            fillRoiTensor<uint16_t>(numRois, maxSize, maxSize, op->inputs[1].get());
        } break;
        default:
            NN_FUZZER_CHECK(false) << "Unsupported data type.";
    }
}

DEFINE_OPERATION_SIGNATURE(HEATMAP_MAX_KEYPOINT_V1_2){
        .opType = ANEURALNETWORKS_HEATMAP_MAX_KEYPOINT,
        .supportedDataTypes = {Type::TENSOR_FLOAT32, Type::TENSOR_QUANT8_ASYMM},
        .supportedRanks = {4},
        .version = HalVersion::V1_2,
        .inputs = {INPUT_DEFAULT, kInputRoiTensor, PARAMETER_CHOICE(Type::BOOL, true, false)},
        .outputs = {OUTPUT_DEFAULT, kOutputRoiTensor},
        .constructor = heatmapMaxKeypointConstructor,
        .finalizer = heatmapMaxKeypointFinalizer};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
