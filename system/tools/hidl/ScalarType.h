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

#ifndef SCALAR_TYPE_H_

#define SCALAR_TYPE_H_

#include "Type.h"

namespace android {

struct ScalarType : public Type {
    enum Kind {
        KIND_BOOL,
        KIND_INT8,
        KIND_UINT8,
        KIND_INT16,
        KIND_UINT16,
        KIND_INT32,
        KIND_UINT32,
        KIND_INT64,
        KIND_UINT64,
        KIND_FLOAT,
        KIND_DOUBLE,
    };

    ScalarType(Kind kind, Scope* parent);

    bool isScalar() const override;

    bool isElidableType() const override;
    const ScalarType *resolveToScalarType() const override;

    bool deepCanCheckEquality(std::unordered_set<const Type*>* visited) const override;

    std::string typeName() const override;
    bool isValidEnumStorageType() const;

    std::string getCppType(
            StorageMode mode,
            bool specifyNamespaces) const override;

    std::string getJavaType(bool forInitializer) const override;
    std::string getJavaTypeClass() const override;

    std::string getJavaSuffix() const override;

    std::string getVtsType() const override;
    std::string getVtsScalarType() const;

    void emitReaderWriter(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const override;

    void emitReaderWriterWithCast(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            bool needsCast) const;

    void emitHexDump(
            Formatter &out,
            const std::string &streamName,
            const std::string &name) const;

    void emitConvertToJavaHexString(
            Formatter &out,
            const std::string &name) const;

    void emitJavaFieldReaderWriter(
            Formatter &out,
            size_t depth,
            const std::string &parcelName,
            const std::string &blobName,
            const std::string &fieldName,
            const std::string &offset,
            bool isReader) const override;

    void emitVtsTypeDeclarations(Formatter& out) const override;

    void getAlignmentAndSize(size_t *align, size_t *size) const override;

    Kind getKind() const;

private:
    Kind mKind;

    DISALLOW_COPY_AND_ASSIGN(ScalarType);
};

}  // namespace android

#endif  // SCALAR_TYPE_H_
