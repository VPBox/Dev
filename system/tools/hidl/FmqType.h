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

#ifndef FMQ_TYPE_H_

#define FMQ_TYPE_H_

#include "Type.h"

namespace android {

struct FmqType : public TemplatedType {
    FmqType(const char* nsp, const char* name, Scope* parent);

    std::string fullName() const;

    std::string templatedTypeName() const override;

    std::string getCppType(
            StorageMode mode,
            bool specifyNamespaces) const override;

    void emitReaderWriter(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const override;

    void emitReaderWriterEmbedded(
            Formatter &out,
            size_t depth,
            const std::string &name,
            const std::string &sanitizedName,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string &parentName,
            const std::string &offsetText) const override;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const override;

    void getAlignmentAndSize(size_t *align, size_t *size) const override;

    bool needsEmbeddedReadWrite() const override;
    bool resultNeedsDeref() const override;
    bool isCompatibleElementType(const Type* elementType) const override;

    std::string getVtsType() const override;
    std::string getVtsValueName() const override;
 private:
    std::string mNamespace;
    std::string mName;

    DISALLOW_COPY_AND_ASSIGN(FmqType);
};

}  // namespace android

#endif  // FMQ_TYPE_H_
