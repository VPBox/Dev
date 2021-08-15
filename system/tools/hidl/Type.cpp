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

#include "Type.h"

#include "ConstantExpression.h"
#include "NamedType.h"
#include "ScalarType.h"
#include "Scope.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <algorithm>
#include <iostream>

namespace android {

Type::Type(Scope* parent) : mParent(parent) {}

Type::~Type() {}

bool Type::isScope() const {
    return false;
}

bool Type::isInterface() const {
    return false;
}

bool Type::isScalar() const {
    return false;
}

bool Type::isString() const {
    return false;
}

bool Type::isEnum() const {
    return false;
}

bool Type::isBitField() const {
    return false;
}

bool Type::isHandle() const {
    return false;
}

bool Type::isTypeDef() const {
    return false;
}

bool Type::isNamedType() const {
    return false;
}

bool Type::isMemory() const {
    return false;
}

bool Type::isCompoundType() const {
    return false;
}

bool Type::isArray() const {
    return false;
}

bool Type::isVector() const {
    return false;
}

bool Type::isTemplatedType() const {
    return false;
}

bool Type::isPointer() const {
    return false;
}

Type* Type::resolve() {
    return const_cast<Type*>(static_cast<const Type*>(this)->resolve());
}

const Type* Type::resolve() const {
    return this;
}

std::vector<Type*> Type::getDefinedTypes() {
    const auto& constRet = static_cast<const Type*>(this)->getDefinedTypes();
    std::vector<Type*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* type) { return const_cast<Type*>(type); });
    return ret;
}

std::vector<const Type*> Type::getDefinedTypes() const {
    return {};
}

std::vector<Reference<Type>*> Type::getReferences() {
    const auto& constRet = static_cast<const Type*>(this)->getReferences();
    std::vector<Reference<Type>*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ref) { return const_cast<Reference<Type>*>(ref); });
    return ret;
}

std::vector<const Reference<Type>*> Type::getReferences() const {
    return {};
}

std::vector<ConstantExpression*> Type::getConstantExpressions() {
    const auto& constRet = static_cast<const Type*>(this)->getConstantExpressions();
    std::vector<ConstantExpression*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ce) { return const_cast<ConstantExpression*>(ce); });
    return ret;
}

std::vector<const ConstantExpression*> Type::getConstantExpressions() const {
    return {};
}

std::vector<Reference<Type>*> Type::getStrongReferences() {
    const auto& constRet = static_cast<const Type*>(this)->getStrongReferences();
    std::vector<Reference<Type>*> ret(constRet.size());
    std::transform(constRet.begin(), constRet.end(), ret.begin(),
                   [](const auto* ref) { return const_cast<Reference<Type>*>(ref); });
    return ret;
}

std::vector<const Reference<Type>*> Type::getStrongReferences() const {
    std::vector<const Reference<Type>*> ret;
    for (const auto* ref : getReferences()) {
        if (!ref->shallowGet()->isNeverStrongReference()) {
            ret.push_back(ref);
        }
    }
    return ret;
}

status_t Type::recursivePass(ParseStage stage, const std::function<status_t(Type*)>& func,
                             std::unordered_set<const Type*>* visited) {
    if (mParseStage > stage) return OK;
    if (mParseStage < stage) return UNKNOWN_ERROR;

    if (visited->find(this) != visited->end()) return OK;
    visited->insert(this);

    status_t err = func(this);
    if (err != OK) return err;

    for (auto* nextType : getDefinedTypes()) {
        err = nextType->recursivePass(stage, func, visited);
        if (err != OK) return err;
    }

    for (auto* nextRef : getReferences()) {
        err = nextRef->shallowGet()->recursivePass(stage, func, visited);
        if (err != OK) return err;
    }

    return OK;
}

