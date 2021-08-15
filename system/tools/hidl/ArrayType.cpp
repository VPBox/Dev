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

#include "ArrayType.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <iostream>

#include "ConstantExpression.h"

namespace android {

ArrayType::ArrayType(const Reference<Type>& elementType, ConstantExpression* size, Scope* parent)
    : Type(parent), mElementType(elementType), mSizes{size} {
    CHECK(!elementType.isEmptyReference());
}

void ArrayType::appendDimension(ConstantExpression *size) {
    mSizes.push_back(size);
}

size_t ArrayType::countDimensions() const {
    return mSizes.size();
}

bool ArrayType::isArray() const {
    return true;
}

bool ArrayType::deepCanCheckEquality(std::unordered_set<const Type*>* visited) const {
    return mElementType->canCheckEquality(visited);
}

const Type* ArrayType::getElementType() const {
    return mElementType.get();
}

std::string ArrayType::typeName() const {
    if (dimension() == 1) {
        return "array of " + mElementType->typeName();
    }

    return std::to_string(dimension()) + "d array of " + mElementType->typeName();
}

std::vector<const Reference<Type>*> ArrayType::getReferences() const {
    return {&mElementType};
}

std::vector<const ConstantExpression*> ArrayType::getConstantExpressions() const {
    std::vector<const ConstantExpression*> ret;
    ret.insert(ret.end(), mSizes.begin(), mSizes.end());
    return ret;
}

status_t ArrayType::resolveInheritance() {
    // Resolve for typedefs
    while (mElementType->isArray()) {
        ArrayType* innerArray = static_cast<ArrayType*>(mElementType.get());
        mSizes.insert(mSizes.end(), innerArray->mSizes.begin(), innerArray->mSizes.end());
        mElementType = innerArray->mElementType;
    }
    return Type::resolveInheritance();
}

status_t ArrayType::validate() const {
    CHECK(!mElementType->isArray());

    if (mElementType->isInterface()) {
        std::cerr << "ERROR: Arrays of interface types are not supported"
                  << " at " << mElementType.location() << "\n";

        return UNKNOWN_ERROR;
    }
    return Type::validate();
}

std::string ArrayType::getCppType(StorageMode mode,
                                  bool specifyNamespaces) const {
    const std::string base = mElementType->getCppStackType(specifyNamespaces);

    std::string space = specifyNamespaces ? "::android::hardware::" : "";
    std::string arrayType = space + "hidl_array<" + base;

    for (size_t i = 0; i < mSizes.size(); ++i) {
        arrayType += ", " + mSizes[i]->cppValue();
    }

    arrayType += ">";

    switch (mode) {
        case StorageMode_Stack:
            return arrayType;

        case StorageMode_Argument:
            return "const " + arrayType + "&";

        case StorageMode_Result:
            return "const " + arrayType + "*";
    }

    CHECK(!"Should not be here");
}

std::string ArrayType::getInternalDataCppType() const {
    std::string result = mElementType->getCppStackType();
    for (size_t i = 0; i < mSizes.size(); ++i) {
        result += "[";
        result += mSizes[i]->cppValue();
        result += "]";
    }
    return result;
}

std::string ArrayType::getJavaType(bool forInitializer) const {
    std::string base =
        mElementType->getJavaType(forInitializer);

    for (size_t i = 0; i < mSizes.size(); ++i) {
        base += "[";

        if (forInitializer) {
            base += mSizes[i]->javaValue();
        } else {
            base += "/* " + mSizes[i]->expression() + " */";
        }

        base += "]";
    }

    return base;
}

std::string ArrayType::getVtsType() const {
    return "TYPE_ARRAY";
}

void ArrayType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    std::string baseType = mElementType->getCppStackType();

    const std::string parentName = "_hidl_" + name + "_parent";

    out << "size_t " << parentName << ";\n\n";

    const std::string parcelObjDeref =
        parcelObj + (parcelObjIsPointer ? "->" : ".");

