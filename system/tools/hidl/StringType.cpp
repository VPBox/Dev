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

#include "StringType.h"

#include "HidlTypeAssertion.h"

#include <hidl-util/Formatter.h>

namespace android {

StringType::StringType(Scope* parent) : Type(parent) {}

bool StringType::isString() const {
    return true;
}

bool StringType::deepCanCheckEquality(std::unordered_set<const Type*>* /* visited */) const {
    return true;
}

std::string StringType::typeName() const {
    return "string";
}

std::string StringType::getCppType(StorageMode mode,
                                   bool specifyNamespaces) const {
    const std::string base =
          std::string(specifyNamespaces ? "::android::hardware::" : "")
        + "hidl_string";

    switch (mode) {
        case StorageMode_Stack:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";

        case StorageMode_Result:
            return "const " + base + "*";
    }
}

std::string StringType::getJavaType(bool /* forInitializer */) const {
    return "String";
}

std::string StringType::getJavaSuffix() const {
    return "String";
}

std::string StringType::getVtsType() const {
    return "TYPE_STRING";
}

void StringType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    const std::string parentName = "_hidl_" + name + "_parent";
    out << "size_t " << parentName << ";\n\n";

    const std::string parcelObjDeref =
        parcelObj + (parcelObjIsPointer ? "->" : ".");

    if (isReader) {
        out << "_hidl_err = "
            << parcelObjDeref
            << "readBuffer("
            << "sizeof(*"
            << name
            << "), &"
            << parentName
            << ", "
            << " reinterpret_cast<const void **>("
            << "&" << name
            << "));\n\n";

        handleError(out, mode);
    } else {
        out << "_hidl_err = "
            << parcelObjDeref
            << "writeBuffer(&"
            << name
            << ", sizeof("
            << name
            << "), &"
            << parentName
            << ");\n";

        handleError(out, mode);
    }

    emitReaderWriterEmbedded(
            out,
            0 /* depth */,
            name,
            name /* sanitizedName */,
            isReader /* nameIsPointer */,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            parentName,
            "0 /* parentOffset */");
}

void StringType::emitReaderWriterEmbedded(
        Formatter &out,
        size_t /* depth */,
        const std::string &name,
        const std::string & /*sanitizedName*/,
        bool nameIsPointer,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode,
        const std::string &parentName,
        const std::string &offsetText) const {
    emitReaderWriterEmbeddedForTypeName(
            out,
            name,
            nameIsPointer,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            parentName,
            offsetText,
            "::android::hardware::hidl_string",
            "" /* childName */,
            "::android::hardware");
}

void StringType::emitJavaFieldInitializer(
        Formatter &out, const std::string &fieldName) const {
    emitJavaFieldDefaultInitialValue(out, "String " + fieldName);
}

void StringType::emitJavaFieldDefaultInitialValue(
        Formatter &out, const std::string &declaredFieldName) const {
    out << declaredFieldName << " = new String();\n";
}

void StringType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t /* depth */,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    if (isReader) {
        out << fieldName
            << " = "
            << blobName
            << ".getString("
            << offset
            << ");\n";

        out << "\n"
            << parcelName
            << ".readEmbeddedBuffer(\n";

        out.indent();
        out.indent();

        // hidl_string's embedded buffer is never null(able), because it defaults to a
        // buffer containing an empty string.
        out << "(" << getJavaTypeCast(fieldName) << ").getBytes().length + 1,\n"
            << blobName
            << ".handle(),\n"
            << offset
            << " + 0 /* offsetof(hidl_string, mBuffer) */,"
            << "false /* nullable */);\n\n";

        out.unindent();
        out.unindent();

        return;
    }

    out << blobName
        << ".putString("
        << offset
        << ", "
        << fieldName
        << ");\n";
}

bool StringType::needsEmbeddedReadWrite() const {
    return true;
}

bool StringType::resultNeedsDeref() const {
    return true;
}

void StringType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
}

static HidlTypeAssertion assertion("hidl_string", 16 /* size */);
void StringType::getAlignmentAndSize(size_t *align, size_t *size) const {
    *align = 8;  // hidl_string
    *size = assertion.size();
}

}  // namespace android

