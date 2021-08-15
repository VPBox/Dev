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

#ifndef ARRAY_TYPE_H_

#define ARRAY_TYPE_H_

#include "Reference.h"
#include "Type.h"

#include <vector>

namespace android {

struct ConstantExpression;

struct ArrayType : public Type {
    ArrayType(const Reference<Type>& elementType, ConstantExpression* size, Scope* parent);

    bool isArray() const override;
    bool deepCanCheckEquality(std::unordered_set<const Type*>* visited) const override;

    const Type* getElementType() const;

    void appendDimension(ConstantExpression *size);
    size_t countDimensions() const;

    std::string typeName() const override;

    std::vector<const Reference<Type>*> getReferences() const override;

    std::vector<const ConstantExpression*> getConstantExpressions() const override;

    // Extends existing array by adding another dimension.
    status_t resolveInheritance() override;

    status_t validate() const override;

    std::string getCppType(StorageMode mode,
                           bool specifyNamespaces) const override;

    std::string getInternalDataCppType() const;

    std::string getJavaType(bool forInitializer) const override;

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

    void emitResolveReferences(
            Formatter &out,
            const std::string &name,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const override;

    void emitResolveReferencesEmbedded(
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

    void emitJavaDump(
            Formatter &out,
            const std::string &streamName,
            const std::string &name) const override;

    bool needsEmbeddedReadWrite() const override;
    bool deepNeedsResolveReferences(std::unordered_set<const Type*>* visited) const override;
    bool resultNeedsDeref() const override;

    void emitJavaReaderWriter(
            Formatter &out,
            const std::string &parcelObj,
            const std::string &argName,
            bool isReader) const override;

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

    void emitVtsTypeDeclarations(Formatter& out) const override;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const override;
    bool deepContainsPointer(std::unordered_set<const Type*>* visited) const override;

    void getAlignmentAndSize(size_t *align, size_t *size) const override;

   private:
    Reference<Type> mElementType;
    std::vector<ConstantExpression*> mSizes;

    size_t dimension() const;

    DISALLOW_COPY_AND_ASSIGN(ArrayType);
};

}  // namespace android

#endif  // ARRAY_TYPE_H_

