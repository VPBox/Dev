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

#include "TestNeuralNetworksWrapper.h"

#include <sys/mman.h>
#include <tuple>
#include <vector>

#include <android-base/macros.h>
#include <android/sharedmem.h>
#include <gtest/gtest.h>

using namespace android::nn::test_wrapper;

namespace {

// We try the following model:
//
//     op2 = ADD(op0, op1)
//     op4 = TRANSPOSE(op2, op3)
//
// where op0 is a required model input, should be of dimension (A, B).
//       op1 is a required constant, should be of dimension (A, 1).
//       op2 is an internal operand, should be of dimension (A, B).
//       op3 is an omitted optional constant / model input, should be of dimension (2).
//       op4 is a model output, should be of dimension (B, A).
//
// For each operand, we test combinations of dimensions specification level during model
// construction time and execution time (if any). All other relevant combinations of the
// basic scenarios are then iterated over in TestAll. Note that we don't want to just use
// googletest's parametrized tests (TEST_P) as the 16k combinations generated too many
// lines of output for the test infrastructure to handle correctly.

// Which operand to test
enum class UnspecifiedOperand {
    INPUT_MANDATORY,
    CONST_MANDATORY,
    TEMPORARY_VARIABLE,
    INPUT_OPTIONAL,
    CONST_OPTIONAL,
    OUTPUT
};
// How well the dimensional information is specified
enum class SpecificationLevel {
    FULLY_SPECIFIED,   // all dimensions are clearly specified without any ambiguity
    UNSPECIFIED_DIM,   // certain dimension is set to 0 as unknown, but rank is well-specified
    UNSPECIFIED_RANK,  // rank is set to 0 as unknown, passing an empty vector for dims
    UNSPECIFIED_TYPE   // only during execution time, passing nullptr for operand type
};
using UnspecifiedDimensionsTestParam = std::tuple<UnspecifiedOperand,
                                                  SpecificationLevel,   // model construction time
                                                  SpecificationLevel>;  // execution time

// Indexing
constexpr uint32_t kIndex0_Model = 0;      // op0, model
constexpr uint32_t kIndex1_Model = 1;      // op1, model
constexpr uint32_t kIndex2_Model = 2;      // op2, model
constexpr uint32_t kIndex3_Model = 3;      // op3, model
constexpr uint32_t kIndex4_Model = 4;      // op4, model
constexpr uint32_t kIndex0_Execution = 5;  // op0, execution
constexpr uint32_t kIndex3_Execution = 6;  // op3, execution
constexpr uint32_t kIndex4_Execution = 7;  // op4, execution
constexpr uint32_t kIndexCount = 8;        // count

constexpr int32_t kValueA = 0;
constexpr int32_t kValueB = 2;
constexpr uint32_t kDimAGood = 2;
constexpr uint32_t kDimABad = 3;

class UnspecifiedDimensionsTest : public ::testing::TestWithParam<UnspecifiedDimensionsTestParam> {
    enum class OptionalType { CONST, INPUT };       // omitted operand op3 is an input or const
    enum class BufferSize { LESS, EQUAL, MORE };    // only used for output buffer size
    enum class OperandLocation { BUFFER, MEMORY };  // where the operand reside
    enum class InOutType { INPUT, OUTPUT };         // parameter for setInOut()

    class SharedMemoryForTest {
       public:
        SharedMemoryForTest() : memory(nullptr), fd(-1), buffer(nullptr), length(0) {}
        ~SharedMemoryForTest() {
            if (buffer != nullptr) {
                munmap(buffer, length);
            }
            if (fd > -1) {
                close(fd);
            }
        }
        void initialize(size_t size, const void* data) {
            length = size;
            fd = ASharedMemory_create(nullptr, size);
            ASSERT_GT(fd, -1);
            buffer = (uint8_t*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            ASSERT_NE(buffer, nullptr);
            memcpy(buffer, data, size);
            memory = std::make_shared<Memory>(size, PROT_READ | PROT_WRITE, fd, 0);
            ASSERT_TRUE(memory->isValid());
        }
        const Memory* getMemory() const { return memory.get(); }
        const uint8_t* getBuffer() const { return buffer; }

       private:
        DISALLOW_COPY_AND_ASSIGN(SharedMemoryForTest);
        std::shared_ptr<Memory> memory;
        int fd;
        uint8_t* buffer;
        size_t length;
    };

