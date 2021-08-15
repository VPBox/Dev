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

#include "EnumType.h"

#include <hidl-util/Formatter.h>
#include <inttypes.h>
#include <iostream>
#include <unordered_map>

#include "Annotation.h"
#include "Location.h"
#include "ScalarType.h"

namespace android {

EnumType::EnumType(const char* localName, const FQName& fullName, const Location& location,
                   const Reference<Type>& storageType, Scope* parent)
    : Scope(localName, fullName, location, parent), mValues(), mStorageType(storageType) {}

const Type *EnumType::storageType() const {
    return mStorageType.get();
}

const std::vector<EnumValue *> &EnumType::values() const {
    return mValues;
}

void EnumType::forEachValueFromRoot(const std::function<void(EnumValue*)> f) const {
    std::vector<const EnumType*> chain = typeChain();
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto& type = *it;
        for (EnumValue* v : type->values()) {
            f(v);
        }
    }
}

size_t EnumType::numValueNames() const {
    size_t count = 0;
    for (const auto it : typeChain()) {
        count += it->values().size();
    }
    return count;
}

void EnumType::addValue(EnumValue* value) {
    CHECK(value != nullptr);
    mValues.push_back(value);
}

status_t EnumType::resolveInheritance() {
    const EnumType* prevType = nullptr;
    EnumValue* prevValue = nullptr;

    for (const auto* type : superTypeChain()) {
        if (!type->values().empty()) {
            prevType = type;
            prevValue = type->values().back();
            break;
        }
    }

    for (auto* value : mValues) {
        value->autofill(prevType, prevValue, mStorageType->resolveToScalarType());
        prevType = this;
        prevValue = value;
    }

    return Scope::resolveInheritance();
}

std::vector<const Reference<Type>*> EnumType::getReferences() const {
    return {&mStorageType};
}

std::vector<const ConstantExpression*> EnumType::getConstantExpressions() const {
    std::vector<const ConstantExpression*> ret;
    for (const auto* value : mValues) {
        ret.push_back(value->constExpr());
    }
    return ret;
}

status_t EnumType::validate() const {
    CHECK(getSubTypes().empty());

    if (!isElidableType() || !mStorageType->isValidEnumStorageType()) {
        std::cerr << "ERROR: Invalid enum storage type (" << (mStorageType)->typeName()
                  << ") specified at " << mStorageType.location() << "\n";
        return UNKNOWN_ERROR;
    }

    status_t err = validateUniqueNames();
    if (err != OK) return err;

    return Scope::validate();
}

status_t EnumType::validateUniqueNames() const {
    std::unordered_map<std::string, const EnumType*> registeredValueNames;
    for (const auto* type : superTypeChain()) {
        for (const auto* enumValue : type->mValues) {
            // No need to check super value uniqueness
            registeredValueNames[enumValue->name()] = type;
        }
    }

    for (const auto* value : mValues) {
        auto registered = registeredValueNames.find(value->name());

        if (registered != registeredValueNames.end()) {
            const EnumType* definedInType = registered->second;

            if (definedInType == this) {
                // Defined in this enum
                std::cerr << "ERROR: Redefinition of value '" << value->name() << "'";
            } else {
                // Defined in super enum
                std::cerr << "ERROR: Redefinition of value '" << value->name()
                          << "' defined in enum '" << definedInType->fullName() << "'";
            }
            std::cerr << " at " << value->location() << "\n";
            return UNKNOWN_ERROR;
        }

        registeredValueNames[value->name()] = this;
    }

    return OK;
}

bool EnumType::isElidableType() const {
    return mStorageType->isElidableType();
}

const ScalarType *EnumType::resolveToScalarType() const {
    return mStorageType->resolveToScalarType();
}

std::string EnumType::typeName() const {
    return "enum " + localName();
}

bool EnumType::isEnum() const {
    return true;
}

bool EnumType::deepCanCheckEquality(std::unordered_set<const Type*>* /* visited */) const {
    return true;
}

std::string EnumType::getCppType(StorageMode,
                                 bool /* specifyNamespaces */) const {
    return fullName();
}

