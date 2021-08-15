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

#include "RandomVariable.h"

#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "RandomGraphGeneratorUtils.h"

namespace android {
namespace nn {
namespace fuzzing_test {

unsigned int RandomVariableBase::globalIndex = 0;
int RandomVariable::defaultValue = 10;

RandomVariableBase::RandomVariableBase(int value)
    : index(globalIndex++),
      type(RandomVariableType::CONST),
      range(value),
      value(value),
      timestamp(RandomVariableNetwork::get()->getGlobalTime()) {}

RandomVariableBase::RandomVariableBase(int lower, int upper)
    : index(globalIndex++),
      type(RandomVariableType::FREE),
      range(lower, upper),
      timestamp(RandomVariableNetwork::get()->getGlobalTime()) {}

RandomVariableBase::RandomVariableBase(const std::vector<int>& choices)
    : index(globalIndex++),
      type(RandomVariableType::FREE),
      range(choices),
      timestamp(RandomVariableNetwork::get()->getGlobalTime()) {}

RandomVariableBase::RandomVariableBase(const RandomVariableNode& lhs, const RandomVariableNode& rhs,
                                       const std::shared_ptr<const IRandomVariableOp>& op)
    : index(globalIndex++),
      type(RandomVariableType::OP),
      range(op->getInitRange(lhs->range, rhs == nullptr ? RandomVariableRange(0) : rhs->range)),
      op(op),
      parent1(lhs),
      parent2(rhs),
      timestamp(RandomVariableNetwork::get()->getGlobalTime()) {}

void RandomVariableRange::setRange(int lower, int upper) {
    // kInvalidValue indicates unlimited bound.
    auto head = lower == kInvalidValue ? mChoices.begin()
                                       : std::lower_bound(mChoices.begin(), mChoices.end(), lower);
    auto tail = upper == kInvalidValue ? mChoices.end()
                                       : std::upper_bound(mChoices.begin(), mChoices.end(), upper);
    NN_FUZZER_CHECK(head <= tail) << "Invalid range!";
    if (head != mChoices.begin() || tail != mChoices.end()) {
        mChoices = std::vector<int>(head, tail);
    }
}

int RandomVariableRange::toConst() {
    if (mChoices.size() > 1) mChoices = {getRandomChoice(mChoices)};
    return mChoices[0];
}

RandomVariableRange operator&(const RandomVariableRange& lhs, const RandomVariableRange& rhs) {
    std::vector<int> result(lhs.size() + rhs.size());
    auto it = std::set_intersection(lhs.mChoices.begin(), lhs.mChoices.end(), rhs.mChoices.begin(),
                                    rhs.mChoices.end(), result.begin());
    result.resize(it - result.begin());
    return RandomVariableRange(std::move(result));
}

void RandomVariableBase::freeze() {
    if (type == RandomVariableType::CONST) return;
    value = range.toConst();
    type = RandomVariableType::CONST;
}

int RandomVariableBase::getValue() const {
    switch (type) {
        case RandomVariableType::CONST:
            return value;
        case RandomVariableType::OP:
            return op->eval(parent1->getValue(), parent2 == nullptr ? 0 : parent2->getValue());
        default:
            NN_FUZZER_CHECK(false) << "Invalid type when getting value of var" << index;
            return 0;
    }
}

void RandomVariableBase::updateTimestamp() {
    timestamp = RandomVariableNetwork::get()->getGlobalTime();
    NN_FUZZER_LOG << "Update timestamp of var" << index << " to " << timestamp;
}

RandomVariable::RandomVariable(int value) : mVar(new RandomVariableBase(value)) {
    NN_FUZZER_LOG << "New RandomVariable " << toString(mVar);
    RandomVariableNetwork::get()->add(mVar);
}
RandomVariable::RandomVariable(int lower, int upper) : mVar(new RandomVariableBase(lower, upper)) {
    NN_FUZZER_LOG << "New RandomVariable " << toString(mVar);
    RandomVariableNetwork::get()->add(mVar);
}
RandomVariable::RandomVariable(const std::vector<int>& choices)
    : mVar(new RandomVariableBase(choices)) {
    NN_FUZZER_LOG << "New RandomVariable " << toString(mVar);
    RandomVariableNetwork::get()->add(mVar);
}
RandomVariable::RandomVariable(RandomVariableType type)
    : mVar(new RandomVariableBase(1, defaultValue)) {
    NN_FUZZER_CHECK(type == RandomVariableType::FREE);
    NN_FUZZER_LOG << "New RandomVariable " << toString(mVar);
    RandomVariableNetwork::get()->add(mVar);
}
RandomVariable::RandomVariable(const RandomVariable& lhs, const RandomVariable& rhs,
                               const std::shared_ptr<const IRandomVariableOp>& op)
    : mVar(new RandomVariableBase(lhs.get(), rhs.get(), op)) {
    // Make a copy if the parent is CONST. This will resolve the fake dependency problem.
    if (mVar->parent1->type == RandomVariableType::CONST) {
        mVar->parent1 = RandomVariable(mVar->parent1->value).get();
    }
    if (mVar->parent2 != nullptr && mVar->parent2->type == RandomVariableType::CONST) {
        mVar->parent2 = RandomVariable(mVar->parent2->value).get();
    }
    mVar->parent1->children.push_back(mVar);
    if (mVar->parent2 != nullptr) mVar->parent2->children.push_back(mVar);
    RandomVariableNetwork::get()->add(mVar);
    NN_FUZZER_LOG << "New RandomVariable " << toString(mVar);
}

void RandomVariable::setRange(int lower, int upper) {
    NN_FUZZER_CHECK(mVar != nullptr) << "setRange() on nullptr";
    NN_FUZZER_LOG << "Set range [" << lower << ", " << upper << "] on var" << mVar->index;
    size_t oldSize = mVar->range.size();
    mVar->range.setRange(lower, upper);
    // Only update the timestamp if the range is *indeed* narrowed down.
    if (mVar->range.size() != oldSize) mVar->updateTimestamp();
}

RandomVariableRange IRandomVariableOp::getInitRange(const RandomVariableRange& lhs,
                                                    const RandomVariableRange& rhs) const {
    std::set<int> st;
    for (auto i : lhs.getChoices()) {
        for (auto j : rhs.getChoices()) {
            int res = this->eval(i, j);
            if (res > kMaxValue || res < -kMaxValue) continue;
            st.insert(res);
        }
    }
    return RandomVariableRange(st);
}

// Check if the range contains exactly all values in [min, max].
static inline bool isContinuous(const std::set<int>* range) {
    return (*(range->rbegin()) - *(range->begin()) + 1) == static_cast<int>(range->size());
}

// Fill the set with a range of values specified by [lower, upper].
static inline void fillRange(std::set<int>* range, int lower, int upper) {
    for (int i = lower; i <= upper; i++) range->insert(i);
}

// The slowest algorithm: iterate through every combinations of parents and save the valid pairs.
void IRandomVariableOp::eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                             const std::set<int>* childIn, std::set<int>* parent1Out,
                             std::set<int>* parent2Out, std::set<int>* childOut) const {
    // Avoid the binary search if the child is a closed range.
    bool isChildInContinuous = isContinuous(childIn);
    std::pair<int, int> child = {*childIn->begin(), *childIn->rbegin()};
    for (auto i : *parent1In) {
        bool valid = false;
        for (auto j : *parent2In) {
            int res = this->eval(i, j);
            // Avoid the binary search if obviously out of range.
            if (res > child.second || res < child.first) continue;
            if (isChildInContinuous || childIn->find(res) != childIn->end()) {
                parent2Out->insert(j);
                childOut->insert(res);
                valid = true;
            }
        }
        if (valid) parent1Out->insert(i);
    }
}

// A helper template to make a class into a Singleton.
template <class T>
class Singleton : public T {
   public:
    static const std::shared_ptr<const T>& get() {
        static std::shared_ptr<const T> instance(new T);
        return instance;
    }
};

// A set of operations that only compute on a single input value.
class IUnaryOp : public IRandomVariableOp {
   public:
    using IRandomVariableOp::eval;
    virtual int eval(int val) const = 0;
    virtual int eval(int lhs, int) const override { return eval(lhs); }
    // The slowest algorithm: iterate through every value of the parent and save the valid one.
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        NN_FUZZER_CHECK(parent2In == nullptr);
        NN_FUZZER_CHECK(parent2Out == nullptr);
        bool isChildInContinuous = isContinuous(childIn);
        std::pair<int, int> child = {*childIn->begin(), *childIn->rbegin()};
        for (auto i : *parent1In) {
            int res = this->eval(i);
            if (res > child.second || res < child.first) continue;
            if (isChildInContinuous || childIn->find(res) != childIn->end()) {
                parent1Out->insert(i);
                childOut->insert(res);
            }
        }
    }
};

// A set of operations that only check conditional constraints.
class IConstraintOp : public IRandomVariableOp {
   public:
    using IRandomVariableOp::eval;
    virtual bool check(int lhs, int rhs) const = 0;
    virtual int eval(int lhs, int rhs) const override {
        return check(lhs, rhs) ? 0 : kInvalidValue;
    }
    // The range for a constraint op is always {0}.
    virtual RandomVariableRange getInitRange(const RandomVariableRange&,
                                             const RandomVariableRange&) const override {
        return RandomVariableRange(0);
    }
    // The slowest algorithm:
    // iterate through every combinations of parents and save the valid pairs.
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>*, std::set<int>* parent1Out, std::set<int>* parent2Out,
                      std::set<int>* childOut) const override {
        for (auto i : *parent1In) {
            bool valid = false;
            for (auto j : *parent2In) {
                if (this->check(i, j)) {
                    parent2Out->insert(j);
                    valid = true;
                }
            }
            if (valid) parent1Out->insert(i);
        }
        if (!parent1Out->empty()) childOut->insert(0);
    }
};