    std::string toString(SpecificationLevel level) {
        switch (level) {
            case SpecificationLevel::FULLY_SPECIFIED:
                return "FULLY_SPECIFIED";
            case SpecificationLevel::UNSPECIFIED_DIM:
                return "UNSPECIFIED_DIM";
            case SpecificationLevel::UNSPECIFIED_RANK:
                return "UNSPECIFIED_RANK";
            case SpecificationLevel::UNSPECIFIED_TYPE:
                return "UNSPECIFIED_TYPE";
            default:
                return "UNKNOWN";
        }
    }

    std::string toString(BufferSize b) {
        switch (b) {
            case BufferSize::LESS:
                return "LESS";
            case BufferSize::EQUAL:
                return "EQUAL";
            case BufferSize::MORE:
                return "MORE";
            default:
                return "UNKNOWN";
        }
    }

    std::string toString(OperandLocation loc) {
        switch (loc) {
            case OperandLocation::BUFFER:
                return "BUFFER";
            case OperandLocation::MEMORY:
                return "MEMORY";
            default:
                return "UNKNOWN";
        }
    }

   protected:
    virtual void SetUp() {
        uint32_t modelIndex, executionIndex;
        switch (kUnspecifiedOperand) {
            case UnspecifiedOperand::INPUT_MANDATORY:
                modelIndex = kIndex0_Model;
                executionIndex = kIndex0_Execution;
                mBadIndexChoices = {kIndexCount, modelIndex, executionIndex};
                mOperandLocationChoices = {OperandLocation::BUFFER, OperandLocation::MEMORY};
                break;
            case UnspecifiedOperand::CONST_MANDATORY:
                modelIndex = kIndex1_Model;
                executionIndex = kIndexCount;
                mBadIndexChoices = {kIndexCount, modelIndex};
                mOperandLocationChoices = {OperandLocation::BUFFER, OperandLocation::MEMORY};
                break;
            case UnspecifiedOperand::TEMPORARY_VARIABLE:
                modelIndex = kIndex2_Model;
                executionIndex = kIndexCount;
                mBadIndexChoices = {kIndexCount, modelIndex};
                mOperandLocationChoices = {OperandLocation::BUFFER};
                break;
            case UnspecifiedOperand::INPUT_OPTIONAL:
                modelIndex = kIndex3_Model;
                executionIndex = kIndex3_Execution;
                mBadIndexChoices = {kIndexCount};
                mOptionalType = OptionalType::INPUT;
                mOperandLocationChoices = {OperandLocation::BUFFER};
                break;
            case UnspecifiedOperand::CONST_OPTIONAL:
                modelIndex = kIndex3_Model;
                executionIndex = kIndexCount;
                mBadIndexChoices = {kIndexCount};
                mOperandLocationChoices = {OperandLocation::BUFFER};
                break;
            case UnspecifiedOperand::OUTPUT:
                modelIndex = kIndex4_Model;
                executionIndex = kIndex4_Execution;
                mBadIndexChoices = {kIndexCount, modelIndex, executionIndex};
                mOperandLocationChoices = {OperandLocation::BUFFER, OperandLocation::MEMORY};
                mBufferSizeChoices = {BufferSize::LESS, BufferSize::EQUAL, BufferSize::MORE};
                break;
            default:
                break;
        }
        std::vector<SpecificationLevel> levels{
                SpecificationLevel::UNSPECIFIED_DIM, SpecificationLevel::FULLY_SPECIFIED,
                SpecificationLevel::UNSPECIFIED_DIM, SpecificationLevel::FULLY_SPECIFIED,
                SpecificationLevel::UNSPECIFIED_DIM, SpecificationLevel::FULLY_SPECIFIED,
                SpecificationLevel::FULLY_SPECIFIED, SpecificationLevel::FULLY_SPECIFIED};
        levels[modelIndex] = kSpecificationLevelModel;
        if (executionIndex < kIndexCount) {
            levels[executionIndex] = kSpecificationLevelExecution;
        }
        mSpecificationLevels = std::move(levels);
    }

