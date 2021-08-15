/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "NeuralNetworks.h"
#include "NeuralNetworksOEM.h"

#include <android/sharedmem.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <future>
#include <string>

#ifndef NNTEST_ONLY_PUBLIC_API
#include "NeuralNetworksExtensions.h"
#include "TypeManager.h"
#endif

// This file tests all the validations done by the Neural Networks API.

namespace {

class ValidationTest : public ::testing::Test {
   protected:
    virtual void SetUp() {}
};

class ValidationTestModel : public ValidationTest {
   protected:
    virtual void SetUp() {
        ValidationTest::SetUp();
        ASSERT_EQ(ANeuralNetworksModel_create(&mModel), ANEURALNETWORKS_NO_ERROR);
    }
    virtual void TearDown() {
        ANeuralNetworksModel_free(mModel);
        ValidationTest::TearDown();
    }

    uint32_t addScalarOperand(int32_t type = ANEURALNETWORKS_INT32) {
        ANeuralNetworksOperandType operandType = {
                .type = type, .dimensionCount = 0, .dimensions = nullptr};
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &operandType), ANEURALNETWORKS_NO_ERROR);
        return mNumOperands++;
    }

    uint32_t addOperand(const ANeuralNetworksOperandType& operandType) {
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &operandType), ANEURALNETWORKS_NO_ERROR);
        return mNumOperands++;
    }

    uint32_t addTensorOperand(int32_t type = ANEURALNETWORKS_TENSOR_FLOAT32) {
        uint32_t dimensions[] = {2};
        ANeuralNetworksOperandType operandType = {
                .type = type,
                .dimensionCount = sizeof(dimensions) / sizeof(dimensions[0]),
                .dimensions = dimensions,
        };
        return addOperand(operandType);
    }

    void createModel() {
        addTensorOperand();
        addTensorOperand();
        addScalarOperand();
        addTensorOperand();
        uint32_t inList[3]{0, 1, 2};
        uint32_t outList[1]{3};
        ASSERT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_ADD, 3, inList, 1,
                                                    outList),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 3, inList, 1, outList),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_finish(mModel), ANEURALNETWORKS_NO_ERROR);
        mNumOperations = 1;
    }

    uint32_t mNumOperands = 0;
    uint32_t mNumOperations = 0;
    ANeuralNetworksModel* mModel = nullptr;

    const uint32_t kDummyDimensionValue = 1;
    const ANeuralNetworksOperandType kInvalidTensorType1{
            .type = ANEURALNETWORKS_TENSOR_FLOAT32,
            // dimensionCount must be consistent with dimensions.
            .dimensionCount = 1,
            .dimensions = nullptr,
    };
    const ANeuralNetworksOperandType kInvalidTensorType2{
            .type = ANEURALNETWORKS_TENSOR_FLOAT32,
            // dimensionCount must be consistent with dimensions.
            .dimensionCount = 0,
            .dimensions = &kDummyDimensionValue,
    };
};

#ifndef NNTEST_ONLY_PUBLIC_API
constexpr const char* kTestExtensionName = "com.android.test_extension";
constexpr int32_t kTestExtensionTensorType = ANEURALNETWORKS_TENSOR_QUANT8_SYMM_PER_CHANNEL;

class ValidationTestModelExtensions : public ValidationTestModel {
   protected:
    virtual void SetUp() {
        ValidationTestModel::SetUp();
        EXPECT_TRUE(::android::nn::TypeManager::get()->forTest_registerExtension({
                .name = kTestExtensionName,
                .operandTypes =
                        {
                                {
                                        .type = kTestExtensionTensorType,
                                        .isTensor = true,
                                        .byteSize = 1,
                                },
                        },
        }));
    }

    virtual void TearDown() {
        ::android::nn::TypeManager::get()->forTest_reset();
        ValidationTestModel::TearDown();
    }

    int32_t getExtensionOperandType(uint16_t typeWithinExtension) {
        int32_t result;
        EXPECT_EQ(ANeuralNetworksModel_getExtensionOperandType(mModel, kTestExtensionName,
                                                               typeWithinExtension, &result),
                  ANEURALNETWORKS_NO_ERROR);
        return result;
    }
};
#endif

class ValidationTestIdentify : public ValidationTestModel {
    virtual void SetUp() {
        ValidationTestModel::SetUp();

        uint32_t dimensions[]{1};
        ANeuralNetworksOperandType tensorType{.type = ANEURALNETWORKS_TENSOR_FLOAT32,
                                              .dimensionCount = 1,
                                              .dimensions = dimensions};
        ANeuralNetworksOperandType scalarType{
                .type = ANEURALNETWORKS_INT32, .dimensionCount = 0, .dimensions = nullptr};
        ASSERT_EQ(ANeuralNetworksModel_addOperand(mModel, &tensorType), ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_addOperand(mModel, &tensorType), ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_addOperand(mModel, &scalarType), ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_addOperand(mModel, &tensorType), ANEURALNETWORKS_NO_ERROR);
        uint32_t inList[3]{0, 1, 2};
        uint32_t outList[1]{3};
        ASSERT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_ADD, 3, inList, 1,
                                                    outList),
                  ANEURALNETWORKS_NO_ERROR);
    }
    virtual void TearDown() { ValidationTestModel::TearDown(); }
};

class ValidationTestCompilation : public ValidationTestModel {
   protected:
    virtual void SetUp() {
        ValidationTestModel::SetUp();
        createModel();
        ASSERT_EQ(ANeuralNetworksCompilation_create(mModel, &mCompilation),
                  ANEURALNETWORKS_NO_ERROR);
    }

    virtual void TearDown() {
        ANeuralNetworksCompilation_free(mCompilation);
        ValidationTestModel::TearDown();
    }

    ANeuralNetworksCompilation* mCompilation = nullptr;
};

class ValidationTestExecution : public ValidationTestCompilation {
   protected:
    virtual void SetUp() {
        ValidationTestCompilation::SetUp();

        ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);

        ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &mExecution),
                  ANEURALNETWORKS_NO_ERROR);
    }
    virtual void TearDown() {
        ANeuralNetworksExecution_free(mExecution);
        ValidationTestCompilation::TearDown();
    }
    ANeuralNetworksExecution* mExecution = nullptr;
};

class ValidationTestBurst : public ValidationTestExecution {
   protected:
    virtual void SetUp() {
        ValidationTestExecution::SetUp();

        ASSERT_EQ(ANeuralNetworksBurst_create(mCompilation, &mBurst), ANEURALNETWORKS_NO_ERROR);
    }
    virtual void TearDown() {
        ANeuralNetworksBurst_free(mBurst);
        ValidationTestExecution::TearDown();
    }
    ANeuralNetworksBurst* mBurst = nullptr;
};