class Addition : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override { return lhs + rhs; }
    virtual RandomVariableRange getInitRange(const RandomVariableRange& lhs,
                                             const RandomVariableRange& rhs) const override {
        return RandomVariableRange(lhs.min() + rhs.min(), lhs.max() + rhs.max());
    }
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        if (!isContinuous(parent1In) || !isContinuous(parent2In) || !isContinuous(childIn)) {
            IRandomVariableOp::eval(parent1In, parent2In, childIn, parent1Out, parent2Out,
                                    childOut);
        } else {
            // For parents and child with close range, the out range can be computed directly
            // without iterations.
            std::pair<int, int> parent1 = {*parent1In->begin(), *parent1In->rbegin()};
            std::pair<int, int> parent2 = {*parent2In->begin(), *parent2In->rbegin()};
            std::pair<int, int> child = {*childIn->begin(), *childIn->rbegin()};

            // From ranges for parent, evalute range for child.
            // [a, b] + [c, d] -> [a + c, b + d]
            fillRange(childOut, std::max(child.first, parent1.first + parent2.first),
                      std::min(child.second, parent1.second + parent2.second));

            // From ranges for child and one parent, evalute range for another parent.
            // [a, b] - [c, d] -> [a - d, b - c]
            fillRange(parent1Out, std::max(parent1.first, child.first - parent2.second),
                      std::min(parent1.second, child.second - parent2.first));
            fillRange(parent2Out, std::max(parent2.first, child.first - parent1.second),
                      std::min(parent2.second, child.second - parent1.first));
        }
    }
    virtual const char* getName() const override { return "ADD"; }
};