    OperandType getType(uint32_t index, const std::vector<uint32_t>& dim) {
        const SpecificationLevel l = mSpecificationLevels[index];
        std::vector<uint32_t> setDim;
        if (l != SpecificationLevel::UNSPECIFIED_RANK) {
            for (auto d : dim) {
                if (d == 0) {
                    setDim.push_back(mBadIndex != index ? kDimAGood : kDimABad);
                } else {
                    setDim.push_back(l == SpecificationLevel::FULLY_SPECIFIED ? d : 0);
                }
            }
        }
        float scale = mOperandTypes[index] == Type::TENSOR_QUANT8_ASYMM ? 1.0 : 0.0;
        return OperandType(mOperandTypes[index], setDim, scale, 0);
    }

    uint32_t getSize(uint32_t index, const std::vector<uint32_t>& dim,
                     BufferSize s = BufferSize::EQUAL) {
        uint32_t n = 1;
        for (auto d : dim) {
            n *= (d == 0 ? (mBadIndex != index ? kDimAGood : kDimABad) : d);
        }
        if (s == BufferSize::LESS) {
            n /= 2;
        } else if (s == BufferSize::MORE) {
            n *= 2;
        }
        return n;
    };

    template <typename T>
    Result setInOut(Execution* execution, uint32_t index, uint32_t opIndex,
                    const std::vector<uint32_t>& dim, void* buffer,
                    const SharedMemoryForTest* memory, InOutType inOutType,
                    BufferSize bufferSize = BufferSize::EQUAL) {
        const auto kLevel = mSpecificationLevels[index];
        size_t size = (buffer == nullptr) ? 0 : getSize(index, dim, bufferSize) * sizeof(T);
        auto type = getType(index, dim);
        ANeuralNetworksOperandType* t =
                (kLevel == SpecificationLevel::UNSPECIFIED_TYPE) ? nullptr : &type.operandType;
        if (mOperandLocation == OperandLocation::MEMORY && memory != nullptr) {
            if (inOutType == InOutType::INPUT) {
                return execution->setInputFromMemory(opIndex, memory->getMemory(), 0, size, t);
            } else {
                return execution->setOutputFromMemory(opIndex, memory->getMemory(), 0, size, t);
            }
        } else {
            if (inOutType == InOutType::INPUT) {
                return execution->setInput(opIndex, buffer, size, t);
            } else {
                return execution->setOutput(opIndex, buffer, size, t);
            }
        }
        return Result::NO_ERROR;
    }

