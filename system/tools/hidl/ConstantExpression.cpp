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

#include "ConstantExpression.h"

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "EnumType.h"
#include "Scope.h"  // LocalIdentifier

// The macros are really nasty here. Consider removing
// as many macros as possible.

#define OPEQ(__y__) (std::string(mOp) == std::string(__y__))
#define COMPUTE_UNARY(__op__)  if (op == std::string(#__op__)) return __op__ val;
#define COMPUTE_BINARY(__op__) if (op == std::string(#__op__)) return lval __op__ rval;
#define OP_IS_BIN_ARITHMETIC  (OPEQ("+") || OPEQ("-") || OPEQ("*") || OPEQ("/") || OPEQ("%"))
#define OP_IS_BIN_BITFLIP     (OPEQ("|") || OPEQ("^") || OPEQ("&"))
#define OP_IS_BIN_COMP        (OPEQ("<") || OPEQ(">") || OPEQ("<=") || OPEQ(">=") || OPEQ("==") || OPEQ("!="))
#define OP_IS_BIN_SHIFT       (OPEQ(">>") || OPEQ("<<"))
#define OP_IS_BIN_LOGICAL     (OPEQ("||") || OPEQ("&&"))
#define SK(__x__) ScalarType::Kind::KIND_##__x__
#define SHOULD_NOT_REACH() CHECK(false) << __LINE__ << ": should not reach here: "

// NOLINT to suppress missing parentheses warnings about __def__.
#define SWITCH_KIND(__cond__, __action__, __def__)           \
        switch(__cond__) {                                        \
            case SK(BOOL): __action__(bool)                         \
            case SK(UINT8): __action__(uint8_t)                     \
            case SK(INT8): __action__(int8_t)                       \
            case SK(UINT16): __action__(uint16_t)                   \
            case SK(INT16): __action__(int16_t)                     \
            case SK(UINT32): __action__(uint32_t)                   \
            case SK(INT32): __action__(int32_t)                     \
            case SK(UINT64): __action__(uint64_t)                   \
            case SK(INT64): __action__(int64_t)                     \
            default: __def__                        /* NOLINT */    \
        }