class Subtraction : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override { return lhs - rhs; }
    virtual RandomVariableRange getInitRange(const RandomVariableRange& lhs,
                                             const RandomVariableRange& rhs) const override {
        return RandomVariableRange(lhs.min() - rhs.max(), lhs.max() - rhs.min());
    }
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        if (!isContinuous(parent1In) || !isContinuous(parent2In) || !isContinuous(childIn)) {
            IRandomVariableOp::eval(parent1In, parent2In, childIn, parent1Out, parent2Out,
                                    childOut);
        } else {
            // Similar algorithm as Addition.
            std::pair<int, int> parent1 = {*parent1In->begin(), *parent1In->rbegin()};
            std::pair<int, int> parent2 = {*parent2In->begin(), *parent2In->rbegin()};
            std::pair<int, int> child = {*childIn->begin(), *childIn->rbegin()};
            fillRange(childOut, std::max(child.first, parent1.first - parent2.second),
                      std::min(child.second, parent1.second - parent2.first));
            fillRange(parent1Out, std::max(parent1.first, child.first + parent2.first),
                      std::min(parent1.second, child.second + parent2.second));
            fillRange(parent2Out, std::max(parent2.first, parent1.first - child.second),
                      std::min(parent2.second, parent1.second - child.first));
        }
    }
    virtual const char* getName() const override { return "SUB"; }
};

class Multiplication : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override { return lhs * rhs; }
    virtual RandomVariableRange getInitRange(const RandomVariableRange& lhs,
                                             const RandomVariableRange& rhs) const override {
        if (lhs.min() < 0 || rhs.min() < 0) {
            return IRandomVariableOp::getInitRange(lhs, rhs);
        } else {
            int lower = std::min(lhs.min() * rhs.min(), kMaxValue);
            int upper = std::min(lhs.max() * rhs.max(), kMaxValue);
            return RandomVariableRange(lower, upper);
        }
    }
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        if (*parent1In->begin() < 0 || *parent2In->begin() < 0 || *childIn->begin() < 0) {
            IRandomVariableOp::eval(parent1In, parent2In, childIn, parent1Out, parent2Out,
                                    childOut);
        } else {
            bool isChildInContinuous = isContinuous(childIn);
            std::pair<int, int> child = {*childIn->begin(), *childIn->rbegin()};
            for (auto i : *parent1In) {
                bool valid = false;
                for (auto j : *parent2In) {
                    int res = this->eval(i, j);
                    // Since MUL increases monotonically with one value, break the loop if the
                    // result is larger than the limit.
                    if (res > child.second) break;
                    if (res < child.first) continue;
                    if (isChildInContinuous || childIn->find(res) != childIn->end()) {
                        valid = true;
                        parent2Out->insert(j);
                        childOut->insert(res);
                    }
                }
                if (valid) parent1Out->insert(i);
            }
        }
    }
    virtual const char* getName() const override { return "MUL"; }
};

class Division : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override {
        return rhs == 0 ? kInvalidValue : lhs / rhs;
    }
    virtual RandomVariableRange getInitRange(const RandomVariableRange& lhs,
                                             const RandomVariableRange& rhs) const override {
        if (lhs.min() < 0 || rhs.min() <= 0) {
            return IRandomVariableOp::getInitRange(lhs, rhs);
        } else {
            return RandomVariableRange(lhs.min() / rhs.max(), lhs.max() / rhs.min());
        }
    }
    virtual const char* getName() const override { return "DIV"; }
};

class ExactDivision : public Division {
   public:
    virtual int eval(int lhs, int rhs) const override {
        return (rhs == 0 || lhs % rhs != 0) ? kInvalidValue : lhs / rhs;
    }
    virtual const char* getName() const override { return "EXACT_DIV"; }
};

class Modulo : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override {
        return rhs == 0 ? kInvalidValue : lhs % rhs;
    }
    virtual RandomVariableRange getInitRange(const RandomVariableRange&,
                                             const RandomVariableRange& rhs) const override {
        return RandomVariableRange(0, rhs.max());
    }
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        if (*childIn->begin() != 0 || childIn->size() != 1u) {
            IRandomVariableOp::eval(parent1In, parent2In, childIn, parent1Out, parent2Out,
                                    childOut);
        } else {
            // For the special case that child is a const 0, it would be faster if the range for
            // parents are evaluated separately.

            // Evalute parent1 directly.
            for (auto i : *parent1In) {
                for (auto j : *parent2In) {
                    if (i % j == 0) {
                        parent1Out->insert(i);
                        break;
                    }
                }
            }
            // Evalute parent2, see if a multiple of parent2 value can be found in parent1.
            int parent1Max = *parent1In->rbegin();
            for (auto i : *parent2In) {
                int jMax = parent1Max / i;
                for (int j = 1; j <= jMax; j++) {
                    if (parent1In->find(i * j) != parent1In->end()) {
                        parent2Out->insert(i);
                        break;
                    }
                }
            }
            if (!parent1Out->empty()) childOut->insert(0);
        }
    }
    virtual const char* getName() const override { return "MOD"; }
};

class Maximum : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override { return std::max(lhs, rhs); }
    virtual const char* getName() const override { return "MAX"; }
};

class Minimum : public IRandomVariableOp {
   public:
    virtual int eval(int lhs, int rhs) const override { return std::min(lhs, rhs); }
    virtual const char* getName() const override { return "MIN"; }
};

class Square : public IUnaryOp {
   public:
    virtual int eval(int val) const override { return val * val; }
    virtual const char* getName() const override { return "SQUARE"; }
};

class UnaryEqual : public IUnaryOp {
   public:
    virtual int eval(int val) const override { return val; }
    virtual const char* getName() const override { return "UNARY_EQUAL"; }
};

class Equal : public IConstraintOp {
   public:
    virtual bool check(int lhs, int rhs) const override { return lhs == rhs; }
    virtual void eval(const std::set<int>* parent1In, const std::set<int>* parent2In,
                      const std::set<int>* childIn, std::set<int>* parent1Out,
                      std::set<int>* parent2Out, std::set<int>* childOut) const override {
        NN_FUZZER_CHECK(childIn->size() == 1u && *childIn->begin() == 0);
        // The intersection of two sets can be found in O(n).
        std::set_intersection(parent1In->begin(), parent1In->end(), parent2In->begin(),
                              parent2In->end(), std::inserter(*parent1Out, parent1Out->begin()));
        *parent2Out = *parent1Out;
        childOut->insert(0);
    }
    virtual const char* getName() const override { return "EQUAL"; }
};

class GreaterThan : public IConstraintOp {
   public:
    virtual bool check(int lhs, int rhs) const override { return lhs > rhs; }
    virtual const char* getName() const override { return "GREATER_THAN"; }
};