    size_t numArrayElements = 1;
    for (auto size : mSizes) {
        numArrayElements *= size->castSizeT();
    }
    if (isReader) {
        out << "_hidl_err = "
            << parcelObjDeref
            << "readBuffer("
            << numArrayElements
            << " * sizeof("
            << baseType
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
            << "writeBuffer("
            << name
            << ".data(), "
            << numArrayElements
            << " * sizeof("
            << baseType
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

void ArrayType::emitReaderWriterEmbedded(
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
        const std::string &offsetText) const {
    if (!mElementType->needsEmbeddedReadWrite()) {
        return;
    }

    const std::string nameDeref = name + (nameIsPointer ? "->" : ".");

    std::string baseType = mElementType->getCppStackType();

    std::string iteratorName = "_hidl_index_" + std::to_string(depth);

    out << "for (size_t "
        << iteratorName
        << " = 0; "
        << iteratorName
        << " < "
        << dimension()
        << "; ++"
        << iteratorName
        << ") {\n";

    out.indent();

    mElementType->emitReaderWriterEmbedded(
            out,
            depth + 1,
            nameDeref + "data()[" + iteratorName + "]",
            sanitizedName + "_indexed",
            false /* nameIsPointer */,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            parentName,
            offsetText
                + " + " + iteratorName + " * sizeof("
                + baseType
                + ")");

    out.unindent();

    out << "}\n\n";
}

void ArrayType::emitResolveReferences(
            Formatter &out,
            const std::string &name,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const {
    emitResolveReferencesEmbedded(
        out,
        0 /* depth */,
        name,
        name /* sanitizedName */,
        nameIsPointer,
        parcelObj,
        parcelObjIsPointer,
        isReader,
        mode,
        "_hidl_" + name + "_parent",
        "0 /* parentOffset */");
}

void ArrayType::emitResolveReferencesEmbedded(
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
            const std::string &offsetText) const {
    CHECK(needsResolveReferences() && mElementType->needsResolveReferences());

    const std::string nameDeref = name + (nameIsPointer ? "->" : ".");

    std::string baseType = mElementType->getCppStackType();

    std::string iteratorName = "_hidl_index_" + std::to_string(depth);

    out << "for (size_t "
        << iteratorName
        << " = 0; "
        << iteratorName
        << " < "
        << dimension()
        << "; ++"
        << iteratorName
        << ") {\n";

    out.indent();

    mElementType->emitResolveReferencesEmbedded(
        out,
        depth + 1,
        nameDeref + "data()[" + iteratorName + "]",
        sanitizedName + "_indexed",
        false /* nameIsPointer */,
        parcelObj,
        parcelObjIsPointer,
        isReader,
        mode,
        parentName,
        offsetText + " + " + iteratorName + " * sizeof("
        + baseType
        + ")");

    out.unindent();

    out << "}\n\n";
}

void ArrayType::emitJavaDump(
        Formatter &out,
        const std::string &streamName,
        const std::string &name) const {
    out << streamName << ".append(java.util.Arrays."
        << (countDimensions() > 1 ? "deepToString" : "toString")
        << "("
        << name
        << "));\n";
}


bool ArrayType::needsEmbeddedReadWrite() const {
    return mElementType->needsEmbeddedReadWrite();
}

bool ArrayType::deepNeedsResolveReferences(std::unordered_set<const Type*>* visited) const {
    if (mElementType->needsResolveReferences(visited)) {
        return true;
    }
    return Type::deepNeedsResolveReferences(visited);
}

bool ArrayType::resultNeedsDeref() const {
    return true;
}

void ArrayType::emitJavaReaderWriter(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader) const {
    size_t align, size;
    getAlignmentAndSize(&align, &size);

    if (isReader) {
        out << "new "
            << getJavaType(true /* forInitializer */)
            << ";\n";
    }

    out << "{\n";
    out.indent();

    out << "android.os.HwBlob _hidl_blob = ";

    if (isReader) {
        out << parcelObj
            << ".readBuffer("
            << size
            << " /* size */);\n";
    } else {
        out << "new android.os.HwBlob("
            << size
            << " /* size */);\n";
    }

    emitJavaFieldReaderWriter(
            out,
            0 /* depth */,
            parcelObj,
            "_hidl_blob",
            argName,
            "0 /* offset */",
            isReader);

    if (!isReader) {
        out << parcelObj << ".writeBuffer(_hidl_blob);\n";
    }

    out.unindent();
    out << "}\n";
}

void ArrayType::emitJavaFieldInitializer(
        Formatter &out, const std::string &fieldName) const {
    const std::string typeName = getJavaType(false /* forInitializer */);
    const std::string fieldDeclaration = typeName + " " + fieldName;

    emitJavaFieldDefaultInitialValue(out, fieldDeclaration);
}

void ArrayType::emitJavaFieldDefaultInitialValue(
        Formatter &out, const std::string &declaredFieldName) const {
    out << declaredFieldName
        << " = new "
        << getJavaType(true /* forInitializer */)
        << ";\n";
}

void ArrayType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t depth,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    out << "{\n";
    out.indent();

    std::string offsetName = "_hidl_array_offset_" + std::to_string(depth);
    out << "long " << offsetName << " = " << offset << ";\n";

    const bool isPrimitiveArray = mElementType->isScalar();

    /* If the element type corresponds to a Java primitive type we can optimize
       the innermost loop by copying a linear range of memory instead of doing
       a per-element copy. As a result the outer nested loop does not include
       the final dimension. */
    const size_t loopDimensions = mSizes.size() - (isPrimitiveArray ? 1 : 0);

    std::string indexString;
    for (size_t dim = 0; dim < loopDimensions; ++dim) {
        std::string iteratorName =
            "_hidl_index_" + std::to_string(depth) + "_" + std::to_string(dim);

        out << "for (int "
            << iteratorName
            << " = 0; "
            << iteratorName
            << " < "
            << mSizes[dim]->javaValue()
            << "; ++"
            << iteratorName
            << ") {\n";

        out.indent();

        indexString += "[" + iteratorName + "]";
    }

    const bool isIndexed = (loopDimensions > 0);
    const std::string fieldNameWithCast = isIndexed
            ? "(" + getJavaTypeCast(fieldName) + ")" + indexString
            : getJavaTypeCast(fieldName);

    if (isReader && mElementType->isCompoundType()) {
        mElementType->emitJavaFieldDefaultInitialValue(out, fieldNameWithCast);
    }

    if (!isPrimitiveArray) {
        mElementType->emitJavaFieldReaderWriter(
                out,
                depth + 1,
                parcelName,
                blobName,
                fieldNameWithCast,
                offsetName,
                isReader);

        size_t elementAlign, elementSize;
        mElementType->getAlignmentAndSize(&elementAlign, &elementSize);

        out << offsetName << " += " << std::to_string(elementSize) << ";\n";
    } else {
        if (isReader) {
            out << blobName
                << ".copyTo"
                << mElementType->getJavaSuffix()
                << "Array("
                << offsetName
                << ", "
                << fieldNameWithCast
                << ", "
                << mSizes.back()->javaValue()
                << " /* size */);\n";
        } else {
            std::string elemName = "_hidl_array_item_" + std::to_string(depth);

            out << mElementType->getJavaType(false /* forInitializer */)
                << "[] "
                << elemName
                << " = "
                << fieldNameWithCast
                << ";\n\n";

            out << "if ("
                << elemName
                << " == null || "
                << elemName
                << ".length != "
                << mSizes.back()->javaValue()
                << ") {\n";

            out.indent();

            out << "throw new IllegalArgumentException("
                << "\"Array element is not of the expected length\");\n";

            out.unindent();
            out << "}\n\n";

            out << blobName
                << ".put"
                << mElementType->getJavaSuffix()
                << "Array("
                << offsetName
                << ", "
                << elemName
                << ");\n";
        }

        size_t elementAlign, elementSize;
        mElementType->getAlignmentAndSize(&elementAlign, &elementSize);

        out << offsetName
            << " += "
            << mSizes.back()->javaValue()
            << " * "
            << elementSize
            << ";\n";
    }

    for (size_t dim = 0; dim < loopDimensions; ++dim) {
        out.unindent();
        out << "}\n";
    }

    out.unindent();
    out << "}\n";
}

void ArrayType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << "vector_size: " << mSizes[0]->rawValue() << "\n";
    out << "vector_value: {\n";
    out.indent();
    // Simple array case.
    if (mSizes.size() == 1) {
        mElementType->emitVtsTypeDeclarations(out);
    } else {  // Multi-dimension array case.
        for (size_t index = 1; index < mSizes.size(); index++) {
            out << "type: " << getVtsType() << "\n";
            out << "vector_size: " << mSizes[index]->rawValue() << "\n";
            out << "vector_value: {\n";
            out.indent();
            if (index == mSizes.size() - 1) {
                mElementType->emitVtsTypeDeclarations(out);
            }
        }
    }
    for (size_t index = 0; index < mSizes.size(); index++) {
        out.unindent();
        out << "}\n";
    }
}

bool ArrayType::deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const {
    if (!mElementType->isJavaCompatible(visited)) {
        return false;
    }
    return Type::deepIsJavaCompatible(visited);
}

bool ArrayType::deepContainsPointer(std::unordered_set<const Type*>* visited) const {
    if (mElementType->containsPointer(visited)) {
        return true;
    }
    return Type::deepContainsPointer(visited);
}

void ArrayType::getAlignmentAndSize(size_t *align, size_t *size) const {
    mElementType->getAlignmentAndSize(align, size);

    for (auto sizeInDimension : mSizes) {
        (*size) *= sizeInDimension->castSizeT();
    }
}

size_t ArrayType::dimension() const {
    size_t numArrayElements = 1;
    for (auto size : mSizes) {
        numArrayElements *= size->castSizeT();
    }
    return numArrayElements;
}

}  // namespace android