TEST_F(ValidationTest, CreateModel) {
    EXPECT_EQ(ANeuralNetworksModel_create(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestModel, AddOperand) {
    ANeuralNetworksOperandType floatType{
            .type = ANEURALNETWORKS_FLOAT32, .dimensionCount = 0, .dimensions = nullptr};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(nullptr, &floatType),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);

    ANeuralNetworksOperandType quant8TypeInvalidScale{
            .type = ANEURALNETWORKS_TENSOR_QUANT8_ASYMM,
            .dimensionCount = 0,
            .dimensions = nullptr,
            // Scale has to be non-negative
            .scale = -1.0f,
            .zeroPoint = 0,
    };
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &quant8TypeInvalidScale),
              ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksOperandType quant8TypeInvalidZeroPoint{
            .type = ANEURALNETWORKS_TENSOR_QUANT8_ASYMM,
            .dimensionCount = 0,
            .dimensions = nullptr,
            .scale = 1.0f,
            // zeroPoint has to be in [0, 255]
            .zeroPoint = -1,
    };
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &quant8TypeInvalidZeroPoint),
              ANEURALNETWORKS_BAD_DATA);

    const uint32_t dim = 2;
    ANeuralNetworksOperandType invalidScalarType{
            .type = ANEURALNETWORKS_INT32,
            // a scalar type must have 0 dimensions.
            .dimensionCount = 1,
            .dimensions = &dim,
    };
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &invalidScalarType),
              ANEURALNETWORKS_BAD_DATA);

    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &kInvalidTensorType1),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &kInvalidTensorType2),
              ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksModel_finish(mModel);
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &floatType), ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, SetOperandSymmPerChannelQuantParams) {
    const int32_t operandIndex = addTensorOperand(ANEURALNETWORKS_TENSOR_QUANT8_SYMM_PER_CHANNEL);

    float scales[2] = {1.0, 2.0};
    ANeuralNetworksSymmPerChannelQuantParams channelQuant = {
            .channelDim = 0,
            .scaleCount = 2,
            .scales = scales,
    };

    EXPECT_EQ(ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(nullptr, operandIndex,
                                                                       &channelQuant),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(
            ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(mModel, operandIndex, nullptr),
            ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(mModel, operandIndex + 1,
                                                                       &channelQuant),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(mModel, operandIndex,
                                                                       &channelQuant),
              ANEURALNETWORKS_NO_ERROR);
}

#ifndef NNTEST_ONLY_PUBLIC_API
TEST_F(ValidationTestModelExtensions, AddOperand_UnknownPrefix) {
    ANeuralNetworksOperandType type = {.type = -1};
    ASSERT_EQ(ANeuralNetworksModel_addOperand(mModel, &type), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestModelExtensions, SetOperandSymmPerChannelQuantParams_ExtensionOperand) {
    const int32_t operandIndex =
            addTensorOperand(getExtensionOperandType(kTestExtensionTensorType));

    float scales[2] = {1.0, 2.0};
    ANeuralNetworksSymmPerChannelQuantParams channelQuant = {
            .channelDim = 0,
            .scaleCount = 2,
            .scales = scales,
    };

    EXPECT_EQ(ANeuralNetworksModel_setOperandSymmPerChannelQuantParams(mModel, operandIndex,
                                                                       &channelQuant),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestModelExtensions, SetOperandExtensionData) {
    const int32_t operandIndex =
            addTensorOperand(getExtensionOperandType(kTestExtensionTensorType));
    const int32_t data = 42;
    const size_t dataLength = sizeof(data);
    EXPECT_EQ(
            ANeuralNetworksModel_setOperandExtensionData(nullptr, operandIndex, &data, dataLength),
            ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(
            ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex, nullptr, dataLength),
            ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex, &data, 0),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex + 1, &data,
                                                           dataLength),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex, &data, dataLength),
              ANEURALNETWORKS_NO_ERROR);
}

TEST_F(ValidationTestModelExtensions, SetOperandExtensionData_Empty) {
    const int32_t operandIndex =
            addTensorOperand(getExtensionOperandType(kTestExtensionTensorType));
    EXPECT_EQ(ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex, nullptr, 0),
              ANEURALNETWORKS_NO_ERROR);
}

TEST_F(ValidationTestModelExtensions, SetOperandExtensionData_NonExtensionOperand) {
    const int32_t operandIndex = addTensorOperand();
    const int32_t data = 42;
    const size_t dataLength = sizeof(data);
    EXPECT_EQ(ANeuralNetworksModel_setOperandExtensionData(mModel, operandIndex, &data, dataLength),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestModelExtensions, SetOperandValue_UnspecifiedDimension) {
    const uint32_t dimensions[2] = {3, 0};
    ANeuralNetworksOperandType type = {
            .type = getExtensionOperandType(kTestExtensionTensorType),
            .dimensionCount = 2,
            .dimensions = dimensions,
    };
    const int32_t operandIndex = addOperand(type);
    char buffer[20];
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, operandIndex, buffer, sizeof(buffer)),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestModelExtensions, SetOperandValue_UnspecifiedRank) {
    ANeuralNetworksOperandType type = {
            .type = getExtensionOperandType(kTestExtensionTensorType),
            .dimensionCount = 0,
            .dimensions = nullptr,
    };
    const int32_t operandIndex = addOperand(type);
    char buffer[20];
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, operandIndex, buffer, sizeof(buffer)),
              ANEURALNETWORKS_BAD_DATA);
}
#endif

TEST_F(ValidationTestModel, SetOptionalOperand) {
    ANeuralNetworksOperandType floatType{
            .type = ANEURALNETWORKS_FLOAT32, .dimensionCount = 0, .dimensions = nullptr};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &floatType), ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, nullptr, 0),
              ANEURALNETWORKS_NO_ERROR);
}

TEST_F(ValidationTestModel, SetOperandValue) {
    ANeuralNetworksOperandType floatType{
            .type = ANEURALNETWORKS_FLOAT32, .dimensionCount = 0, .dimensions = nullptr};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &floatType), ANEURALNETWORKS_NO_ERROR);

    char buffer[20];
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(nullptr, 0, buffer, sizeof(buffer)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, nullptr, sizeof(buffer)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since buffer is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, buffer, sizeof(buffer)),
              ANEURALNETWORKS_BAD_DATA);

    // This should succeed.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, buffer, sizeof(float)),
              ANEURALNETWORKS_NO_ERROR);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 1, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksModel_finish(mModel);
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, SetOperandValueFromMemory) {
    uint32_t dimensions[]{1};
    ANeuralNetworksOperandType floatType{
            .type = ANEURALNETWORKS_TENSOR_FLOAT32, .dimensionCount = 1, .dimensions = dimensions};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &floatType), ANEURALNETWORKS_NO_ERROR);

    const size_t memorySize = 20;
    int memoryFd = ASharedMemory_create("nnMemory", memorySize);
    ASSERT_GT(memoryFd, 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd, 0,
                                                 &memory),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(nullptr, 0, memory, 0, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, nullptr, 0, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, -1, memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, 0, memorySize),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 1, memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, memorySize + 1,
                                                             sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, memorySize - 3,
                                                             sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksModel_finish(mModel);
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_STATE);

    // close memory
    close(memoryFd);
}

TEST_F(ValidationTestModel, SetOperandValueFromAHardwareBuffer) {
    uint32_t dimensions[]{1};
    ANeuralNetworksOperandType quant8Type{.type = ANEURALNETWORKS_TENSOR_QUANT8_ASYMM,
                                          .dimensionCount = 1,
                                          .dimensions = dimensions,
                                          .scale = 1.0,
                                          .zeroPoint = 0};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &quant8Type), ANEURALNETWORKS_NO_ERROR);

    AHardwareBuffer_Desc desc{
            .width = 16,
            .height = 16,
            .layers = 1,
            .format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM,
            .usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
    };

    AHardwareBuffer* buffer = nullptr;
    ASSERT_EQ(AHardwareBuffer_allocate(&desc, &buffer), 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromAHardwareBuffer(buffer, &memory),
              ANEURALNETWORKS_NO_ERROR);

    // This should fail, since non-BLOB AHardwareBuffer is not allowed.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, 0, sizeof(uint8_t)),
              ANEURALNETWORKS_UNMAPPABLE);

    AHardwareBuffer_release(buffer);
}