std::string EnumType::getJavaType(bool forInitializer) const {
    return mStorageType->resolveToScalarType()->getJavaType(forInitializer);
}

std::string EnumType::getJavaSuffix() const {
    return mStorageType->resolveToScalarType()->getJavaSuffix();
}

std::string EnumType::getJavaTypeClass() const {
    return mStorageType->resolveToScalarType()->getJavaTypeClass();
}

std::string EnumType::getVtsType() const {
    return "TYPE_ENUM";
}

std::string EnumType::getBitfieldCppType(StorageMode /* mode */, bool specifyNamespaces) const {
    const std::string space = specifyNamespaces ? "::android::hardware::" : "";
    return space + "hidl_bitfield<" + (specifyNamespaces ? fullName() : localName()) + ">";
}

std::string EnumType::getBitfieldJavaType(bool forInitializer) const {
    return resolveToScalarType()->getJavaType(forInitializer);
}

std::string EnumType::getBitfieldJavaTypeClass() const {
    return resolveToScalarType()->getJavaTypeClass();
}

LocalIdentifier *EnumType::lookupIdentifier(const std::string &name) const {
    std::vector<const EnumType*> chain = typeChain();
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        const auto &type = *it;
        for(EnumValue *v : type->values()) {
            if(v->name() == name) {
                return v;
            }
        }
    }
    return nullptr;
}

void EnumType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    scalarType->emitReaderWriterWithCast(
            out,
            name,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            true /* needsCast */);
}

void EnumType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t depth,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    return mStorageType->emitJavaFieldReaderWriter(
            out, depth, parcelName, blobName, fieldName, offset, isReader);
}

void EnumType::emitTypeDeclarations(Formatter& out) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    const std::string storageType = scalarType->getCppStackType();

    out << "enum class "
        << localName()
        << " : "
        << storageType
        << " {\n";

    out.indent();

    std::vector<const EnumType*> chain = typeChain();

    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto &type = *it;

        for (const auto &entry : type->values()) {
            entry->emitDocComment(out);

            out << entry->name();

            std::string value = entry->cppValue(scalarType->getKind());
            CHECK(!value.empty()); // use autofilled values for c++.
            out << " = " << value << ",\n";
        }
    }

    out.unindent();
    out << "};\n\n";
}

void EnumType::emitTypeForwardDeclaration(Formatter& out) const {
    const ScalarType* scalarType = mStorageType->resolveToScalarType();
    const std::string storageType = scalarType->getCppStackType();

    out << "enum class " << localName() << " : " << storageType << ";\n";
}

void EnumType::emitIteratorDeclaration(Formatter& out) const {
    size_t elementCount = 0;
    for (const auto* type : typeChain()) {
        elementCount += type->mValues.size();
    }

    out << "template<> constexpr std::array<" << getCppStackType() << ", " << elementCount
        << "> hidl_enum_values<" << getCppStackType() << "> = ";
    out.block([&] {
        auto enumerators = typeChain();
        std::reverse(enumerators.begin(), enumerators.end());
        for (const auto* type : enumerators) {
            for (const auto* enumValue : type->mValues) {
                out << fullName() << "::" << enumValue->name() << ",\n";
            }
        }
    }) << ";\n";
}

void EnumType::emitEnumBitwiseOperator(
        Formatter &out,
        bool lhsIsEnum,
        bool rhsIsEnum,
        const std::string &op) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    const std::string storageType = scalarType->getCppStackType();

    out << "constexpr "
        << storageType
        << " operator"
        << op
        << "(const "
        << (lhsIsEnum ? fullName() : storageType)
        << " lhs, const "
        << (rhsIsEnum ? fullName() : storageType)
        << " rhs) {\n";

    out.indent([&] {
        out << "return static_cast<"
            << storageType
            << ">(";

        if (lhsIsEnum) {
            out << "static_cast<"
                << storageType
                << ">(lhs)";
        } else {
            out << "lhs";
        }
        out << " " << op << " ";
        if (rhsIsEnum) {
            out << "static_cast<"
                << storageType
                << ">(rhs)";
        } else {
            out << "rhs";
        }
        out << ");\n";
    });

    out << "}\n";
}

