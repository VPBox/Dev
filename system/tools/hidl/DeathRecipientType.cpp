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

#include "DeathRecipientType.h"

#include <hidl-util/Formatter.h>
#include <android-base/logging.h>

namespace android {

DeathRecipientType::DeathRecipientType(Scope* parent) : Type(parent) {}

std::string DeathRecipientType::typeName() const {
    return "death recipient";
}

std::string DeathRecipientType::getCppType(StorageMode mode,
                                   bool specifyNamespaces) const {
    const std::string base =
          std::string(specifyNamespaces ? "::android::" : "")
        + "sp<"
        + (specifyNamespaces ? "::android::hardware::" : "")
        + "hidl_death_recipient>";

    switch (mode) {
        case StorageMode_Stack:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";

        case StorageMode_Result:
            return "const " + base + "*";
    }
}

std::string DeathRecipientType::getJavaType(bool /* forInitializer */) const {
    // TODO(b/33440494) decouple from hwbinder
    return "android.os.IHwBinder.DeathRecipient";
}

std::string DeathRecipientType::getVtsType() const {
    return "TYPE_DEATH_RECIPIENT";
}

void DeathRecipientType::emitReaderWriter(
        Formatter& out,
        const std::string& /* name */,
        const std::string& /* parcelObj */,
        bool /* parcelObjIsPointer */,
        bool /* isReader */,
        ErrorMode /* mode */) const {
    out << "LOG_ALWAYS_FATAL(\"DeathRecipient is only supported in passthrough mode\");\n";
}

bool DeathRecipientType::needsEmbeddedReadWrite() const {
    return false;
}

bool DeathRecipientType::resultNeedsDeref() const {
    return true;
}

void DeathRecipientType::getAlignmentAndSize(size_t *align, size_t *size) const {
    *align = *size = 0; // this object should only be used in passthrough mode
}

void DeathRecipientType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
}

}  // namespace android

