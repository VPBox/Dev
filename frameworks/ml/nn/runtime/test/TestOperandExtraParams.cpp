/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "NeuralNetworksOEM.h"
#include "NeuralNetworksWrapper.h"
#ifndef NNTEST_ONLY_PUBLIC_API
#include "Utils.h"
#endif

#include <gtest/gtest.h>

namespace {

using namespace android::nn::wrapper;

class OperandExtraParamsTest : public ::testing::Test {
   protected:
    virtual void SetUp() {
        ::testing::Test::SetUp();
        ASSERT_EQ(ANeuralNetworksModel_create(&mModel), ANEURALNETWORKS_NO_ERROR);
        nextOperandIndex = 0;
    }
    virtual void TearDown() {
        ANeuralNetworksModel_free(mModel);
        ::testing::Test::TearDown();
    }

    static const uint32_t CHANNEL_DIM_SIZE = 4;

    ANeuralNetworksOperandType createOperand(int32_t dataType) {
        static uint32_t dims[4] = {1, 2, 3, CHANNEL_DIM_SIZE};
        switch (dataType) {
            case ANEURALNETWORKS_FLOAT32:
            case ANEURALNETWORKS_FLOAT16:
            case ANEURALNETWORKS_INT32:
            case ANEURALNETWORKS_UINT32:
            case ANEURALNETWORKS_BOOL:
            case ANEURALNETWORKS_OEM_SCALAR:
                return {.type = dataType,
                        .dimensionCount = 0,
                        .dimensions = nullptr,
                        .scale = 0.0f,
                        .zeroPoint = 0};
            case ANEURALNETWORKS_TENSOR_OEM_BYTE:
            case ANEURALNETWORKS_TENSOR_FLOAT32:
            case ANEURALNETWORKS_TENSOR_FLOAT16:
            case ANEURALNETWORKS_TENSOR_INT32:
            case ANEURALNETWORKS_TENSOR_BOOL8:
            case ANEURALNETWORKS_TENSOR_QUANT8_SYMM_PER_CHANNEL:
                return {.type = dataType,
                        .dimensionCount = 4,
                        .dimensions = dims,
                        .scale = 0.0f,
                        .zeroPoint = 0};
            case ANEURALNETWORKS_TENSOR_QUANT8_ASYMM:
                return {.type = dataType,
                        .dimensionCount = 4,
                        .dimensions = dims,
                        .scale = 1.0,
                        .zeroPoint = 128};
            case ANEURALNETWORKS_TENSOR_QUANT8_SYMM:
                return {.type = dataType,
                        .dimensionCount = 4,
                        .dimensions = dims,
                        .scale = 1.0,
                        .zeroPoint = 0};
            case ANEURALNETWORKS_TENSOR_QUANT16_SYMM:
                return {.type = dataType,
                        .dimensionCount = 4,
                        .dimensions = dims,
                        .scale = 1.0,
                        .zeroPoint = 0};
            case ANEURALNETWORKS_TENSOR_QUANT16_ASYMM:
                return {.type = dataType,
                        .dimensionCount = 4,
                        .dimensions = dims,
                        .scale = 1.0,
                        .zeroPoint = 32768};
            default:
                ADD_FAILURE();
                return {};
        }
    }

    ANeuralNetworksSymmPerChannelQuantParams createSymmPerChannelQuantParams() {
        static float scales[CHANNEL_DIM_SIZE] = {1.0, 2.0, 3.0, 4.0};
        return {
                .channelDim = 3,
                .scaleCount = CHANNEL_DIM_SIZE,
                .scales = scales,
        };
    }

    void testAddingWithSymmPerChannelQuantParams(int32_t dataType,
                                                 ANeuralNetworksSymmPerChannelQuantParams params,
                                                 bool expectExtraParamsSuccess) {
        ANeuralNetworksOperandType operandType = createOperand(dataType);
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &operandType), ANEURALNETWORKS_NO_ERROR);
        int operandIndex = nextOperandIndex++;
        EXPECT_EQ(ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(mModel, operandIndex,
                                                                           &params),
                  expectExtraParamsSuccess ? ANEURALNETWORKS_NO_ERROR : ANEURALNETWORKS_BAD_DATA);
    }

    ANeuralNetworksModel* mModel = nullptr;
    int nextOperandIndex = 0;
};