status_t Type::recursivePass(ParseStage stage, const std::function<status_t(const Type*)>& func,
                             std::unordered_set<const Type*>* visited) const {
    if (mParseStage > stage) return OK;
    if (mParseStage < stage) return UNKNOWN_ERROR;

    if (visited->find(this) != visited->end()) return OK;
    visited->insert(this);

    status_t err = func(this);
    if (err != OK) return err;

    for (const auto* nextType : getDefinedTypes()) {
        err = nextType->recursivePass(stage, func, visited);
        if (err != OK) return err;
    }

    for (const auto* nextRef : getReferences()) {
        err = nextRef->shallowGet()->recursivePass(stage, func, visited);
        if (err != OK) return err;
    }

    return OK;
}

status_t Type::resolveInheritance() {
    return OK;
}

status_t Type::validate() const {
    return OK;
}

Type::CheckAcyclicStatus::CheckAcyclicStatus(status_t status, const Type* cycleEnd)
    : status(status), cycleEnd(cycleEnd) {
    CHECK(cycleEnd == nullptr || status != OK);
}

Type::CheckAcyclicStatus Type::topologicalOrder(
    std::unordered_map<const Type*, size_t>* reversedOrder,
    std::unordered_set<const Type*>* stack) const {
    if (stack->find(this) != stack->end()) {
        std::cerr << "ERROR: Cyclic declaration:\n";
        return CheckAcyclicStatus(UNKNOWN_ERROR, this);
    }

    if (reversedOrder->find(this) != reversedOrder->end()) return CheckAcyclicStatus(OK);
    stack->insert(this);

    for (const auto* nextType : getDefinedTypes()) {
        auto err = nextType->topologicalOrder(reversedOrder, stack);

        if (err.status != OK) {
            if (err.cycleEnd == nullptr) return err;

            std::cerr << "  '" << nextType->typeName() << "' in '" << typeName() << "'";
            if (nextType->isNamedType()) {
                std::cerr << " at " << static_cast<const NamedType*>(nextType)->location();
            }
            std::cerr << "\n";

            if (err.cycleEnd == this) {
                return CheckAcyclicStatus(err.status);
            }
            return err;
        }
    }

    for (const auto* nextRef : getStrongReferences()) {
        const auto* nextType = nextRef->shallowGet();
        auto err = nextType->topologicalOrder(reversedOrder, stack);

        if (err.status != OK) {
            if (err.cycleEnd == nullptr) return err;

            std::cerr << "  '" << nextType->typeName() << "' in '" << typeName() << "' at "
                      << nextRef->location() << "\n";

            if (err.cycleEnd == this) {
                return CheckAcyclicStatus(err.status);
            }
            return err;
        }
    }

    CHECK(stack->find(this) != stack->end());
    stack->erase(this);

    CHECK(reversedOrder->find(this) == reversedOrder->end());
    // Do not call insert and size in one statement to not rely on
    // evaluation order.
    size_t index = reversedOrder->size();
    reversedOrder->insert({this, index});

    return CheckAcyclicStatus(OK);
}

status_t Type::checkForwardReferenceRestrictions(const Reference<Type>& ref) const {
    const Location& refLoc = ref.location();
    const Type* refType = ref.shallowGet();

    // Not NamedTypes are avaiable everywhere.
    // Only ArrayType and TemplatedType contain additional types in
    // their reference (which is actually a part of type definition),
    // so they are proceeded in this case.
    //
    // If we support named templated types one day, we will need to change
    // this logic.
    if (!refType->isNamedType()) {
        for (const Reference<Type>* innerRef : refType->getReferences()) {
            status_t err = checkForwardReferenceRestrictions(*innerRef);
            if (err != OK) return err;
        }
        return OK;
    }

    const Location& typeLoc = static_cast<const NamedType*>(refType)->location();

    // If referenced type is declared in another file or before reference,
    // there is no forward reference here.
    if (!Location::inSameFile(refLoc, typeLoc) ||
        (!Location::intersect(refLoc, typeLoc) && typeLoc < refLoc)) {
        return OK;
    }

    // Type must be declared somewhere in the current stack to make it
    // available for forward referencing.
    const Type* refTypeParent = refType->parent();
    for (const Type* ancestor = this; ancestor != nullptr; ancestor = ancestor->parent()) {
        if (ancestor == refTypeParent) return OK;
    }

    std::cerr << "ERROR: Forward reference of '" << refType->typeName() << "' at " << ref.location()
              << " is not supported.\n"
              << "C++ forward declaration doesn't support inner types.\n";

    return UNKNOWN_ERROR;
}

