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

#include "VarDeclaration.h"

namespace android {

VarDeclaration::VarDeclaration(Type *type, const std::string &name)
    : Declaration(""),
      mType(type)
    {
        setName(name);
    }

VarDeclaration::~VarDeclaration() {
    delete mType;
}

void VarDeclaration::setName(const std::string &name) {
    Declaration::setName(name);
    // special case for varargs
    if(getName() == "...")
        return;
    forceCamelCase();
}

Type* VarDeclaration::getType() const {
    return mType;
}

void VarDeclaration::generateSource(Formatter &out) const {
    out << getType()->decorateName(getName())
        << ";\n";
}

void VarDeclaration::generateParameterSource(Formatter &out) const {
    out << getType()->decorateName(getName());
}

void VarDeclaration::processContents(AST &) {
    // nothing to do
}

} // namespace android