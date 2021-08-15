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

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <hidl-util/StringHelper.h>
#include <string>

namespace android {

struct AST;
struct Define;

struct Expression {
    Expression() {}
    virtual ~Expression() {}

    enum Type {
        S32 = 0, // 0b00
        S64 = 1, // 0b01
        U32 = 2, // 0b10
        U64 = 3, // 0b11
        UNKNOWN = -1
    };

    static std::string getTypeDescription(Type type) {
        switch (type) {
            case S32: return "S32";
            case S64: return "S64";
            case U32: return "U32";
            case U64: return "U64";
            case UNKNOWN:
            default:
                return "UNKNOWN";
        }
    }

    static std::string getTypeName(Type type) {
        switch (type) {
            case S32: return "int32_t";
            case S64: return "int64_t";
            case U32: return "uint32_t";
            case U64: return "uint64_t";
            case UNKNOWN:
            default:
                return "/* UNKNOWN */";
        }
    }

    static Type integralType(const std::string& integer);
    static Type coalesceTypes(Type lhs, Type rhs);

    static Expression *parenthesize(Expression *inner);
    static Expression *atom(Type type, const std::string &value, bool isId = false);
    static Expression *unary(std::string op, Expression *rhs);
    static Expression *binary(Expression *lhs, std::string op, Expression *rhs);
    static Expression *ternary(Expression *lhs, Expression *mhs, Expression *rhs);
    static Expression *arraySubscript(std::string id, Expression *subscript);
    static Expression *functionCall(std::string id, std::vector<Expression *> *args);

    virtual Type getType(const AST &scope) = 0;

    // convert this expression to a string.
    // atomCase: when it comes to atoms, force identifiers into a certain case.
    //           numerical values are not affected.
    virtual std::string toString(StringHelper::Case atomCase = StringHelper::kNoCase) = 0;

private:

    DISALLOW_COPY_AND_ASSIGN(Expression);
};

}  // namespace android

#endif  // EXPRESSION_H_