TEST_F(ValidationTestModel, SetOperandValueFromAHardwareBufferBlob) {
    uint32_t dimensions[]{1};
    ANeuralNetworksOperandType floatType{
            .type = ANEURALNETWORKS_TENSOR_FLOAT32, .dimensionCount = 1, .dimensions = dimensions};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &floatType), ANEURALNETWORKS_NO_ERROR);

    const size_t memorySize = 20;
    AHardwareBuffer_Desc desc{
            .width = memorySize,
            .height = 1,
            .layers = 1,
            .format = AHARDWAREBUFFER_FORMAT_BLOB,
            .usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
    };

    AHardwareBuffer* buffer = nullptr;
    ASSERT_EQ(AHardwareBuffer_allocate(&desc, &buffer), 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromAHardwareBuffer(buffer, &memory),
              ANEURALNETWORKS_NO_ERROR);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, memorySize + 1,
                                                             sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksModel_setOperandValueFromMemory(mModel, 0, memory, memorySize - 3,
                                                             sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    AHardwareBuffer_release(buffer);
}

TEST_F(ValidationTestModel, AddOEMOperand) {
    ANeuralNetworksOperandType OEMScalarType{
            .type = ANEURALNETWORKS_OEM_SCALAR, .dimensionCount = 0, .dimensions = nullptr};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &OEMScalarType), ANEURALNETWORKS_NO_ERROR);
    char buffer[20];
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 0, buffer, sizeof(buffer)),
              ANEURALNETWORKS_NO_ERROR);

    const size_t kByteSizeOfOEMTensor = 4;
    uint32_t dimensions[]{kByteSizeOfOEMTensor};
    ANeuralNetworksOperandType OEMTensorType{
            .type = ANEURALNETWORKS_TENSOR_OEM_BYTE, .dimensionCount = 1, .dimensions = dimensions};
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &OEMTensorType), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksModel_setOperandValue(mModel, 1, buffer, kByteSizeOfOEMTensor),
              ANEURALNETWORKS_NO_ERROR);

    ANeuralNetworksModel_finish(mModel);
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &OEMTensorType), ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, AddOperation) {
    uint32_t input = 0;
    uint32_t output = 0;
    EXPECT_EQ(ANeuralNetworksModel_addOperation(nullptr, ANEURALNETWORKS_AVERAGE_POOL_2D, 1, &input,
                                                1, &output),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_AVERAGE_POOL_2D, 0, nullptr,
                                                1, &output),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_AVERAGE_POOL_2D, 1, &input,
                                                0, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    ANeuralNetworksOperationType invalidOp = -1;
    EXPECT_EQ(ANeuralNetworksModel_addOperation(mModel, invalidOp, 1, &input, 1, &output),
              ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksModel_finish(mModel);
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_AVERAGE_POOL_2D, 1, &input,
                                                1, &output),
              ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, IdentifyInputsAndOutputs) {
    uint32_t input = 0;
    uint32_t output = 0;
    EXPECT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(nullptr, 1, &input, 1, &output),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 0, nullptr, 1, &output),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 1, &input, 0, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    createModel();
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 1, &input, 1, &output),
              ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, RelaxComputationFloat32toFloat16) {
    EXPECT_EQ(ANeuralNetworksModel_relaxComputationFloat32toFloat16(nullptr, true),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    createModel();
    // This should fail, as the model is already finished.
    EXPECT_EQ(ANeuralNetworksModel_relaxComputationFloat32toFloat16(mModel, true),
              ANEURALNETWORKS_BAD_STATE);
    EXPECT_EQ(ANeuralNetworksModel_relaxComputationFloat32toFloat16(mModel, false),
              ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, Finish) {
    EXPECT_EQ(ANeuralNetworksModel_finish(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    createModel();
    EXPECT_EQ(ANeuralNetworksModel_finish(mModel), ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, EmptyModel) {
    // An empty model is invalid
    EXPECT_EQ(ANeuralNetworksModel_finish(mModel), ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestModel, CreateCompilation) {
    ANeuralNetworksCompilation* compilation = nullptr;
    EXPECT_EQ(ANeuralNetworksCompilation_create(nullptr, &compilation),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_create(mModel, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_create(mModel, &compilation), ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestModel, CreateCompilationForDevices) {
    createModel();
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    if (numDevices > 0) {
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(0, &device), ANEURALNETWORKS_NO_ERROR);
        ANeuralNetworksCompilation* compilation = nullptr;
        EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(nullptr, &device, 1, &compilation),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, &device, 1, nullptr),
                  ANEURALNETWORKS_UNEXPECTED_NULL);

        // empty device list
        EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, &device, 0, &compilation),
                  ANEURALNETWORKS_BAD_DATA);

        // duplicate devices in the list.
        ANeuralNetworksDevice* invalidDevices[2] = {device, device};
        EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, invalidDevices, 2,
                                                              &compilation),
                  ANEURALNETWORKS_BAD_DATA);
        // nullptr in the list.
        invalidDevices[1] = nullptr;
        EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, invalidDevices, 2,
                                                              &compilation),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
    }

    ANeuralNetworksCompilation* compilation = nullptr;
    EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(nullptr, nullptr, 1, &compilation),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, nullptr, 1, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, nullptr, 1, &compilation),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestModel, GetSupportedOperationsForDevices) {
    createModel();
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    bool supportedOps[20];
    ASSERT_LE(mNumOperations, sizeof(supportedOps) / sizeof(supportedOps[0]));
    if (numDevices > 0) {
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(0, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(nullptr, &device, 1,
                                                                        supportedOps),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(
                ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, &device, 1, nullptr),
                ANEURALNETWORKS_UNEXPECTED_NULL);

        // empty device list
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, &device, 0,
                                                                        supportedOps),
                  ANEURALNETWORKS_BAD_DATA);

        // duplicate devices in the list.
        ANeuralNetworksDevice* invalidDevices[2] = {device, device};
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, invalidDevices, 2,
                                                                        supportedOps),
                  ANEURALNETWORKS_BAD_DATA);
        // nullptr in the list.
        invalidDevices[1] = nullptr;
        EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, invalidDevices, 2,
                                                                        supportedOps),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
    }

    EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(nullptr, nullptr, 1,
                                                                    supportedOps),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, nullptr, 1, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(
            ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, nullptr, 1, supportedOps),
            ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestIdentify, Ok) {
    uint32_t inList[3]{0, 1, 2};
    uint32_t outList[1]{3};

    ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 3, inList, 1, outList),
              ANEURALNETWORKS_NO_ERROR);

    ASSERT_EQ(ANeuralNetworksModel_finish(mModel), ANEURALNETWORKS_NO_ERROR);
}

TEST_F(ValidationTestIdentify, InputIsOutput) {
    uint32_t inList[3]{0, 1, 2};
    uint32_t outList[2]{3, 0};

    ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 3, inList, 2, outList),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestIdentify, OutputIsInput) {
    uint32_t inList[4]{0, 1, 2, 3};
    uint32_t outList[1]{3};

    ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 4, inList, 1, outList),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestIdentify, DuplicateInputs) {
    uint32_t inList[4]{0, 1, 2, 0};
    uint32_t outList[1]{3};

    ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 4, inList, 1, outList),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestIdentify, DuplicateOutputs) {
    uint32_t inList[3]{0, 1, 2};
    uint32_t outList[2]{3, 3};

    ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 3, inList, 2, outList),
              ANEURALNETWORKS_BAD_DATA);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, SetPreference)