class GreaterEqual : public IConstraintOp {
   public:
    virtual bool check(int lhs, int rhs) const override { return lhs >= rhs; }
    virtual const char* getName() const override { return "GREATER_EQUAL"; }
};

class FloatMultiplication : public IUnaryOp {
   public:
    FloatMultiplication(float multiplicand) : mMultiplicand(multiplicand) {}
    virtual int eval(int val) const override {
        return static_cast<int>(std::floor(static_cast<float>(val) * mMultiplicand));
    }
    virtual const char* getName() const override { return "MUL_FLOAT"; }

   private:
    float mMultiplicand;
};

// Arithmetic operators and methods on RandomVariables will create OP RandomVariableNodes.
// Since there must be at most one edge between two RandomVariableNodes, we have to do something
// special when both sides are refering to the same node.

RandomVariable operator+(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? RandomVariable(lhs, 2, Singleton<Multiplication>::get())
                                  : RandomVariable(lhs, rhs, Singleton<Addition>::get());
}
RandomVariable operator-(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? RandomVariable(0)
                                  : RandomVariable(lhs, rhs, Singleton<Subtraction>::get());
}
RandomVariable operator*(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? RandomVariable(lhs, RandomVariable(), Singleton<Square>::get())
                                  : RandomVariable(lhs, rhs, Singleton<Multiplication>::get());
}
RandomVariable operator*(const RandomVariable& lhs, const float& rhs) {
    return RandomVariable(lhs, RandomVariable(), std::make_shared<FloatMultiplication>(rhs));
}
RandomVariable operator/(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? RandomVariable(1)
                                  : RandomVariable(lhs, rhs, Singleton<Division>::get());
}
RandomVariable operator%(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? RandomVariable(0)
                                  : RandomVariable(lhs, rhs, Singleton<Modulo>::get());
}
RandomVariable max(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? lhs : RandomVariable(lhs, rhs, Singleton<Maximum>::get());
}
RandomVariable min(const RandomVariable& lhs, const RandomVariable& rhs) {
    return lhs.get() == rhs.get() ? lhs : RandomVariable(lhs, rhs, Singleton<Minimum>::get());
}

RandomVariable RandomVariable::exactDiv(const RandomVariable& other) {
    return mVar == other.get() ? RandomVariable(1)
                               : RandomVariable(*this, other, Singleton<ExactDivision>::get());
}

RandomVariable RandomVariable::setEqual(const RandomVariable& other) const {
    RandomVariableNode node1 = mVar, node2 = other.get();
    NN_FUZZER_LOG << "Set equality of var" << node1->index << " and var" << node2->index;

    // Do not setEqual on the same pair twice.
    if (node1 == node2 || (node1->op == Singleton<UnaryEqual>::get() && node1->parent1 == node2) ||
        (node2->op == Singleton<UnaryEqual>::get() && node2->parent1 == node1)) {
        NN_FUZZER_LOG << "Already equal. Return.";
        return RandomVariable();
    }

    // If possible, always try UnaryEqual first to reduce the search space.
    // UnaryEqual can be used if node B is FREE and is evaluated later than node A.
    // TODO: Reduce code duplication.
    if (RandomVariableNetwork::get()->isSubordinate(node1, node2)) {
        NN_FUZZER_LOG << "  Make var" << node2->index << " a child of var" << node1->index;
        node2->type = RandomVariableType::OP;
        node2->parent1 = node1;
        node2->op = Singleton<UnaryEqual>::get();
        node1->children.push_back(node2);
        RandomVariableNetwork::get()->join(node1, node2);
        node1->updateTimestamp();
        return other;
    }
    if (RandomVariableNetwork::get()->isSubordinate(node2, node1)) {
        NN_FUZZER_LOG << "  Make var" << node1->index << " a child of var" << node2->index;
        node1->type = RandomVariableType::OP;
        node1->parent1 = node2;
        node1->op = Singleton<UnaryEqual>::get();
        node2->children.push_back(node1);
        RandomVariableNetwork::get()->join(node2, node1);
        node1->updateTimestamp();
        return *this;
    }
    return RandomVariable(*this, other, Singleton<Equal>::get());
}

RandomVariable RandomVariable::setGreaterThan(const RandomVariable& other) const {
    NN_FUZZER_CHECK(mVar != other.get());
    return RandomVariable(*this, other, Singleton<GreaterThan>::get());
}
RandomVariable RandomVariable::setGreaterEqual(const RandomVariable& other) const {
    return mVar == other.get() ? *this
                               : RandomVariable(*this, other, Singleton<GreaterEqual>::get());
}

void DisjointNetwork::add(const RandomVariableNode& var) {
    // Find the subnet index of the parents and decide the index for var.
    int ind1 = var->parent1 == nullptr ? -1 : mIndexMap[var->parent1];
    int ind2 = var->parent2 == nullptr ? -1 : mIndexMap[var->parent2];
    int ind = join(ind1, ind2);
    // If no parent, put it into a new subnet component.
    if (ind == -1) ind = mNextIndex++;
    NN_FUZZER_LOG << "Add RandomVariable var" << var->index << " to network #" << ind;
    mIndexMap[var] = ind;
    mEvalOrderMap[ind].push_back(var);
}

int DisjointNetwork::join(int ind1, int ind2) {
    if (ind1 == -1) return ind2;
    if (ind2 == -1) return ind1;
    if (ind1 == ind2) return ind1;
    NN_FUZZER_LOG << "Join network #" << ind1 << " and #" << ind2;
    auto &order1 = mEvalOrderMap[ind1], &order2 = mEvalOrderMap[ind2];
    // Append every node in ind2 to the end of ind1
    for (const auto& var : order2) {
        order1.push_back(var);
        mIndexMap[var] = ind1;
    }
    // Remove ind2 from mEvalOrderMap.
    mEvalOrderMap.erase(mEvalOrderMap.find(ind2));
    return ind1;
}

RandomVariableNetwork* RandomVariableNetwork::get() {
    static RandomVariableNetwork instance;
    return &instance;
}

