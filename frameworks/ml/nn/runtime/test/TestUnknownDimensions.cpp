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

#include "TestHarness.h"
#include "TestNeuralNetworksWrapper.h"

#include <gtest/gtest.h>

#include <tuple>
#include <vector>

using namespace android::nn::test_wrapper;
using namespace test_helper;

namespace {

const uint32_t INTENDED_SIZE = 3;
const uint32_t OTHER_SIZE    = 2;
const uint32_t UNKNOWN_SIZE  = 0;

// We test three basic scenarios for each tensor dimension:
//     INTENDED_AT_COMPILE_AND_EXECUTE: set the dimension at compile
//     (addOperand) time to INTENDED_SIZE, use same size at execution
//     (setInput/setOutput) time. This should always work.
//
//     INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE: set the dimension at compile
//     (addOperand) time to INTENDED_SIZE, give no size at execution time.
//     This should always work.
//
//     UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE: don't set the dimension at
//     compile (addOperand) time, use INTENDED_SIZE at execute
//     (setInput/setOutput) time. Note for constants, this just means using an
//     unknown dimension at addOperand as there is no type parameter to
//     setOperandValue. This should work for inputs and outputs and give an
//     error for constants at compile time.
//
//     UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE: don't set the dimension at compile
//     (addOperand) time, use OTHER_SIZE at execute (setInput/setOutput) time.
//     This should give an error at execute time (as the constant value will
//     have a different size).
//
// All relevant combinations of the basic scenarios are then iterated over in
// TestAll. Note that we don't want to just use googletest's parametrized tests (TEST_P) as
// the 16k combinations generated too many lines of output for the test
// infrastructure to handle correctly. However, running all 16k in one test
// makes the ASAN version take so long that the automatic test runner things the
// command has become unresponsinve, so we split on the first level.
enum class DimensionKind { INTENDED_AT_COMPILE_AND_EXECUTE,
                           INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE,
                           UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE,
                           UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE };
typedef std::tuple<DimensionKind, DimensionKind> OperandParams;
std::vector<DimensionKind> ioDimensionValues = {
    DimensionKind::INTENDED_AT_COMPILE_AND_EXECUTE,
    DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE,
    DimensionKind::UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE,
    DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE };
std::vector<DimensionKind> constantDimensionValues = {
        DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE,
        DimensionKind::UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE };
std::vector<OperandParams> Combine(const std::vector<DimensionKind>& firsts,
                                   const std::vector<DimensionKind>& seconds);
auto ioValues = Combine(ioDimensionValues, ioDimensionValues);
auto constantValues = Combine(constantDimensionValues, constantDimensionValues);

class UnknownDimensionsTest : public ::testing::TestWithParam<OperandParams> {
   protected:
    template <class T, Type TensorType>
    void TestOne(const OperandParams& paramsForInput0, const OperandParams& paramsForInput1,
                 const OperandParams& paramsForConst, const OperandParams& paramsForOutput);
    template <class T, Type TensorType>
    void TestAll();