TEST_F(ValidationTestCompilation, SetPreference) {
    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(nullptr, ANEURALNETWORKS_PREFER_LOW_POWER),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(mCompilation, 40), ANEURALNETWORKS_BAD_DATA);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, SetCaching)
TEST_F(ValidationTestCompilation, SetCaching) {
    std::vector<uint8_t> token(ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN, 0);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(nullptr, "/data/local/tmp", token.data()),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, nullptr, token.data()),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, "/data/local/tmp", nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, CreateExecution)
TEST_F(ValidationTestCompilation, CreateExecution) {
    ANeuralNetworksExecution* execution = nullptr;
    EXPECT_EQ(ANeuralNetworksExecution_create(nullptr, &execution),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_BAD_STATE);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, Finish)
TEST_F(ValidationTestCompilation, Finish) {
    EXPECT_EQ(ANeuralNetworksCompilation_finish(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(mCompilation,
                                                       ANEURALNETWORKS_PREFER_FAST_SINGLE_ANSWER),
              ANEURALNETWORKS_BAD_STATE);
    std::vector<uint8_t> token(ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN, 0);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, "/data/local/tmp", token.data()),
              ANEURALNETWORKS_BAD_STATE);
    EXPECT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_STATE);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, ExecutionTiming)
// Also see TEST_F(ValidationTestCompilationForDevices_2, ExecutionTiming)
TEST_F(ValidationTestCompilation, ExecutionTiming) {
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
    ANeuralNetworksExecution* execution;
    ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);
    // Cannot setMeasureTiming() with Compilation rather than CompilationForDevices.
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, false),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, true), ANEURALNETWORKS_BAD_DATA);
}

// Also see TEST_F(ValidationTestCompilationForDevices_1, ExecutionTiming)
TEST_F(ValidationTestCompilation, ExecutionUsability) {
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);

    enum class ExecutionType : uint32_t { ASYNC, SYNC, BURST };
    for (auto executionType : {ExecutionType::ASYNC, ExecutionType::SYNC, ExecutionType::BURST}) {
        SCOPED_TRACE(static_cast<uint32_t>(executionType));

        ANeuralNetworksExecution* execution;
        ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution),
                  ANEURALNETWORKS_NO_ERROR);

        float in0[] = {0.0f, 0.0f}, in1[] = {1.0f, 1.0f}, out0[2];
        int in2 = 0;
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, &in0, sizeof(in0)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 1, nullptr, &in1, sizeof(in1)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 2, nullptr, &in2, sizeof(in2)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setOutput(execution, 0, nullptr, &out0, sizeof(out0)),
                  ANEURALNETWORKS_NO_ERROR);

        const size_t memorySize = std::max(sizeof(in0), sizeof(out0));
        int memoryFd = ASharedMemory_create("nnMemory", memorySize);
        ASSERT_GT(memoryFd, 0);
        ANeuralNetworksMemory* memory;
        EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd,
                                                     0, &memory),
                  ANEURALNETWORKS_NO_ERROR);

        auto testTooLate = [this, execution, &in0, &out0, memory] {
            // Try a bunch of things that are impermissible if the execution has started.

            // Set inputs and outputs.
            ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, &in0, sizeof(in0)),
                      ANEURALNETWORKS_BAD_STATE);
            ASSERT_EQ(
                    ANeuralNetworksExecution_setOutput(execution, 0, nullptr, &out0, sizeof(out0)),
                    ANEURALNETWORKS_BAD_STATE);
            ASSERT_EQ(ANeuralNetworksExecution_setInputFromMemory(execution, 0, nullptr, memory, 0,
                                                                  sizeof(in0)),
                      ANEURALNETWORKS_BAD_STATE);
            ASSERT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory, 0,
                                                                   sizeof(out0)),
                      ANEURALNETWORKS_BAD_STATE);

            // Reuse for asynchronous execution.
            {
                ANeuralNetworksEvent* event;
                ASSERT_EQ(ANeuralNetworksExecution_startCompute(execution, &event),
                          ANEURALNETWORKS_BAD_STATE);
            }

            // Reuse for synchronous execution.
            ASSERT_EQ(ANeuralNetworksExecution_compute(execution), ANEURALNETWORKS_BAD_STATE);

            // Reuse for burst execution.
            {
                ANeuralNetworksBurst* burst;
                ASSERT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst),
                          ANEURALNETWORKS_NO_ERROR);
                ASSERT_EQ(ANeuralNetworksExecution_burstCompute(execution, burst),
                          ANEURALNETWORKS_BAD_STATE);
                ANeuralNetworksBurst_free(burst);
            }
        };

        // Compute.
        switch (executionType) {
            case ExecutionType::ASYNC: {
                ANeuralNetworksEvent* event;
                ASSERT_EQ(ANeuralNetworksExecution_startCompute(execution, &event),
                          ANEURALNETWORKS_NO_ERROR);
                testTooLate();
                ASSERT_EQ(ANeuralNetworksEvent_wait(event), ANEURALNETWORKS_NO_ERROR);
                testTooLate();
                ANeuralNetworksEvent_free(event);
                break;
            }
            case ExecutionType::SYNC: {
                ASSERT_EQ(ANeuralNetworksExecution_compute(execution), ANEURALNETWORKS_NO_ERROR);
                testTooLate();
                break;
            }
            case ExecutionType::BURST: {
                ANeuralNetworksBurst* burst;
                ASSERT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst),
                          ANEURALNETWORKS_NO_ERROR);
                ASSERT_EQ(ANeuralNetworksExecution_burstCompute(execution, burst),
                          ANEURALNETWORKS_NO_ERROR);
                testTooLate();
                ANeuralNetworksBurst_free(burst);
                break;
            }
            default:
                FAIL() << "Unreachable";
        }
    }
}

