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

#ifndef ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_VARIABLE_H
#define ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_VARIABLE_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace android {
namespace nn {
namespace fuzzing_test {

static const int kMaxValue = 10000;
static const int kInvalidValue = INT_MIN;

// Describe the search range for the value of a random variable.
class RandomVariableRange {
   public:
    RandomVariableRange() = default;
    explicit RandomVariableRange(int value) : mChoices({value}) {}
    RandomVariableRange(int lower, int upper) : mChoices(upper - lower + 1) {
        std::iota(mChoices.begin(), mChoices.end(), lower);
    }
    explicit RandomVariableRange(const std::vector<int>& vec) : mChoices(vec) {}
    explicit RandomVariableRange(const std::set<int>& st) : mChoices(st.begin(), st.end()) {}
    RandomVariableRange(const RandomVariableRange&) = default;
    RandomVariableRange& operator=(const RandomVariableRange&) = default;

    bool empty() const { return mChoices.empty(); }
    bool has(int value) const {
        return std::binary_search(mChoices.begin(), mChoices.end(), value);
    }
    size_t size() const { return mChoices.size(); }
    int min() const { return *mChoices.begin(); }
    int max() const { return *mChoices.rbegin(); }
    const std::vector<int>& getChoices() const { return mChoices; }

    // Narrow down the range to fit [lower, upper]. Use kInvalidValue to indicate unlimited bound.
    void setRange(int lower, int upper);
    // Narrow down the range to a random selected choice. Return the chosen value.
    int toConst();

    // Calculate the intersection of two ranges.
    friend RandomVariableRange operator&(const RandomVariableRange& lhs,
                                         const RandomVariableRange& rhs);

   private:
    // Always in ascending order.
    std::vector<int> mChoices;
};

// Defines the interface for an operation applying to RandomVariables.
class IRandomVariableOp {
   public:
    virtual ~IRandomVariableOp() {}
    // Forward evaluation of two values.
    virtual int eval(int lhs, int rhs) const = 0;
    // Gets the range of the operation outcomes. The returned range must include all possible
    // outcomes of this operation, but may contain invalid results.
    virtual RandomVariableRange getInitRange(const RandomVariableRange& lhs,
                                             const RandomVariableRange& rhs) const;
    // Provides faster range evaluation for evalSubnetSingleOpHelper if possible.
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const;
    // For debugging purpose.
    virtual const char* getName() const = 0;
};

enum class RandomVariableType { FREE = 0, CONST = 1, OP = 2 };

struct RandomVariableBase {
    // Each RandomVariableBase is assigned an unique index for debugging purpose.
    static unsigned int globalIndex;
    int index;

    RandomVariableType type;
    RandomVariableRange range;
    int value = 0;
    std::shared_ptr<const IRandomVariableOp> op = nullptr;

    // Network structural information.
    std::shared_ptr<RandomVariableBase> parent1 = nullptr;
    std::shared_ptr<RandomVariableBase> parent2 = nullptr;
    std::vector<std::shared_ptr<RandomVariableBase>> children;

    // The last time that this RandomVariableBase is modified.
    int timestamp;

    explicit RandomVariableBase(int value);
    RandomVariableBase(int lower, int upper);
    explicit RandomVariableBase(const std::vector<int>& choices);
    RandomVariableBase(const std::shared_ptr<RandomVariableBase>& lhs,
                       const std::shared_ptr<RandomVariableBase>& rhs,
                       const std::shared_ptr<const IRandomVariableOp>& op);
    RandomVariableBase(const RandomVariableBase&) = delete;
    RandomVariableBase& operator=(const RandomVariableBase&) = delete;

    // Freeze FREE RandomVariable to one valid choice.
    // Should only invoke on FREE RandomVariable.
    void freeze();

    // Get CONST value or calculate from parents.
    // Should not invoke on FREE RandomVariable.
    int getValue() const;

    // Update the timestamp to the latest global time.
    void updateTimestamp();
};

using RandomVariableNode = std::shared_ptr<RandomVariableBase>;

// A wrapper class of RandomVariableBase that manages RandomVariableBase with shared_ptr and
// provides useful methods and operator overloading to build the random variable network.
class RandomVariable {
   public:
    // Construct a dummy RandomVariable with nullptr.
    RandomVariable() : mVar(nullptr) {}

    // Construct a CONST RandomVariable with specified value.
    /* implicit */ RandomVariable(int value);

    // Construct a FREE RandomVariable with range [lower, upper].
    RandomVariable(int lower, int upper);

    // Construct a FREE RandomVariable with specified value choices.
    explicit RandomVariable(const std::vector<int>& choices);

    // This is for RandomVariableType::FREE only.
    // Construct a FREE RandomVariable with default range [1, defaultValue].
    /* implicit */ RandomVariable(RandomVariableType type);