    template <typename T>
    void CompareResults(std::map<int, std::vector<T>>& expected,
                        std::map<int, std::vector<T>>& actual);
};

template <>
void UnknownDimensionsTest::CompareResults<float>(std::map<int, std::vector<float>>& golden,
                                                  std::map<int, std::vector<float>>& test) {
    size_t totalNumberOfErrors = 0;
    float fpAtol = 1e-5f, fpRtol = 1e-5f;
    compare_<float>(golden, test,
                    [&totalNumberOfErrors, fpAtol, fpRtol](float expected, float actual) {
                        // Compute the range based on both absolute tolerance and relative tolerance
                        float fpRange = fpAtol + fpRtol * std::abs(expected);
                        if (totalNumberOfErrors < gMaximumNumberOfErrorMessages) {
                            EXPECT_NEAR(expected, actual, fpRange);
                        }
                        if (std::abs(expected - actual) > fpRange) {
                            totalNumberOfErrors++;
                        }
                    });
    EXPECT_EQ(size_t{0}, totalNumberOfErrors);
}

template <>
void UnknownDimensionsTest::CompareResults<uint8_t>(std::map<int, std::vector<uint8_t>>& golden,
                                                    std::map<int, std::vector<uint8_t>>& test) {
    size_t totalNumberOfErrors = 0;
    compare_<uint8_t>(golden, test, [&totalNumberOfErrors](uint8_t expected, uint8_t actual) {
        if (totalNumberOfErrors < gMaximumNumberOfErrorMessages) {
            EXPECT_NEAR(expected, actual, 1);
        }
        if (std::abs(expected - actual) > 1) {
            totalNumberOfErrors++;
        }
    });
    EXPECT_EQ(size_t{0}, totalNumberOfErrors);
}

template <>
void UnknownDimensionsTest::CompareResults<_Float16>(std::map<int, std::vector<_Float16>>& golden,
                                                     std::map<int, std::vector<_Float16>>& test) {
    size_t totalNumberOfErrors = 0;
    float fpAtol = 5.0f * 0.0009765625f, fpRtol = 5.0f * 0.0009765625f;
    compare_<_Float16>(golden, test,
                       [&totalNumberOfErrors, fpAtol, fpRtol](_Float16 expected, _Float16 actual) {
                           // Compute the range based on both absolute tolerance and relative
                           // tolerance
                           float fpRange = fpAtol + fpRtol * std::abs(static_cast<float>(expected));
                           if (totalNumberOfErrors < gMaximumNumberOfErrorMessages) {
                               EXPECT_NEAR(expected, actual, fpRange);
                           }
                           if (std::abs(static_cast<float>(expected - actual)) > fpRange) {
                               totalNumberOfErrors++;
                           }
                       });
    EXPECT_EQ(size_t{0}, totalNumberOfErrors);
}

template<class T, Type TensorType> void UnknownDimensionsTest::TestOne(
        const OperandParams& paramsForInput0, const OperandParams& paramsForInput1,
        const OperandParams& paramsForConst, const OperandParams& paramsForOutput) {
    typedef T IntendedMatrix[INTENDED_SIZE][INTENDED_SIZE];
    static const IntendedMatrix ones = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
    static const IntendedMatrix twos = { { 2, 2, 2 }, { 2, 2, 2 }, { 2, 2, 2 } };
    static const IntendedMatrix fives = { { 5, 5, 5 }, { 5, 5, 5 }, { 5, 5, 5 } };
    const float scale = TensorType == Type::TENSOR_QUANT8_ASYMM ? 1.f : 0.f;

    Model model;
    std::string input0Scope("Input 0:"), input1Scope("Input 1:"),
                constantScope("Constant:"), outputScope("Output:");

    auto getDimForCompile = [](DimensionKind kind, std::string* scope) {
        switch (kind) {
            case DimensionKind::INTENDED_AT_COMPILE_AND_EXECUTE:
                if (scope) scope->append(" INTENDED_AT_COMPILE_AND_EXECUTE");
                return INTENDED_SIZE;
            case DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE:
                if (scope) scope->append(" INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE");
                return INTENDED_SIZE;
            case DimensionKind::UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE:
                if (scope) scope->append(" UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE");
                return UNKNOWN_SIZE;
            case DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE:
                if (scope) scope->append(" UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE");
                return UNKNOWN_SIZE;
        }
    };
    auto addOperand = [&model, &getDimForCompile, scale](OperandParams params,
                                                         std::string* scope = nullptr) {
        OperandType matrixTypeWithPotentiallyUnknownDims(
                TensorType,
                { getDimForCompile(std::get<0>(params), scope),
                  getDimForCompile(std::get<1>(params), scope) },
                scale);
        return model.addOperand(&matrixTypeWithPotentiallyUnknownDims);
    };
    auto inputOpd0 = addOperand(paramsForInput0, &input0Scope);
    auto inputOpd1 = addOperand(paramsForInput1, &input1Scope);
    auto intermediateOpd0 = addOperand(OperandParams{
            // Dimensions for intermediate operand actually deduced at execution time
            DimensionKind::UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE,
            DimensionKind::UNKNOWN_AT_COMPILE_INTENDED_AT_EXECUTE});
    auto constantOpd0 = addOperand(paramsForConst, &constantScope);
    auto outputOpd0 = addOperand(paramsForOutput, &outputScope);

    // Make the gtest failure easier to read
    SCOPED_TRACE(input0Scope);
    SCOPED_TRACE(input1Scope);
    SCOPED_TRACE(constantScope);
    SCOPED_TRACE(outputScope);

    OperandType scalarType(Type::INT32, {});
    int32_t activation(ANEURALNETWORKS_FUSED_NONE);
    auto activationOpd0 = model.addOperand(&scalarType);

    model.setOperandValue(activationOpd0, &activation, sizeof(activation));
    model.setOperandValue(constantOpd0, twos, sizeof(twos));
    model.addOperation(ANEURALNETWORKS_ADD,
                       {inputOpd0, inputOpd1, activationOpd0},
                       {intermediateOpd0});
    model.addOperation(ANEURALNETWORKS_ADD,
                       {intermediateOpd0, constantOpd0, activationOpd0},
                       {outputOpd0});
    model.identifyInputsAndOutputs({inputOpd0, inputOpd1}, {outputOpd0});
    if (std::get<0>(paramsForConst) == DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE &&
        std::get<1>(paramsForConst) == DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE) {
        ASSERT_TRUE(model.isValid());
        ASSERT_EQ(model.finish(), Result::NO_ERROR);
    } else {
        ASSERT_FALSE(model.isValid());
        // There is no contract (yet) for specific errors in NeuralNetworks.h,
        // so we just assert on not being successful.
        ASSERT_NE(model.finish(), Result::NO_ERROR);
        return;
    }

    Compilation compilation(&model);
    ASSERT_EQ(compilation.finish(), Result::NO_ERROR);

    IntendedMatrix actual = { { 10, 10, 10 }, { 10, 10, 10 }, { 10, 10, 10 } };
    Execution execution(&compilation);

    OperandType matrixTypeIntended(TensorType, {INTENDED_SIZE, INTENDED_SIZE}, scale);
    OperandType matrixTypeFirstOther(TensorType, {OTHER_SIZE, INTENDED_SIZE}, scale);
    OperandType matrixTypeSecondOther(TensorType, {INTENDED_SIZE, OTHER_SIZE}, scale);
    OperandType matrixTypeBothOther(TensorType, {OTHER_SIZE, OTHER_SIZE}, scale);
    bool allAreIntendedSizeAtExecution = true;

    // Helper to return appropriate "type" parameter to setInput/setOutput based
    // on OperandParams
    auto typeAtSet = [&](OperandParams params) {
        auto first = std::get<0>(params), second = std::get<1>(params);
        if (first == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE &&
            second == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE) {
            allAreIntendedSizeAtExecution = false;
            return &matrixTypeBothOther.operandType;
        } else if (first == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE) {
            allAreIntendedSizeAtExecution = false;
            return &matrixTypeFirstOther.operandType;
        } else if (second == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE) {
            allAreIntendedSizeAtExecution = false;
            return &matrixTypeSecondOther.operandType;
        } else if (first == DimensionKind::INTENDED_AT_COMPILE_AND_EXECUTE &&
                   second == DimensionKind::INTENDED_AT_COMPILE_AND_EXECUTE) {
            return &matrixTypeIntended.operandType;
        } else if (first == DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE &&
                   second == DimensionKind::INTENDED_AT_COMPILE_NOT_SET_AT_EXECUTE) {
            return static_cast<ANeuralNetworksOperandType*>(nullptr);
        } else {
            return &matrixTypeIntended.operandType;
        }
    };
    // Helper to return appropriate "size" parameter to setInput/setOutput based
    // on OperandParams
    auto sizeAtSet = [](OperandParams params) {
        auto first = std::get<0>(params), second = std::get<1>(params);
        size_t firstDim = (first == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE) ?
            OTHER_SIZE : INTENDED_SIZE;
        size_t secondDim = (second == DimensionKind::UNKNOWN_AT_COMPILE_OTHER_AT_EXECUTE) ?
            OTHER_SIZE : INTENDED_SIZE;
        return firstDim * secondDim * sizeof(fives[0][0]);
    };
    ASSERT_EQ(execution.setInput(0, ones, sizeAtSet(paramsForInput0), typeAtSet(paramsForInput0)),
              Result::NO_ERROR);
    ASSERT_EQ(execution.setInput(1, twos, sizeAtSet(paramsForInput1), typeAtSet(paramsForInput1)),
              Result::NO_ERROR);
    ASSERT_EQ(execution.setOutput(0, actual, sizeAtSet(paramsForOutput),
                                  typeAtSet(paramsForOutput)),
              Result::NO_ERROR);

    if (allAreIntendedSizeAtExecution) {
        ASSERT_EQ(execution.compute(), Result::NO_ERROR);
    } else {
        // There is no contract (yet) for specific errors in NeuralNetworks.h,
        // so we just assert on not being successful.
        ASSERT_NE(execution.compute(), Result::NO_ERROR);
        return;
    }

    typedef std::vector<T> vec;
    typedef std::map<int, vec> Operands;
    constexpr size_t count = sizeof(fives) / sizeof(fives[0][0]);
    Operands expected_opds{{0, vec{&fives[0][0], &fives[0][0] + count}}};
    Operands actual_opds{{0, vec{&actual[0][0], &actual[0][0] + count}}};
    CompareResults(expected_opds, actual_opds);
}

std::vector<OperandParams> Combine(const std::vector<DimensionKind>& firsts,
                                   const std::vector<DimensionKind>& seconds) {
    std::vector<OperandParams> ret;
    for (auto first: firsts) {
        for (auto second: seconds) {
            ret.push_back({first, second});
        }
    }
    return ret;
}

template<class T, Type TensorType> void UnknownDimensionsTest::TestAll() {
    const OperandParams paramsForInput0 = GetParam();
    for (auto paramsForInput1: ioValues) {
        for (auto paramsForConst: constantValues) {
            for (auto paramsForOutput: ioValues) {
                TestOne<T, TensorType>(paramsForInput0, paramsForInput1,
                                       paramsForConst, paramsForOutput);
            }
        }
    }
}

TEST_P(UnknownDimensionsTest, Float) {
    TestAll<float, Type::TENSOR_FLOAT32>();
}

TEST_P(UnknownDimensionsTest, Quantized) {
    TestAll<uint8_t, Type::TENSOR_QUANT8_ASYMM>();
}

TEST_P(UnknownDimensionsTest, Float16) {
    TestAll<_Float16, Type::TENSOR_FLOAT16>();
}

INSTANTIATE_TEST_CASE_P(UnknownCombinationsTest, UnknownDimensionsTest,
                        ::testing::ValuesIn(ioValues));
}  // end namespace
