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

#include "VectorType.h"

#include "ArrayType.h"
#include "CompoundType.h"
#include "HidlTypeAssertion.h"

#include <hidl-util/Formatter.h>
#include <android-base/logging.h>

namespace android {

VectorType::VectorType(Scope* parent) : TemplatedType(parent) {}

std::string VectorType::templatedTypeName() const {
    return "vector";
}

bool VectorType::isCompatibleElementType(const Type* elementType) const {
    if (elementType->isScalar()) {
        return true;
    }
    if (elementType->isString()) {
        return true;
    }
    if (elementType->isEnum()) {
        return true;
    }
    if (elementType->isBitField()) {
        return true;
    }
    if (elementType->isCompoundType()) {
        if (static_cast<const CompoundType*>(elementType)->containsInterface()) {
            return false;
        }
        return true;
    }
    if (elementType->isInterface()) {
        return true;
    }
    if (elementType->isHandle()) {
        return true;
    }
    if (elementType->isMemory()) {
        return true;
    }
    if (elementType->isTemplatedType()) {
        const Type* inner = static_cast<const TemplatedType*>(elementType)->getElementType();
        return this->isCompatibleElementType(inner) && !inner->isInterface();
    }
    if (elementType->isArray()) {
        const Type* inner = static_cast<const ArrayType*>(elementType)->getElementType();
        return this->isCompatibleElementType(inner) && !inner->isInterface();
    }
    return false;
}

bool VectorType::isVector() const {
    return true;
}

bool VectorType::isVectorOfBinders() const {
    return mElementType->isInterface();
}

bool VectorType::deepCanCheckEquality(std::unordered_set<const Type*>* visited) const {
    return mElementType->canCheckEquality(visited);
}

std::vector<const Reference<Type>*> VectorType::getStrongReferences() const {
    return {};
}

std::string VectorType::getCppType(StorageMode mode,
                                   bool specifyNamespaces) const {
    const std::string base =
          std::string(specifyNamespaces ? "::android::hardware::" : "")
        + "hidl_vec<"
        + mElementType->getCppStackType( specifyNamespaces)
        + ">";

    switch (mode) {
        case StorageMode_Stack:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";

        case StorageMode_Result:
        {
            if (isVectorOfBinders()) {
                return base;
            }

            return "const " + base + "*";
        }
    }
}

std::string VectorType::getJavaType(bool /* forInitializer */) const {
    const std::string elementJavaType = mElementType->isTemplatedType()
        ? mElementType->getJavaType()
        : mElementType->getJavaTypeClass();

    return "java.util.ArrayList<" + elementJavaType + ">";
}

std::string VectorType::getJavaTypeClass() const {
    return "java.util.ArrayList";
}

std::string VectorType::getVtsType() const {
    return "TYPE_VECTOR";
}

std::string VectorType::getVtsValueName() const {
    return "vector_value";
}

void VectorType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    if (isVectorOfBinders()) {
        emitReaderWriterForVectorOfBinders(
                out, name, parcelObj, parcelObjIsPointer, isReader, mode);

        return;
    }

    std::string baseType = mElementType->getCppStackType();

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
            name /* sanitizedName */ ,
            isReader /* nameIsPointer */,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            parentName,
            "0 /* parentOffset */");
}