void EnumType::emitBitFieldBitwiseAssignmentOperator(
        Formatter &out,
        const std::string &op) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    const std::string storageType = scalarType->getCppStackType();

    out << "constexpr " << storageType << " &operator" << op << "=("
        << storageType << "& v, const " << fullName() << " e) {\n";

    out.indent([&] {
        out << "v " << op << "= static_cast<" << storageType << ">(e);\n";
        out << "return v;\n";
    });

    out << "}\n";
}

void EnumType::emitGlobalTypeDeclarations(Formatter& out) const {
    out << "namespace android {\n";
    out << "namespace hardware {\n";
    out << "namespace details {\n";

    emitIteratorDeclaration(out);

    out << "}  // namespace details\n";
    out << "}  // namespace hardware\n";
    out << "}  // namespace android\n\n";
}

void EnumType::emitPackageTypeDeclarations(Formatter& out) const {
    out << "template<typename>\n"
        << "static inline std::string toString(" << resolveToScalarType()->getCppArgumentType()
        << " o);\n";
    out << "static inline std::string toString(" << getCppArgumentType() << " o);\n\n";

    emitEnumBitwiseOperator(out, true  /* lhsIsEnum */, true  /* rhsIsEnum */, "|");
    emitEnumBitwiseOperator(out, false /* lhsIsEnum */, true  /* rhsIsEnum */, "|");
    emitEnumBitwiseOperator(out, true  /* lhsIsEnum */, false /* rhsIsEnum */, "|");
    emitEnumBitwiseOperator(out, true  /* lhsIsEnum */, true  /* rhsIsEnum */, "&");
    emitEnumBitwiseOperator(out, false /* lhsIsEnum */, true  /* rhsIsEnum */, "&");
    emitEnumBitwiseOperator(out, true  /* lhsIsEnum */, false /* rhsIsEnum */, "&");

    emitBitFieldBitwiseAssignmentOperator(out, "|");
    emitBitFieldBitwiseAssignmentOperator(out, "&");

    out.endl();
}

void EnumType::emitPackageTypeHeaderDefinitions(Formatter& out) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    out << "template<>\n"
        << "inline std::string toString<" << getCppStackType() << ">("
        << scalarType->getCppArgumentType() << " o) ";
    out.block([&] {
        // include toHexString for scalar types
        out << "using ::android::hardware::details::toHexString;\n"
            << "std::string os;\n"
            << getBitfieldCppType(StorageMode_Stack) << " flipped = 0;\n"
            << "bool first = true;\n";
        forEachValueFromRoot([&](EnumValue* value) {
            std::string valueName = fullName() + "::" + value->name();
            out.sIf("(o & " + valueName + ")" +
                    " == static_cast<" + scalarType->getCppStackType() +
                    ">(" + valueName + ")", [&] {
                out << "os += (first ? \"\" : \" | \");\n"
                    << "os += \"" << value->name() << "\";\n"
                    << "first = false;\n"
                    << "flipped |= " << valueName << ";\n";
            }).endl();
        });
        // put remaining bits
        out.sIf("o != flipped", [&] {
            out << "os += (first ? \"\" : \" | \");\n";
            scalarType->emitHexDump(out, "os", "o & (~flipped)");
        });
        out << "os += \" (\";\n";
        scalarType->emitHexDump(out, "os", "o");
        out << "os += \")\";\n";

        out << "return os;\n";
    }).endl().endl();

    out << "static inline std::string toString(" << getCppArgumentType() << " o) ";

    out.block([&] {
        out << "using ::android::hardware::details::toHexString;\n";
        forEachValueFromRoot([&](EnumValue* value) {
            out.sIf("o == " + fullName() + "::" + value->name(), [&] {
                out << "return \"" << value->name() << "\";\n";
            }).endl();
        });
        out << "std::string os;\n";
        scalarType->emitHexDump(out, "os",
            "static_cast<" + scalarType->getCppStackType() + ">(o)");
        out << "return os;\n";
    }).endl().endl();
}