void RandomVariableNetwork::initialize(int defaultValue) {
    RandomVariableBase::globalIndex = 0;
    RandomVariable::defaultValue = defaultValue;
    mIndexMap.clear();
    mEvalOrderMap.clear();
    mDimProd.clear();
    mNextIndex = 0;
    mGlobalTime = 0;
    mTimestamp = -1;
}

bool RandomVariableNetwork::isSubordinate(const RandomVariableNode& node1,
                                          const RandomVariableNode& node2) {
    if (node2->type != RandomVariableType::FREE) return false;
    int ind1 = mIndexMap[node1];
    // node2 is of a different subnet.
    if (ind1 != mIndexMap[node2]) return true;
    for (const auto& node : mEvalOrderMap[ind1]) {
        if (node == node2) return false;
        // node2 is of the same subnet but evaluated later than node1.
        if (node == node1) return true;
    }
    NN_FUZZER_CHECK(false) << "Code executed in non-reachable region.";
    return false;
}

struct EvalInfo {
    // The RandomVariableNode that this EvalInfo is associated with.
    // var->value is the current value during evaluation.
    RandomVariableNode var;

    // The RandomVariable value is staged when a valid combination is found.
    std::set<int> staging;

    // The staging values are committed after a subnet evaluation.
    std::set<int> committed;

    // Keeps track of the latest timestamp that committed is updated.
    int timestamp;

    // For evalSubnetWithLocalNetwork.
    RandomVariableType originalType;

    // Should only invoke eval on OP RandomVariable.
    bool eval() {
        NN_FUZZER_CHECK(var->type == RandomVariableType::OP);
        var->value = var->op->eval(var->parent1->value,
                                   var->parent2 == nullptr ? 0 : var->parent2->value);
        if (var->value == kInvalidValue) return false;
        return committed.find(var->value) != committed.end();
    }
    void stage() { staging.insert(var->value); }
    void commit() {
        // Only update committed and timestamp if the range is *indeed* changed.
        if (staging.size() != committed.size()) {
            committed = std::move(staging);
            timestamp = RandomVariableNetwork::get()->getGlobalTime();
        }
        staging.clear();
    }
    void updateRange() {
        // Only update range and timestamp if the range is *indeed* changed.
        if (committed.size() != var->range.size()) {
            var->range = RandomVariableRange(committed);
            var->timestamp = timestamp;
        }
        committed.clear();
    }

    EvalInfo(const RandomVariableNode& var)
        : var(var),
          committed(var->range.getChoices().begin(), var->range.getChoices().end()),
          timestamp(var->timestamp) {}
};
using EvalContext = std::unordered_map<RandomVariableNode, EvalInfo>;

// For logging only.
inline std::string toString(const RandomVariableNode& var, EvalContext* context) {
    std::stringstream ss;
    ss << "var" << var->index << " = ";
    const auto& committed = context->at(var).committed;
    switch (var->type) {
        case RandomVariableType::FREE:
            ss << "FREE ["
               << joinStr(", ", 20, std::vector<int>(committed.begin(), committed.end())) << "]";
            break;
        case RandomVariableType::CONST:
            ss << "CONST " << toString(var->value);
            break;
        case RandomVariableType::OP:
            ss << "var" << var->parent1->index << " " << var->op->getName();
            if (var->parent2 != nullptr) ss << " var" << var->parent2->index;
            ss << ", [" << joinStr(", ", 20, std::vector<int>(committed.begin(), committed.end()))
               << "]";
            break;
        default:
            NN_FUZZER_CHECK(false);
    }
    ss << ", timestamp = " << context->at(var).timestamp;
    return ss.str();
}

// Check if the subnet needs to be re-evaluated by comparing the timestamps.
static inline bool needEvaluate(const EvaluationOrder& evalOrder, int subnetTime,
                                EvalContext* context = nullptr) {
    for (const auto& var : evalOrder) {
        int timestamp = context == nullptr ? var->timestamp : context->at(var).timestamp;
        // If we find a node that has been modified since last evaluation, the subnet needs to be
        // re-evaluated.
        if (timestamp > subnetTime) return true;
    }
    return false;
}

// Helper function to evaluate the subnet recursively.
// Iterate through all combinations of FREE RandomVariables choices.
static void evalSubnetHelper(const EvaluationOrder& evalOrder, EvalContext* context, size_t i = 0) {
    if (i == evalOrder.size()) {
        // Reach the end of the evaluation, find a valid combination.
        for (auto& var : evalOrder) context->at(var).stage();
        return;
    }
    const auto& var = evalOrder[i];
    if (var->type == RandomVariableType::FREE) {
        // For FREE RandomVariable, iterate through all valid choices.
        for (int val : context->at(var).committed) {
            var->value = val;
            evalSubnetHelper(evalOrder, context, i + 1);
        }
        return;
    } else if (var->type == RandomVariableType::OP) {
        // For OP RandomVariable, evaluate from parents and terminate if the result is invalid.
        if (!context->at(var).eval()) return;
    }
    evalSubnetHelper(evalOrder, context, i + 1);
}

// Check if the subnet has only one single OP RandomVariable.
static inline bool isSingleOpSubnet(const EvaluationOrder& evalOrder) {
    int numOp = 0;
    for (const auto& var : evalOrder) {
        if (var->type == RandomVariableType::OP) numOp++;
        if (numOp > 1) return false;
    }
    return numOp != 0;
}

// Evaluate with a potentially faster approach provided by IRandomVariableOp.
static inline void evalSubnetSingleOpHelper(const EvaluationOrder& evalOrder,
                                            EvalContext* context) {
    NN_FUZZER_LOG << "Identified as single op subnet";
    const auto& var = evalOrder.back();
    NN_FUZZER_CHECK(var->type == RandomVariableType::OP);
    var->op->eval(&context->at(var->parent1).committed,
                  var->parent2 == nullptr ? nullptr : &context->at(var->parent2).committed,
                  &context->at(var).committed, &context->at(var->parent1).staging,
                  var->parent2 == nullptr ? nullptr : &context->at(var->parent2).staging,
                  &context->at(var).staging);
}