void VectorType::emitReaderWriterForVectorOfBinders(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {
    const std::string parcelObjDeref =
        parcelObj + (parcelObjIsPointer ? "->" : ".");

    if (isReader) {
        out << "{\n";
        out.indent();

        const std::string sizeName = "_hidl_" + name + "_size";

        out << "uint64_t "
            << sizeName
            << ";\n";

        out << "_hidl_err = "
            << parcelObjDeref
            << "readUint64(&"
            << sizeName
            << ");\n";

        handleError(out, mode);

        out << name
            << ".resize("
            << sizeName
            << ");\n\n"
            << "for (size_t _hidl_index = 0; _hidl_index < "
            << sizeName
            << "; ++_hidl_index) {\n";

        out.indent();

        out << mElementType->getCppStackType(true /* specifyNamespaces */)
            << " _hidl_base;\n";

        mElementType->emitReaderWriter(
                out,
                "_hidl_base",
                parcelObj,
                parcelObjIsPointer,
                isReader,
                mode);

        out << name
            << "[_hidl_index] = _hidl_base;\n";

        out.unindent();
        out << "}\n";

        out.unindent();
        out << "}\n";
    } else {
        out << "_hidl_err = "
            << parcelObjDeref
            << "writeUint64("
            << name
            << ".size());\n";

        handleError(out, mode);

        out << "for (size_t _hidl_index = 0; _hidl_index < "
            << name
            << ".size(); ++_hidl_index) {\n";

        out.indent();

        mElementType->emitReaderWriter(
                out,
                name + "[_hidl_index]",
                parcelObj,
                parcelObjIsPointer,
                isReader,
                mode);

        out.unindent();
        out << "}\n";
    }
}

void VectorType::emitReaderWriterEmbedded(
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
    std::string baseType = getCppStackType();

    const std::string childName = "_hidl_" + sanitizedName + "_child";
    out << "size_t " << childName << ";\n\n";

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
            baseType,
            childName,
            "::android::hardware");

    if (!mElementType->needsEmbeddedReadWrite()) {
        return;
    }

    const std::string nameDeref = name + (nameIsPointer ? "->" : ".");

    baseType = mElementType->getCppStackType();

    std::string iteratorName = "_hidl_index_" + std::to_string(depth);

    out << "for (size_t "
        << iteratorName
        << " = 0; "
        << iteratorName
        << " < "
        << nameDeref
        << "size(); ++"
        << iteratorName
        << ") {\n";

    out.indent();

    mElementType->emitReaderWriterEmbedded(
            out,
            depth + 1,
            (nameIsPointer ? "(*" + name + ")" : name)
                + "[" + iteratorName + "]",
            sanitizedName + (nameIsPointer ? "_deref" : "") + "_indexed",
            false /* nameIsPointer */,
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode,
            childName,
            iteratorName + " * sizeof(" + baseType + ")");

    out.unindent();

    out << "}\n\n";
}

