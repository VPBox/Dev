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

// This test only tests internal APIs, and has dependencies on internal header
// files, including NN API HIDL definitions.
// It is not part of CTS.

#include "TestMemory.h"

#include "Manager.h"
#include "Memory.h"
#include "TestNeuralNetworksWrapper.h"

#include <android/sharedmem.h>
#include <gtest/gtest.h>

#include <fstream>
#include <string>

using WrapperCompilation = ::android::nn::test_wrapper::Compilation;
using WrapperExecution = ::android::nn::test_wrapper::Execution;
using WrapperMemory = ::android::nn::test_wrapper::Memory;
using WrapperModel = ::android::nn::test_wrapper::Model;
using WrapperOperandType = ::android::nn::test_wrapper::OperandType;
using WrapperResult = ::android::nn::test_wrapper::Result;
using WrapperType = ::android::nn::test_wrapper::Type;

namespace {

// Tests to ensure that various kinds of memory leaks do not occur.
//
// The fixture checks that no anonymous shared memory regions are leaked by
// comparing the count of /dev/ashmem mappings in SetUp and TearDown. This could
// break if the test or framework starts lazily instantiating something that
// creates a mapping - at that point the way the test works needs to be
// reinvestigated. The filename /dev/ashmem is a documented part of the Android
// kernel interface (see
// https://source.android.com/devices/architecture/kernel/reqs-interfaces).
//
// (We can also get very unlucky and mask a memory leak by unrelated unmapping
// somewhere else. This seems unlikely enough to not deal with.)
class MemoryLeakTest : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

private:
    size_t GetAshmemMappingsCount();

    size_t mStartingMapCount = 0;
    bool mIsCpuOnly;
};

void MemoryLeakTest::SetUp() {
    mIsCpuOnly = android::nn::DeviceManager::get()->getUseCpuOnly();
    mStartingMapCount = GetAshmemMappingsCount();
}

void MemoryLeakTest::TearDown() {
    android::nn::DeviceManager::get()->setUseCpuOnly(mIsCpuOnly);
    const size_t endingMapCount = GetAshmemMappingsCount();
    ASSERT_EQ(mStartingMapCount, endingMapCount);
}

size_t MemoryLeakTest::GetAshmemMappingsCount() {
    std::ifstream mappingsStream("/proc/self/maps");
    if (! mappingsStream.good()) {
        // errno is set by std::ifstream on Linux
        ADD_FAILURE() << "Failed to open /proc/self/maps: " << std::strerror(errno);
        return 0;
    }
    std::string line;
    int mapCount = 0;
    while (std::getline(mappingsStream, line)) {
      if (line.find("/dev/ashmem") != std::string::npos) {
        ++mapCount;
      }
    }
    return mapCount;
}