    template <typename T, Type TensorType>
    void TestOne() {
        // Phase 1: Build Model
        Model model;
        auto type0 = getType(kIndex0_Model, {kValueA, kValueB});
        auto type1 = getType(kIndex1_Model, {kValueA, 1});
        auto type2 = getType(kIndex2_Model, {kValueA, kValueB});
        auto type3 = getType(kIndex3_Model, {2});
        auto type4 = getType(kIndex4_Model, {kValueB, kValueA});
        OperandType typeActivation(Type::INT32, {});  // activation

        auto op0 = model.addOperand(&type0);
        auto op1 = model.addOperand(&type1);
        auto op2 = model.addOperand(&type2);
        auto op3 = model.addOperand(&type3);
        auto op4 = model.addOperand(&type4);
        auto act = model.addOperand(&typeActivation);

        T bufferOp1[2] = {1, 2};
        SharedMemoryForTest memoryOp1;
        memoryOp1.initialize(sizeof(bufferOp1), bufferOp1);
        if (mOperandLocation == OperandLocation::BUFFER) {
            model.setOperandValue(op1, bufferOp1, sizeof(bufferOp1));
        } else {
            model.setOperandValueFromMemory(op1, memoryOp1.getMemory(), 0, sizeof(bufferOp1));
        }
        int32_t kActivation = 0;
        model.setOperandValue(act, &kActivation, sizeof(int32_t));
        if (mOptionalType == OptionalType::CONST) {
            model.setOperandValue(op3, nullptr, 0);
        }

        model.addOperation(ANEURALNETWORKS_ADD, {op0, op1, act}, {op2});
        model.addOperation(ANEURALNETWORKS_TRANSPOSE, {op2, op3}, {op4});
        if (mOptionalType == OptionalType::CONST) {
            model.identifyInputsAndOutputs({op0}, {op4});
        } else {
            model.identifyInputsAndOutputs({op0, op3}, {op4});
        }

        bool expected = expectModelIsValid();
        ASSERT_EQ(model.isValid(), expected);
        Result result = model.finish();
        if (expected) {
            ASSERT_EQ(result, Result::NO_ERROR);
        } else {
            // There is no contract (yet) for specific errors in NeuralNetworks.h,
            // so we just assert on not being successful.
            ASSERT_NE(result, Result::NO_ERROR);
            return;
        }

        // Phase 2: Compile Model, should always pass
        Compilation compilation(&model);
        ASSERT_EQ(compilation.finish(), Result::NO_ERROR);

        std::vector<uint32_t> valueBChoices = {1, 2};
        for (const auto valueB : valueBChoices) {
            SCOPED_TRACE("ValueB: " + std::to_string(valueB));
            if (valueB != kValueB &&
                (mSpecificationLevels[kIndex0_Model] == SpecificationLevel::FULLY_SPECIFIED ||
                 mSpecificationLevels[kIndex2_Model] == SpecificationLevel::FULLY_SPECIFIED ||
                 mSpecificationLevels[kIndex4_Model] == SpecificationLevel::FULLY_SPECIFIED)) {
                continue;
            }

            // Phase 3: Set Execution Input/Output
            Execution execution(&compilation);

            // Set input0
            Result result;
            T bufferOp0[6] = {1, 2, 3, 4, 5, 6};
            SharedMemoryForTest memoryOp0;
            memoryOp0.initialize(sizeof(bufferOp0), bufferOp0);
            result = setInOut<T>(&execution, kIndex0_Execution, 0, {kValueA, valueB}, bufferOp0,
                                 &memoryOp0, InOutType::INPUT);
            ASSERT_EQ(result, expectSetInput0());
            if (result != Result::NO_ERROR) continue;

            // Set input1, omitted
            if (mOptionalType == OptionalType::INPUT) {
                result = setInOut<T>(&execution, kIndex3_Execution, 1, {2}, nullptr, nullptr,
                                     InOutType::INPUT);
                ASSERT_EQ(result, expectSetInput1());
                if (result != Result::NO_ERROR) continue;
            }

            // Set output0
            T bufferOp4[16];
            SharedMemoryForTest memoryOp4;
            memoryOp4.initialize(sizeof(bufferOp4), bufferOp4);
            result = setInOut<T>(&execution, kIndex4_Execution, 0, {valueB, kValueA}, bufferOp4,
                                 &memoryOp4, InOutType::OUTPUT, mOutputBufferSize);
            ASSERT_EQ(result, expectSetOutput0());
            if (result != Result::NO_ERROR) continue;

            // Phase 4: Compute and Compare Results
            result = execution.compute();
            ASSERT_EQ(result, expectCompute());
            if (result == Result::OP_FAILED) continue;

            std::vector<uint32_t> outputShape;
            ASSERT_EQ(execution.getOutputOperandDimensions(0, &outputShape), result);
            std::vector<uint32_t> expectedOutputShape = {valueB, kDimAGood};
            ASSERT_EQ(outputShape, expectedOutputShape);
            if (result == Result::OUTPUT_INSUFFICIENT_SIZE) continue;

            const T* outputBuffer = mOperandLocation == OperandLocation::MEMORY
                                            ? reinterpret_cast<const T*>(memoryOp4.getBuffer())
                                            : bufferOp4;
            T expected_1x2[2] = {2, 4};
            T expected_2x2[4] = {2, 5, 3, 6};
            for (uint32_t i = 0; i < kDimAGood * valueB; i++) {
                ASSERT_EQ(outputBuffer[i], valueB == 1 ? expected_1x2[i] : expected_2x2[i]);
            }
        }
    }

    // Expect invalid model for the following cases
    // - op1 is not fully specified (const operand must be fully specified)
    // - op1 has bad dimension value (const operand size is checked with buffer size)
    bool expectModelIsValid() {
        const auto kLevel1_Model = mSpecificationLevels[kIndex1_Model];
        if (kLevel1_Model != SpecificationLevel::FULLY_SPECIFIED || mBadIndex == kIndex1_Model) {
            return false;
        }
        return true;
    }

