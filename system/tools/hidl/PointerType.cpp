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

#include "PointerType.h"

#include <hidl-util/Formatter.h>
#include <android-base/logging.h>

namespace android {

PointerType::PointerType(Scope* parent) : Type(parent) {}

bool PointerType::isPointer() const {
    return true;
}

bool PointerType::isElidableType() const {
    return true;
}

std::string PointerType::getCppType(StorageMode /*mode*/,
                                    bool /*specifyNamespaces*/) const {
    return "void*";
}

std::string PointerType::typeName() const {
    return "local pointer";
}

std::string PointerType::getVtsType() const {
    return "TYPE_POINTER";
}

void PointerType::emitReaderWriter(
        Formatter& out,
        const std::string& name,
        const std::string& /*parcelObj*/,
        bool /*parcelObjIsPointer*/,
        bool /*isReader*/,
        ErrorMode /*mode*/) const {
    out << "(void)" << name << ";\n";
    out << "LOG_ALWAYS_FATAL(\"Pointer is only supported in passthrough mode\");\n\n";
}

void PointerType::emitReaderWriterEmbedded(
        Formatter& out,
        size_t /*depth*/,
        const std::string& name,
        const std::string& /*sanitizedName*/,
        bool /*nameIsPointer*/,
        const std::string& parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode,
        const std::string& parentName,
        const std::string& offsetText) const {
    out << "(void) " << parcelObj << ";\n";
    out << "(void) " << parentName << ";\n";
    out << "(void) (" << offsetText << ");\n";

    // same exact code
    emitReaderWriter(out, name, parcelObj, parcelObjIsPointer, isReader, mode);
}

bool PointerType::needsEmbeddedReadWrite() const {
    return true;
}

bool PointerType::resultNeedsDeref() const {
    return false;
}

bool PointerType::deepIsJavaCompatible(std::unordered_set<const Type*>* /* visited */) const {
    return false;
}

bool PointerType::deepContainsPointer(std::unordered_set<const Type*>* /* visited */) const {
    return true;
}

void PointerType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
}

}  // namespace android