const uint32_t kOperandCodeNoExtraParams[]{
        ANEURALNETWORKS_FLOAT32,
        ANEURALNETWORKS_FLOAT16,
        ANEURALNETWORKS_INT32,
        ANEURALNETWORKS_UINT32,
        ANEURALNETWORKS_BOOL,
        ANEURALNETWORKS_OEM_SCALAR,
        ANEURALNETWORKS_TENSOR_OEM_BYTE,
        ANEURALNETWORKS_TENSOR_FLOAT32,
        ANEURALNETWORKS_TENSOR_INT32,
        ANEURALNETWORKS_TENSOR_QUANT8_ASYMM,
        ANEURALNETWORKS_TENSOR_QUANT16_ASYMM,
        ANEURALNETWORKS_TENSOR_QUANT16_SYMM,
        ANEURALNETWORKS_TENSOR_FLOAT16,
        ANEURALNETWORKS_TENSOR_BOOL8,
        ANEURALNETWORKS_TENSOR_QUANT8_SYMM,
};

#ifndef NNTEST_ONLY_PUBLIC_API
// android::nn::k* consts are defined in private headers
static_assert(sizeof(kOperandCodeNoExtraParams) / sizeof(kOperandCodeNoExtraParams[0]) ==
                      android::nn::kNumberOfDataTypes + android::nn::kNumberOfDataTypesOEM - 1,
              "New type added, OperandExtraParamsTest needs an update");
#endif

const uint32_t kOperandCodeChannelQuant[]{
        ANEURALNETWORKS_TENSOR_QUANT8_SYMM_PER_CHANNEL,
};

TEST_F(OperandExtraParamsTest, TestNoExtraParams) {
    // Test for operands that are expected to not have additonal parameters
    for (uint32_t dataType : kOperandCodeNoExtraParams) {
        testAddingWithSymmPerChannelQuantParams(dataType, createSymmPerChannelQuantParams(),
                                                /*expectExtraParamsSuccess=*/false);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuant) {
    // Test for operands that are expected to have SymmPerChannelQuantParams value associated
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        testAddingWithSymmPerChannelQuantParams(dataType, createSymmPerChannelQuantParams(),
                                                /*expectExtraParamsSuccess=*/true);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesBadDim) {
    // Bad .channelDim value
    static float scales[4] = {1.0, 2.0, 3.0, 4.0};
    ANeuralNetworksSymmPerChannelQuantParams ext = {
            .channelDim = 7,
            .scaleCount = 4,
            .scales = scales,
    };
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        testAddingWithSymmPerChannelQuantParams(dataType, ext, /*expectExtraParamsSuccess=*/false);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesBadScalesCount) {
    // Bad .scaleCount value
    static float scales[4] = {1.0, 2.0, 3.0, 4.0};
    ANeuralNetworksSymmPerChannelQuantParams lowScaleCountExt = {
            .channelDim = 3,
            .scaleCount = 3,
            .scales = scales,
    };
    ANeuralNetworksSymmPerChannelQuantParams highScaleCountExt = {
            .channelDim = 3,
            .scaleCount = 10,
            .scales = scales,
    };

    for (uint32_t dataType : kOperandCodeChannelQuant) {
        testAddingWithSymmPerChannelQuantParams(dataType, lowScaleCountExt,
                                                /*expectExtraParamsSuccess=*/false);
        testAddingWithSymmPerChannelQuantParams(dataType, highScaleCountExt,
                                                /*expectExtraParamsSuccess=*/false);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesBadScalesNegative) {
    // Bad .scales value
    static float scales[4] = {1.0, 2.0, -3.0, 4.0};
    ANeuralNetworksSymmPerChannelQuantParams ext = {
            .channelDim = 3,
            .scaleCount = 4,
            .scales = scales,
    };
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        testAddingWithSymmPerChannelQuantParams(dataType, ext, /*expectExtraParamsSuccess=*/false);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesNullScales) {
    // .scales == nullptr value
    ANeuralNetworksSymmPerChannelQuantParams ext = {
            .channelDim = 3,
            .scaleCount = 4,
            .scales = nullptr,
    };
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        testAddingWithSymmPerChannelQuantParams(dataType, ext, /*expectExtraParamsSuccess=*/false);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesOperandScale) {
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        ANeuralNetworksOperandType operandType = createOperand(dataType);
        operandType.scale = 1.0f;
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &operandType), ANEURALNETWORKS_BAD_DATA);
    }
}

TEST_F(OperandExtraParamsTest, TestChannelQuantValuesOperandZeroPoint) {
    for (uint32_t dataType : kOperandCodeChannelQuant) {
        ANeuralNetworksOperandType operandType = createOperand(dataType);
        operandType.zeroPoint = 1;
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &operandType), ANEURALNETWORKS_BAD_DATA);
    }
}

}  // namespace