TEST_F(ValidationTestExecution, SetInput) {
    char buffer[20];
    EXPECT_EQ(ANeuralNetworksExecution_setInput(nullptr, 0, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, nullptr, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, buffer, 20),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 999, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, -1, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, &kInvalidTensorType1, buffer,
                                                sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, &kInvalidTensorType2, buffer,
                                                sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestExecution, SetOutput) {
    char buffer[20];
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(nullptr, 0, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, nullptr, sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, buffer, 20),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 999, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, as this operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, -1, nullptr, buffer, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, &kInvalidTensorType1, buffer,
                                                 sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(mExecution, 0, &kInvalidTensorType2, buffer,
                                                 sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
}

TEST_F(ValidationTestExecution, SetInputFromMemory) {
    const size_t memorySize = 20;
    int memoryFd = ASharedMemory_create("nnMemory", memorySize);
    ASSERT_GT(memoryFd, 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd, 0,
                                                 &memory),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(nullptr, 0, nullptr, memory, 0,
                                                          sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, nullptr, 0,
                                                          sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 999, nullptr, memory, 0,
                                                          sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, -1, nullptr, memory, 0,
                                                          sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory, 0,
                                                          memorySize),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory,
                                                          memorySize + 1, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory,
                                                          memorySize - 3, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, &kInvalidTensorType1,
                                                          memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, &kInvalidTensorType2,
                                                          memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // close memory
    close(memoryFd);
}

TEST_F(ValidationTestExecution, SetInputFromAHardwareBufferBlob) {
    const size_t memorySize = 20;

    AHardwareBuffer_Desc desc{
            .width = memorySize,
            .height = 1,
            .layers = 1,
            .format = AHARDWAREBUFFER_FORMAT_BLOB,
            .usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
    };

    AHardwareBuffer* buffer = nullptr;
    ASSERT_EQ(AHardwareBuffer_allocate(&desc, &buffer), 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromAHardwareBuffer(buffer, &memory),
              ANEURALNETWORKS_NO_ERROR);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory, 0,
                                                          memorySize),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory,
                                                          memorySize + 1, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, nullptr, memory,
                                                          memorySize - 3, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, &kInvalidTensorType1,
                                                          memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setInputFromMemory(mExecution, 0, &kInvalidTensorType2,
                                                          memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    AHardwareBuffer_release(buffer);
}

TEST_F(ValidationTestExecution, SetOutputFromMemory) {
    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);

    const size_t memorySize = 20;
    int memoryFd = ASharedMemory_create("nnMemory", memorySize);
    ASSERT_GT(memoryFd, 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd, 0,
                                                 &memory),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(nullptr, 0, nullptr, memory, 0,
                                                           sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, nullptr, 0,
                                                           sizeof(float)),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 999, nullptr, memory, 0,
                                                           sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, -1, nullptr, memory, 0,
                                                           sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory, 0,
                                                           memorySize),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory,
                                                           memorySize + 1, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory,
                                                           memorySize - 3, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, &kInvalidTensorType1,
                                                           memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, &kInvalidTensorType2,
                                                           memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // close memory
    close(memoryFd);
}

TEST_F(ValidationTestExecution, SetOutputFromAHardwareBufferBlob) {
    const size_t memorySize = 20;

    AHardwareBuffer_Desc desc{
            .width = memorySize,
            .height = 1,
            .layers = 1,
            .format = AHARDWAREBUFFER_FORMAT_BLOB,
            .usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
    };

    AHardwareBuffer* buffer = nullptr;
    ASSERT_EQ(AHardwareBuffer_allocate(&desc, &buffer), 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromAHardwareBuffer(buffer, &memory),
              ANEURALNETWORKS_NO_ERROR);

    // This should fail, since memory is not the size of a float32.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(mExecution, 0, nullptr, memory, 0,
                                                           memorySize),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since offset is larger than memorySize.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(mExecution, 0, nullptr, memory,
                                                           memorySize + 1, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // This should fail, since requested size is larger than the memory.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(mExecution, 0, nullptr, memory,
                                                           memorySize - 3, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    // These should fail, since the tensor types are invalid.
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(mExecution, 0, &kInvalidTensorType1,
                                                           memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(mExecution, 0, &kInvalidTensorType2,
                                                           memory, 0, sizeof(float)),
              ANEURALNETWORKS_BAD_DATA);

    AHardwareBuffer_release(buffer);
}