namespace android {

static inline bool isSupported(ScalarType::Kind kind) {
    return SK(BOOL) == kind || ScalarType(kind, nullptr /* parent */).isValidEnumStorageType();
}

/* See docs at the end for details on integral promotion. */
ScalarType::Kind integralPromotion(ScalarType::Kind in) {
    return SK(INT32) < in ? in : SK(INT32); // note that KIND_INT32 < KIND_UINT32
}

/* See docs at the end for details on usual arithmetic conversion. */
ScalarType::Kind usualArithmeticConversion(ScalarType::Kind lft,
                                           ScalarType::Kind rgt) {
    CHECK(isSupported(lft) && isSupported(rgt));
    // Kinds in concern: bool, (u)int[8|16|32|64]
    if(lft == rgt) return lft; // easy case
    if(lft == SK(BOOL)) return rgt;
    if(rgt == SK(BOOL)) return lft;
    bool isLftSigned = (lft == SK(INT8))  || (lft == SK(INT16))
                    || (lft == SK(INT32)) || (lft == SK(INT64));
    bool isRgtSigned = (rgt == SK(INT8))  || (rgt == SK(INT16))
                    || (rgt == SK(INT32)) || (rgt == SK(INT64));
    if(isLftSigned == isRgtSigned) return lft < rgt ? rgt : lft;
    ScalarType::Kind unsignedRank = isLftSigned ? rgt : lft;
    ScalarType::Kind signedRank   = isLftSigned ? lft : rgt;
    if(unsignedRank >= signedRank) return unsignedRank;
    if(signedRank > unsignedRank)  return signedRank;

    // Although there is such rule to return "the unsigned counterpart of
    // the signed operand", it should not reach here in our HIDL grammar.
    CHECK(false) << "Could not do usual arithmetic conversion for type " << lft << "and" << rgt;
    switch(signedRank) {
        case SK(INT8):  return SK(UINT8);
        case SK(INT16): return SK(UINT16);
        case SK(INT32): return SK(UINT32);
        case SK(INT64): return SK(UINT64);
        default: return SK(UINT64);
    }
}

template <class T>
T handleUnary(const std::string& op, T val) {
    COMPUTE_UNARY(+)
    COMPUTE_UNARY(-)
    COMPUTE_UNARY(!)
    COMPUTE_UNARY(~)
    // Should not reach here.
    SHOULD_NOT_REACH() << "Could not handleUnary for " << op << " " << val;
    return static_cast<T>(0xdeadbeef);
}

template <class T>
T handleBinaryCommon(T lval, const std::string& op, T rval) {
    COMPUTE_BINARY(+)
    COMPUTE_BINARY(-)
    COMPUTE_BINARY(*)
    COMPUTE_BINARY(/)
    COMPUTE_BINARY(%)
    COMPUTE_BINARY(|)
    COMPUTE_BINARY(^)
    COMPUTE_BINARY(&)
    // comparison operators: return 0 or 1 by nature.
    COMPUTE_BINARY(==)
    COMPUTE_BINARY(!=)
    COMPUTE_BINARY(<)
    COMPUTE_BINARY(>)
    COMPUTE_BINARY(<=)
    COMPUTE_BINARY(>=)
    // Should not reach here.
    SHOULD_NOT_REACH() << "Could not handleBinaryCommon for "
                       << lval << " " << op << " " << rval;
    return static_cast<T>(0xdeadbeef);
}

template <class T>
T handleShift(T lval, const std::string& op, int64_t rval) {
    // just cast rval to int64_t and it should fit.
    COMPUTE_BINARY(>>)
    COMPUTE_BINARY(<<)
    // Should not reach here.
    SHOULD_NOT_REACH() << "Could not handleShift for "
                       << lval << " " << op << " " << rval;
    return static_cast<T>(0xdeadbeef);
}

bool handleLogical(bool lval, const std::string& op, bool rval) {
    COMPUTE_BINARY(||);
    COMPUTE_BINARY(&&);
    // Should not reach here.
    SHOULD_NOT_REACH() << "Could not handleLogical for "
                       << lval << " " << op << " " << rval;
    return false;
}

std::unique_ptr<ConstantExpression> ConstantExpression::Zero(ScalarType::Kind kind) {
    return ValueOf(kind, 0);
}

std::unique_ptr<ConstantExpression> ConstantExpression::One(ScalarType::Kind kind) {
    return ValueOf(kind, 1);
}

std::unique_ptr<ConstantExpression> ConstantExpression::ValueOf(ScalarType::Kind kind,
                                                                uint64_t value) {
    return std::make_unique<LiteralConstantExpression>(kind, value);
}

bool ConstantExpression::isEvaluated() const {
    return mIsEvaluated;
}

LiteralConstantExpression::LiteralConstantExpression(
    ScalarType::Kind kind, uint64_t value, const std::string& expr) {

    CHECK(!expr.empty());
    CHECK(isSupported(kind));
    mTrivialDescription = std::to_string(value) == expr;
    mExpr = expr;
    mValueKind = kind;
    mValue = value;
    mIsEvaluated = true;
}

LiteralConstantExpression::LiteralConstantExpression(ScalarType::Kind kind, uint64_t value)
  : LiteralConstantExpression(kind, value, std::to_string(value)) {}

LiteralConstantExpression* LiteralConstantExpression::tryParse(const std::string& value) {
    CHECK(!value.empty());

    bool isLong = false, isUnsigned = false;
    bool isHex = (value[0] == '0' && value.length() > 1 && (value[1] == 'x' || value[1] == 'X'));

    auto rbegin = value.rbegin();
    auto rend = value.rend();
    for (; rbegin != rend && (*rbegin == 'u' || *rbegin == 'U' || *rbegin == 'l' || *rbegin == 'L');
         ++rbegin) {
        isUnsigned |= (*rbegin == 'u' || *rbegin == 'U');
        isLong |= (*rbegin == 'l' || *rbegin == 'L');
    }
    std::string newVal(value.begin(), rbegin.base());
    CHECK(!newVal.empty());

    uint64_t rawValue = 0;

    bool parseOK = base::ParseUint(newVal, &rawValue);
    if (!parseOK) {
        return nullptr;
    }

    ScalarType::Kind kind;

    // guess literal type.
    if(isLong) {
        if(isUnsigned) // ul
            kind = SK(UINT64);
        else // l
            kind = SK(INT64);
    } else { // no l suffix
        if(isUnsigned) { // u
            if(rawValue <= UINT32_MAX)
                kind = SK(UINT32);
            else
                kind = SK(UINT64);
        } else { // no suffix
            if(isHex) {
                if(rawValue <= INT32_MAX) // rawValue always >= 0
                    kind = SK(INT32);
                else if(rawValue <= UINT32_MAX)
                    kind = SK(UINT32);
                else if(rawValue <= INT64_MAX) // rawValue always >= 0
                    kind = SK(INT64);
                else if(rawValue <= UINT64_MAX)
                    kind = SK(UINT64);
                else
                    return nullptr;
            } else {
                if(rawValue <= INT32_MAX) // rawValue always >= 0
                    kind = SK(INT32);
                else
                    kind = SK(INT64);
            }
        }
    }

    return new LiteralConstantExpression(kind, rawValue, value);
}

void LiteralConstantExpression::evaluate() {
    // Evaluated in constructor
    CHECK(isEvaluated());
}

void UnaryConstantExpression::evaluate() {
    if (isEvaluated()) return;
    CHECK(mUnary->isEvaluated());
    mIsEvaluated = true;

    mExpr = std::string("(") + mOp + mUnary->mExpr + ")";
    mValueKind = mUnary->mValueKind;

#define CASE_UNARY(__type__)                                          \
    mValue = handleUnary(mOp, static_cast<__type__>(mUnary->mValue)); \
    return;

    SWITCH_KIND(mValueKind, CASE_UNARY, SHOULD_NOT_REACH(); return;)
}

void BinaryConstantExpression::evaluate() {
    if (isEvaluated()) return;
    CHECK(mLval->isEvaluated());
    CHECK(mRval->isEvaluated());
    mIsEvaluated = true;

    mExpr = std::string("(") + mLval->mExpr + " " + mOp + " " + mRval->mExpr + ")";

    bool isArithmeticOrBitflip = OP_IS_BIN_ARITHMETIC || OP_IS_BIN_BITFLIP;

    // CASE 1: + - *  / % | ^ & < > <= >= == !=
    if(isArithmeticOrBitflip || OP_IS_BIN_COMP) {
        // promoted kind for both operands.
        ScalarType::Kind promoted = usualArithmeticConversion(integralPromotion(mLval->mValueKind),
                                                              integralPromotion(mRval->mValueKind));
        // result kind.
        mValueKind = isArithmeticOrBitflip
                    ? promoted // arithmetic or bitflip operators generates promoted type
                    : SK(BOOL); // comparison operators generates bool

#define CASE_BINARY_COMMON(__type__)                                       \
    mValue = handleBinaryCommon(static_cast<__type__>(mLval->mValue), mOp, \
                                static_cast<__type__>(mRval->mValue));     \
    return;

        SWITCH_KIND(promoted, CASE_BINARY_COMMON, SHOULD_NOT_REACH(); return;)
    }

    // CASE 2: << >>
    std::string newOp = mOp;
    if(OP_IS_BIN_SHIFT) {
        mValueKind = integralPromotion(mLval->mValueKind);
        // instead of promoting rval, simply casting it to int64 should also be good.
        int64_t numBits = mRval->cast<int64_t>();
        if(numBits < 0) {
            // shifting with negative number of bits is undefined in C. In HIDL it
            // is defined as shifting into the other direction.
            newOp = OPEQ("<<") ? std::string(">>") : std::string("<<");
            numBits = -numBits;
        }

#define CASE_SHIFT(__type__)                                                    \
    mValue = handleShift(static_cast<__type__>(mLval->mValue), newOp, numBits); \
    return;

        SWITCH_KIND(mValueKind, CASE_SHIFT, SHOULD_NOT_REACH(); return;)
    }

    // CASE 3: && ||
    if(OP_IS_BIN_LOGICAL) {
        mValueKind = SK(BOOL);
        // easy; everything is bool.
        mValue = handleLogical(mLval->mValue, mOp, mRval->mValue);
        return;
    }

    SHOULD_NOT_REACH();
}

void TernaryConstantExpression::evaluate() {
    if (isEvaluated()) return;
    CHECK(mCond->isEvaluated());
    CHECK(mTrueVal->isEvaluated());
    CHECK(mFalseVal->isEvaluated());
    mIsEvaluated = true;

    mExpr = std::string("(") + mCond->mExpr + "?" + mTrueVal->mExpr + ":" + mFalseVal->mExpr + ")";

    // note: for ?:, unlike arithmetic ops, integral promotion is not processed.
    mValueKind = usualArithmeticConversion(mTrueVal->mValueKind, mFalseVal->mValueKind);

#define CASE_TERNARY(__type__)                                           \
    mValue = mCond->mValue ? (static_cast<__type__>(mTrueVal->mValue))   \
                           : (static_cast<__type__>(mFalseVal->mValue)); \
    return;

    SWITCH_KIND(mValueKind, CASE_TERNARY, SHOULD_NOT_REACH(); return;)
}

void ReferenceConstantExpression::evaluate() {
    if (isEvaluated()) return;
    CHECK(mReference->constExpr() != nullptr);

    ConstantExpression* expr = mReference->constExpr();
    CHECK(expr->isEvaluated());

    mValueKind = expr->mValueKind;
    mValue = expr->mValue;
    mIsEvaluated = true;
}

status_t AttributeConstantExpression::validate() const {
    if (mTag == "len") {
        if (!mReference->isEnum()) {
            std::cerr << "ERROR: " << mExpr << " refers to " << mReference->typeName()
                      << " but should refer to an enum." << std::endl;
            return UNKNOWN_ERROR;
        }
    } else {
        std::cerr << "ERROR: " << mExpr << " is not a supported tag" << std::endl;
        return UNKNOWN_ERROR;
    }

    return OK;
}

void AttributeConstantExpression::evaluate() {
    if (isEvaluated()) return;

    CHECK(mTag == "len");
    CHECK(mReference->isEnum());

    EnumType* enumType = static_cast<EnumType*>(mReference.get());
    mValue = enumType->numValueNames();

    if (mValue <= INT32_MAX)
        mValueKind = SK(INT32);
    else
        mValueKind = SK(INT64);

    mIsEvaluated = true;
}

std::unique_ptr<ConstantExpression> ConstantExpression::addOne(ScalarType::Kind baseKind) {
    auto ret = std::make_unique<BinaryConstantExpression>(
        this, "+", ConstantExpression::One(baseKind).release());
    return ret;
}

std::string ConstantExpression::value() const {
    return value(mValueKind);
}

std::string ConstantExpression::value(ScalarType::Kind castKind) const {
    CHECK(isEvaluated());
    return rawValue(castKind) + descriptionSuffix();
}

std::string ConstantExpression::cppValue() const {
    return cppValue(mValueKind);
}

std::string ConstantExpression::cppValue(ScalarType::Kind castKind) const {
    CHECK(isEvaluated());
    std::string literal(rawValue(castKind));
    // this is a hack to translate
    //       enum x : int64_t {  y = 1l << 63 };
    // into
    //       enum class x : int64_t { y = (int64_t)-9223372036854775808ull };
    // by adding the explicit cast.
    // Because 9223372036854775808 is uint64_t, and
    // -(uint64_t)9223372036854775808 == 9223372036854775808 could not
    // be narrowed to int64_t.
    if(castKind == SK(INT64) && (int64_t)mValue == INT64_MIN) {
        literal = "static_cast<" +
                  ScalarType(SK(INT64), nullptr /* parent */).getCppStackType()  // "int64_t"
                  + ">(" + literal + "ull)";
    } else {
        // add suffix if necessary.
        if (castKind == SK(UINT32) || castKind == SK(UINT64)) literal += "u";
        if (castKind == SK(UINT64) || castKind == SK(INT64)) literal += "ll";
    }

    return literal + descriptionSuffix();
}

std::string ConstantExpression::javaValue() const {
    return javaValue(mValueKind);
}

std::string ConstantExpression::javaValue(ScalarType::Kind castKind) const {
    CHECK(isEvaluated());
    std::string literal;

    switch(castKind) {
        case SK(UINT64):
            literal = rawValue(SK(INT64)) + "L";
            break;
        case SK(INT64):
            literal = rawValue(SK(INT64)) + "L";
            break;
        case SK(UINT32):
            literal = rawValue(SK(INT32));
            break;
        case SK(UINT16):
            literal = rawValue(SK(INT16));
            break;
        case SK(UINT8):
            literal = rawValue(SK(INT8));
            break;
        case SK(BOOL)  :
            literal = this->cast<bool>() ? "true" : "false";
            break;
        default:
            literal = rawValue(castKind);
            break;
    }

    return literal + descriptionSuffix();
}

const std::string& ConstantExpression::expression() const {
    CHECK(isEvaluated());
    return mExpr;
}

std::string ConstantExpression::rawValue() const {
    return rawValue(mValueKind);
}

std::string ConstantExpression::rawValue(ScalarType::Kind castKind) const {
    CHECK(isEvaluated());

#define CASE_STR(__type__) return std::to_string(this->cast<__type__>());

    SWITCH_KIND(castKind, CASE_STR, SHOULD_NOT_REACH(); return nullptr; );
}

template<typename T>
T ConstantExpression::cast() const {
    CHECK(isEvaluated());

#define CASE_CAST_T(__type__) return static_cast<T>(static_cast<__type__>(mValue));

    SWITCH_KIND(mValueKind, CASE_CAST_T, SHOULD_NOT_REACH(); return 0; );
}

std::string ConstantExpression::descriptionSuffix() const {
    CHECK(isEvaluated());

    if (!mTrivialDescription) {
        CHECK(!mExpr.empty());

        return " /* " + mExpr + " */";
    }
    return "";
}

size_t ConstantExpression::castSizeT() const {
    CHECK(isEvaluated());
    return this->cast<size_t>();
}

bool ConstantExpression::isReferenceConstantExpression() const {
    return false;
}

status_t ConstantExpression::validate() const {
    return OK;
}

std::vector<ConstantExpression*> ConstantExpression::getConstantExpressions() {
    const auto& constRet = static_cast<const ConstantExpression*>(this)->getConstantExpressions();
    std::vector<ConstantExpression*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ce) { return const_cast<ConstantExpression*>(ce); });
    return ret;
}