    // Expect BAD_DATA on input0 for the following cases
    // - the provided type is not fully specified
    // - the provided type does not agree with the type set at model construction time
    // - no type is provided and the type is not fully specified at model construction time
    Result expectSetInput0() {
        const auto kLevel0_Model = mSpecificationLevels[kIndex0_Model];
        const auto kLevel0_Execution = mSpecificationLevels[kIndex0_Execution];
        switch (kLevel0_Execution) {
            case SpecificationLevel::UNSPECIFIED_DIM:
            case SpecificationLevel::UNSPECIFIED_RANK:
                return Result::BAD_DATA;
            case SpecificationLevel::FULLY_SPECIFIED:
                if ((mBadIndex == kIndex0_Execution || mBadIndex == kIndex0_Model) &&
                    kLevel0_Model != SpecificationLevel::UNSPECIFIED_RANK) {
                    return Result::BAD_DATA;
                }
                break;
            case SpecificationLevel::UNSPECIFIED_TYPE:
                if (kLevel0_Model == SpecificationLevel::UNSPECIFIED_DIM ||
                    kLevel0_Model == SpecificationLevel::UNSPECIFIED_RANK ||
                    mBadIndex == kIndex0_Model) {
                    return Result::BAD_DATA;
                }
                break;
            default:
                break;
        }
        return Result::NO_ERROR;
    }

    // Expect BAD_DATA on input1 for the following cases
    // - the provided type is less detailed as the type set at model construction time
    Result expectSetInput1() {
        const auto kLevel3_Model = mSpecificationLevels[kIndex3_Model];
        const auto kLevel3_Execution = mSpecificationLevels[kIndex3_Execution];
        switch (kLevel3_Execution) {
            case SpecificationLevel::UNSPECIFIED_DIM:
                if (kLevel3_Model == SpecificationLevel::FULLY_SPECIFIED) {
                    return Result::BAD_DATA;
                }
                break;
            case SpecificationLevel::UNSPECIFIED_RANK:
                if (kLevel3_Model != SpecificationLevel::UNSPECIFIED_RANK) {
                    return Result::BAD_DATA;
                }
                break;
            default:
                break;
        }
        return Result::NO_ERROR;
    }

    // Expect BAD_DATA on output0 for the following cases
    // - the provided type is less detailed as the type set at model construction time
    // - the provided type does not agree with the type set at model construction time
    // - the buffer size does not agree with a fully specified type
    Result expectSetOutput0() {
        const auto kLevel4_Model = mSpecificationLevels[kIndex4_Model];
        const auto kLevel4_Execution = mSpecificationLevels[kIndex4_Execution];
        switch (kLevel4_Execution) {
            case SpecificationLevel::UNSPECIFIED_DIM:
                if (kLevel4_Model == SpecificationLevel::FULLY_SPECIFIED ||
                    (kLevel4_Model == SpecificationLevel::UNSPECIFIED_DIM &&
                     (mBadIndex == kIndex4_Model || mBadIndex == kIndex4_Execution))) {
                    return Result::BAD_DATA;
                }
                break;
            case SpecificationLevel::UNSPECIFIED_RANK:
                if (kLevel4_Model != SpecificationLevel::UNSPECIFIED_RANK) {
                    return Result::BAD_DATA;
                }
                break;
            case SpecificationLevel::FULLY_SPECIFIED:
                if (((mBadIndex == kIndex4_Model || mBadIndex == kIndex4_Execution) &&
                     kLevel4_Model != SpecificationLevel::UNSPECIFIED_RANK) ||
                    mOutputBufferSize != BufferSize::EQUAL) {
                    return Result::BAD_DATA;
                }
                break;
            case SpecificationLevel::UNSPECIFIED_TYPE:
                if (kLevel4_Model == SpecificationLevel::FULLY_SPECIFIED &&
                    (mOutputBufferSize != BufferSize::EQUAL || mBadIndex == kIndex4_Model ||
                     mBadIndex == kIndex4_Execution)) {
                    return Result::BAD_DATA;
                }
                break;
            default:
                break;
        }
        return Result::NO_ERROR;
    }

    // Expect failure for the following cases
    // - one of the operands has bad dimension -> OP_FAILED
    // - insufficient output buffer -> OUTPUT_INSUFFICIENT_SIZE
    Result expectCompute() {
        if (mBadIndex < 8) {
            return Result::OP_FAILED;
        } else if (mOutputBufferSize == BufferSize::LESS) {
            return Result::OUTPUT_INSUFFICIENT_SIZE;
        }
        return Result::NO_ERROR;
    }