TEST_F(ValidationTestExecution, Compute) {
    EXPECT_EQ(ANeuralNetworksExecution_compute(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestExecution, StartCompute) {
    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);

    ANeuralNetworksEvent* event;
    EXPECT_EQ(ANeuralNetworksExecution_startCompute(nullptr, &event),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_startCompute(execution, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestExecution, EventWait) {
    EXPECT_EQ(ANeuralNetworksEvent_wait(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestExecution, GetOutputOperandRankAndDimensions) {
    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);

    float input0[] = {1.0f, 1.0f}, input1[] = {2.0f, 2.0f}, output0[2];
    int32_t input2[] = {0};
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, input0, sizeof(input0)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 1, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 2, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(execution, 0, nullptr, output0, sizeof(output0)),
              ANEURALNETWORKS_NO_ERROR);

    uint32_t rank, dims[4], expectedRank = 1, expectedDims = 2;
    // This should fail, since the execution has not yet started to compute.
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(execution, 0, &rank),
              ANEURALNETWORKS_BAD_STATE);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(execution, 0, dims),
              ANEURALNETWORKS_BAD_STATE);

    ANeuralNetworksEvent* event;
    EXPECT_EQ(ANeuralNetworksExecution_startCompute(execution, &event), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksEvent_wait(event), ANEURALNETWORKS_NO_ERROR);

    // This should fail, since unexpected nullptr.
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(nullptr, 0, &rank),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(nullptr, 0, dims),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(execution, 0, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(execution, 0, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    // This should fail, since the operand does not exist.
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(execution, -1, &rank),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(execution, 999, &rank),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(execution, -1, dims),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(execution, 999, dims),
              ANEURALNETWORKS_BAD_DATA);

    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandRank(execution, 0, &rank),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_getOutputOperandDimensions(execution, 0, dims),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(rank, expectedRank);
    EXPECT_EQ(dims[0], expectedDims);
}

TEST_F(ValidationTestBurst, BurstComputeNull) {
    EXPECT_EQ(ANeuralNetworksExecution_burstCompute(mExecution, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_burstCompute(nullptr, mBurst),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST_F(ValidationTestBurst, BurstComputeBadCompilation) {
    ANeuralNetworksCompilation* compilation;
    ASSERT_EQ(ANeuralNetworksCompilation_create(mModel, &compilation), ANEURALNETWORKS_NO_ERROR);
    // NOTE: ANeuralNetworksCompilation_finish not called

    ANeuralNetworksBurst* burst;
    EXPECT_EQ(ANeuralNetworksBurst_create(compilation, &burst), ANEURALNETWORKS_BAD_STATE);
}

TEST_F(ValidationTestBurst, BurstComputeDifferentCompilations) {
    ANeuralNetworksCompilation* secondCompilation;
    ASSERT_EQ(ANeuralNetworksCompilation_create(mModel, &secondCompilation),
              ANEURALNETWORKS_NO_ERROR);
    ASSERT_EQ(ANeuralNetworksCompilation_finish(secondCompilation), ANEURALNETWORKS_NO_ERROR);

    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(secondCompilation, &execution),
              ANEURALNETWORKS_NO_ERROR);

    EXPECT_EQ(ANeuralNetworksExecution_burstCompute(execution, mBurst), ANEURALNETWORKS_BAD_DATA);

    ANeuralNetworksExecution_free(execution);
    ANeuralNetworksCompilation_free(secondCompilation);
}

TEST_F(ValidationTestBurst, BurstComputeConcurrent) {
    ANeuralNetworksExecution* secondExecution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &secondExecution),
              ANEURALNETWORKS_NO_ERROR);

    // set inputs of first execution
    float inputA0[] = {1.0f, 1.0f}, inputA1[] = {2.0f, 2.0f}, outputA0[2];
    int32_t inputA2[] = {0};
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 0, nullptr, inputA0, sizeof(inputA0)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 1, nullptr, inputA1, sizeof(inputA1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(mExecution, 2, nullptr, inputA2, sizeof(inputA2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(
            ANeuralNetworksExecution_setOutput(mExecution, 0, nullptr, outputA0, sizeof(outputA0)),
            ANEURALNETWORKS_NO_ERROR);

    // set inputs of second execution
    float inputB0[] = {1.0f, 1.0f}, inputB1[] = {2.0f, 2.0f}, outputB0[2];
    int32_t inputB2[] = {0};
    EXPECT_EQ(ANeuralNetworksExecution_setInput(secondExecution, 0, nullptr, inputB0,
                                                sizeof(inputB0)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(secondExecution, 1, nullptr, inputB1,
                                                sizeof(inputB1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(secondExecution, 2, nullptr, inputB2,
                                                sizeof(inputB2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutput(secondExecution, 0, nullptr, outputB0,
                                                 sizeof(outputB0)),
              ANEURALNETWORKS_NO_ERROR);

    // Execute on the same burst concurrently. At least one result must be
    // ANEURALNETWORKS_NO_ERROR. One may return ANEURALNETWORKS_BAD_STATE if the
    // other is already executing on the burst.
    auto first = std::async(std::launch::async, [this] {
        return ANeuralNetworksExecution_burstCompute(mExecution, mBurst);
    });
    auto second = std::async(std::launch::async, [this, secondExecution] {
        return ANeuralNetworksExecution_burstCompute(secondExecution, mBurst);
    });

    const int result1 = first.get();
    const int result2 = second.get();
    EXPECT_TRUE(result1 == ANEURALNETWORKS_BAD_STATE || result1 == ANEURALNETWORKS_NO_ERROR);
    EXPECT_TRUE(result2 == ANEURALNETWORKS_BAD_STATE || result2 == ANEURALNETWORKS_NO_ERROR);
    EXPECT_TRUE(result1 == ANEURALNETWORKS_NO_ERROR || result2 == ANEURALNETWORKS_NO_ERROR);

    ANeuralNetworksExecution_free(secondExecution);
}

// The burst object maintains a local cache of memory objects. Because the burst
// is intended to live for multiple executions, and because memory might be
// created and freed for each execution, burst includes internal mechanisms to
// purge memory objects from its cache that have been freed by the NNAPI client.
// The following two test cases (FreeMemoryBeforeBurst and
// FreeBurstBeforeMemory) ensure that this internal cleanup is tested in both
// freeing orders.
//
// These two test cases explicitly create a new burst object and a new execution
// object so that the order of freeing can be specified. If these tests instead
// relied on the provided mExecution and mBurst, mBurst would always be freed
// before mExecution.

TEST_F(ValidationTestBurst, FreeMemoryBeforeBurst) {
    ANeuralNetworksBurst* burst;
    EXPECT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst), ANEURALNETWORKS_NO_ERROR);

    // prepare data for execution
    float input0[] = {1.0f, 1.0f}, input1[] = {2.0f, 2.0f}, output0[2];
    int32_t input2[] = {0};

    const size_t memorySize = sizeof(output0);
    int memoryFd = ASharedMemory_create("nnMemory", memorySize);
    ASSERT_GT(memoryFd, 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd, 0,
                                                 &memory),
              ANEURALNETWORKS_NO_ERROR);

    // create and configure execution
    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, input0, sizeof(input0)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 1, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 2, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory, 0,
                                                           sizeof(output0)),
              ANEURALNETWORKS_NO_ERROR);

    // preform execution to cache memory into burst
    EXPECT_EQ(ANeuralNetworksExecution_burstCompute(execution, burst), ANEURALNETWORKS_NO_ERROR);
    ANeuralNetworksExecution_free(execution);

    // free memory before burst
    ANeuralNetworksMemory_free(memory);
    ANeuralNetworksBurst_free(burst);

    // close memory
    close(memoryFd);
}

TEST_F(ValidationTestBurst, FreeBurstBeforeMemory) {
    ANeuralNetworksBurst* burst;
    EXPECT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst), ANEURALNETWORKS_NO_ERROR);

    // prepare data for execution
    float input0[] = {1.0f, 1.0f}, input1[] = {2.0f, 2.0f}, output0[2];
    int32_t input2[] = {0};
    const size_t memorySize = sizeof(output0);
    int memoryFd = ASharedMemory_create("nnMemory", memorySize);
    ASSERT_GT(memoryFd, 0);

    ANeuralNetworksMemory* memory;
    EXPECT_EQ(ANeuralNetworksMemory_createFromFd(memorySize, PROT_READ | PROT_WRITE, memoryFd, 0,
                                                 &memory),
              ANEURALNETWORKS_NO_ERROR);

    // create and configure execution
    ANeuralNetworksExecution* execution;
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, input0, sizeof(input0)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 1, nullptr, input1, sizeof(input1)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setInput(execution, 2, nullptr, input2, sizeof(input2)),
              ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksExecution_setOutputFromMemory(execution, 0, nullptr, memory, 0,
                                                           sizeof(output0)),
              ANEURALNETWORKS_NO_ERROR);

    // preform execution to cache memory into burst
    EXPECT_EQ(ANeuralNetworksExecution_burstCompute(execution, burst), ANEURALNETWORKS_NO_ERROR);
    ANeuralNetworksExecution_free(execution);

    // free burst before memory
    ANeuralNetworksBurst_free(burst);
    ANeuralNetworksMemory_free(memory);

    // close memory
    close(memoryFd);
}

TEST(ValidationTestIntrospection, GetNumDevices) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST(ValidationTestIntrospection, GetDevice) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    ANeuralNetworksDevice* device = nullptr;
    for (uint32_t i = 0; i < numDevices; i++) {
        SCOPED_TRACE(i);
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_NE(device, nullptr);
    }
    EXPECT_EQ(ANeuralNetworks_getDevice(0, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworks_getDevice(numDevices, &device), ANEURALNETWORKS_BAD_DATA);
}

static void deviceStringCheck(std::function<int(const ANeuralNetworksDevice*, const char**)> func) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    const char* buffer;
    for (uint32_t i = 0; i < numDevices; i++) {
        SCOPED_TRACE(i);
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(func(device, &buffer), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(func(device, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    }
    EXPECT_EQ(func(nullptr, &buffer), ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(func(nullptr, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST(ValidationTestIntrospection, DeviceGetName) {
    deviceStringCheck(ANeuralNetworksDevice_getName);
}

TEST(ValidationTestIntrospection, DeviceGetNameUnique) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    std::set<std::string> deviceNames;
    for (uint32_t i = 0; i < numDevices; i++) {
        ANeuralNetworksDevice* device = nullptr;
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        const char* buffer = nullptr;
        EXPECT_EQ(ANeuralNetworksDevice_getName(device, &buffer), ANEURALNETWORKS_NO_ERROR);
        std::string name(buffer);
        EXPECT_EQ(deviceNames.count(name), (uint32_t)0);
        deviceNames.insert(name);
    }
}

TEST(ValidationTestIntrospection, DeviceGetVersion) {
    deviceStringCheck(ANeuralNetworksDevice_getVersion);
}

TEST(ValidationTestIntrospection, DeviceGetFeatureLevel) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    int64_t featureLevel;
    for (uint32_t i = 0; i < numDevices; i++) {
        SCOPED_TRACE(i);
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksDevice_getFeatureLevel(device, &featureLevel),
                  ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksDevice_getFeatureLevel(device, nullptr),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
    }
    EXPECT_EQ(ANeuralNetworksDevice_getFeatureLevel(nullptr, &featureLevel),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksDevice_getFeatureLevel(nullptr, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

TEST(ValidationTestIntrospection, DeviceGetType) {
    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    int32_t validTypes[] = {ANEURALNETWORKS_DEVICE_UNKNOWN, ANEURALNETWORKS_DEVICE_OTHER,
                            ANEURALNETWORKS_DEVICE_CPU, ANEURALNETWORKS_DEVICE_GPU,
                            ANEURALNETWORKS_DEVICE_ACCELERATOR};
    int32_t deviceType;
    for (uint32_t i = 0; i < numDevices; i++) {
        SCOPED_TRACE(i);
        // Initialize the deviceType to be an invalid type.
        deviceType = -1;
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksDevice_getType(device, &deviceType), ANEURALNETWORKS_NO_ERROR);
        EXPECT_TRUE(std::find(std::begin(validTypes), std::end(validTypes), deviceType) !=
                    std::end(validTypes));
        EXPECT_EQ(ANeuralNetworksDevice_getType(device, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    }
    EXPECT_EQ(ANeuralNetworksDevice_getType(nullptr, &deviceType), ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksDevice_getType(nullptr, nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
}

class ValidationTestCompilationForDevices_1 : public ValidationTestModel {
   protected:
    virtual void SetUp() override {
        ValidationTestModel::SetUp();
        createModel();

        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

        if (numDevices > 0) {
            EXPECT_EQ(ANeuralNetworks_getDevice(0, &mDevice), ANEURALNETWORKS_NO_ERROR);
            bool supported = false;
            ASSERT_EQ(mNumOperations, static_cast<uint32_t>(1));
            EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, &mDevice, 1,
                                                                            &supported),
                      ANEURALNETWORKS_NO_ERROR);
            if (supported) {
                ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, &mDevice, 1,
                                                                      &mCompilation),
                          ANEURALNETWORKS_NO_ERROR);
            }
        }
    }

    virtual void TearDown() {
        ANeuralNetworksCompilation_free(mCompilation);
        ValidationTestModel::TearDown();
    }

    ANeuralNetworksDevice* mDevice = nullptr;
    ANeuralNetworksCompilation* mCompilation = nullptr;
};

// Also see TEST_F(ValidationTestCompilation, SetPreference)
TEST_F(ValidationTestCompilationForDevices_1, SetPreference) {
    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(nullptr, ANEURALNETWORKS_PREFER_LOW_POWER),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    if (!mCompilation) {
        return;
    }
    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(mCompilation, 40), ANEURALNETWORKS_BAD_DATA);
}

// Also see TEST_F(ValidationTestCompilation, SetCaching)
TEST_F(ValidationTestCompilationForDevices_1, SetCaching) {
    std::vector<uint8_t> token(ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN, 0);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(nullptr, "/data/local/tmp", token.data()),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    if (!mCompilation) {
        return;
    }
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, nullptr, token.data()),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, "/data/local/tmp", nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
}

// Also see TEST_F(ValidationTestCompilation, CreateExecution)
TEST_F(ValidationTestCompilationForDevices_1, CreateExecution) {
    ANeuralNetworksExecution* execution = nullptr;
    EXPECT_EQ(ANeuralNetworksExecution_create(nullptr, &execution),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    if (!mCompilation) {
        return;
    }
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, nullptr),
              ANEURALNETWORKS_UNEXPECTED_NULL);
    EXPECT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_BAD_STATE);
}

// Also see TEST_F(ValidationTestCompilation, Finish)
TEST_F(ValidationTestCompilationForDevices_1, Finish) {
    EXPECT_EQ(ANeuralNetworksCompilation_finish(nullptr), ANEURALNETWORKS_UNEXPECTED_NULL);
    if (!mCompilation) {
        return;
    }
    EXPECT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
    EXPECT_EQ(ANeuralNetworksCompilation_setPreference(mCompilation,
                                                       ANEURALNETWORKS_PREFER_FAST_SINGLE_ANSWER),
              ANEURALNETWORKS_BAD_STATE);
    std::vector<uint8_t> token(ANEURALNETWORKS_BYTE_SIZE_OF_CACHE_TOKEN, 0);
    EXPECT_EQ(ANeuralNetworksCompilation_setCaching(mCompilation, "/data/local/tmp", token.data()),
              ANEURALNETWORKS_BAD_STATE);
    EXPECT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_BAD_STATE);
}

class ValidationTestCompilationForDevices_2 : public ValidationTestModel {
   protected:
    virtual void SetUp() override {
        ValidationTestModel::SetUp();
        createModel();

        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

        if (numDevices > 1) {
            EXPECT_EQ(ANeuralNetworks_getDevice(0, &mDevices[0]), ANEURALNETWORKS_NO_ERROR);
            EXPECT_EQ(ANeuralNetworks_getDevice(1, &mDevices[1]), ANEURALNETWORKS_NO_ERROR);
            bool supported = false;
            ASSERT_EQ(mNumOperations, static_cast<uint32_t>(1));
            EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, mDevices, 2,
                                                                            &supported),
                      ANEURALNETWORKS_NO_ERROR);
            if (supported) {
                ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, mDevices, 2,
                                                                      &mCompilation),
                          ANEURALNETWORKS_NO_ERROR);
            }
        }
    }

    virtual void TearDown() {
        ANeuralNetworksCompilation_free(mCompilation);
        ValidationTestModel::TearDown();
    }

    ANeuralNetworksDevice* mDevices[2] = {nullptr, nullptr};
    ANeuralNetworksCompilation* mCompilation = nullptr;
};