std::vector<Reference<LocalIdentifier>*> ConstantExpression::getReferences() {
    const auto& constRet = static_cast<const ConstantExpression*>(this)->getReferences();
    std::vector<Reference<LocalIdentifier>*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ce) { return const_cast<Reference<LocalIdentifier>*>(ce); });
    return ret;
}

std::vector<const Reference<LocalIdentifier>*> ConstantExpression::getReferences() const {
    return {};
}

std::vector<Reference<Type>*> ConstantExpression::getTypeReferences() {
    const auto& constRet = static_cast<const ConstantExpression*>(this)->getTypeReferences();
    std::vector<Reference<Type>*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ce) { return const_cast<Reference<Type>*>(ce); });
    return ret;
}

std::vector<const Reference<Type>*> ConstantExpression::getTypeReferences() const {
    return {};
}

status_t ConstantExpression::recursivePass(const std::function<status_t(ConstantExpression*)>& func,
                                           std::unordered_set<const ConstantExpression*>* visited,
                                           bool processBeforeDependencies) {
    if (mIsPostParseCompleted) return OK;

    if (visited->find(this) != visited->end()) return OK;
    visited->insert(this);

    if (processBeforeDependencies) {
        status_t err = func(this);
        if (err != OK) return err;
    }

    for (auto* nextCE : getConstantExpressions()) {
        status_t err = nextCE->recursivePass(func, visited, processBeforeDependencies);
        if (err != OK) return err;
    }

    for (auto* nextRef : getReferences()) {
        auto* nextCE = nextRef->shallowGet()->constExpr();
        CHECK(nextCE != nullptr) << "Local identifier is not a constant expression";
        status_t err = nextCE->recursivePass(func, visited, processBeforeDependencies);
        if (err != OK) return err;
    }

    if (!processBeforeDependencies) {
        status_t err = func(this);
        if (err != OK) return err;
    }

    return OK;
}

