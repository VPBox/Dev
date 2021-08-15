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

#include "FunctionDeclaration.h"
#include "VarDeclaration.h"
#include "Type.h"

#include <hidl-util/StringHelper.h>

namespace android {

FunctionDeclaration::FunctionDeclaration(Type* type,
                  const std::string &name,
                  std::vector<Declaration *> *params)
    : Declaration(""),
      mType(type),
      mParams(params)
    {
        setName(name);
    }

FunctionDeclaration::~FunctionDeclaration() {
    delete mType;

    if(mParams != nullptr) {
        for(auto* param : *mParams) {
            delete param;
        }
    }
    delete mParams;
}

void FunctionDeclaration::setName(const std::string &name) {
    Declaration::setName(name);
    forceCamelCase();
}

const Type* FunctionDeclaration::getType() const {
    return mType;
}

void FunctionDeclaration::generateSource(Formatter &out) const {
    out << getName();

    generateParams(out);

    if (!getType()->isVoid()) {
        out << " generates ("
            << getType()->decorateName(getName() + "Ret")
            << ")";
    }

    out << ";\n";
}

void FunctionDeclaration::generateParameterSource(Formatter &out) const {
    out << getType()->decorateName("(*" + getName() + ")");

    generateParams(out);
}

void FunctionDeclaration::processContents(AST &) {
    if (mParams->size() == 1 &&
        (*mParams)[0]->decType() == VarDeclaration::type()) {

        VarDeclaration* var = (VarDeclaration *)(*mParams)[0];
        if (var->getType()->isVoid()) {
            mParams->clear();
        }
    }
}

void FunctionDeclaration::generateParams(Formatter &out) const {
    out << "(";

    for (auto it = mParams->begin(); it != mParams->end(); ++it) {
        if (it != mParams->begin()) {
            out << ", ";
        }

        (*it)->generateParameterSource(out);
    }

    out << ")";
}

} //namespace android