// Also see TEST_F(ValidationTestCompilation, ExecutionTiming)
// Also see TEST_F(ValidationTestCompilationForDevices_1, ExecutionTiming)
TEST_F(ValidationTestCompilationForDevices_2, ExecutionTiming) {
    if (!mCompilation) {
        return;
    }
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);
    ANeuralNetworksExecution* execution;
    ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution), ANEURALNETWORKS_NO_ERROR);
    // Cannot setMeasureTiming() if there are two or more devices.
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, false),
              ANEURALNETWORKS_BAD_DATA);
    EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, true), ANEURALNETWORKS_BAD_DATA);
}

class ValidationTestInvalidCompilation : public ValidationTestModel {
   protected:
    virtual void SetUp() override {
        ValidationTestModel::SetUp();

        // Create a model with an OEM operation
        uint32_t dimensions[]{1};
        ANeuralNetworksOperandType OEMTensorType{.type = ANEURALNETWORKS_TENSOR_OEM_BYTE,
                                                 .dimensionCount = 1,
                                                 .dimensions = dimensions};
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &OEMTensorType),
                  ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksModel_addOperand(mModel, &OEMTensorType),
                  ANEURALNETWORKS_NO_ERROR);
        uint32_t inList[1]{0};
        uint32_t outList[1]{1};
        ASSERT_EQ(ANeuralNetworksModel_addOperation(mModel, ANEURALNETWORKS_OEM_OPERATION, 1,
                                                    inList, 1, outList),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_identifyInputsAndOutputs(mModel, 1, inList, 1, outList),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksModel_finish(mModel), ANEURALNETWORKS_NO_ERROR);

        // Find a device that cannot handle OEM operation and create compilation on that
        uint32_t numDevices = 0;
        EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);
        for (uint32_t i = 0; i < numDevices; i++) {
            ANeuralNetworksDevice* device;
            EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
            bool supported = false;
            EXPECT_EQ(ANeuralNetworksModel_getSupportedOperationsForDevices(mModel, &device, 1,
                                                                            &supported),
                      ANEURALNETWORKS_NO_ERROR);
            if (!supported) {
                ASSERT_EQ(ANeuralNetworksCompilation_createForDevices(mModel, &device, 1,
                                                                      &mInvalidCompilation),
                          ANEURALNETWORKS_NO_ERROR);
                break;
            }
        }
    }

    virtual void TearDown() {
        ANeuralNetworksCompilation_free(mInvalidCompilation);
        ValidationTestModel::TearDown();
    }

    ANeuralNetworksCompilation* mInvalidCompilation = nullptr;
};