const ScalarType *Type::resolveToScalarType() const {
    return nullptr;
}

bool Type::isValidEnumStorageType() const {
    const ScalarType *scalarType = resolveToScalarType();

    if (scalarType == nullptr) {
        return false;
    }

    return scalarType->isValidEnumStorageType();
}

bool Type::isElidableType() const {
    return false;
}

bool Type::canCheckEquality() const {
    std::unordered_set<const Type*> visited;
    return canCheckEquality(&visited);
}

bool Type::canCheckEquality(std::unordered_set<const Type*>* visited) const {
    // See isJavaCompatible for similar structure.
    if (visited->find(this) != visited->end()) {
        return true;
    }
    visited->insert(this);
    return deepCanCheckEquality(visited);
}

bool Type::deepCanCheckEquality(std::unordered_set<const Type*>* /* visited */) const {
    return false;
}

Type::ParseStage Type::getParseStage() const {
    return mParseStage;
}

void Type::setParseStage(ParseStage stage) {
    CHECK(mParseStage < stage);
    mParseStage = stage;
}

Scope* Type::parent() {
    return mParent;
}

const Scope* Type::parent() const {
    return mParent;
}

std::string Type::getCppType(StorageMode, bool) const {
    CHECK(!"Should not be here") << typeName();
    return std::string();
}

std::string Type::decorateCppName(
        const std::string &name, StorageMode mode, bool specifyNamespaces) const {
    return getCppType(mode, specifyNamespaces) + " " + name;
}

std::string Type::getJavaType(bool /* forInitializer */) const {
    CHECK(!"Should not be here") << typeName();
    return std::string();
}

std::string Type::getJavaTypeClass() const {
    return getJavaType();
}

std::string Type::getJavaTypeCast(const std::string& objName) const {
    return "(" + getJavaType() + ") " + objName;
}

std::string Type::getJavaSuffix() const {
    CHECK(!"Should not be here") << typeName();
    return std::string();
}

std::string Type::getVtsType() const {
    CHECK(!"Should not be here") << typeName();
    return std::string();
}

std::string Type::getVtsValueName() const {
    CHECK(!"Should not be here") << typeName();
    return std::string();
}

void Type::emitReaderWriter(
        Formatter &,
        const std::string &,
        const std::string &,
        bool,
        bool,
        ErrorMode) const {
    CHECK(!"Should not be here") << typeName();
}

void Type::emitResolveReferences(
        Formatter &,
        const std::string &,
        bool,
        const std::string &,
        bool,
        bool,
        ErrorMode) const {
    CHECK(!"Should not be here") << typeName();
}

void Type::emitResolveReferencesEmbedded(
        Formatter &,
        size_t,
        const std::string &,
        const std::string &,
        bool,
        const std::string &,
        bool,
        bool,
        ErrorMode,
        const std::string &,
        const std::string &) const {
    CHECK(!"Should not be here") << typeName();
}

void Type::emitDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    emitDumpWithMethod(out, streamName, "::android::hardware::toString", name);
}

void Type::emitDumpWithMethod(
        Formatter &out,
        const std::string &streamName,
        const std::string &methodName,
        const std::string &name) const {
    out << streamName
        << " += "
        << methodName
        << "("
        << name
        << ");\n";
}

void Type::emitJavaDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << ".append(" << name << ");\n";
}

bool Type::useParentInEmitResolveReferencesEmbedded() const {
    return needsResolveReferences();
}

void Type::emitReaderWriterEmbedded(
        Formatter &,
        size_t,
        const std::string &,
        const std::string &,
        bool,
        const std::string &,
        bool,
        bool,
        ErrorMode,
        const std::string &,
        const std::string &) const {
    CHECK(!"Should not be here") << typeName();
}

void Type::emitJavaReaderWriter(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader) const {
    emitJavaReaderWriterWithSuffix(
            out,
            parcelObj,
            argName,
            isReader,
            getJavaSuffix(),
            "" /* extra */);
}

