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

#ifndef ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_OPERATION_MANAGER_H
#define ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_OPERATION_MANAGER_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "TestNeuralNetworksWrapper.h"
#include "fuzzing/RandomGraphGenerator.h"
#include "fuzzing/RandomVariable.h"

namespace android {
namespace nn {
namespace fuzzing_test {

using test_wrapper::Type;

struct OperandSignature {
    // Possible values are [INPUT | CONST | OUTPUT].
    // If CONST, the generator will avoid feeding the operand with another operation’s output.
    RandomOperandType type;

    // The operand constructor is invoked before the operation constructor. This is for
    // setting the data type, quantization parameters, or optionally the scalar value.
    std::function<void(Type, uint32_t, RandomOperand*)> constructor = nullptr;

    // The operand finalizer is invoked after the graph structure is frozen but before the operation
    // finalizer. This is for generating the buffer values for the operand.
    std::function<void(RandomOperand*)> finalizer = nullptr;
};

enum class HalVersion : int32_t { V1_0 = 0, V1_1 = 1, V1_2 = 2 };

// A filter applied to OperationSignatures. An OperationSignature is filtered out if the opType, all
// of supportedDataTypes, all of supportedRanks, or the version does not match with any entry in the
// corresponding vector in the filter. Use empty vector in the filter for "no filter".
//
// E.g.
// - To get all 1.0 ADD operation signatures
//       {.opcodes = {ANEURALNETWORKS_ADD}, .versions = {HalVersion::V1_0}}
//
// - To get all 1.0 and 1.1 operations with rank 2 or 4
//       {.ranks = {2, 4}, .versions = {HalVersion::V1_0, HalVersion::V1_1}}
//
struct OperationFilter {
    std::vector<ANeuralNetworksOperationType> opcodes;
    std::vector<Type> dataTypes;
    std::vector<uint32_t> ranks;
    std::vector<HalVersion> versions;
};

struct OperationSignature {
    // Defines the basic metadata of an operation filterable by OperationFilter.
    // Upon generation, the random graph generator will randomly choose a supported data type and
    // rank, and pass the information to the constructors.
    ANeuralNetworksOperationType opType;
    std::vector<Type> supportedDataTypes;
    std::vector<uint32_t> supportedRanks;
    HalVersion version;

    // OperandSignatures for inputs and outputs.
    std::vector<OperandSignature> inputs;
    std::vector<OperandSignature> outputs;

    // The operation constructor is invoked after the inputs and outputs constructor. This is for
    // setting the dimension relationship of random operands, and/or generating parameter values at
    // the operation level, e.g. a parameter depends on or affects another operand in the same
    // operation.
    std::function<void(Type, uint32_t, RandomOperation*)> constructor = nullptr;

    // The operation finalizer is invoked after the graph structure is frozen and inputs and outputs
    // constructors are invoked. This is for generating operand buffers at the operation level, e.g.
    // an operand buffer that depends on another operand in the same operation.
    std::function<void(RandomOperation*)> finalizer = nullptr;

    // Check if the OperationSignature matches with the filter. Shrink supported* if needed.
    bool matchFilter(const OperationFilter& filter);
};

// A manager manages all available OperationSignatures.
class OperationManager {
   public:
    // Returns the singleton manager.
    static OperationManager* get();

    // Add the signature to the managed mOperationSignatures if matches with mFilter.
    void addSignature(const std::string& name, const OperationSignature& signature);

    // Reinitialize mFilteredSignatures by applying a filter to all of the operation signatures in
    // mOperationSignatures.
    void applyFilter(const OperationFilter& filter);

    // Randomly returns a valid OperationSignature. RandomGraphGenerator uses this method to
    // generate a random list of operations in the graph.
    const OperationSignature& getRandomOperation() const;

   protected:
    OperationManager() = default;

    // Contains all operation signatures defined in operation_signatures/*.cpp.
    // The signatures are sorted by name to ensure a predictable order.
    std::map<std::string, const OperationSignature> mOperationSignatures;

    // A subset of mOperationSignatures using by the random graph generator.
    // This vector should be initialized by applyFilter() before generating the graph.
    std::vector<OperationSignature> mFilteredSignatures;
};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android

#endif  // ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_OPERATION_MANAGER_H