// Check if the number of combinations of FREE RandomVariables exceeds the limit.
static inline uint64_t getNumCombinations(const EvaluationOrder& evalOrder,
                                          EvalContext* context = nullptr) {
    constexpr uint64_t kLimit = 1e8;
    uint64_t numCombinations = 1;
    for (const auto& var : evalOrder) {
        if (var->type == RandomVariableType::FREE) {
            size_t size =
                    context == nullptr ? var->range.size() : context->at(var).committed.size();
            numCombinations *= size;
            // To prevent overflow.
            if (numCombinations > kLimit) return kLimit;
        }
    }
    return numCombinations;
}

// Evaluate the subnet recursively. Will return fail if the number of combinations of FREE
// RandomVariable exceeds the threshold kMaxNumCombinations.
static bool evalSubnetWithBruteForce(const EvaluationOrder& evalOrder, EvalContext* context) {
    constexpr uint64_t kMaxNumCombinations = 1e7;
    NN_FUZZER_LOG << "Evaluate with brute force";
    if (isSingleOpSubnet(evalOrder)) {
        // If the network only have one single OP, dispatch to a faster evaluation.
        evalSubnetSingleOpHelper(evalOrder, context);
    } else {
        if (getNumCombinations(evalOrder, context) > kMaxNumCombinations) {
            NN_FUZZER_LOG << "Terminate the evaluation because of large search range";
            std::cout << "[          ]   Terminate the evaluation because of large search range"
                      << std::endl;
            return false;
        }
        evalSubnetHelper(evalOrder, context);
    }
    for (auto& var : evalOrder) {
        if (context->at(var).staging.empty()) {
            NN_FUZZER_LOG << "Evaluation failed at " << toString(var, context);
            return false;
        }
        context->at(var).commit();
    }
    return true;
}

struct LocalNetwork {
    EvaluationOrder evalOrder;
    std::vector<RandomVariableNode> bridgeNodes;
    int timestamp = 0;

    bool eval(EvalContext* context) {
        NN_FUZZER_LOG << "Evaluate local network with timestamp = " << timestamp;
        // Temporarily treat bridge nodes as FREE RandomVariables.
        for (const auto& var : bridgeNodes) {
            context->at(var).originalType = var->type;
            var->type = RandomVariableType::FREE;
        }
        for (const auto& var : evalOrder) {
            context->at(var).staging.clear();
            NN_FUZZER_LOG << "  - " << toString(var, context);
        }
        bool success = evalSubnetWithBruteForce(evalOrder, context);
        // Reset the RandomVariable types for bridge nodes.
        for (const auto& var : bridgeNodes) var->type = context->at(var).originalType;
        return success;
    }
};

// Partition the network further into LocalNetworks based on the result from bridge annotation
// algorithm.
class GraphPartitioner : public DisjointNetwork {
   public:
    GraphPartitioner() = default;

    std::vector<LocalNetwork> partition(const EvaluationOrder& evalOrder, int timestamp) {
        annotateBridge(evalOrder);
        for (const auto& var : evalOrder) add(var);
        return get(timestamp);
    }

   private:
    GraphPartitioner(const GraphPartitioner&) = delete;
    GraphPartitioner& operator=(const GraphPartitioner&) = delete;

    // Find the parent-child relationship between var1 and var2, and reset the bridge.
    void setBridgeFlag(const RandomVariableNode& var1, const RandomVariableNode& var2) {
        if (var1->parent1 == var2) {
            mBridgeInfo[var1].isParent1Bridge = true;
        } else if (var1->parent2 == var2) {
            mBridgeInfo[var1].isParent2Bridge = true;
        } else {
            setBridgeFlag(var2, var1);
        }
    }

    // Annoate the bridges with DFS -- an edge [u, v] is a bridge if none of u's ancestor is
    // reachable from a node in the subtree of b. The complexity is O(V + E).
    // discoveryTime: The timestamp a node is visited
    // lowTime: The min discovery time of all reachable nodes from the subtree of the node.
    void annotateBridgeHelper(const RandomVariableNode& var, int* time) {
        mBridgeInfo[var].visited = true;
        mBridgeInfo[var].discoveryTime = mBridgeInfo[var].lowTime = (*time)++;

        // The algorithm operates on undirected graph. First find all adjacent nodes.
        auto adj = var->children;
        if (var->parent1 != nullptr) adj.push_back(var->parent1);
        if (var->parent2 != nullptr) adj.push_back(var->parent2);

        for (const auto& child : adj) {
            if (mBridgeInfo.find(child) == mBridgeInfo.end()) continue;
            if (!mBridgeInfo[child].visited) {
                mBridgeInfo[child].parent = var;
                annotateBridgeHelper(child, time);

                // If none of nodes in the subtree of child is connected to any ancestors of var,
                // then it is a bridge.
                mBridgeInfo[var].lowTime =
                        std::min(mBridgeInfo[var].lowTime, mBridgeInfo[child].lowTime);
                if (mBridgeInfo[child].lowTime > mBridgeInfo[var].discoveryTime)
                    setBridgeFlag(var, child);
            } else if (mBridgeInfo[var].parent != child) {
                mBridgeInfo[var].lowTime =
                        std::min(mBridgeInfo[var].lowTime, mBridgeInfo[child].discoveryTime);
            }
        }
    }

    // Find all bridges in the subnet with DFS.
    void annotateBridge(const EvaluationOrder& evalOrder) {
        for (const auto& var : evalOrder) mBridgeInfo[var];
        int time = 0;
        for (const auto& var : evalOrder) {
            if (!mBridgeInfo[var].visited) annotateBridgeHelper(var, &time);
        }
    }

    // Re-partition the network by treating bridges as no edge.
    void add(const RandomVariableNode& var) {
        auto parent1 = var->parent1;
        auto parent2 = var->parent2;
        if (mBridgeInfo[var].isParent1Bridge) var->parent1 = nullptr;
        if (mBridgeInfo[var].isParent2Bridge) var->parent2 = nullptr;
        DisjointNetwork::add(var);
        var->parent1 = parent1;
        var->parent2 = parent2;
    }