void Type::emitJavaFieldInitializer(
        Formatter &out,
        const std::string &fieldName) const {
    out << getJavaType()
        << " "
        << fieldName
        << ";\n";
}

void Type::emitJavaFieldDefaultInitialValue(Formatter &, const std::string &) const {}

void Type::emitJavaFieldReaderWriter(
        Formatter &,
        size_t,
        const std::string &,
        const std::string &,
        const std::string &,
        const std::string &,
        bool) const {
    CHECK(!"Should not be here") << typeName();
}

void Type::handleError(Formatter &out, ErrorMode mode) const {
    switch (mode) {
        case ErrorMode_Ignore:
        {
            out << "/* _hidl_err ignored! */\n\n";
            break;
        }

        case ErrorMode_Goto:
        {
            out << "if (_hidl_err != ::android::OK) { goto _hidl_error; }\n\n";
            break;
        }

        case ErrorMode_Break:
        {
            out << "if (_hidl_err != ::android::OK) { break; }\n\n";
            break;
        }

        case ErrorMode_Return:
        {
            out << "if (_hidl_err != ::android::OK) { return _hidl_err; }\n\n";
            break;
        }
    }
}

void Type::emitReaderWriterEmbeddedForTypeName(
        Formatter &out,
        const std::string &name,
        bool nameIsPointer,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode,
        const std::string &parentName,
        const std::string &offsetText,
        const std::string &typeName,
        const std::string &childName,
        const std::string &funcNamespace) const {

        const std::string parcelObjDeref =
        parcelObjIsPointer ? ("*" + parcelObj) : parcelObj;

    const std::string parcelObjPointer =
        parcelObjIsPointer ? parcelObj : ("&" + parcelObj);

    const std::string nameDerefed = nameIsPointer ? ("*" + name) : name;
    const std::string namePointer = nameIsPointer ? name : ("&" + name);

    out << "_hidl_err = ";

    if (!funcNamespace.empty()) {
        out << funcNamespace << "::";
    }

    out << (isReader ? "readEmbeddedFromParcel(\n" : "writeEmbeddedToParcel(\n");

    out.indent();
    out.indent();

    if (isReader) {
        out << "const_cast<"
            << typeName
            << " &>("
            << nameDerefed
            << "),\n";
    } else {
        out << nameDerefed
            << ",\n";
    }

    out << (isReader ? parcelObjDeref : parcelObjPointer)
        << ",\n"
        << parentName
        << ",\n"
        << offsetText;

    if (!childName.empty()) {
        out << ", &"
            << childName;
    }

    out << ");\n\n";

    out.unindent();
    out.unindent();

    handleError(out, mode);
}

void Type::emitTypeDeclarations(Formatter&) const {}

void Type::emitTypeForwardDeclaration(Formatter&) const {}

void Type::emitGlobalTypeDeclarations(Formatter&) const {}

void Type::emitPackageTypeDeclarations(Formatter&) const {}

void Type::emitPackageTypeHeaderDefinitions(Formatter&) const {}

void Type::emitPackageHwDeclarations(Formatter&) const {}

void Type::emitTypeDefinitions(Formatter&, const std::string&) const {}

void Type::emitJavaTypeDeclarations(Formatter&, bool) const {}

bool Type::needsEmbeddedReadWrite() const {
    return false;
}

bool Type::resultNeedsDeref() const {
    return false;
}

bool Type::needsResolveReferences() const {
    std::unordered_set<const Type*> visited;
    return needsResolveReferences(&visited);
}

bool Type::needsResolveReferences(std::unordered_set<const Type*>* visited) const {
    // See isJavaCompatible for similar structure.
    if (visited->find(this) != visited->end()) {
        return false;
    }
    visited->insert(this);
    return deepNeedsResolveReferences(visited);
}

bool Type::deepNeedsResolveReferences(std::unordered_set<const Type*>* /* visited */) const {
    return false;
}

std::string Type::getCppStackType(bool specifyNamespaces) const {
    return getCppType(StorageMode_Stack, specifyNamespaces);
}

