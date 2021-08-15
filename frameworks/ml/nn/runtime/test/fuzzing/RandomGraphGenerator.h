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

#ifndef ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_GRAPH_GENERATOR_H
#define ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_GRAPH_GENERATOR_H

#include <string>
#include <vector>

#include "TestNeuralNetworksWrapper.h"
#include "fuzzing/RandomVariable.h"

namespace android {
namespace nn {
namespace fuzzing_test {

using test_wrapper::Type;
using OperandBuffer = std::vector<int32_t>;

struct OperandSignature;
struct OperationSignature;
class OperationManager;

enum class RandomOperandType { INPUT = 0, OUTPUT = 1, INTERNAL = 2, CONST = 3 };

struct RandomOperand {
    RandomOperandType type;
    Type dataType;
    float scale = 0.0f;
    int32_t zeroPoint = 0;
    std::vector<RandomVariable> dimensions;
    OperandBuffer buffer;
    std::vector<RandomVariable> randomBuffer;

    // The finalizer will be invoked after RandomVariableNetwork::freeze().
    // Operand buffer will be set during this step (if not set before).
    std::function<void(RandomOperand*)> finalizer = nullptr;

    // The index of the operand in the model as returned from model->addOperand(...).
    int32_t opIndex = -1;
    // The index of the input/output as specified in model->identifyInputsAndOutputs(...).
    int32_t ioIndex = -1;

    // If set true, this operand will be ignored during the accuracy checking step.
    bool doNotCheckAccuracy = false;

    // If set true, this operand will not be connected to another operation, e.g. if this operand is
    // an operation output, then it will not be used as an input to another operation, and will
    // eventually end up being a model output.
    bool doNotConnect = false;

    RandomOperand(const OperandSignature& op, Type dataType, uint32_t rank);

    // Resize the underlying operand buffer.
    template <typename T>
    void resizeBuffer(uint32_t len) {
        constexpr size_t valueSize = sizeof(OperandBuffer::value_type);
        uint32_t bufferSize = (sizeof(T) * len + valueSize - 1) / valueSize;
        buffer.resize(bufferSize);
    }

    // Get the operand value as the specified type. The caller is reponsible for making sure that
    // the index is not out of range.
    template <typename T>
    T& value(uint32_t index = 0) {
        return reinterpret_cast<T*>(buffer.data())[index];
    }
    template <>
    RandomVariable& value<RandomVariable>(uint32_t index) {
        return randomBuffer[index];
    }

    // The caller is reponsible for making sure that the operand is indeed a scalar.
    template <typename T>
    void setScalarValue(const T& val) {
        resizeBuffer<T>(/*len=*/1);
        value<T>() = val;
    }

    // Check if a directed edge between [other -> this] is valid. If yes, add the edge.
    // Where "this" must be of type INPUT and "other" must be of type OUTPUT.
    bool createEdgeIfValid(const RandomOperand& other) const;

    // The followings are only intended to be used after RandomVariableNetwork::freeze().
    std::vector<uint32_t> getDimensions() const;
    uint32_t getNumberOfElements() const;
    size_t getBufferSize() const;
};

struct RandomOperation {
    ANeuralNetworksOperationType opType;
    std::vector<std::shared_ptr<RandomOperand>> inputs;
    std::vector<std::shared_ptr<RandomOperand>> outputs;
    std::function<void(RandomOperation*)> finalizer = nullptr;
    RandomOperation(const OperationSignature& operation);
};

// TODO: Consider relative bias and mse on floating point data types?
struct AccuracyCriterion {
    // We expect the driver results to be unbiased.
    // Formula: abs(sum_{i}(diff)) <= bias, where
    // * fixed point: diff = actual - expected
    // * floating point: diff = (actual - expected) / max(1, abs(expected))
    float bias = std::numeric_limits<float>::max();

    // Set the threshold on Mean Square Error (MSE).
    // Formula: sum_{i}(diff ^ 2) / sum(1) <= mse
    float mse = std::numeric_limits<float>::max();

    // We also set accuracy thresholds on each element to detect any particular edge cases that may
    // be shadowed in bias or MSE. We use the similar approach as our CTS unit tests, but with much
    // relaxed criterion.
    // Formula: abs(actual - expected) <= atol + rtol * abs(expected)
    //   where atol stands for Absolute TOLerance and rtol for Relative TOLerance.
    float atol = 0.0f;
    float rtol = 0.0f;
};

struct AccuracyCriteria {
    AccuracyCriterion float32;
    AccuracyCriterion float16;
    AccuracyCriterion int32;
    AccuracyCriterion quant8Asymm;
    AccuracyCriterion quant8Symm;
    AccuracyCriterion quant16Asymm;
    AccuracyCriterion quant16Symm;
};

// The main interface of the random graph generator.
class RandomGraph {
   public:
    RandomGraph() = default;

    // Generate a random graph with numOperations and dimensionRange from a seed.
    bool generate(uint32_t seed, uint32_t numOperations, uint32_t dimensionRange);

    // Create a NDK model from the random graph.
    void createModel(test_wrapper::Model* model);

    // Set the input/output buffers to an NDK execution object. The input buffer resides in
    // RandomOperand.buffer, the output buffer is either provided by "buffers" argument, or set
    // buffers to nullptr to use RandomOperand.buffer to record reference result.
    void createRequest(test_wrapper::Execution* execution,
                       std::vector<OperandBuffer>* buffers = nullptr);

    // Check if the results in buffers meet the given accuracy criteria.
    void checkResults(const std::vector<OperandBuffer>& buffers,
                      const AccuracyCriteria& criteria) const;

    // Dump the generated random graph to a spec file for debugging and visualization purpose.
    void dumpSpecFile(std::string filename, std::string testname);

    const std::vector<RandomOperation>& getOperations() const { return mOperations; }

   private:
    // Generate the graph structure.
    bool generateGraph(uint32_t numOperations);

    // Fill in random values for dimensions, constants, and inputs.
    bool generateValue();

    std::vector<RandomOperation> mOperations;
    std::vector<std::shared_ptr<RandomOperand>> mOperands;
};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android

#endif  // ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_GRAPH_GENERATOR_H
