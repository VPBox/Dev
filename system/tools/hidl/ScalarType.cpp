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

#include "ScalarType.h"

#include <hidl-util/Formatter.h>

namespace android {

ScalarType::ScalarType(Kind kind, Scope* parent) : Type(parent), mKind(kind) {}

const ScalarType *ScalarType::resolveToScalarType() const {
    return this;
}

bool ScalarType::isValidEnumStorageType() const {
    // Only integer types.
    return mKind >= KIND_INT8 && mKind <= KIND_UINT64;
}

bool ScalarType::isScalar() const {
    return true;
}

bool ScalarType::isElidableType() const {
    return true;
}

bool ScalarType::deepCanCheckEquality(std::unordered_set<const Type*>* /* visited */) const {
    return true;
}

std::string ScalarType::typeName() const {
    return getCppStackType();
}

std::string ScalarType::getCppType(StorageMode, bool) const {
    static const char *const kName[] = {
        "bool",
        "int8_t",
        "uint8_t",
        "int16_t",
        "uint16_t",
        "int32_t",
        "uint32_t",
        "int64_t",
        "uint64_t",
        "float",
        "double"
    };

    return kName[mKind];
}

std::string ScalarType::getJavaType(bool /* forInitializer */) const {
    static const char *const kName[] = {
        "boolean",
        "byte",
        "byte",
        "short",
        "short",
        "int",
        "int",
        "long",
        "long",
        "float",
        "double"
    };

    return kName[mKind];
}

std::string ScalarType::getJavaTypeClass() const {
    static const char *const kName[] = {
        "Boolean",
        "Byte",
        "Byte",
        "Short",
        "Short",
        "Integer",
        "Integer",
        "Long",
        "Long",
        "Float",
        "Double"
    };

    return kName[mKind];
}

std::string ScalarType::getJavaSuffix() const {
    static const char *const kSuffix[] = {
        "Bool",
        "Int8",
        "Int8",
        "Int16",
        "Int16",
        "Int32",
        "Int32",
        "Int64",
        "Int64",
        "Float",
        "Double"
    };

    return kSuffix[mKind];
}

std::string ScalarType::getVtsType() const {
    return "TYPE_SCALAR";
}

std::string ScalarType::getVtsScalarType() const {
    static const char * const kName[] = {
            "bool_t",
            "int8_t",
            "uint8_t",
            "int16_t",
            "uint16_t",
            "int32_t",
            "uint32_t",
            "int64_t",
            "uint64_t",
            "float_t",
            "double_t"
    };

    return kName[mKind];
}

void ScalarType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    emitReaderWriterWithCast(
            out,
            name,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            false /* needsCast */);
}

void ScalarType::emitReaderWriterWithCast(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode,
        bool needsCast) const {
    static const char *const kSuffix[] = {
        "Bool",
        "Int8",
        "Uint8",
        "Int16",
        "Uint16",
        "Int32",
        "Uint32",
        "Int64",
        "Uint64",
        "Float",
        "Double"
    };

    const std::string parcelObjDeref =
        parcelObj + (parcelObjIsPointer ? "->" : ".");

    out << "_hidl_err = "
        << parcelObjDeref
        << (isReader ? "read" : "write")
        << kSuffix[mKind]
        << "(";

    if (needsCast) {
        out << "("
            << getCppStackType()
            << (isReader ? " *)" : ")");
    }

    if (isReader) {
        out << "&";
    }

    out << name
        << ");\n";

    handleError(out, mode);
}

void ScalarType::emitHexDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << " += toHexString(" << name << ");\n";
}

void ScalarType::emitConvertToJavaHexString(
        Formatter &out,
        const std::string &name) const {
    switch(mKind) {
        case KIND_BOOL: {
            out << "((" << name << ") ? \"0x1\" : \"0x0\")";
            break;
        }
        case KIND_INT8:     // fallthrough
        case KIND_UINT8:    // fallthrough
        case KIND_INT16:    // fallthrough
        case KIND_UINT16: {
            // Because Byte and Short doesn't have toHexString, we have to use Integer.toHexString.
            out << "Integer.toHexString(" << getJavaTypeClass() << ".toUnsignedInt(("
                << getJavaType(false /* forInitializer */) << ")(" << name << ")))";
            break;
        }
        case KIND_INT32:    // fallthrough
        case KIND_UINT32:   // fallthrough
        case KIND_INT64:    // fallthrough
        case KIND_UINT64: {
            out << getJavaTypeClass() << ".toHexString(" << name << ")";
            break;
        }
        case KIND_FLOAT:    // fallthrough
        case KIND_DOUBLE:   // fallthrough
        default: {
            // no hex for floating point numbers.
            out << name;
            break;
        }
    }
}

void ScalarType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t /* depth */,
        const std::string & /* parcelName */,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    if (isReader) {
        out << fieldName
            << " = "
            << blobName
            << ".get"
            << getJavaSuffix()
            << "("
            << offset
            << ");\n";

        return;
    }

    out << blobName
        << ".put"
        << getJavaSuffix()
        << "("
        << offset
        << ", "
        << fieldName
        << ");\n";
}

void ScalarType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << "scalar_type: \"" << getVtsScalarType() << "\"\n";
}

void ScalarType::getAlignmentAndSize(size_t *align, size_t *size) const {
    static const size_t kAlign[] = {
        1,  // bool, this is NOT standardized!
        1,  // int8_t
        1,  // uint8_t
        2,  // int16_t
        2,  // uint16_t
        4,  // int32_t
        4,  // uint32_t
        8,  // int64_t
        8,  // uint64_t
        4,  // float
        8   // double
    };

    *align = *size = kAlign[mKind];
}

ScalarType::Kind ScalarType::getKind() const {
    return mKind;
}

}  // namespace android