void EnumType::emitJavaTypeDeclarations(Formatter& out, bool atTopLevel) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    out << "public "
        << (atTopLevel ? "" : "static ")
        << "final class "
        << localName()
        << " {\n";

    out.indent();

    const std::string typeName =
        scalarType->getJavaType(false /* forInitializer */);

    std::vector<const EnumType*> chain = typeChain();

    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto &type = *it;

        for (const auto &entry : type->values()) {
            entry->emitDocComment(out);

            out << "public static final "
                << typeName
                << " "
                << entry->name()
                << " = ";

            // javaValue will make the number signed.
            std::string value = entry->javaValue(scalarType->getKind());
            CHECK(!value.empty()); // use autofilled values for java.
            out << value << ";\n";
        }
    }

    out << "public static final String toString("
        << typeName << " o) ";
    out.block([&] {
        forEachValueFromRoot([&](EnumValue* value) {
            out.sIf("o == " + value->name(), [&] {
                out << "return \"" << value->name() << "\";\n";
            }).endl();
        });
        out << "return \"0x\" + ";
        scalarType->emitConvertToJavaHexString(out, "o");
        out << ";\n";
    }).endl();

    auto bitfieldType = getBitfieldJavaType(false /* forInitializer */);
    out << "\n"
        << "public static final String dumpBitfield("
        << bitfieldType << " o) ";
    out.block([&] {
        out << "java.util.ArrayList<String> list = new java.util.ArrayList<>();\n";
        out << bitfieldType << " flipped = 0;\n";
        forEachValueFromRoot([&](EnumValue* value) {
            if (value->constExpr()->castSizeT() == 0) {
                out << "list.add(\"" << value->name() << "\"); // " << value->name() << " == 0\n";
                return;  // continue to next value
            }
            out.sIf("(o & " + value->name() + ") == " + value->name(), [&] {
                out << "list.add(\"" << value->name() << "\");\n";
                out << "flipped |= " << value->name() << ";\n";
            }).endl();
        });
        // put remaining bits
        out.sIf("o != flipped", [&] {
            out << "list.add(\"0x\" + ";
            scalarType->emitConvertToJavaHexString(out, "o & (~flipped)");
            out << ");\n";
        }).endl();
        out << "return String.join(\" | \", list);\n";
    }).endl().endl();

    out.unindent();
    out << "};\n\n";
}

void EnumType::emitVtsTypeDeclarations(Formatter& out) const {
    const ScalarType *scalarType = mStorageType->resolveToScalarType();

    out << "name: \"" << fullName() << "\"\n";
    out << "type: " << getVtsType() << "\n";
    out << "enum_value: {\n";
    out.indent();

    out << "scalar_type: \""
        << scalarType->getVtsScalarType()
        << "\"\n\n";
    std::vector<const EnumType*> chain = typeChain();

    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto &type = *it;

        for (const auto &entry : type->values()) {
            out << "enumerator: \"" << entry->name() << "\"\n";
            out << "scalar_value: {\n";
            out.indent();
            // use autofilled values for vts.
            std::string value = entry->rawValue(scalarType->getKind());
            CHECK(!value.empty());
            out << mStorageType->resolveToScalarType()->getVtsScalarType()
                << ": "
                << value
                << "\n";
            out.unindent();
            out << "}\n";
        }
    }

    out.unindent();
    out << "}\n";
}

void EnumType::emitVtsAttributeType(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << "predefined_type: \"" << fullName() << "\"\n";
}

void EnumType::emitJavaDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << ".append(" << fqName().javaName() << ".toString("
        << name << "));\n";
}

std::vector<const EnumType*> EnumType::typeChain() const {
    std::vector<const EnumType*> types;
    for (const EnumType* type = this; type != nullptr;) {
        types.push_back(type);

        const Type* superType = type->storageType();
        if (superType != nullptr && superType->isEnum()) {
            type = static_cast<const EnumType*>(superType);
        } else {
            type = nullptr;
        }
    }

    return types;
}