status_t ConstantExpression::recursivePass(
    const std::function<status_t(const ConstantExpression*)>& func,
    std::unordered_set<const ConstantExpression*>* visited, bool processBeforeDependencies) const {
    if (mIsPostParseCompleted) return OK;

    if (visited->find(this) != visited->end()) return OK;
    visited->insert(this);

    if (processBeforeDependencies) {
        status_t err = func(this);
        if (err != OK) return err;
    }

    for (const auto* nextCE : getConstantExpressions()) {
        status_t err = nextCE->recursivePass(func, visited, processBeforeDependencies);
        if (err != OK) return err;
    }

    for (const auto* nextRef : getReferences()) {
        const auto* nextCE = nextRef->shallowGet()->constExpr();
        CHECK(nextCE != nullptr) << "Local identifier is not a constant expression";
        status_t err = nextCE->recursivePass(func, visited, processBeforeDependencies);
        if (err != OK) return err;
    }

    if (!processBeforeDependencies) {
        status_t err = func(this);
        if (err != OK) return err;
    }

    return OK;
}

ConstantExpression::CheckAcyclicStatus::CheckAcyclicStatus(
    status_t status, const ConstantExpression* cycleEnd,
    const ReferenceConstantExpression* lastReference)
    : status(status), cycleEnd(cycleEnd), lastReference(lastReference) {
    CHECK(cycleEnd == nullptr || status != OK);
    CHECK((cycleEnd == nullptr) == (lastReference == nullptr));
}

