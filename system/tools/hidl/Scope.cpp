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

#include "Scope.h"

#include "Annotation.h"
#include "ConstantExpression.h"
#include "Interface.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <hidl-util/StringHelper.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace android {

Scope::Scope(const char* localName, const FQName& fullName, const Location& location, Scope* parent)
    : NamedType(localName, fullName, location, parent) {}
Scope::~Scope(){}

void Scope::addType(NamedType* type) {
    size_t index = mTypes.size();
    mTypes.push_back(type);
    mTypeIndexByName[type->localName()] = index;
}

status_t Scope::validateUniqueNames() const {
    for (const auto* type : mTypes) {
        if (mTypes[mTypeIndexByName.at(type->localName())] != type) {
            std::cerr << "ERROR: A type named '" << type->localName()
                      << "' is already declared in the scope at " << type->location() << std::endl;
            return UNKNOWN_ERROR;
        }
    }
    return OK;
}

NamedType *Scope::lookupType(const FQName &fqName) const {
    CHECK(fqName.package().empty() && fqName.version().empty());
    if (!fqName.valueName().empty()) {
        std::cerr << "ERROR: " << fqName.string() << " does not refer to a type." << std::endl;
        return nullptr;
    }
    std::vector<std::string> names = fqName.names();
    CHECK_GT(names.size(), 0u);
    auto it = mTypeIndexByName.find(names[0]);

    if (it == mTypeIndexByName.end()) {
        return nullptr;
    }

    NamedType *outerType = mTypes[it->second];
    if (names.size() == 1) {
        return outerType;
    }
    if (!outerType->isScope()) {
        // more than one names, but the first name is not a scope
        return nullptr;
    }
    Scope *outerScope = static_cast<Scope *>(outerType);
    // *slowly* pop first element
    names.erase(names.begin());
    FQName innerName;
    CHECK(FQName::parse(StringHelper::JoinStrings(names, "."), &innerName));
    return outerScope->lookupType(innerName);
}

LocalIdentifier *Scope::lookupIdentifier(const std::string & /*name*/) const {
    return nullptr;
}

bool Scope::isScope() const {
    return true;
}

Interface *Scope::getInterface() const {
    if (mTypes.size() == 1 && mTypes[0]->isInterface()) {
        return static_cast<Interface *>(mTypes[0]);
    }

    return nullptr;
}

bool Scope::definesInterfaces() const {
    for (const NamedType *type : mTypes) {
        if (type->isInterface()) {
            return true;
        }
    }

    return false;
}

const std::vector<Annotation*>& Scope::annotations() const {
    return mAnnotations;
}

void Scope::setAnnotations(std::vector<Annotation*>* annotations) {
    CHECK(mAnnotations.empty());
    CHECK(annotations != nullptr);
    mAnnotations = *annotations;
}

std::vector<const Type*> Scope::getDefinedTypes() const {
    std::vector<const Type*> ret;
    ret.insert(ret.end(), mTypes.begin(), mTypes.end());
    return ret;
}

std::vector<const ConstantExpression*> Scope::getConstantExpressions() const {
    std::vector<const ConstantExpression*> ret;
    for (const auto* annotation : mAnnotations) {
        const auto& retAnnotation = annotation->getConstantExpressions();
        ret.insert(ret.end(), retAnnotation.begin(), retAnnotation.end());
    }
    return ret;
}

void Scope::topologicalReorder(const std::unordered_map<const Type*, size_t>& reversedOrder) {
    auto less = [&](const Type* lhs, const Type* rhs) {
        return reversedOrder.at(lhs) < reversedOrder.at(rhs);
    };

    if (std::is_sorted(mTypes.begin(), mTypes.end(), less)) return;

    mTypeOrderChanged = true;
    std::sort(mTypes.begin(), mTypes.end(), less);

    for (size_t i = 0; i != mTypes.size(); ++i) {
        mTypeIndexByName.at(mTypes[i]->localName()) = i;
    }
}

void Scope::emitTypeDeclarations(Formatter& out) const {
    if (mTypes.empty()) return;

    out << "// Forward declaration for forward reference support:\n";
    for (const Type* type : mTypes) {
        type->emitTypeForwardDeclaration(out);
    }
    out << "\n";

    if (mTypeOrderChanged) {
        out << "// Order of inner types was changed for forward reference support.\n\n";
    }

    for (const Type* type : mTypes) {
        type->emitDocComment(out);
        type->emitTypeDeclarations(out);
    }
}

void Scope::emitGlobalTypeDeclarations(Formatter& out) const {
    for (const Type* type : mTypes) {
        type->emitGlobalTypeDeclarations(out);
    }
}

void Scope::emitPackageTypeDeclarations(Formatter& out) const {
    for (const Type* type : mTypes) {
        type->emitPackageTypeDeclarations(out);
    }
}

void Scope::emitPackageTypeHeaderDefinitions(Formatter& out) const {
    for (const Type* type : mTypes) {
        type->emitPackageTypeHeaderDefinitions(out);
    }
}

void Scope::emitPackageHwDeclarations(Formatter& out) const {
    for (const Type* type : mTypes) {
        type->emitPackageHwDeclarations(out);
    }
}

void Scope::emitJavaTypeDeclarations(Formatter& out, bool atTopLevel) const {
    if (mTypeOrderChanged) {
        out << "// Order of inner types was changed for forward reference support.\n\n";
    }

    for (const Type* type : mTypes) {
        type->emitDocComment(out);
        type->emitJavaTypeDeclarations(out, atTopLevel);
    }
}

void Scope::emitTypeDefinitions(Formatter& out, const std::string& prefix) const {
    for (const Type* type : mTypes) {
        type->emitTypeDefinitions(out, prefix);
    }
}

const std::vector<NamedType *> &Scope::getSubTypes() const {
    return mTypes;
}

void Scope::emitVtsTypeDeclarations(Formatter& out) const {
    for (const Type* type : mTypes) {
        type->emitVtsTypeDeclarations(out);
    }
}

bool Scope::deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const {
    for (const Type* type : mTypes) {
        if (!type->isJavaCompatible(visited)) {
            return false;
        }
    }
    return Type::deepIsJavaCompatible(visited);
}

void Scope::appendToExportedTypesVector(
        std::vector<const Type *> *exportedTypes) const {
    for (const Type* type : mTypes) {
        type->appendToExportedTypesVector(exportedTypes);
    }
}

////////////////////////////////////////

RootScope::RootScope(const char* localName, const FQName& fullName, const Location& location,
                     Scope* parent)
    : Scope(localName, fullName, location, parent) {}
RootScope::~RootScope() {}

std::string RootScope::typeName() const {
    return "(root scope)";
}

status_t RootScope::validate() const {
    CHECK(annotations().empty());
    return Scope::validate();
}

////////////////////////////////////////

LocalIdentifier::LocalIdentifier(){}
LocalIdentifier::~LocalIdentifier(){}

bool LocalIdentifier::isEnumValue() const {
    return false;
}

const LocalIdentifier* LocalIdentifier::resolve() const {
    return this;
}

LocalIdentifier* LocalIdentifier::resolve() {
    return this;
}

ConstantExpression* LocalIdentifier::constExpr() const {
    return nullptr;
}

}  // namespace android