std::vector<const EnumType*> EnumType::superTypeChain() const {
    const Type* superType = storageType();
    if (superType == nullptr || !superType->isEnum()) {
        return {};
    }
    return static_cast<const EnumType*>(superType)->typeChain();
}

void EnumType::getAlignmentAndSize(size_t *align, size_t *size) const {
    mStorageType->getAlignmentAndSize(align, size);
}

const Annotation *EnumType::findExportAnnotation() const {
    for (const auto &annotation : annotations()) {
        if (annotation->name() == "export") {
            return annotation;
        }
    }

    return nullptr;
}

void EnumType::appendToExportedTypesVector(
        std::vector<const Type *> *exportedTypes) const {
    if (findExportAnnotation() != nullptr) {
        exportedTypes->push_back(this);
    }
}

void EnumType::emitExportedHeader(Formatter& out, bool forJava) const {
    const Annotation *annotation = findExportAnnotation();
    CHECK(annotation != nullptr);

    std::string name = localName();

    const AnnotationParam *nameParam = annotation->getParam("name");
    if (nameParam != nullptr) {
        name = nameParam->getSingleString();
    }

    bool exportParent = true;
    const AnnotationParam *exportParentParam = annotation->getParam("export_parent");
    if (exportParentParam != nullptr) {
        exportParent = exportParentParam->getSingleBool();
    }

    std::string valuePrefix;
    const AnnotationParam *prefixParam = annotation->getParam("value_prefix");
    if (prefixParam != nullptr) {
        valuePrefix = prefixParam->getSingleString();
    }

    std::string valueSuffix;
    const AnnotationParam *suffixParam = annotation->getParam("value_suffix");
    if (suffixParam != nullptr) {
        valueSuffix = suffixParam->getSingleString();
    }

    const ScalarType *scalarType = mStorageType->resolveToScalarType();
    CHECK(scalarType != nullptr);

    std::vector<const EnumType *> chain;
    if (exportParent) {
        chain = typeChain();
    } else {
        chain = { this };
    }

    if (forJava) {
        if (!name.empty()) {
            out << "public final class "
                << name
                << " {\n";

            out.indent();
        } else {
            out << "// Values declared in " << localName() << " follow.\n";
        }

        const std::string typeName =
            scalarType->getJavaType(false /* forInitializer */);

        for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
            const auto &type = *it;

            for (const auto &entry : type->values()) {
                out << "public static final "
                    << typeName
                    << " "
                    << valuePrefix
                    << entry->name()
                    << valueSuffix
                    << " = ";

                // javaValue will make the number signed.
                std::string value = entry->javaValue(scalarType->getKind());
                CHECK(!value.empty()); // use autofilled values for java.
                out << value << ";\n";
            }
        }

        if (!name.empty()) {
            out.unindent();
            out << "};\n";
        }
        out << "\n";

        return;
    }

    if (!name.empty()) {
        out << "typedef ";
    }

    out << "enum {\n";

    out.indent();

    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const auto &type = *it;

        for (const auto &entry : type->values()) {
            out << valuePrefix << entry->name() << valueSuffix;

            std::string value = entry->cppValue(scalarType->getKind());
            CHECK(!value.empty()); // use autofilled values for c++.
            out << " = " << value << ",\n";
        }
    }

    out.unindent();
    out << "}";

    if (!name.empty()) {
        out << " " << name;
    }

    out << ";\n\n";
}

////////////////////////////////////////////////////////////////////////////////

EnumValue::EnumValue(const char* name, ConstantExpression* value, const Location& location)
    : mName(name), mValue(value), mLocation(location), mIsAutoFill(false) {}

std::string EnumValue::name() const {
    return mName;
}

std::string EnumValue::rawValue(ScalarType::Kind castKind) const {
    CHECK(mValue != nullptr);
    return mValue->rawValue(castKind);
}

std::string EnumValue::cppValue(ScalarType::Kind castKind) const {
    CHECK(mValue != nullptr);
    return mValue->cppValue(castKind);
}
std::string EnumValue::javaValue(ScalarType::Kind castKind) const {
    CHECK(mValue != nullptr);
    return mValue->javaValue(castKind);
}