std::string Type::getCppResultType(bool specifyNamespaces) const {
    return getCppType(StorageMode_Result, specifyNamespaces);
}

std::string Type::getCppArgumentType(bool specifyNamespaces) const {
    return getCppType(StorageMode_Argument, specifyNamespaces);
}

std::string Type::getCppTypeCast(const std::string& objName, bool specifyNamespaces) const {
    return "(" + getCppStackType(specifyNamespaces) + ") " + objName;
}

void Type::emitJavaReaderWriterWithSuffix(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader,
        const std::string &suffix,
        const std::string &extra) const {
    out << parcelObj
        << "."
        << (isReader ? "read" : "write")
        << suffix
        << "(";

    if (isReader) {
        out << extra;
    } else {
        out << (extra.empty() ? "" : (extra + ", "));
        out << argName;
    }

    out << ");\n";
}

void Type::emitVtsTypeDeclarations(Formatter&) const {}

void Type::emitVtsAttributeType(Formatter& out) const {
    emitVtsTypeDeclarations(out);
}

bool Type::isJavaCompatible() const {
    std::unordered_set<const Type*> visited;
    return isJavaCompatible(&visited);
}

bool Type::containsPointer() const {
    std::unordered_set<const Type*> visited;
    return containsPointer(&visited);
}

bool Type::isJavaCompatible(std::unordered_set<const Type*>* visited) const {
    // We need to find al least one path from requested vertex
    // to not java compatible.
    // That means that if we have already visited some vertex,
    // there is no need to determine whether it is java compatible
    // (and we can assume that it is java compatible),
    // as if not, the information about that would appear in the
    // requested vertex through another path.
    if (visited->find(this) != visited->end()) {
        return true;
    }
    visited->insert(this);
    return deepIsJavaCompatible(visited);
}

bool Type::containsPointer(std::unordered_set<const Type*>* visited) const {
    // See isJavaCompatible for similar structure.
    if (visited->find(this) != visited->end()) {
        return false;
    }
    visited->insert(this);
    return deepContainsPointer(visited);
}

bool Type::deepIsJavaCompatible(std::unordered_set<const Type*>* /* visited */) const {
    return true;
}

bool Type::deepContainsPointer(std::unordered_set<const Type*>* /* visited */) const {
    return false;
}

void Type::getAlignmentAndSize(
        size_t * /* align */, size_t * /* size */) const {
    CHECK(!"Should not be here.");
}

void Type::appendToExportedTypesVector(
        std::vector<const Type *> * /* exportedTypes */) const {
}

void Type::emitExportedHeader(Formatter& /* out */, bool /* forJava */) const {}

bool Type::isNeverStrongReference() const {
    return false;
}

////////////////////////////////////////

TemplatedType::TemplatedType(Scope* parent) : Type(parent) {}

std::string TemplatedType::typeName() const {
    return templatedTypeName() + " of " + mElementType->typeName();
}

void TemplatedType::setElementType(const Reference<Type>& elementType) {
    // can only be set once.
    CHECK(mElementType.isEmptyReference());
    CHECK(!elementType.isEmptyReference());

    mElementType = elementType;
}

const Type* TemplatedType::getElementType() const {
    return mElementType.get();
}

bool TemplatedType::isTemplatedType() const {
    return true;
}

std::vector<const Reference<Type>*> TemplatedType::getReferences() const {
    return {&mElementType};
}

status_t TemplatedType::validate() const {
    if (!isCompatibleElementType(mElementType.get())) {
        std::cerr << "ERROR: " << typeName() /* contains element type */
                  << " is not supported at " << mElementType.location() << "\n";
        return UNKNOWN_ERROR;
    }

    return Type::validate();
}

void TemplatedType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << getVtsValueName() << ": {\n";
    out.indent();
    mElementType->emitVtsTypeDeclarations(out);
    out.unindent();
    out << "}\n";
}

void TemplatedType::emitVtsAttributeType(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << getVtsValueName() << ": {\n";
    out.indent();
    mElementType->emitVtsAttributeType(out);
    out.unindent();
    out << "}\n";
}

}  // namespace android