ConstantExpression::CheckAcyclicStatus ConstantExpression::checkAcyclic(
    std::unordered_set<const ConstantExpression*>* visited,
    std::unordered_set<const ConstantExpression*>* stack) const {
    if (stack->find(this) != stack->end()) {
        CHECK(isReferenceConstantExpression())
            << "Only reference constant expression could be the cycle end";

        std::cerr << "ERROR: Cyclic declaration:\n";
        return CheckAcyclicStatus(UNKNOWN_ERROR, this,
                                  static_cast<const ReferenceConstantExpression*>(this));
    }

    if (visited->find(this) != visited->end()) return CheckAcyclicStatus(OK);
    visited->insert(this);
    stack->insert(this);

    for (const auto* nextCE : getConstantExpressions()) {
        auto err = nextCE->checkAcyclic(visited, stack);
        if (err.status != OK) {
            return err;
        }
    }

    for (const auto* nextRef : getReferences()) {
        const auto* nextCE = nextRef->shallowGet()->constExpr();
        CHECK(nextCE != nullptr) << "Local identifier is not a constant expression";
        auto err = nextCE->checkAcyclic(visited, stack);

        if (err.status != OK) {
            if (err.cycleEnd == nullptr) return err;

            // Only ReferenceConstantExpression has references,
            CHECK(isReferenceConstantExpression())
                << "Only reference constant expression could have refereneces";

            // mExpr is defined explicitly before evaluation
            std::cerr << "  '" << err.lastReference->mExpr << "' in '" << mExpr << "' at "
                      << nextRef->location() << "\n";

            if (err.cycleEnd == this) {
                return CheckAcyclicStatus(err.status);
            }
            return CheckAcyclicStatus(err.status, err.cycleEnd,
                                      static_cast<const ReferenceConstantExpression*>(this));
        }
    }

    CHECK(stack->find(this) != stack->end());
    stack->erase(this);
    return CheckAcyclicStatus(OK);
}