ConstantExpression *EnumValue::constExpr() const {
    CHECK(mValue != nullptr);
    return mValue;
}

void EnumValue::autofill(const EnumType* prevType, EnumValue* prevValue, const ScalarType* type) {
    // Value is defined explicitly
    if (mValue != nullptr) return;

    CHECK((prevType == nullptr) == (prevValue == nullptr));

    mIsAutoFill = true;
    if (prevValue == nullptr) {
        mValue = ConstantExpression::Zero(type->getKind()).release();
    } else {
        std::string description = prevType->fullName() + "." + prevValue->name() + " implicitly";
        auto* prevReference = new ReferenceConstantExpression(
            Reference<LocalIdentifier>(prevValue, mLocation), description);
        mValue = prevReference->addOne(type->getKind()).release();
    }
}

bool EnumValue::isAutoFill() const {
    return mIsAutoFill;
}

bool EnumValue::isEnumValue() const {
    return true;
}

const Location& EnumValue::location() const {
    return mLocation;
}

////////////////////////////////////////////////////////////////////////////////

BitFieldType::BitFieldType(Scope* parent) : TemplatedType(parent) {}

bool BitFieldType::isBitField() const {
    return true;
}

const EnumType* BitFieldType::getElementEnumType() const {
    CHECK(mElementType.get() != nullptr && mElementType->isEnum());
    return static_cast<const EnumType*>(mElementType.get());
}

std::string BitFieldType::templatedTypeName() const {
    return "mask";
}

bool BitFieldType::isCompatibleElementType(const Type* elementType) const {
    return elementType->isEnum();
}

const ScalarType *BitFieldType::resolveToScalarType() const {
    return mElementType->resolveToScalarType();
}

std::string BitFieldType::getCppType(StorageMode mode,
                                 bool specifyNamespaces) const {
    return getElementEnumType()->getBitfieldCppType(mode, specifyNamespaces);
}

std::string BitFieldType::getJavaType(bool forInitializer) const {
    return getElementEnumType()->getBitfieldJavaType(forInitializer);
}

std::string BitFieldType::getJavaSuffix() const {
    return resolveToScalarType()->getJavaSuffix();
}

std::string BitFieldType::getJavaTypeClass() const {
    return getElementEnumType()->getBitfieldJavaTypeClass();
}

std::string BitFieldType::getVtsType() const {
    return "TYPE_MASK";
}

bool BitFieldType::isElidableType() const {
    return resolveToScalarType()->isElidableType();
}

bool BitFieldType::deepCanCheckEquality(std::unordered_set<const Type*>* visited) const {
    return resolveToScalarType()->canCheckEquality(visited);
}

void BitFieldType::emitVtsAttributeType(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << "scalar_type: \""
        << mElementType->resolveToScalarType()->getVtsScalarType()
        << "\"\n";
    out << "predefined_type: \"" << static_cast<const NamedType*>(mElementType.get())->fullName()
        << "\"\n";
}

void BitFieldType::getAlignmentAndSize(size_t *align, size_t *size) const {
    resolveToScalarType()->getAlignmentAndSize(align, size);
}

void BitFieldType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    resolveToScalarType()->emitReaderWriterWithCast(
            out,
            name,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            true /* needsCast */);
}

const EnumType* BitFieldType::getEnumType() const {
    CHECK(mElementType->isEnum());
    return static_cast<const EnumType*>(mElementType.get());
}

// a bitfield maps to the underlying scalar type in C++, so operator<< is
// already defined. We can still emit useful information if the bitfield is
// in a struct / union by overriding emitDump as below.
void BitFieldType::emitDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << " += "<< getEnumType()->fqName().cppNamespace()
        << "::toString<" << getEnumType()->getCppStackType()
        << ">(" << name << ");\n";
}

void BitFieldType::emitJavaDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << ".append(" << getEnumType()->fqName().javaName() << ".dumpBitfield("
        << name << "));\n";
}

void BitFieldType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t depth,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    return resolveToScalarType()->emitJavaFieldReaderWriter(
            out, depth, parcelName, blobName, fieldName, offset, isReader);
}

}  // namespace android