// As well as serving as a functional test for ASharedMemory, also
// serves as a regression test for http://b/69685100 "RunTimePoolInfo
// leaks shared memory regions".
//
// TODO: test non-zero offset.
TEST_F(MemoryLeakTest, TestASharedMemory) {
    // Layout where to place matrix2 and matrix3 in the memory we'll allocate.
    // We have gaps to test that we don't assume contiguity.
    constexpr uint32_t offsetForMatrix2 = 20;
    constexpr uint32_t offsetForMatrix3 = offsetForMatrix2 + sizeof(matrix2) + 30;
    constexpr uint32_t weightsSize = offsetForMatrix3 + sizeof(matrix3) + 60;

    int weightsFd = ASharedMemory_create("weights", weightsSize);
    ASSERT_GT(weightsFd, -1);
    uint8_t* weightsData = (uint8_t*)mmap(nullptr, weightsSize, PROT_READ | PROT_WRITE,
                                          MAP_SHARED, weightsFd, 0);
    ASSERT_NE(weightsData, nullptr);
    memcpy(weightsData + offsetForMatrix2, matrix2, sizeof(matrix2));
    memcpy(weightsData + offsetForMatrix3, matrix3, sizeof(matrix3));
    WrapperMemory weights(weightsSize, PROT_READ | PROT_WRITE, weightsFd, 0);
    ASSERT_TRUE(weights.isValid());

    WrapperModel model;
    WrapperOperandType matrixType(WrapperType::TENSOR_FLOAT32, {3, 4});
    WrapperOperandType scalarType(WrapperType::INT32, {});
    int32_t activation(0);
    auto a = model.addOperand(&matrixType);
    auto b = model.addOperand(&matrixType);
    auto c = model.addOperand(&matrixType);
    auto d = model.addOperand(&matrixType);
    auto e = model.addOperand(&matrixType);
    auto f = model.addOperand(&scalarType);

    model.setOperandValueFromMemory(e, &weights, offsetForMatrix2, sizeof(Matrix3x4));
    model.setOperandValueFromMemory(a, &weights, offsetForMatrix3, sizeof(Matrix3x4));
    model.setOperandValue(f, &activation, sizeof(activation));
    model.addOperation(ANEURALNETWORKS_ADD, {a, c, f}, {b});
    model.addOperation(ANEURALNETWORKS_ADD, {b, e, f}, {d});
    model.identifyInputsAndOutputs({c}, {d});
    ASSERT_TRUE(model.isValid());
    model.finish();

    // Test the two node model.
    constexpr uint32_t offsetForMatrix1 = 20;
    constexpr size_t inputSize = offsetForMatrix1 + sizeof(Matrix3x4);
    int inputFd = ASharedMemory_create("input", inputSize);
    ASSERT_GT(inputFd, -1);
    uint8_t* inputData = (uint8_t*)mmap(nullptr, inputSize,
                                        PROT_READ | PROT_WRITE, MAP_SHARED, inputFd, 0);
    ASSERT_NE(inputData, nullptr);
    memcpy(inputData + offsetForMatrix1, matrix1, sizeof(Matrix3x4));
    WrapperMemory input(inputSize, PROT_READ, inputFd, 0);
    ASSERT_TRUE(input.isValid());

    constexpr uint32_t offsetForActual = 32;
    constexpr size_t outputSize = offsetForActual + sizeof(Matrix3x4);
    int outputFd = ASharedMemory_create("output", outputSize);
    ASSERT_GT(outputFd, -1);
    uint8_t* outputData = (uint8_t*)mmap(nullptr, outputSize,
                                         PROT_READ | PROT_WRITE, MAP_SHARED, outputFd, 0);
    ASSERT_NE(outputData, nullptr);
    memset(outputData, 0, outputSize);
    WrapperMemory actual(outputSize, PROT_READ | PROT_WRITE, outputFd, 0);
    ASSERT_TRUE(actual.isValid());

    WrapperCompilation compilation2(&model);
    ASSERT_EQ(compilation2.finish(), WrapperResult::NO_ERROR);

    WrapperExecution execution2(&compilation2);
    ASSERT_EQ(execution2.setInputFromMemory(0, &input, offsetForMatrix1, sizeof(Matrix3x4)),
              WrapperResult::NO_ERROR);
    ASSERT_EQ(execution2.setOutputFromMemory(0, &actual, offsetForActual, sizeof(Matrix3x4)),
              WrapperResult::NO_ERROR);
    ASSERT_EQ(execution2.compute(), WrapperResult::NO_ERROR);
    ASSERT_EQ(CompareMatrices(expected3,
                              *reinterpret_cast<Matrix3x4*>(outputData + offsetForActual)), 0);

    munmap(weightsData, weightsSize);
    munmap(inputData, inputSize);
    munmap(outputData, outputSize);
    close(weightsFd);
    close(inputFd);
    close(outputFd);
}

