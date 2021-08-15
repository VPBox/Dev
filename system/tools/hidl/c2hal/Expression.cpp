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

#include "Expression.h"
#include "Define.h"
#include "AST.h"
#include "Scope.h"

#include <vector>
#include <regex>

namespace android {

static const std::regex RE_S32("[^ul]$");
static const std::regex RE_U32("[^ul]u$");
static const std::regex RE_S64("[^ul](l|ll)$");
static const std::regex RE_U64("[^ul](ul|ull)$");

Expression::Type Expression::integralType(const std::string& integer) {
    if (std::regex_search(integer, RE_S32)) {
        return Type::S32;
    }

    if (std::regex_search(integer, RE_U32)) {
        return Type::U32;
    }

    if (std::regex_search(integer, RE_S64)) {
        return Type::S64;
    }

    if (std::regex_search(integer, RE_U64)) {
        return Type::U64;
    }

    LOG(WARNING) << "UNKNOWN INTEGER LITERAL: " << integer;

    return Type::UNKNOWN;
}

Expression::Type Expression::coalesceTypes(Type lhs, Type rhs) {
    // because we are reducing everything to two ranks, we can heavily simplify
    // conversion rules

#define SIGNED(i) ((i) & 2) // i & 0b10
#define MAX_RANK(i) ((i) | 1) // i | 0b01

    if (lhs == rhs) {
        return lhs;
    }

    // lhs != rhs
    if (SIGNED(lhs) == SIGNED(rhs)) {
        return (Type)MAX_RANK(lhs);
    }

    // lhs != rhs && SIGNED(lhs) != SIGNED(rhs)
    if (lhs == U32 || rhs == U32) {
        return S64;
    }

    return Type::UNKNOWN;

#undef SIGNED
#undef MAX_RANK

}

struct ParenthesizedExpression : Expression {
    ParenthesizedExpression(Expression* inner)
    : mInner(inner) {}
    ~ParenthesizedExpression() override {
        delete mInner;
    }

    Type getType(const AST &ast) override {
        return mInner->getType(ast);
    }
    std::string toString(StringHelper::Case atomCase) override {
        return "(" + mInner->toString(atomCase) + ")";
    }

private:
    Expression* mInner;

    DISALLOW_COPY_AND_ASSIGN(ParenthesizedExpression);
};

struct AtomExpression : Expression {
    AtomExpression(Type type, const std::string &value, bool isId)
    : mType(type), mValue(value), mIsId(isId)
    {}

    Type getType(const AST &ast) override {
        if (mType != Type::UNKNOWN) {
            return mType;
        }

        Define *define = ast.getDefinesScope().lookup(mValue);

        if (define == nullptr) {
            return Type::UNKNOWN;
        }

        return define->getExpressionType();
    }
    std::string toString(StringHelper::Case atomCase) override {
        // do not enforce case if it is not an identifier.
        return mIsId ? StringHelper::ToCase(atomCase, mValue) : mValue;
    }

private:
    Type mType;
    std::string mValue;
    bool mIsId;

    DISALLOW_COPY_AND_ASSIGN(AtomExpression);
};

struct UnaryExpression : Expression {
    UnaryExpression(std::string op, Expression* rhs)
    : mOp(op), mRhs(rhs)
    {}
    ~UnaryExpression() override {
        delete mRhs;
    }

    Type getType(const AST &ast) override {
        return mRhs->getType(ast);
    }
    std::string toString(StringHelper::Case atomCase) override {
        return mOp + mRhs->toString(atomCase);
    }

private:
    std::string mOp;
    Expression* mRhs;

    DISALLOW_COPY_AND_ASSIGN(UnaryExpression);
};

struct BinaryExpression : Expression {
    BinaryExpression(Expression *lhs, std::string op, Expression* rhs)
    : mLhs(lhs), mOp(op), mRhs(rhs)
    {}
    ~BinaryExpression() override {
        delete mLhs;
        delete mRhs;
    }

    Type getType(const AST &ast) override {
        return coalesceTypes(mLhs->getType(ast), mRhs->getType(ast));
    }
    std::string toString(StringHelper::Case atomCase) override {
        return mLhs->toString(atomCase) + " " + mOp + " " + mRhs->toString(atomCase);
    }

private:
    Expression* mLhs;
    std::string mOp;
    Expression* mRhs;

    DISALLOW_COPY_AND_ASSIGN(BinaryExpression);
};

struct TernaryExpression : Expression {
    TernaryExpression(Expression *lhs, Expression *mhs, Expression* rhs)
    : mLhs(lhs), mMhs(mhs), mRhs(rhs)
    {}
    ~TernaryExpression() override {
        delete mLhs;
        delete mMhs;
        delete mRhs;
    }

    Type getType(const AST &ast) override {
        return coalesceTypes(mMhs->getType(ast), mRhs->getType(ast));
    }
    std::string toString(StringHelper::Case atomCase) override {
        return mLhs->toString(atomCase) + " ? " + mMhs->toString(atomCase) + " : " + mRhs->toString(atomCase);
    }

private:
    Expression* mLhs;
    Expression* mMhs;
    Expression* mRhs;

    DISALLOW_COPY_AND_ASSIGN(TernaryExpression);
};

struct ArraySubscript : Expression {
    ArraySubscript(std::string id, Expression* subscript)
    : mId(id), mSubscript(subscript)
    {}
    ~ArraySubscript() override {
        delete mSubscript;
    }

    Type getType(const AST &) override {
        return Type::UNKNOWN;
    }
    std::string toString(StringHelper::Case atomCase) override {
        return mId + "[" + mSubscript->toString(atomCase) + "]";
    }

private:
    std::string mId;
    Expression* mSubscript;

    DISALLOW_COPY_AND_ASSIGN(ArraySubscript);
};

struct FunctionCall : Expression {
    FunctionCall(std::string id, std::vector<Expression *> *args)
    : mId(id), mArgs(args)
    {}
    ~FunctionCall() override {
        if(mArgs != nullptr) {
            for(auto* args : *mArgs) {
                delete args;
            }
        }
        delete mArgs;
    }

    Type getType(const AST &) override {
        return Type::UNKNOWN;
    }
    std::string toString(StringHelper::Case atomCase) override {
        std::string out = mId + "(";

        for (auto it = mArgs->begin(); it != mArgs->end(); ++it) {
            if (it != mArgs->begin()) {
                out += ", ";
            }

            out += (*it)->toString(atomCase);
        }

        out += ")";

        return out;
    }

private:
    std::string mId;
    std::vector<Expression *> *mArgs;

    DISALLOW_COPY_AND_ASSIGN(FunctionCall);
};

Expression *Expression::parenthesize(Expression *inner) {
    return new ParenthesizedExpression(inner);
}

Expression *Expression::atom(Type type, const std::string &value, bool isId) {
    return new AtomExpression(type, value, isId);
}

Expression *Expression::unary(std::string op, Expression *rhs) {
    return new UnaryExpression(op, rhs);
}

Expression *Expression::binary(Expression *lhs, std::string op, Expression *rhs) {
    return new BinaryExpression(lhs, op, rhs);
}

Expression *Expression::ternary(Expression *lhs, Expression *mhs, Expression *rhs) {
    return new TernaryExpression(lhs, mhs, rhs);
}

Expression *Expression::arraySubscript(std::string id, Expression *subscript) {
    return new ArraySubscript(id, subscript);
}

Expression *Expression::functionCall(std::string id, std::vector<Expression *> *args) {
    return new FunctionCall(id, args);
}


} //namespace android