void ConstantExpression::setPostParseCompleted() {
    CHECK(!mIsPostParseCompleted);
    mIsPostParseCompleted = true;
}

std::vector<const ConstantExpression*> LiteralConstantExpression::getConstantExpressions() const {
    return {};
}

UnaryConstantExpression::UnaryConstantExpression(const std::string& op, ConstantExpression* value)
    : mUnary(value), mOp(op) {}

std::vector<const ConstantExpression*> UnaryConstantExpression::getConstantExpressions() const {
    return {mUnary};
}

BinaryConstantExpression::BinaryConstantExpression(ConstantExpression* lval, const std::string& op,
                                                   ConstantExpression* rval)
    : mLval(lval), mRval(rval), mOp(op) {}

std::vector<const ConstantExpression*> BinaryConstantExpression::getConstantExpressions() const {
    return {mLval, mRval};
}

TernaryConstantExpression::TernaryConstantExpression(ConstantExpression* cond,
                                                     ConstantExpression* trueVal,
                                                     ConstantExpression* falseVal)
    : mCond(cond), mTrueVal(trueVal), mFalseVal(falseVal) {}

std::vector<const ConstantExpression*> TernaryConstantExpression::getConstantExpressions() const {
    return {mCond, mTrueVal, mFalseVal};
}