// Regression test for http://b/69621433 "MemoryFd leaks shared memory regions".
TEST_F(MemoryLeakTest, GetPointer) {
    static const size_t size = 1;

    int fd = ASharedMemory_create(nullptr, size);
    ASSERT_GE(fd, 0);

    uint8_t* buf = (uint8_t*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ASSERT_NE(buf, nullptr);
    *buf = 0;

    {
        // Scope "mem" in such a way that any shared memory regions it
        // owns will be released before we check the value of *buf: We
        // want to verify that the explicit mmap() above is not
        // perturbed by any mmap()/munmap() that results from methods
        // invoked on "mem".

        WrapperMemory mem(size, PROT_READ | PROT_WRITE, fd, 0);
        ASSERT_TRUE(mem.isValid());

        auto internalMem = reinterpret_cast<::android::nn::Memory*>(mem.get());
        uint8_t *dummy;
        ASSERT_EQ(internalMem->getPointer(&dummy), ANEURALNETWORKS_NO_ERROR);
        (*dummy)++;
    }

    ASSERT_EQ(*buf, (uint8_t)1);
    ASSERT_EQ(munmap(buf, size), 0);

    close(fd);
}

// Regression test for http://b/69621433 "MemoryFd leaks shared memory regions".
TEST_F(MemoryLeakTest, Instantiate) {
    static const size_t size = 1;
    int fd = ASharedMemory_create(nullptr, size);
    ASSERT_GE(fd, 0);
    WrapperMemory mem(size, PROT_READ | PROT_WRITE, fd, 0);
    ASSERT_TRUE(mem.isValid());

    auto internalMem = reinterpret_cast<::android::nn::Memory*>(mem.get());
    uint8_t *dummy;
    ASSERT_EQ(internalMem->getPointer(&dummy), ANEURALNETWORKS_NO_ERROR);

    close(fd);
}

#ifndef NNTEST_ONLY_PUBLIC_API
// Regression test for http://b/73663843, conv_2d trying to allocate too much memory.
TEST_F(MemoryLeakTest, convTooLarge) {
    android::nn::DeviceManager::get()->setUseCpuOnly(true);
    WrapperModel model;

    // This kernel/input size will make convQuant8 allocate 12 * 13 * 13 * 128 * 92 * 92, which is
    // just outside of signed int range (0x82F56000) - this will fail due to CPU implementation
    // limitations
    WrapperOperandType type3(WrapperType::INT32, {});
    WrapperOperandType type2(WrapperType::TENSOR_INT32, {128}, 0.25, 0);
    WrapperOperandType type0(WrapperType::TENSOR_QUANT8_ASYMM, {12, 104, 104, 128}, 0.5, 0);
    WrapperOperandType type4(WrapperType::TENSOR_QUANT8_ASYMM, {12, 92, 92, 128}, 1.0, 0);
    WrapperOperandType type1(WrapperType::TENSOR_QUANT8_ASYMM, {128, 13, 13, 128}, 0.5, 0);

    // Operands
    auto op1 = model.addOperand(&type0);
    auto op2 = model.addOperand(&type1);
    auto op3 = model.addOperand(&type2);
    auto pad0 = model.addOperand(&type3);
    auto act = model.addOperand(&type3);
    auto stride = model.addOperand(&type3);
    auto op4 = model.addOperand(&type4);

    // Operations
    uint8_t op2_init[128 * 13 * 13 * 128] = {};
    model.setOperandValue(op2, op2_init, sizeof(op2_init));
    int32_t op3_init[128] = {};
    model.setOperandValue(op3, op3_init, sizeof(op3_init));
    int32_t pad0_init[] = {0};
    model.setOperandValue(pad0, pad0_init, sizeof(pad0_init));
    int32_t act_init[] = {0};
    model.setOperandValue(act, act_init, sizeof(act_init));
    int32_t stride_init[] = {1};
    model.setOperandValue(stride, stride_init, sizeof(stride_init));
    model.addOperation(ANEURALNETWORKS_CONV_2D, {op1, op2, op3, pad0, pad0, pad0, pad0, stride, stride, act}, {op4});

    // Inputs and outputs
    model.identifyInputsAndOutputs({op1}, {op4});
    ASSERT_TRUE(model.isValid());
    model.finish();

    // Compilation
    WrapperCompilation compilation(&model);
    ASSERT_EQ(WrapperResult::NO_ERROR,compilation.finish());
    WrapperExecution execution(&compilation);

    // Set input and outputs
    static uint8_t input[12 * 104 * 104 * 128] = {};
    ASSERT_EQ(WrapperResult::NO_ERROR, execution.setInput(0, input, sizeof(input)));
    static uint8_t output[12 * 92 * 92 * 128] = {};
    ASSERT_EQ(WrapperResult::NO_ERROR, execution.setOutput(0, output, sizeof(output)));

    // This shouldn't segfault
    WrapperResult r = execution.compute();

    ASSERT_EQ(WrapperResult::OP_FAILED, r);
}
#endif // NNTEST_ONLY_PUBLIC_API

}  // end namespace
