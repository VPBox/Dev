/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CONSTANT_EXPRESSION_H_

#define CONSTANT_EXPRESSION_H_

#include <android-base/macros.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "Reference.h"
#include "ScalarType.h"

namespace android {

struct LocalIdentifier;

struct LiteralConstantExpression;
struct UnaryConstantExpression;
struct BinaryConstantExpression;
struct TernaryConstantExpression;
struct ReferenceConstantExpression;

/**
 * A constant expression is represented by a tree.
 */
struct ConstantExpression {
    static std::unique_ptr<ConstantExpression> Zero(ScalarType::Kind kind);
    static std::unique_ptr<ConstantExpression> One(ScalarType::Kind kind);
    static std::unique_ptr<ConstantExpression> ValueOf(ScalarType::Kind kind, uint64_t value);

    virtual ~ConstantExpression() {}

    virtual bool isReferenceConstantExpression() const;

    // Proceeds recursive pass
    // Makes sure to visit each node only once
    // Used to provide lookup and lazy evaluation
    status_t recursivePass(const std::function<status_t(ConstantExpression*)>& func,
                           std::unordered_set<const ConstantExpression*>* visited,
                           bool processBeforeDependencies);
    status_t recursivePass(const std::function<status_t(const ConstantExpression*)>& func,
                           std::unordered_set<const ConstantExpression*>* visited,
                           bool processBeforeDependencies) const;

    // If this object is in an invalid state.
    virtual status_t validate() const;

    // Evaluates current constant expression
    // Doesn't call recursive evaluation, so must be called after dependencies
    virtual void evaluate() = 0;

    std::vector<ConstantExpression*> getConstantExpressions();
    virtual std::vector<const ConstantExpression*> getConstantExpressions() const = 0;

    std::vector<Reference<LocalIdentifier>*> getReferences();
    virtual std::vector<const Reference<LocalIdentifier>*> getReferences() const;

    std::vector<Reference<Type>*> getTypeReferences();
    virtual std::vector<const Reference<Type>*> getTypeReferences() const;

    // Recursive tree pass checkAcyclic return type.
    // Stores cycle end for nice error messages.
    struct CheckAcyclicStatus {
        CheckAcyclicStatus(status_t status, const ConstantExpression* cycleEnd = nullptr,
                           const ReferenceConstantExpression* lastReferenceExpression = nullptr);

        status_t status;

        // If a cycle is found, stores the end of cycle.
        // While going back in recursion, this is used to stop printing the cycle.
        const ConstantExpression* cycleEnd;

        // The last ReferenceConstantExpression visited on the cycle.
        const ReferenceConstantExpression* lastReference;
    };

    // Recursive tree pass that ensures that constant expressions definitions
    // are acyclic.
    CheckAcyclicStatus checkAcyclic(std::unordered_set<const ConstantExpression*>* visited,
                                    std::unordered_set<const ConstantExpression*>* stack) const;

    /* Returns true iff the value has already been evaluated. */
    bool isEvaluated() const;
    /* Evaluated result in a string form with comment if applicable. */
    std::string value() const;
    /* Evaluated result in a string form with comment if applicable. */
    std::string cppValue() const;
    /* Evaluated result in a string form with comment if applicable. */
    std::string javaValue() const;
    /* Evaluated result in a string form, with given contextual kind. */
    std::string value(ScalarType::Kind castKind) const;
    /* Evaluated result in a string form, with given contextual kind. */
    std::string cppValue(ScalarType::Kind castKind) const;
    /* Evaluated result in a string form, with given contextual kind. */
    std::string javaValue(ScalarType::Kind castKind) const;

    /* The expression representing this value for use in comments when the value is not needed */
    const std::string& expression() const;

    /* Return a ConstantExpression that is 1 plus the original. */
    std::unique_ptr<ConstantExpression> addOne(ScalarType::Kind baseKind);

    size_t castSizeT() const;