ReferenceConstantExpression::ReferenceConstantExpression(const Reference<LocalIdentifier>& value,
                                                         const std::string& expr)
    : mReference(value) {
    mExpr = expr;
    mTrivialDescription = mExpr.empty();
}

bool ReferenceConstantExpression::isReferenceConstantExpression() const {
    return true;
}

std::vector<const ConstantExpression*> ReferenceConstantExpression::getConstantExpressions() const {
    // Returns reference instead
    return {};
}

std::vector<const Reference<LocalIdentifier>*> ReferenceConstantExpression::getReferences() const {
    return {&mReference};
}

AttributeConstantExpression::AttributeConstantExpression(const Reference<Type>& value,
                                                         const std::string& fqname,
                                                         const std::string& tag)
    : mReference(value), mTag(tag) {
    mExpr = fqname + "#" + tag;
}

std::vector<const ConstantExpression*> AttributeConstantExpression::getConstantExpressions() const {
    // Returns reference instead
    return {};
}

std::vector<const Reference<Type>*> AttributeConstantExpression::getTypeReferences() const {
    return {&mReference};
}

/*

Evaluating expressions in HIDL language

The following rules are mostly like that in:
http://en.cppreference.com/w/cpp/language/operator_arithmetic
http://en.cppreference.com/w/cpp/language/operator_logical
http://en.cppreference.com/w/cpp/language/operator_comparison
http://en.cppreference.com/w/cpp/language/operator_other

The type of literal is the first type which the value
can fit from the list of types depending on the suffix and bases.

suffix              decimal bases           hexadecimal bases
no suffix           int32_t                 int32_t
                    int64_t                 uint32_t
                                            int64_t
                                            uint64_t

u/U                 uint32_t                (same as left)
                    uint64_t

l/L                 int64_t                 int64_t

ul/UL/uL/Ul         uint64_t                uint64_t


Note: There are no negative integer literals.
      -1 is the unary minus applied to 1.

Unary arithmetic and bitwise operators (~ + -):
  don't change the type of the argument.
  (so -1u = -(1u) has type uint32_t)

Binary arithmetic and bitwise operators (except shifts) (+ - * / % & | ^):
1. Integral promotion is first applied on both sides.
2. If both operands have the same type, no promotion is necessary.
3. Usual arithmetic conversions.

Integral promotion: if an operand is of a type with less than 32 bits,
(including bool), it is promoted to int32_t.

Usual arithmetic conversions:
1. If operands are both signed or both unsigned, lesser conversion rank is
   converted to greater conversion rank.
2. Otherwise, if unsigned's rank >= signed's rank, -> unsigned's type
3. Otherwise, if signed's type can hold all values in unsigned's type,
   -> signed's type
4. Otherwise, both converted to the unsigned counterpart of the signed operand's
   type.
rank: bool < int8_t < int16_t < int32_t < int64_t


Shift operators (<< >>):
1. Integral promotion is applied on both sides.
2. For unsigned a, a << b discards bits that shifts out.
   For signed non-negative a, a << b is legal if no bits shifts out, otherwise error.
   For signed negative a, a << b gives error.
3. For unsigned and signed non-negative a, a >> b discards bits that shifts out.
   For signed negative a, a >> b discards bits that shifts out, and the signed
   bit gets extended. ("arithmetic right shift")
4. Shifting with negative number of bits is undefined. (Currently, the
   parser will shift into the other direction. This behavior may change.)
5. Shifting with number of bits exceeding the width of the type is undefined.
   (Currently, 1 << 32 == 1. This behavior may change.)

Logical operators (!, &&, ||):
1. Convert first operand to bool. (true if non-zero, false otherwise)
2. If short-circuited, return the result as type bool, value 1 or 0.
3. Otherwise, convert second operand to bool, evaluate the result, and return
   the result in the same fashion.

Arithmetic comparison operators (< > <= >= == !=):
1. Promote operands in the same way as binary arithmetic and bitwise operators.
   (Integral promotion + Usual arithmetic conversions)
2. Return type bool, value 0 or 1 the same way as logical operators.

Ternary conditional operator (?:):
1. Evaluate the conditional and evaluate the operands.
2. Return type of expression is the type under usual arithmetic conversions on
   the second and third operand. (No integral promotions necessary.)

*/

}  // namespace android

