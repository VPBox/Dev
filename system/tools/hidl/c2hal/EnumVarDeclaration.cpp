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

#include <hidl-util/StringHelper.h>

#include "EnumVarDeclaration.h"
#include "Expression.h"

namespace android {

EnumVarDeclaration::EnumVarDeclaration(const std::string &name, Expression *expression)
    : Declaration(""), mExpression(expression)
{
    setName(name);
}

EnumVarDeclaration::~EnumVarDeclaration() {
    delete mExpression;
}

void EnumVarDeclaration::setName(const std::string &name) {
    Declaration::setName(name);
    forceUpperSnakeCase();
}

Expression *EnumVarDeclaration::getExpression() const {
    return mExpression;
}

void EnumVarDeclaration::generateSource(Formatter &out) const {
    out << getName();

    if(mExpression != nullptr) {
        out << " = " << mExpression->toString(StringHelper::kUpperSnakeCase);
    }

    out << ",\n";
}

void EnumVarDeclaration::processContents(AST &) {
    // nothing to do
}

} //namespace android