void VectorType::emitResolveReferences(
            Formatter &out,
            const std::string &name,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const {
    emitResolveReferencesEmbeddedHelper(
        out,
        0, /* depth */
        name,
        name /* sanitizedName */,
        nameIsPointer,
        parcelObj,
        parcelObjIsPointer,
        isReader,
        mode,
        "_hidl_" + name + "_child",
        "0 /* parentOffset */");
}

void VectorType::emitResolveReferencesEmbedded(
            Formatter &out,
            size_t depth,
            const std::string &name,
            const std::string &sanitizedName,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string & /* parentName */,
            const std::string & /* offsetText */) const {
    emitResolveReferencesEmbeddedHelper(
        out, depth, name, sanitizedName, nameIsPointer, parcelObj,
        parcelObjIsPointer, isReader, mode, "", "");
}

bool VectorType::useParentInEmitResolveReferencesEmbedded() const {
    // parentName and offsetText is not used in emitResolveReferencesEmbedded
    return false;
}

void VectorType::emitResolveReferencesEmbeddedHelper(
            Formatter &out,
            size_t depth,
            const std::string &name,
            const std::string &sanitizedName,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string &childName,
            const std::string &childOffsetText) const {
    CHECK(needsResolveReferences() && mElementType->needsResolveReferences());

    const std::string nameDeref = name + (nameIsPointer ? "->" : ".");
    const std::string nameDerefed = (nameIsPointer ? "*" : "") + name;
    std::string elementType = mElementType->getCppStackType();

    std::string myChildName = childName, myChildOffset = childOffsetText;

    if(myChildName.empty() && myChildOffset.empty()) {
        myChildName = "_hidl_" + sanitizedName + "_child";
        myChildOffset = "0";

        out << "size_t " << myChildName << ";\n";
        out << "_hidl_err = ::android::hardware::findInParcel("
            << nameDerefed << ", "
            << (parcelObjIsPointer ? "*" : "") << parcelObj << ", "
            << "&" << myChildName
            << ");\n";

        handleError(out, mode);
    }

    std::string iteratorName = "_hidl_index_" + std::to_string(depth);

    out << "for (size_t "
        << iteratorName
        << " = 0; "
        << iteratorName
        << " < "
        << nameDeref
        << "size(); ++"
        << iteratorName
        << ") {\n";

    out.indent();

    mElementType->emitResolveReferencesEmbedded(
        out,
        depth + 1,
        (nameIsPointer ? "(*" + name + ")" : name) + "[" + iteratorName + "]",
        sanitizedName + (nameIsPointer ? "_deref" : "") + "_indexed",
        false /* nameIsPointer */,
        parcelObj,
        parcelObjIsPointer,
        isReader,
        mode,
        myChildName,
        myChildOffset + " + " +
                iteratorName + " * sizeof(" + elementType + ")");

    out.unindent();

    out << "}\n\n";
}

void VectorType::emitJavaReaderWriter(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader) const {
    if (mElementType->isCompoundType()) {

        if (isReader) {
            out << mElementType->getJavaType()
                << ".readVectorFromParcel("
                << parcelObj
                << ");\n";
        } else {
            out << mElementType->getJavaType()
                << ".writeVectorToParcel("
                << parcelObj
                << ", "
                << argName
                << ");\n";
        }

        return;
    }

    if (mElementType->isArray()) {
        size_t align, size;
        getAlignmentAndSize(&align, &size);
        if (isReader) {
            out << " new "
                << getJavaType(false /* forInitializer */)
                << "();\n";
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
        };

        out.unindent();
        out << "}\n";

        return;
    }

    emitJavaReaderWriterWithSuffix(
            out,
            parcelObj,
            argName,
            isReader,
            mElementType->getJavaSuffix() + "Vector",
            "" /* extra */);
}

void VectorType::emitJavaFieldInitializer(
        Formatter &out, const std::string &fieldName) const {
    const std::string typeName = getJavaType(false /* forInitializer */);
    const std::string fieldDeclaration = typeName + " " + fieldName;

    emitJavaFieldDefaultInitialValue(out, fieldDeclaration);
}

void VectorType::emitJavaFieldDefaultInitialValue(
        Formatter &out, const std::string &declaredFieldName) const {
    out << declaredFieldName
        << " = new "
        << getJavaType(false /* forInitializer */)
        << "();\n";
}

void VectorType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t depth,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {

    const std::string fieldNameWithCast = isReader
        ? "(" + getJavaTypeCast(fieldName) + ")"
        : fieldName;

    VectorType::EmitJavaFieldReaderWriterForElementType(
            out,
            depth,
            mElementType.get(),
            parcelName,
            blobName,
            fieldNameWithCast,
            offset,
            isReader);
}

void VectorType::EmitJavaFieldReaderWriterForElementType(
        Formatter &out,
        size_t depth,
        const Type *elementType,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) {
    size_t elementAlign, elementSize;
    elementType->getAlignmentAndSize(&elementAlign, &elementSize);

    if (isReader) {
        out << "{\n";
        out.indent();

        out << "int _hidl_vec_size = "
            << blobName
            << ".getInt32("
            << offset
            << " + 8 /* offsetof(hidl_vec<T>, mSize) */);\n";

        out << "android.os.HwBlob childBlob = "
            << parcelName
            << ".readEmbeddedBuffer(\n";

        out.indent();
        out.indent();

        out << "_hidl_vec_size * "
            << elementSize << ","
            << blobName
            << ".handle(),\n"
            << offset
            << " + 0 /* offsetof(hidl_vec<T>, mBuffer) */,"
            << "true /* nullable */);\n\n";

        out.unindent();
        out.unindent();

        out << fieldName << ".clear();\n";
        std::string iteratorName = "_hidl_index_" + std::to_string(depth);

        out << "for (int "
            << iteratorName
            << " = 0; "
            << iteratorName
            << " < _hidl_vec_size; "
            << "++"
            << iteratorName
            << ") {\n";

        out.indent();

        elementType->emitJavaFieldInitializer(out, "_hidl_vec_element");

        elementType->emitJavaFieldReaderWriter(
                out,
                depth + 1,
                parcelName,
                "childBlob",
                "_hidl_vec_element",
                iteratorName + " * " + std::to_string(elementSize),
                true /* isReader */);

        out << fieldName
            << ".add(_hidl_vec_element);\n";

        out.unindent();

        out << "}\n";

        out.unindent();
        out << "}\n";

        return;
    }

    out << "{\n";
    out.indent();

    out << "int _hidl_vec_size = "
        << fieldName
        << ".size();\n";

    out << blobName
        << ".putInt32("
        << offset
        << " + 8 /* offsetof(hidl_vec<T>, mSize) */, _hidl_vec_size);\n";

    out << blobName
        << ".putBool("
        << offset
        << " + 12 /* offsetof(hidl_vec<T>, mOwnsBuffer) */, false);\n";

    // XXX make HwBlob constructor take a long instead of an int?
    out << "android.os.HwBlob childBlob = new android.os.HwBlob((int)(_hidl_vec_size * "
        << elementSize
        << "));\n";

    std::string iteratorName = "_hidl_index_" + std::to_string(depth);

    out << "for (int "
        << iteratorName
        << " = 0; "
        << iteratorName
        << " < _hidl_vec_size; "
        << "++"
        << iteratorName
        << ") {\n";

    out.indent();

    elementType->emitJavaFieldReaderWriter(
            out,
            depth + 1,
            parcelName,
            "childBlob",
            fieldName + ".get(" + iteratorName + ")",
            iteratorName + " * " + std::to_string(elementSize),
            false /* isReader */);

    out.unindent();

    out << "}\n";

    out << blobName
        << ".putBlob("
        << offset
        << " + 0 /* offsetof(hidl_vec<T>, mBuffer) */, childBlob);\n";

    out.unindent();
    out << "}\n";
}

bool VectorType::needsEmbeddedReadWrite() const {
    return true;
}

bool VectorType::deepNeedsResolveReferences(std::unordered_set<const Type*>* visited) const {
    if (mElementType->needsResolveReferences(visited)) {
        return true;
    }
    return TemplatedType::deepNeedsResolveReferences(visited);
}

bool VectorType::resultNeedsDeref() const {
    return !isVectorOfBinders();
}

bool VectorType::deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const {
    if (!mElementType->isJavaCompatible(visited)) {
        return false;
    }

    if (mElementType->isArray()) {
        return static_cast<const ArrayType*>(mElementType.get())->countDimensions() == 1;
    }

    if (mElementType->isVector()) {
        return false;
    }

    if (isVectorOfBinders()) {
        return false;
    }

    return TemplatedType::deepIsJavaCompatible(visited);
}

bool VectorType::deepContainsPointer(std::unordered_set<const Type*>* visited) const {
    if (mElementType->containsPointer(visited)) {
        return true;
    }
    return TemplatedType::deepContainsPointer(visited);
}

// All hidl_vec<T> have the same size.
static HidlTypeAssertion assertion("hidl_vec<char>", 16 /* size */);

void VectorType::getAlignmentAndSizeStatic(size_t *align, size_t *size) {
    *align = 8;  // hidl_vec<T>
    *size = assertion.size();
}

void VectorType::getAlignmentAndSize(size_t *align, size_t *size) const {
    VectorType::getAlignmentAndSizeStatic(align, size);
}

}  // namespace android