TEST_F(ValidationTestInvalidCompilation, CreateExecutionWithInvalidCompilation) {
    if (!mInvalidCompilation) {
        return;
    }
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mInvalidCompilation), ANEURALNETWORKS_BAD_DATA);
    ANeuralNetworksExecution* execution = nullptr;
    EXPECT_EQ(ANeuralNetworksExecution_create(mInvalidCompilation, &execution),
              ANEURALNETWORKS_BAD_STATE);
}

// Also see TEST_F(ValidationTestCompilation, ExecutionTiming)
// Also see TEST_F(ValidationTestCompilationForDevices_2, ExecutionTiming)
// Also see TEST_F(ValidationTestCompilation, ExecutionUsability)
TEST_F(ValidationTestCompilationForDevices_1, ExecutionTiming) {
    if (!mCompilation) {
        return;
    }
    ASSERT_EQ(ANeuralNetworksCompilation_finish(mCompilation), ANEURALNETWORKS_NO_ERROR);

    enum class ExecutionType : uint32_t { ASYNC, SYNC, BURST };
    for (auto executionType : {ExecutionType::ASYNC, ExecutionType::SYNC, ExecutionType::BURST}) {
        SCOPED_TRACE(static_cast<uint32_t>(executionType));

        ANeuralNetworksExecution* execution;
        ASSERT_EQ(ANeuralNetworksExecution_create(mCompilation, &execution),
                  ANEURALNETWORKS_NO_ERROR);

        EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(nullptr, false),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(nullptr, true),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, false),
                  ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, true),
                  ANEURALNETWORKS_NO_ERROR);

        float in0[] = {0.0f, 0.0f}, in1[] = {1.0f, 1.0f}, out0[2];
        int in2 = 0;
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 0, nullptr, &in0, sizeof(in0)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 1, nullptr, &in1, sizeof(in1)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setInput(execution, 2, nullptr, &in2, sizeof(in2)),
                  ANEURALNETWORKS_NO_ERROR);
        ASSERT_EQ(ANeuralNetworksExecution_setOutput(execution, 0, nullptr, &out0, sizeof(out0)),
                  ANEURALNETWORKS_NO_ERROR);

        // Cannot getDuration until the execution has finished.
        uint64_t duration;
        EXPECT_EQ(ANeuralNetworksExecution_getDuration(
                          execution, ANEURALNETWORKS_DURATION_ON_HARDWARE, &duration),
                  ANEURALNETWORKS_BAD_STATE);
        EXPECT_EQ(ANeuralNetworksExecution_getDuration(
                          execution, ANEURALNETWORKS_DURATION_IN_DRIVER, &duration),
                  ANEURALNETWORKS_BAD_STATE);

        auto testMeasureTooLate = [execution] {
            // Cannot setMeasureTiming if the execution has started.
            EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, false),
                      ANEURALNETWORKS_BAD_STATE);
            EXPECT_EQ(ANeuralNetworksExecution_setMeasureTiming(execution, true),
                      ANEURALNETWORKS_BAD_STATE);
        };

        // Compute.
        switch (executionType) {
            case ExecutionType::ASYNC: {
                ANeuralNetworksEvent* event;
                ASSERT_EQ(ANeuralNetworksExecution_startCompute(execution, &event),
                          ANEURALNETWORKS_NO_ERROR);
                testMeasureTooLate();
                ASSERT_EQ(ANeuralNetworksEvent_wait(event), ANEURALNETWORKS_NO_ERROR);
                testMeasureTooLate();
                ANeuralNetworksEvent_free(event);
                break;
            }
            case ExecutionType::SYNC: {
                ASSERT_EQ(ANeuralNetworksExecution_compute(execution), ANEURALNETWORKS_NO_ERROR);
                testMeasureTooLate();
                break;
            }
            case ExecutionType::BURST: {
                ANeuralNetworksBurst* burst;
                ASSERT_EQ(ANeuralNetworksBurst_create(mCompilation, &burst),
                          ANEURALNETWORKS_NO_ERROR);
                ASSERT_EQ(ANeuralNetworksExecution_burstCompute(execution, burst),
                          ANEURALNETWORKS_NO_ERROR);
                testMeasureTooLate();
                ANeuralNetworksBurst_free(burst);
                break;
            }
            default:
                FAIL() << "Unreachable";
        }

        auto testDuration = [](ANeuralNetworksExecution* e, int32_t durationCode,
                               bool nullDuration) {
            SCOPED_TRACE(e);
            SCOPED_TRACE(durationCode);
            SCOPED_TRACE(nullDuration);

            // Strictly speaking, a duration COULD have this value, but it is
            // exceedingly unlikely. We'll use it as an initial value that we expect
            // to be modified by getDuration().
            const uint64_t kBogusDuration = UINT64_MAX - 1;

            uint64_t duration = kBogusDuration;
            uint64_t* durationPtr = nullDuration ? nullptr : &duration;

            int expectedResultCode = ANEURALNETWORKS_NO_ERROR;
            if (e == nullptr | durationPtr == nullptr) {
                expectedResultCode = ANEURALNETWORKS_UNEXPECTED_NULL;
            } else if (durationCode < 0) {
                expectedResultCode = ANEURALNETWORKS_BAD_DATA;
            }

            EXPECT_EQ(ANeuralNetworksExecution_getDuration(e, durationCode, durationPtr),
                      expectedResultCode);
            if (expectedResultCode == ANEURALNETWORKS_NO_ERROR) {
                EXPECT_NE(duration, kBogusDuration);
            }
        };

        std::vector<ANeuralNetworksExecution*> executions = {nullptr, execution};
        std::vector<int32_t> durationCodes = {-1, ANEURALNETWORKS_DURATION_ON_HARDWARE,
                                              ANEURALNETWORKS_DURATION_IN_DRIVER};
        std::vector<bool> nullDurations = {false, true};
        for (auto e : executions) {
            for (auto d : durationCodes) {
                for (auto n : nullDurations) {
                    testDuration(e, d, n);
                }
            }
        }
    }
}

#ifndef NNTEST_ONLY_PUBLIC_API
TEST(ValidationTestDevice, GetExtensionSupport) {
    bool result;
    EXPECT_EQ(ANeuralNetworksDevice_getExtensionSupport(nullptr, kTestExtensionName, &result),
              ANEURALNETWORKS_UNEXPECTED_NULL);

    uint32_t numDevices = 0;
    EXPECT_EQ(ANeuralNetworks_getDeviceCount(&numDevices), ANEURALNETWORKS_NO_ERROR);

    for (uint32_t i = 0; i < numDevices; i++) {
        SCOPED_TRACE(i);
        ANeuralNetworksDevice* device;
        EXPECT_EQ(ANeuralNetworks_getDevice(i, &device), ANEURALNETWORKS_NO_ERROR);
        EXPECT_EQ(ANeuralNetworksDevice_getExtensionSupport(device, kTestExtensionName, nullptr),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(ANeuralNetworksDevice_getExtensionSupport(device, nullptr, &result),
                  ANEURALNETWORKS_UNEXPECTED_NULL);
        EXPECT_EQ(ANeuralNetworksDevice_getExtensionSupport(device, kTestExtensionName, &result),
                  ANEURALNETWORKS_NO_ERROR);
    }
}
#endif

}  // namespace
