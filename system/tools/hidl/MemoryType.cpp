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

#include "MemoryType.h"

#include "HidlTypeAssertion.h"

#include <hidl-util/Formatter.h>
#include <android-base/logging.h>

namespace android {

MemoryType::MemoryType(Scope* parent) : Type(parent) {}

std::string MemoryType::getCppType(StorageMode mode,
                                   bool specifyNamespaces) const {
    const std::string base =
          std::string(specifyNamespaces ? "::android::hardware::" : "")
        + "hidl_memory";

    switch (mode) {
        case StorageMode_Stack:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";

        case StorageMode_Result:
            return "const " + base + "*";
    }
}

std::string MemoryType::typeName() const {
    return "memory";
}

std::string MemoryType::getVtsType() const {
    return "TYPE_HIDL_MEMORY";
}

void MemoryType::emitReaderWriter(
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

void MemoryType::emitReaderWriterEmbedded(
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
            "::android::hardware::hidl_memory",
            "" /* childName */,
            "::android::hardware");
}

bool MemoryType::needsEmbeddedReadWrite() const {
    return true;
}

bool MemoryType::resultNeedsDeref() const {
    return true;
}

bool MemoryType::isMemory() const {
    return true;
}

bool MemoryType::deepIsJavaCompatible(std::unordered_set<const Type*>* /* visited */) const {
    return false;
}

static HidlTypeAssertion assertion("hidl_memory", 40 /* size */);
void MemoryType::getAlignmentAndSize(size_t *align, size_t *size) const {
    *align = 8;  // hidl_memory
    *size = assertion.size();
}

void MemoryType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
}

}  // namespace android