    // Add bridge nodes to the local network and remove single node subnet.
    std::vector<LocalNetwork> get(int timestamp) {
        std::vector<LocalNetwork> res;
        for (auto& pair : mEvalOrderMap) {
            // We do not need to evaluate subnet with only a single node.
            if (pair.second.size() == 1 && pair.second[0]->parent1 == nullptr) continue;
            res.emplace_back();
            for (const auto& var : pair.second) {
                if (mBridgeInfo[var].isParent1Bridge) {
                    res.back().evalOrder.push_back(var->parent1);
                    res.back().bridgeNodes.push_back(var->parent1);
                }
                if (mBridgeInfo[var].isParent2Bridge) {
                    res.back().evalOrder.push_back(var->parent2);
                    res.back().bridgeNodes.push_back(var->parent2);
                }
                res.back().evalOrder.push_back(var);
            }
            res.back().timestamp = timestamp;
        }
        return res;
    }

    // For bridge discovery algorithm.
    struct BridgeInfo {
        bool isParent1Bridge = false;
        bool isParent2Bridge = false;
        int discoveryTime = 0;
        int lowTime = 0;
        bool visited = false;
        std::shared_ptr<RandomVariableBase> parent = nullptr;
    };
    std::unordered_map<RandomVariableNode, BridgeInfo> mBridgeInfo;
};

// Evaluate subnets repeatedly until converge.
// Class T_Subnet must have member evalOrder, timestamp, and member function eval.
template <class T_Subnet>
inline bool evalSubnetsRepeatedly(std::vector<T_Subnet>* subnets, EvalContext* context) {
    bool terminate = false;
    while (!terminate) {
        terminate = true;
        for (auto& subnet : *subnets) {
            if (needEvaluate(subnet.evalOrder, subnet.timestamp, context)) {
                if (!subnet.eval(context)) return false;
                subnet.timestamp = RandomVariableNetwork::get()->getGlobalTime();
                terminate = false;
            }
        }
    }
    return true;
}

// Evaluate the subnet by first partitioning it further into LocalNetworks.
static bool evalSubnetWithLocalNetwork(const EvaluationOrder& evalOrder, int timestamp,
                                       EvalContext* context) {
    NN_FUZZER_LOG << "Evaluate with local network";
    auto localNetworks = GraphPartitioner().partition(evalOrder, timestamp);
    return evalSubnetsRepeatedly(&localNetworks, context);
}

struct LeafNetwork {
    EvaluationOrder evalOrder;
    int timestamp = 0;
    LeafNetwork(const RandomVariableNode& var, int timestamp) : timestamp(timestamp) {
        std::set<RandomVariableNode> visited;
        constructorHelper(var, &visited);
    }
    // Construct the leaf network by recursively including parent nodes.
    void constructorHelper(const RandomVariableNode& var, std::set<RandomVariableNode>* visited) {
        if (var == nullptr || visited->find(var) != visited->end()) return;
        constructorHelper(var->parent1, visited);
        constructorHelper(var->parent2, visited);
        visited->insert(var);
        evalOrder.push_back(var);
    }
    bool eval(EvalContext* context) {
        return evalSubnetWithLocalNetwork(evalOrder, timestamp, context);
    }
};

// Evaluate the subnet by leaf network.
// NOTE: This algorithm will only produce correct result for *most* of the time (> 99%).
//       The random graph generator is expected to retry if it fails.
static bool evalSubnetWithLeafNetwork(const EvaluationOrder& evalOrder, int timestamp,
                                      EvalContext* context) {
    NN_FUZZER_LOG << "Evaluate with leaf network";
    // Construct leaf networks.
    std::vector<LeafNetwork> leafNetworks;
    for (const auto& var : evalOrder) {
        if (var->children.empty()) {
            NN_FUZZER_LOG << "Found leaf " << toString(var, context);
            leafNetworks.emplace_back(var, timestamp);
        }
    }
    return evalSubnetsRepeatedly(&leafNetworks, context);
}

void RandomVariableNetwork::addDimensionProd(const std::vector<RandomVariable>& dims) {
    if (dims.size() <= 1) return;
    EvaluationOrder order;
    for (const auto& dim : dims) order.push_back(dim.get());
    mDimProd.push_back(order);
}

bool enforceDimProd(const std::vector<EvaluationOrder>& mDimProd,
                    const std::unordered_map<RandomVariableNode, int>& indexMap,
                    EvalContext* context, std::unordered_set<int>* dirtySubnets) {
    for (auto& evalOrder : mDimProd) {
        NN_FUZZER_LOG << "  Dimension product network size = " << evalOrder.size();
        // Initialize EvalInfo of each RandomVariable.
        for (auto& var : evalOrder) {
            if (context->find(var) == context->end()) context->emplace(var, var);
            NN_FUZZER_LOG << "  - " << toString(var, context);
        }

        // Enforce the product of the dimension values below kMaxValue:
        // max(dimA) = kMaxValue / (min(dimB) * min(dimC) * ...)
        int prod = 1;
        for (const auto& var : evalOrder) prod *= (*context->at(var).committed.begin());
        for (auto& var : evalOrder) {
            auto& committed = context->at(var).committed;
            int maxValue = kMaxValue / (prod / *committed.begin());
            auto it = committed.upper_bound(maxValue);
            // var has empty range -> no solution.
            if (it == committed.begin()) return false;
            // The range is not modified -> continue.
            if (it == committed.end()) continue;
            // The range is modified -> the subnet of var is dirty, i.e. needs re-evaluation.
            committed.erase(it, committed.end());
            context->at(var).timestamp = RandomVariableNetwork::get()->getGlobalTime();
            dirtySubnets->insert(indexMap.at(var));
        }
    }
    return true;
}