    // Iterate over combinations of
    // - mBadIndexChoices: which operand has incorrect dimension
    // - mOperandLocationChoices: where the operand reside, buffer or shared memory
    // - mBufferSizeChoices: whether the provided output buffer/memory size is sufficient
    template <typename T, Type TensorType>
    void TestAll() {
        SCOPED_TRACE("Model: " + toString(kSpecificationLevelModel));
        SCOPED_TRACE("Execution: " + toString(kSpecificationLevelExecution));
        mOperandTypes = {TensorType, TensorType, TensorType,         Type::TENSOR_INT32,
                         TensorType, TensorType, Type::TENSOR_INT32, TensorType};
        for (const auto kBadIndex : mBadIndexChoices) {
            SCOPED_TRACE("Bad Index: " + std::to_string(mBadIndex));
            mBadIndex = kBadIndex;
            if (mBadIndex < 8 &&
                (mSpecificationLevels[mBadIndex] == SpecificationLevel::UNSPECIFIED_RANK ||
                 mSpecificationLevels[mBadIndex] == SpecificationLevel::UNSPECIFIED_TYPE)) {
                continue;
            }
            for (const auto kOperandLocation : mOperandLocationChoices) {
                mOperandLocation = kOperandLocation;
                SCOPED_TRACE("Operand Location: " + toString(mOperandLocation));
                for (const auto kOutputBufferSize : mBufferSizeChoices) {
                    mOutputBufferSize = kOutputBufferSize;
                    SCOPED_TRACE("Output Buffer Size: " + toString(mOutputBufferSize));
                    TestOne<T, TensorType>();
                }
            }
        }
    }

    const UnspecifiedOperand kUnspecifiedOperand = std::get<0>(GetParam());
    const SpecificationLevel kSpecificationLevelModel = std::get<1>(GetParam());
    const SpecificationLevel kSpecificationLevelExecution = std::get<2>(GetParam());

    std::vector<SpecificationLevel> mSpecificationLevels;
    std::vector<Type> mOperandTypes;
    OptionalType mOptionalType = OptionalType::CONST;

    // Iterate all combinations in TestAll()
    std::vector<uint32_t> mBadIndexChoices;
    std::vector<OperandLocation> mOperandLocationChoices;
    std::vector<BufferSize> mBufferSizeChoices = {BufferSize::EQUAL};

    uint32_t mBadIndex;
    OperandLocation mOperandLocation;
    BufferSize mOutputBufferSize;
};

TEST_P(UnspecifiedDimensionsTest, Float32) {
    TestAll<float, Type::TENSOR_FLOAT32>();
}

TEST_P(UnspecifiedDimensionsTest, Quant8) {
    TestAll<uint8_t, Type::TENSOR_QUANT8_ASYMM>();
}

TEST_P(UnspecifiedDimensionsTest, Float16) {
    TestAll<_Float16, Type::TENSOR_FLOAT16>();
}

static const auto kAllSpecificationLevelsModel =
        testing::Values(SpecificationLevel::FULLY_SPECIFIED, SpecificationLevel::UNSPECIFIED_DIM,
                        SpecificationLevel::UNSPECIFIED_RANK);
static const auto kAllSpecificationLevelsExecution =
        testing::Values(SpecificationLevel::FULLY_SPECIFIED, SpecificationLevel::UNSPECIFIED_DIM,
                        SpecificationLevel::UNSPECIFIED_RANK, SpecificationLevel::UNSPECIFIED_TYPE);
static const auto kFullySpecified = testing::Values(SpecificationLevel::FULLY_SPECIFIED);

INSTANTIATE_TEST_CASE_P(ModelInputTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::INPUT_MANDATORY),
                                         kAllSpecificationLevelsModel,
                                         kAllSpecificationLevelsExecution));

INSTANTIATE_TEST_CASE_P(ConstantParameterTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::CONST_MANDATORY),
                                         kAllSpecificationLevelsModel, kFullySpecified));

INSTANTIATE_TEST_CASE_P(TemporaryVariableTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::TEMPORARY_VARIABLE),
                                         kAllSpecificationLevelsModel, kFullySpecified));

INSTANTIATE_TEST_CASE_P(OptionalConstantTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::CONST_OPTIONAL),
                                         kAllSpecificationLevelsModel, kFullySpecified));

INSTANTIATE_TEST_CASE_P(OptionalInputTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::INPUT_OPTIONAL),
                                         kAllSpecificationLevelsModel,
                                         kAllSpecificationLevelsExecution));

INSTANTIATE_TEST_CASE_P(ModelOutputTest, UnspecifiedDimensionsTest,
                        testing::Combine(testing::Values(UnspecifiedOperand::OUTPUT),
                                         kAllSpecificationLevelsModel,
                                         kAllSpecificationLevelsExecution));

}  // end namespace
