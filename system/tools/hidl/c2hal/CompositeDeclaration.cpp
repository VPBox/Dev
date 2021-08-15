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

#include "CompositeDeclaration.h"
#include "FunctionDeclaration.h"
#include "VarDeclaration.h"
#include "Declaration.h"

#include <algorithm>
#include <iostream>
#include <string>

namespace android {

CompositeDeclaration::CompositeDeclaration(
        const Type::Qualifier::Qualification qualifier,
        const std::string &name,
        std::vector<android::Declaration *> *fieldDeclarations)
    : Declaration(""),
      mQualifier(qualifier),
      mFieldDeclarations(fieldDeclarations)
    {
        setName(name);
    }

CompositeDeclaration::~CompositeDeclaration() {
    if(mFieldDeclarations != nullptr) {
        for(auto* decl : *mFieldDeclarations) {
            delete decl;
        }
    }
    delete mFieldDeclarations;
}

void CompositeDeclaration::setName(const std::string &name) {
    Declaration::setName(name);
    forcePascalCase();
}

const Type::Qualifier::Qualification &CompositeDeclaration::getQualifier() const {
    return mQualifier;
}
const std::vector<android::Declaration *>*
    CompositeDeclaration::getFieldDeclarations() const {
    return mFieldDeclarations;
}

void CompositeDeclaration::generateInterface(Formatter &out) const {
    generateCommentText(out);
    out << "interface " << getInterfaceName() << " {\n\n";

    generateBody(out);

    out << "};\n";
}

void CompositeDeclaration::generateSource(Formatter &out) const {
    CHECK(mQualifier == Type::Qualifier::STRUCT ||
          mQualifier == Type::Qualifier::UNION ||
          mQualifier == Type::Qualifier::ENUM);

    out << Type::qualifierText(mQualifier) << " " << getName();

    if (mQualifier == Type::Qualifier::ENUM) {
        out << " : ";

        if (mEnumTypeName.empty()) {
            out << "int32_t /* NOTE: type is guessed */";
        } else {
            out << mEnumTypeName;
        }

    }

    out << " {\n";

    generateBody(out);

    out << "};\n";
}

void CompositeDeclaration::generateBody(Formatter &out) const {
    out.indent();

    for (auto *declaration : *mFieldDeclarations) {
        declaration->generateCommentText(out);
        declaration->generateSource(out);
        out << "\n";
    }

    out.unindent();
}

void CompositeDeclaration::processContents(AST &ast) {
    for (auto &declaration : *mFieldDeclarations) {
        declaration->processContents(ast);
    }

    if (isInterface()) {
        // move non function fields into a containing struct

        auto nonFpDecs = new std::vector<Declaration*>;

        auto it = mFieldDeclarations->begin();
        while (it != mFieldDeclarations->end()) {
            if((*it)->decType() != FunctionDeclaration::type()) {
                bool keep = true;
                if((*it)->decType() == VarDeclaration::type()) {
                  VarDeclaration* var = (VarDeclaration *)(*it);
                  // Conventional HALs were all required to have
                  // a member of this type.
                  // This member is no longer needed for HIDL
                  if(var->getType()->isHwDevice()) {
                    keep = false;
                  }
                }

                if (keep) {
                  nonFpDecs->push_back(*it);
                }
                it = mFieldDeclarations->erase(it);
            } else {
                it++;
            }
        }

        if (!nonFpDecs->empty()) {
            auto subStruct = new CompositeDeclaration(Type::Qualifier::STRUCT,
                                                      getName(),
                                                      nonFpDecs);

            mFieldDeclarations->insert(mFieldDeclarations->begin(), subStruct);
        }
    }
}

std::string CompositeDeclaration::getInterfaceName() const {
    return "I" + getName();
}

bool CompositeDeclaration::isInterface() const {
    if (mQualifier != Type::Qualifier::STRUCT) {
        return false;
    }

    for (auto &declaration : *mFieldDeclarations) {
        if (declaration->decType() == FunctionDeclaration::type()) {
            return true;
        }
    }
    return false;
}

void CompositeDeclaration::setEnumTypeName(const std::string &name) {
    CHECK(mQualifier == Type::Qualifier::ENUM);

    mEnumTypeName = name;
}

} //namespace android