    // RandomVariables share the same RandomVariableBase if copied or copy-assigned.
    RandomVariable(const RandomVariable& other) = default;
    RandomVariable& operator=(const RandomVariable& other) = default;

    // Get the value of the RandomVariable, the value must be deterministic.
    int getValue() const { return mVar->getValue(); }

    // Get the underlying managed RandomVariableNode.
    RandomVariableNode get() const { return mVar; };

    bool operator==(nullptr_t) const { return mVar == nullptr; }
    bool operator!=(nullptr_t) const { return mVar != nullptr; }

    // Arithmetic operators and methods on RandomVariables.
    friend RandomVariable operator+(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable operator-(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable operator*(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable operator*(const RandomVariable& lhs, const float& rhs);
    friend RandomVariable operator/(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable operator%(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable max(const RandomVariable& lhs, const RandomVariable& rhs);
    friend RandomVariable min(const RandomVariable& lhs, const RandomVariable& rhs);
    RandomVariable exactDiv(const RandomVariable& other);

    // Set constraints on the RandomVariable. Use kInvalidValue to indicate unlimited bound.
    void setRange(int lower, int upper);
    RandomVariable setEqual(const RandomVariable& other) const;
    RandomVariable setGreaterThan(const RandomVariable& other) const;
    RandomVariable setGreaterEqual(const RandomVariable& other) const;

    // A FREE RandomVariable is constructed with default range [1, defaultValue].
    static int defaultValue;

   private:
    // Construct a RandomVariable as the result of an OP between two other RandomVariables.
    RandomVariable(const RandomVariable& lhs, const RandomVariable& rhs,
                   const std::shared_ptr<const IRandomVariableOp>& op);
    RandomVariableNode mVar;
};

using EvaluationOrder = std::vector<RandomVariableNode>;

// The base class of a network consisting of disjoint subnets.
class DisjointNetwork {
   public:
    // Add a node to the network, join the parent subnets if needed.
    void add(const RandomVariableNode& var);

    // Similar to join(int, int), but accept RandomVariableNodes.
    int join(const RandomVariableNode& var1, const RandomVariableNode& var2) {
        return DisjointNetwork::join(mIndexMap[var1], mIndexMap[var2]);
    }

   protected:
    DisjointNetwork() = default;
    DisjointNetwork(const DisjointNetwork&) = default;
    DisjointNetwork& operator=(const DisjointNetwork&) = default;

    // Join two subnets by appending every node in ind2 after ind1, return the resulting subnet
    // index. Use -1 for invalid subnet index.
    int join(int ind1, int ind2);

    // A map from the network node to the corresponding subnet index.
    std::unordered_map<RandomVariableNode, int> mIndexMap;

    // A map from the subnet index to the set of nodes within the subnet. The nodes are maintained
    // in a valid evaluation order, that is, a valid topological sort.
    std::unordered_map<int, EvaluationOrder> mEvalOrderMap;

    // The next index for a new disjoint subnet component.
    int mNextIndex = 0;
};

// Manages the active RandomVariable network. Only one instance of this class will exist.
class RandomVariableNetwork : public DisjointNetwork {
   public:
    // Returns the singleton network instance.
    static RandomVariableNetwork* get();

    // Re-initialization. Should be called every time a new random graph is being generated.
    void initialize(int defaultValue);

    // Set the elementwise equality of the two vectors of RandomVariables iff it results in a
    // soluble network.
    bool setEqualIfCompatible(const std::vector<RandomVariable>& lhs,
                              const std::vector<RandomVariable>& rhs);

    // Freeze all FREE RandomVariables in the network to a random valid combination.
    bool freeze();

    // Check if node2 is FREE and can be evaluated after node1.
    bool isSubordinate(const RandomVariableNode& node1, const RandomVariableNode& node2);

    // Get and then advance the current global timestamp.
    int getGlobalTime() { return mGlobalTime++; }

    // Add a special constraint on dimension product.
    void addDimensionProd(const std::vector<RandomVariable>& dims);

   private:
    RandomVariableNetwork() = default;
    RandomVariableNetwork(const RandomVariableNetwork&) = default;
    RandomVariableNetwork& operator=(const RandomVariableNetwork&) = default;

    // A class to revert all the changes made to RandomVariableNetwork since the Reverter object is
    // constructed. Only used when setEqualIfCompatible results in incompatible.
    class Reverter;

    // Find valid choices for all RandomVariables in the network. Update the RandomVariableRange
    // if the network is soluble, otherwise, return false and leave the ranges unchanged.
    bool evalRange();

    int mGlobalTime = 0;
    int mTimestamp = -1;

    std::vector<EvaluationOrder> mDimProd;
};

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android

#endif  // ANDROID_FRAMEWORK_ML_NN_RUNTIME_TEST_FUZZING_RANDOM_VARIABLE_H