    // Marks that package proceeding is completed
    // Post parse passes must be proceeded during owner package parsin
    void setPostParseCompleted();

    /*
     * Helper function for all cpp/javaValue methods.
     * Returns a plain string (without any prefixes or suffixes, just the
     * digits) converted from mValue.
     */
    std::string rawValue() const;
    std::string rawValue(ScalarType::Kind castKind) const;

   private:
    /* If the result value has been evaluated. */
    bool mIsEvaluated = false;
    /* The formatted expression. */
    std::string mExpr;
    /* The kind of the result value. */
    ScalarType::Kind mValueKind;
    /* The stored result value. */
    uint64_t mValue;
    /* true if description() does not offer more information than value(). */
    bool mTrivialDescription = false;

    bool mIsPostParseCompleted = false;

    /*
     * Helper function, gives suffix comment to add to value/cppValue/javaValue
     */
    std::string descriptionSuffix() const;

    /*
     * Return the value casted to the given type.
     * First cast it according to mValueKind, then cast it to T.
     * Assumes !containsIdentifiers()
     */
    template <typename T>
    T cast() const;

    friend struct LiteralConstantExpression;
    friend struct UnaryConstantExpression;
    friend struct BinaryConstantExpression;
    friend struct TernaryConstantExpression;
    friend struct ReferenceConstantExpression;
    friend struct AttributeConstantExpression;
};

struct LiteralConstantExpression : public ConstantExpression {
    LiteralConstantExpression(ScalarType::Kind kind, uint64_t value);
    LiteralConstantExpression(ScalarType::Kind kind, uint64_t value, const std::string& expr);
    void evaluate() override;
    std::vector<const ConstantExpression*> getConstantExpressions() const override;

    static LiteralConstantExpression* tryParse(const std::string& value);
};

struct UnaryConstantExpression : public ConstantExpression {
    UnaryConstantExpression(const std::string& mOp, ConstantExpression* value);
    void evaluate() override;
    std::vector<const ConstantExpression*> getConstantExpressions() const override;

   private:
    ConstantExpression* const mUnary;
    std::string mOp;
};

struct BinaryConstantExpression : public ConstantExpression {
    BinaryConstantExpression(ConstantExpression* lval, const std::string& op,
                             ConstantExpression* rval);
    void evaluate() override;
    std::vector<const ConstantExpression*> getConstantExpressions() const override;

   private:
    ConstantExpression* const mLval;
    ConstantExpression* const mRval;
    const std::string mOp;
};

struct TernaryConstantExpression : public ConstantExpression {
    TernaryConstantExpression(ConstantExpression* cond, ConstantExpression* trueVal,
                              ConstantExpression* falseVal);
    void evaluate() override;
    std::vector<const ConstantExpression*> getConstantExpressions() const override;

   private:
    ConstantExpression* const mCond;
    ConstantExpression* const mTrueVal;
    ConstantExpression* const mFalseVal;
};

struct ReferenceConstantExpression : public ConstantExpression {
    ReferenceConstantExpression(const Reference<LocalIdentifier>& value, const std::string& expr);

    bool isReferenceConstantExpression() const override;
    void evaluate() override;
    std::vector<const ConstantExpression*> getConstantExpressions() const override;
    std::vector<const Reference<LocalIdentifier>*> getReferences() const override;

   private:
    Reference<LocalIdentifier> mReference;
};

// This constant expression is a compile-time calculatable expression based on another type
struct AttributeConstantExpression : public ConstantExpression {
    AttributeConstantExpression(const Reference<Type>& value, const std::string& fqname,
                                const std::string& tag);

    status_t validate() const override;
    void evaluate() override;

    std::vector<const ConstantExpression*> getConstantExpressions() const override;
    std::vector<const Reference<Type>*> getTypeReferences() const override;

   private:
    Reference<Type> mReference;
    const std::string mTag;
};

}  // namespace android

#endif  // CONSTANT_EXPRESSION_H_