bool RandomVariableNetwork::evalRange() {
    constexpr uint64_t kMaxNumCombinationsWithBruteForce = 500;
    constexpr uint64_t kMaxNumCombinationsWithLocalNetwork = 1e5;
    NN_FUZZER_LOG << "Evaluate on " << mEvalOrderMap.size() << " sub-networks";
    EvalContext context;
    std::unordered_set<int> dirtySubnets;  // Which subnets needs evaluation.
    for (auto& pair : mEvalOrderMap) {
        const auto& evalOrder = pair.second;
        // Decide whether needs evaluation by timestamp -- if no range has changed after the last
        // evaluation, then the subnet does not need re-evaluation.
        if (evalOrder.size() == 1 || !needEvaluate(evalOrder, mTimestamp)) continue;
        dirtySubnets.insert(pair.first);
    }
    if (!enforceDimProd(mDimProd, mIndexMap, &context, &dirtySubnets)) return false;

    // Repeat until the ranges converge.
    while (!dirtySubnets.empty()) {
        for (int ind : dirtySubnets) {
            const auto& evalOrder = mEvalOrderMap[ind];
            NN_FUZZER_LOG << "  Sub-network #" << ind << " size = " << evalOrder.size();

            // Initialize EvalInfo of each RandomVariable.
            for (auto& var : evalOrder) {
                if (context.find(var) == context.end()) context.emplace(var, var);
                NN_FUZZER_LOG << "  - " << toString(var, &context);
            }

            // Dispatch to different algorithm according to search range.
            bool success;
            uint64_t numCombinations = getNumCombinations(evalOrder);
            if (numCombinations <= kMaxNumCombinationsWithBruteForce) {
                success = evalSubnetWithBruteForce(evalOrder, &context);
            } else if (numCombinations <= kMaxNumCombinationsWithLocalNetwork) {
                success = evalSubnetWithLocalNetwork(evalOrder, mTimestamp, &context);
            } else {
                success = evalSubnetWithLeafNetwork(evalOrder, mTimestamp, &context);
            }
            if (!success) return false;
        }
        dirtySubnets.clear();
        if (!enforceDimProd(mDimProd, mIndexMap, &context, &dirtySubnets)) return false;
    }
    // A successful evaluation, update RandomVariables from EvalContext.
    for (auto& pair : context) pair.second.updateRange();
    mTimestamp = getGlobalTime();
    NN_FUZZER_LOG << "Finish range evaluation";
    return true;
}

static void unsetEqual(const RandomVariableNode& node) {
    if (node == nullptr) return;
    NN_FUZZER_LOG << "Unset equality of var" << node->index;
    RandomVariableNode parent1 = node->parent1, parent2 = node->parent2;
    parent1->children.erase(std::find(parent1->children.begin(), parent1->children.end(), node));
    node->parent1 = nullptr;
    if (parent2 != nullptr) {
        // For Equal.
        parent2->children.erase(
                std::find(parent2->children.begin(), parent2->children.end(), node));
        node->parent2 = nullptr;
    } else {
        // For UnaryEqual.
        node->type = RandomVariableType::FREE;
        node->op = nullptr;
    }
}

// A class to revert all the changes made to RandomVariableNetwork since the Reverter object is
// constructed. Only used when setEqualIfCompatible results in incompatible.
class RandomVariableNetwork::Reverter {
   public:
    // Take a snapshot of RandomVariableNetwork when Reverter is constructed.
    Reverter() : mSnapshot(*RandomVariableNetwork::get()) {}
    // Add constraint (Equal) nodes to the reverter.
    void addNode(const RandomVariableNode& node) { mEqualNodes.push_back(node); }
    void revert() {
        NN_FUZZER_LOG << "Revert RandomVariableNetwork";
        // Release the constraints.
        for (const auto& node : mEqualNodes) unsetEqual(node);
        // Reset all member variables.
        *RandomVariableNetwork::get() = std::move(mSnapshot);
    }

   private:
    Reverter(const Reverter&) = delete;
    Reverter& operator=(const Reverter&) = delete;
    RandomVariableNetwork mSnapshot;
    std::vector<RandomVariableNode> mEqualNodes;
};

bool RandomVariableNetwork::setEqualIfCompatible(const std::vector<RandomVariable>& lhs,
                                                 const std::vector<RandomVariable>& rhs) {
    NN_FUZZER_LOG << "Check compatibility of {" << joinStr(", ", lhs) << "} and {"
                  << joinStr(", ", rhs) << "}";
    if (lhs.size() != rhs.size()) return false;
    Reverter reverter;
    bool result = true;
    for (size_t i = 0; i < lhs.size(); i++) {
        auto node = lhs[i].setEqual(rhs[i]).get();
        reverter.addNode(node);
        // Early terminate if there is no common choice between two ranges.
        if (node != nullptr && node->range.empty()) result = false;
    }
    result = result && evalRange();
    if (!result) reverter.revert();
    NN_FUZZER_LOG << "setEqualIfCompatible: " << (result ? "[COMPATIBLE]" : "[INCOMPATIBLE]");
    return result;
}

bool RandomVariableNetwork::freeze() {
    NN_FUZZER_LOG << "Freeze the random network";
    if (!evalRange()) return false;
    for (const auto& pair : mEvalOrderMap) {
        // Find all FREE RandomVariables in the subnet.
        std::vector<RandomVariableNode> nodes;
        for (const auto& var : pair.second) {
            if (var->type == RandomVariableType::FREE) nodes.push_back(var);
        }
        // Randomly shuffle the order, this is for a more uniform randomness.
        randomShuffle(&nodes);
        // An inefficient algorithm that does freeze -> re-evaluate for every FREE RandomVariable.
        // TODO: Might be able to optimize this.
        for (const auto& var : nodes) {
            size_t size = var->range.size();
            NN_FUZZER_LOG << "Freeze " << toString(var);
            var->freeze();
            NN_FUZZER_LOG << "  " << toString(var);
            // There is no need to re-evaluate if the FREE RandomVariable have only one choice.
            if (size > 1) {
                var->updateTimestamp();
                if (!evalRange()) {
                    NN_FUZZER_LOG << "Freeze failed at " << toString(var);
                    return false;
                }
            }
        }
    }
    NN_FUZZER_LOG << "Finish freezing the random network";
    return true;
}

}  // namespace fuzzing_test
}  // namespace nn
}  // namespace android
