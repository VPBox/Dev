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

#ifndef DEATH_RECIPIENT_TYPE_H_

#define DEATH_RECIPIENT_TYPE_H_

#include "Type.h"

namespace android {

struct DeathRecipientType : public Type {
    DeathRecipientType(Scope* parent);

    std::string getCppType(
            StorageMode mode,
            bool specifyNamespaces) const override;

    std::string getJavaType(bool forInitializer) const override;

    std::string getVtsType() const override;
    std::string typeName() const override;

    void emitReaderWriter(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const override;

    bool needsEmbeddedReadWrite() const override;
    bool resultNeedsDeref() const override;

    void getAlignmentAndSize(size_t *align, size_t *size) const override;

    void emitVtsTypeDeclarations(Formatter& out) const override;
};

}  // namespace android

#endif  // DEATH_RECIPIENT_TYPE_H_

