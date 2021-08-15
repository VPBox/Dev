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

#include "Define.h"

#include "AST.h"

extern android::status_t parseExpression(android::AST *ast, std::string str);

namespace android {

Define::Define(const std::string &name, const std::string &slurp)
    : Declaration(name), mSlurp(slurp)
    {}

Define::~Define() {
    delete mExpression;
}

Expression::Type Define::getExpressionType() const {
    return mExpressionType;
}
Expression *Define::getExpression() const {
    return mExpression;
}
void Define::setExpression(Expression* expression) {
    mExpression = expression;
}

void Define::generateSource(Formatter &out) const {
    out << "/* #define " << getName() << " " << mSlurp << " */\n";
}

void Define::processContents(AST &ast) {
    status_t res = parseExpression(&ast, mSlurp);

    if (res != 0) {
        mExpressionType = Expression::Type::UNKNOWN;
        return;
    }

    mExpression = ast.getExpression();
    ast.setExpression(nullptr);

    mExpressionType = mExpression->getType(ast);

    ast.getDefinesScope().enter(getName(), this);
}

} //namespace android
