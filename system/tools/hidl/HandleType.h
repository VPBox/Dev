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

#ifndef HANDLE_TYPE_H_

#define HANDLE_TYPE_H_

#include "Type.h"

namespace android {

struct HandleType : public Type {
    HandleType(Scope* parent);

    bool isHandle() const override;

    std::string typeName() const override;

    std::string getCppType(
            StorageMode mode,
            bool specifyNamespaces) const override;

    std::string getJavaType(bool forInitializer) const override;

    std::string getJavaSuffix() const override;

    std::string getVtsType() const override;

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

    void emitJavaFieldInitializer(
            Formatter &out, const std::string &fieldName) const override;

    void emitJavaFieldDefaultInitialValue(
            Formatter &out, const std::string &declaredFieldName) const override;

    void emitJavaFieldReaderWriter(
            Formatter &out,
            size_t depth,
            const std::string &parcelName,
            const std::string &blobName,
            const std::string &fieldName,
            const std::string &offset,
            bool isReader) const override;

    bool needsEmbeddedReadWrite() const override;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const override;

    void getAlignmentAndSize(size_t *align, size_t *size) const override;

    void emitVtsTypeDeclarations(Formatter& out) const override;
};

}  // namespace android

#endif  // HANDLE_TYPE_H_

