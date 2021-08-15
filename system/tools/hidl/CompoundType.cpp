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

#include "CompoundType.h"

#include "ArrayType.h"
#include "ScalarType.h"
#include "VectorType.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <iostream>
#include <unordered_set>

namespace android {

CompoundType::CompoundType(Style style, const char* localName, const FQName& fullName,
                           const Location& location, Scope* parent)
    : Scope(localName, fullName, location, parent), mStyle(style), mFields(nullptr) {}

CompoundType::Style CompoundType::style() const {
    return mStyle;
}

void CompoundType::setFields(std::vector<NamedReference<Type>*>* fields) {
    mFields = fields;
}

std::vector<const Reference<Type>*> CompoundType::getReferences() const {
    std::vector<const Reference<Type>*> ret;
    ret.insert(ret.begin(), mFields->begin(), mFields->end());
    return ret;
}

status_t CompoundType::validate() const {
    for (const auto* field : *mFields) {
        const Type& type = field->type();

        if ((type.isVector() && static_cast<const VectorType*>(&type)->isVectorOfBinders())) {
            std::cerr << "ERROR: Struct/union cannot contain vectors of interfaces at "
                      << field->location() << "\n";
            return UNKNOWN_ERROR;
        }

        if (mStyle == STYLE_UNION) {
            if (type.isInterface()) {
                std::cerr << "ERROR: Union cannot contain interfaces at " << field->location()
                          << "\n";
                return UNKNOWN_ERROR;
            }

            if (type.needsEmbeddedReadWrite()) {
                std::cerr << "ERROR: Union must not contain any types that need fixup at "
                          << field->location() << "\n";
                return UNKNOWN_ERROR;
            }
        }
    }

    if (mStyle == STYLE_SAFE_UNION && mFields->size() < 2) {
        std::cerr << "ERROR: Safe union must contain at least two types to be useful at "
                  << location() << "\n";
        return UNKNOWN_ERROR;
    }

    status_t err = validateUniqueNames();
    if (err != OK) return err;

    err = validateSubTypeNames();
    if (err != OK) return err;

    return Scope::validate();
}

status_t CompoundType::validateUniqueNames() const {
    std::unordered_set<std::string> names;

    for (const auto* field : *mFields) {
        if (names.find(field->name()) != names.end()) {
            std::cerr << "ERROR: Redefinition of field '" << field->name() << "' at "
                      << field->location() << "\n";
            return UNKNOWN_ERROR;
        }
        names.insert(field->name());
    }

    return OK;
}

void CompoundType::emitInvalidSubTypeNamesError(const std::string& subTypeName,
                                                const Location& location) const {
    std::cerr << "ERROR: Type name '" << subTypeName << "' defined at "
              << location << " conflicts with a member function of "
              << "safe_union " << localName() << ". Consider renaming or "
              << "moving its definition outside the safe_union scope.\n";
}

status_t CompoundType::validateSubTypeNames() const {
    if (mStyle != STYLE_SAFE_UNION) { return OK; }
    const auto& subTypes = Scope::getSubTypes();

    for (const auto& subType : subTypes) {
        if (subType->localName() == "getDiscriminator") {
            emitInvalidSubTypeNamesError(subType->localName(),
                                         subType->location());
            return UNKNOWN_ERROR;
        }
    }

    return OK;
}

bool CompoundType::isCompoundType() const {
    return true;
}

bool CompoundType::deepCanCheckEquality(std::unordered_set<const Type*>* visited) const {
    if (mStyle == STYLE_UNION) {
        return false;
    }
    for (const auto* field : *mFields) {
        if (!field->get()->canCheckEquality(visited)) {
            return false;
        }
    }
    return true;
}

std::string CompoundType::typeName() const {
    switch (mStyle) {
        case STYLE_STRUCT: {
            return "struct " + localName();
        }
        case STYLE_UNION: {
            return "union " + localName();
        }
        case STYLE_SAFE_UNION: {
            return "safe_union " + localName();
        }
    }
    CHECK(!"Should not be here");
}

std::string CompoundType::getCppType(
        StorageMode mode,
        bool /* specifyNamespaces */) const {
    const std::string base = fullName();

    switch (mode) {
        case StorageMode_Stack:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";

        case StorageMode_Result:
            return base + (containsInterface()?"":"*");
    }
    CHECK(!"Should not be here");
}

std::string CompoundType::getJavaType(bool /* forInitializer */) const {
    return fullJavaName();
}

std::string CompoundType::getVtsType() const {
    switch (mStyle) {
        case STYLE_STRUCT:
        {
            return "TYPE_STRUCT";
        }
        case STYLE_UNION:
        {
            return "TYPE_UNION";
        }
        case STYLE_SAFE_UNION:
        {
            return "TYPE_SAFE_UNION";
        }
    }
    CHECK(!"Should not be here");
}

bool CompoundType::containsInterface() const {
    for (const auto& field : *mFields) {
        if (field->type().isCompoundType()) {
            const Type& t = field->type();
            const CompoundType* ct = static_cast<const CompoundType*>(&t);
            if (ct->containsInterface()) {
                return true;
            }
        }
        if (field->type().isInterface()) {
            return true;
        }
    }
    return false;
}

void CompoundType::emitSafeUnionUnknownDiscriminatorError(Formatter& out, const std::string& value,
                                                          bool fatal) const {
    if (fatal) {
        out << "::android::hardware::details::logAlwaysFatal(";
    } else {
        out << "ALOGE(\"%s\", ";
    }
    out << "(\n";
    out.indent(2, [&] {
        out << "\"Unknown union discriminator (value: \" +\n"
            << "std::to_string(" << getUnionDiscriminatorType()->getCppTypeCast(value)
            << ") + \").\").c_str());\n";
    });
}

void CompoundType::emitSafeUnionReaderWriterForInterfaces(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {

    CHECK(mStyle == STYLE_SAFE_UNION);

    out.block([&] {
        const auto discriminatorType = getUnionDiscriminatorType();
        if (isReader) {
            out << discriminatorType->getCppStackType()
                << " _hidl_d_primitive;\n";
        } else {
            out << "const "
                << discriminatorType->getCppStackType()
                << " _hidl_d_primitive = "
                << discriminatorType->getCppTypeCast(name + ".getDiscriminator()")
                << ";\n";
        }

        getUnionDiscriminatorType()->emitReaderWriter(out, "_hidl_d_primitive", parcelObj,
                                                    parcelObjIsPointer, isReader, mode);
        out << "switch (("
            << fullName()
            << "::hidl_discriminator) _hidl_d_primitive) ";

        out.block([&] {
            for (const auto& field : *mFields) {
                out << "case "
                    << fullName()
                    << "::hidl_discriminator::"
                    << field->name()
                    << ": ";

                const std::string tempFieldName = "_hidl_temp_" + field->name();
                out.block([&] {
                    if (isReader) {
                        out << field->type().getCppResultType()
                            << " "
                            << tempFieldName
                            << ";\n";

                        field->type().emitReaderWriter(out, tempFieldName, parcelObj,
                                                       parcelObjIsPointer, isReader, mode);

                        const std::string derefOperator = field->type().resultNeedsDeref()
                                                          ? "*" : "";
                        out << name
                            << "."
                            << field->name()
                            << "(std::move("
                            << derefOperator
                            << tempFieldName
                            << "));\n";
                    } else {
                        const std::string fieldValue = name + "." + field->name() + "()";
                        out << field->type().getCppArgumentType()
                            << " "
                            << tempFieldName
                            << " = "
                            << fieldValue
                            << ";\n";

                        field->type().emitReaderWriter(out, tempFieldName, parcelObj,
                                                       parcelObjIsPointer, isReader, mode);
                    }
                    out << "break;\n";
                }).endl();
            }

            out << "default: ";
            out.block([&] {
                   emitSafeUnionUnknownDiscriminatorError(out, "_hidl_d_primitive",
                                                          !isReader /*fatal*/);
                   if (isReader) {
                       out << "_hidl_err = BAD_VALUE;\n";
                       handleError(out, mode);
                   }
               })
                .endl();
        }).endl();
    }).endl();
}

void CompoundType::emitReaderWriter(
        Formatter &out,
        const std::string &name,
        const std::string &parcelObj,
        bool parcelObjIsPointer,
        bool isReader,
        ErrorMode mode) const {

    const std::string parcelObjDeref =
        parcelObj + (parcelObjIsPointer ? "->" : ".");

    if(containsInterface()){
        if (mStyle == STYLE_SAFE_UNION) {
            emitSafeUnionReaderWriterForInterfaces(out, name, parcelObj,
                                                   parcelObjIsPointer,
                                                   isReader, mode);
            return;
        }

        for (const auto& field : *mFields) {
            const std::string tempFieldName = "_hidl_temp_" + field->name();
            const std::string fieldValue = name + "." + field->name();

            out.block([&] {
                if (isReader) {
                    out << field->type().getCppResultType()
                        << " "
                        << tempFieldName
                        << ";\n";
                } else {
                    out << field->type().getCppArgumentType()
                        << " "
                        << tempFieldName
                        << " = "
                        << fieldValue
                        << ";\n";
                }

                field->type().emitReaderWriter(out, tempFieldName, parcelObj,
                                               parcelObjIsPointer, isReader, mode);
                if (isReader) {
                    const std::string derefOperator = field->type().resultNeedsDeref()
                                                      ? "*" : "";
                    out << fieldValue
                        << " = std::move("
                        << derefOperator
                        << tempFieldName
                        << ");\n";
                }
            }).endl();
        }
    } else {
        const std::string parentName = "_hidl_" + name + "_parent";

        out << "size_t " << parentName << ";\n\n";

        if (isReader) {
            out << "_hidl_err = " << parcelObjDeref << "readBuffer("
                << "sizeof(*" << name << "), &" << parentName << ", "
                << " const_cast<const void**>(reinterpret_cast<void **>("
                << "&" << name << ")));\n";
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

        bool needEmbeddedReadWrite = needsEmbeddedReadWrite();
        CHECK(mStyle != STYLE_UNION || !needEmbeddedReadWrite);

        if (needEmbeddedReadWrite) {
            emitReaderWriterEmbedded(out, 0 /* depth */, name, name, /* sanitizedName */
                                     isReader /* nameIsPointer */, parcelObj, parcelObjIsPointer,
                                     isReader, mode, parentName, "0 /* parentOffset */");
        }
    }
}

void CompoundType::emitReaderWriterEmbedded(
        Formatter &out,
        size_t /* depth */,
        const std::string &name,
        const std::string & /*sanitizedName */,
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
            fullName(),
            "" /* childName */,
            "" /* namespace */);
}

void CompoundType::emitJavaReaderWriter(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader) const {
    if (isReader) {
        out << "new " << fullJavaName() << "();\n";
    }

    out << "(" << getJavaTypeCast(argName) << ")."
        << (isReader ? "readFromParcel" : "writeToParcel") << "(" << parcelObj << ");\n";
}

void CompoundType::emitJavaFieldInitializer(
        Formatter &out, const std::string &fieldName) const {
    const std::string fieldDeclaration = fullJavaName() + " " + fieldName;
    emitJavaFieldDefaultInitialValue(out, fieldDeclaration);
}

void CompoundType::emitJavaFieldDefaultInitialValue(
        Formatter &out, const std::string &declaredFieldName) const {
    out << declaredFieldName
        << " = new "
        << fullJavaName()
        << "();\n";
}

void CompoundType::emitJavaFieldReaderWriter(
        Formatter &out,
        size_t /* depth */,
        const std::string &parcelName,
        const std::string &blobName,
        const std::string &fieldName,
        const std::string &offset,
        bool isReader) const {
    if (isReader) {
        out << "("
            << getJavaTypeCast(fieldName)
            << ").readEmbeddedFromParcel("
            << parcelName
            << ", "
            << blobName
            << ", "
            << offset
            << ");\n";

        return;
    }

    out << fieldName
        << ".writeEmbeddedToBlob("
        << blobName
        << ", "
        << offset
        << ");\n";
}
void CompoundType::emitResolveReferences(
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

void CompoundType::emitResolveReferencesEmbedded(
            Formatter &out,
            size_t /* depth */,
            const std::string &name,
            const std::string &/* sanitizedName */,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string &parentName,
            const std::string &offsetText) const {
    CHECK(needsResolveReferences());

    const std::string parcelObjDeref =
        parcelObjIsPointer ? ("*" + parcelObj) : parcelObj;

    const std::string parcelObjPointer =
        parcelObjIsPointer ? parcelObj : ("&" + parcelObj);

    const std::string nameDerefed = nameIsPointer ? ("*" + name) : name;
    const std::string namePointer = nameIsPointer ? name : ("&" + name);

    out << "_hidl_err = ";

    if (isReader) {
        out << "readEmbeddedReferenceFromParcel(\n";
    } else {
        out << "writeEmbeddedReferenceToParcel(\n";
    }

    out.indent(2, [&]{
        if (isReader) {
            out << "const_cast<"
                << fullName()
                << " *"
                << ">("
                << namePointer
                << "),\n"
                << parcelObjDeref;
        } else {
            out << nameDerefed
                << ",\n"
                << parcelObjPointer;
        }

        out << ",\n"
            << parentName
            << ",\n"
            << offsetText
            << ");\n\n";
    });

    handleError(out, mode);
}

void CompoundType::emitLayoutAsserts(Formatter& out, const Layout& layout,
                                     const std::string& layoutName) const {
    out << "static_assert(sizeof("
        << fullName()
        << layoutName
        << ") == "
        << layout.size
        << ", \"wrong size\");\n";

    out << "static_assert(__alignof("
        << fullName()
        << layoutName
        << ") == "
        << layout.align
        << ", \"wrong alignment\");\n";
}

void CompoundType::emitSafeUnionTypeDeclarations(Formatter& out) const {
    out << "struct "
        << localName()
        << " final {\n";

    out.indent();

    Scope::emitTypeDeclarations(out);

    bool hasPointer = containsPointer();
    CompoundLayout layout = hasPointer
                            ? CompoundLayout()
                            : getCompoundAlignmentAndSize();

    out << "enum class hidl_discriminator : "
        << getUnionDiscriminatorType()->getCppType(StorageMode_Stack, false)
        << " ";

    out.block([&] {
        const auto elements = getSafeUnionEnumElements(true /* useCppTypeName */);
        for (size_t i = 0; i < elements.size(); i++) {
            out << elements[i].fieldName
                << " = "
                << i
                << ",";

            if (!elements[i].fieldTypeName.empty()) {
                out << "  // "
                    << elements[i].fieldTypeName;
            }
            out << "\n";
        }
    });
    out << ";\n\n";

    out << localName() << "();\n"  // Constructor
        << "~" << localName() << "();\n"  // Destructor
        << localName() << "(" << localName() << "&&);\n"  // Move constructor
        << localName() << "(const " << localName() << "&);\n"  // Copy constructor
        << localName() << "& operator=(" << localName() << "&&);\n"  // Move assignment
        << localName() << "& operator=(const " << localName() << "&);\n\n";  // Copy assignment

    for (const auto& field : *mFields) {
        // Setter (copy)
        out << "void "
            << field->name()
            << "("
            << field->type().getCppArgumentType()
            << ");\n";

        if (field->type().resolveToScalarType() == nullptr) {
            // Setter (move)
            out << "void "
                << field->name()
                << "("
                << field->type().getCppStackType()
                << "&&);\n";
        }

        // Getter (mutable)
        out << field->type().getCppStackType()
            << "& "
            << field->name()
            << "();\n";

        // Getter (immutable)
        out << field->type().getCppArgumentType()
            << " "
            << field->name()
            << "() const;\n\n";
    }

    out << "// Utility methods\n";
    out << "hidl_discriminator getDiscriminator() const;\n\n";

    out << "constexpr size_t hidl_getUnionOffset() const ";
    out.block([&] {
        out << "return offsetof(" << fullName() << ", hidl_u);\n";
    }).endl().endl();

    out.unindent();
    out << "private:\n";
    out.indent();

    out << "void hidl_destructUnion();\n\n";

    out << "hidl_discriminator hidl_d";
    if (!hasPointer) {
        out << " __attribute__ ((aligned("
            << layout.discriminator.align << "))) ";
    }
    out << ";\n";
    out << "union hidl_union final {\n";
    out.indent();

    for (const auto& field : *mFields) {

        size_t fieldAlign, fieldSize;
        field->type().getAlignmentAndSize(&fieldAlign, &fieldSize);

        out << field->type().getCppStackType()
            << " "
            << field->name();

        if (!hasPointer) {
            out << " __attribute__ ((aligned("
                << fieldAlign
                << ")))";
        }
        out << ";\n";
    }

    out << "\n"
        << "hidl_union();\n"
        << "~hidl_union();\n";

    out.unindent();
    out << "} hidl_u;\n";

    if (!hasPointer) {
        out << "\n";

        emitLayoutAsserts(out, layout.innerStruct, "::hidl_union");
        emitLayoutAsserts(out, layout.discriminator, "::hidl_discriminator");
    }

    out.unindent();
    out << "};\n\n";

    if (!hasPointer) {
        emitLayoutAsserts(out, layout.overall, "");
        out << "\n";
    }
}

void CompoundType::emitTypeDeclarations(Formatter& out) const {
    if (mStyle == STYLE_SAFE_UNION) {
        emitSafeUnionTypeDeclarations(out);
        return;
    }

    out << ((mStyle == STYLE_STRUCT) ? "struct" : "union")
        << " "
        << localName()
        << " final {\n";

    out.indent();

    Scope::emitTypeDeclarations(out);

    if (containsPointer()) {
        for (const auto &field : *mFields) {
            field->emitDocComment(out);
            out << field->type().getCppStackType()
                << " "
                << field->name()
                << ";\n";
        }

        out.unindent();
        out << "};\n\n";

        return;
    }

    for (int pass = 0; pass < 2; ++pass) {
        size_t offset = 0;
        for (const auto &field : *mFields) {
            size_t fieldAlign, fieldSize;
            field->type().getAlignmentAndSize(&fieldAlign, &fieldSize);

            offset += Layout::getPad(offset, fieldAlign);

            if (pass == 0) {
                out << field->type().getCppStackType()
                    << " "
                    << field->name()
                    << " __attribute__ ((aligned("
                    << fieldAlign
                    << ")));\n";
            } else {
                out << "static_assert(offsetof("
                    << fullName()
                    << ", "
                    << field->name()
                    << ") == "
                    << offset
                    << ", \"wrong offset\");\n";
            }

            if (mStyle == STYLE_STRUCT) {
                offset += fieldSize;
            }
        }

        if (pass == 0) {
            out.unindent();
            out << "};\n\n";
        }
    }

    CompoundLayout layout = getCompoundAlignmentAndSize();
    emitLayoutAsserts(out, layout.overall, "");
    out << "\n";
}

void CompoundType::emitTypeForwardDeclaration(Formatter& out) const {
    switch (mStyle) {
        case STYLE_UNION: {
            out << "union";
            break;
        }
        case STYLE_STRUCT:
        case STYLE_SAFE_UNION: {
            out << "struct";
            break;
        }
        default: {
            CHECK(!"Should not be here");
        }
    }
    out << " " << localName() << ";\n";
}

void CompoundType::emitPackageTypeDeclarations(Formatter& out) const {
    Scope::emitPackageTypeDeclarations(out);

    out << "static inline std::string toString("
        << getCppArgumentType()
        << (mFields->empty() ? "" : " o")
        << ");\n";

    if (canCheckEquality()) {
        out << "static inline bool operator==("
            << getCppArgumentType() << " lhs, " << getCppArgumentType() << " rhs);\n";

        out << "static inline bool operator!=("
            << getCppArgumentType() << " lhs, " << getCppArgumentType() << " rhs);\n";
    } else {
        out << "// operator== and operator!= are not generated for " << localName() << "\n";
    }

    out.endl();
}

void CompoundType::emitPackageTypeHeaderDefinitions(Formatter& out) const {
    Scope::emitPackageTypeHeaderDefinitions(out);

    out << "static inline std::string toString("
        << getCppArgumentType()
        << (mFields->empty() ? "" : " o")
        << ") ";

    out.block([&] {
        // include toString for scalar types
        out << "using ::android::hardware::toString;\n"
            << "std::string os;\n";
        out << "os += \"{\";\n";

        if (mStyle == STYLE_SAFE_UNION) {
            out << "\nswitch (o.getDiscriminator()) {\n";
            out.indent();
        }

        for (const NamedReference<Type>* field : *mFields) {
            if (mStyle == STYLE_SAFE_UNION) {
                out << "case "
                    << fullName()
                    << "::hidl_discriminator::"
                    << field->name()
                    << ": ";

                out.block([&] {
                    out << "os += \"."
                    << field->name()
                    << " = \";\n"
                    << "os += toString(o."
                    << field->name()
                    << "());\n"
                    << "break;\n";
                }).endl();
            } else {
                out << "os += \"";
                if (field != *(mFields->begin())) {
                    out << ", ";
                }
                out << "." << field->name() << " = \";\n";
                field->type().emitDump(out, "os", "o." + field->name());
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] {
                   emitSafeUnionUnknownDiscriminatorError(out, "o.getDiscriminator()",
                                                          true /*fatal*/);
               })
                .endl();

            out.unindent();
            out << "}\n";
        }
        out << "os += \"}\"; return os;\n";
    }).endl().endl();

    if (canCheckEquality()) {
        out << "static inline bool operator==("
            << getCppArgumentType() << " " << (mFields->empty() ? "/* lhs */" : "lhs") << ", "
            << getCppArgumentType() << " " << (mFields->empty() ? "/* rhs */" : "rhs") << ") ";
        out.block([&] {
            if (mStyle == STYLE_SAFE_UNION) {
                out.sIf("lhs.getDiscriminator() != rhs.getDiscriminator()", [&] {
                    out << "return false;\n";
                }).endl();

                out << "switch (lhs.getDiscriminator()) {\n";
                out.indent();
            }

            for (const auto& field : *mFields) {
                if (mStyle == STYLE_SAFE_UNION) {
                    out << "case "
                        << fullName()
                        << "::hidl_discriminator::"
                        << field->name()
                        << ": ";

                    out.block([&] {
                        out << "return (lhs."
                        << field->name()
                        << "() == rhs."
                        << field->name()
                        << "());\n";
                    }).endl();
                } else {
                    out.sIf("lhs." + field->name() + " != rhs." + field->name(), [&] {
                        out << "return false;\n";
                    }).endl();
                }
            }

            if (mStyle == STYLE_SAFE_UNION) {
                out << "default: ";
                out.block([&] {
                       emitSafeUnionUnknownDiscriminatorError(out, "lhs.getDiscriminator()",
                                                              true /*fatal*/);
                   })
                    .endl();

                out.unindent();
                out << "}\n";
            }
            out << "return true;\n";
        }).endl().endl();

        out << "static inline bool operator!=("
            << getCppArgumentType() << " lhs, " << getCppArgumentType() << " rhs)";
        out.block([&] {
            out << "return !(lhs == rhs);\n";
        }).endl().endl();
    } else {
        out << "// operator== and operator!= are not generated for " << localName() << "\n\n";
    }
}

void CompoundType::emitPackageHwDeclarations(Formatter& out) const {
    if (needsEmbeddedReadWrite()) {
        out << "::android::status_t readEmbeddedFromParcel(\n";

        out.indent(2);

        out << "const " << fullName() << " &obj,\n"
            << "const ::android::hardware::Parcel &parcel,\n"
            << "size_t parentHandle,\n"
            << "size_t parentOffset);\n\n";

        out.unindent(2);

        out << "::android::status_t writeEmbeddedToParcel(\n";

        out.indent(2);

        out << "const " << fullName() << " &obj,\n"
            << "::android::hardware::Parcel *parcel,\n"
            << "size_t parentHandle,\n"
            << "size_t parentOffset);\n\n";

        out.unindent(2);
    }

    if(needsResolveReferences()) {
        out << "::android::status_t readEmbeddedReferenceFromParcel(\n";
        out.indent(2);
        out << fullName() << " *obj,\n"
            << "const ::android::hardware::Parcel &parcel,\n"
            << "size_t parentHandle, size_t parentOffset);\n\n";
        out.unindent(2);
        out << "::android::status_t writeEmbeddedReferenceToParcel(\n";
        out.indent(2);
        out << "const " << fullName() << " &obj,\n"
            << "::android::hardware::Parcel *,\n"
            << "size_t parentHandle, size_t parentOffset);\n\n";
        out.unindent(2);
    }
}

static void emitSafeUnionFieldConstructor(Formatter& out,
                                          const NamedReference<Type>* field,
                                          const std::string& initializerObjectName) {
    out << "new (&hidl_u."
        << field->name()
        << ") "
        << field->type().getCppStackType()
        << "("
        << initializerObjectName
        << ");\n";
}

static void emitSafeUnionSetterDefinition(Formatter& out,
                                          const NamedReference<Type>* field,
                                          const std::string& parameterName,
                                          bool usesMoveSemantics) {
    out.block([&] {
        out << "if (hidl_d != hidl_discriminator::"
            << field->name()
            << ") ";

        const std::string argumentName = usesMoveSemantics
                                         ? ("std::move(" + parameterName + ")")
                                         : parameterName;
        out.block([&] {
            out << "hidl_destructUnion();\n"
                << "::std::memset(&hidl_u, 0, sizeof(hidl_u));\n\n";

            emitSafeUnionFieldConstructor(out, field, argumentName);
            out << "hidl_d = hidl_discriminator::"
                << field->name()
                << ";\n";
        }).endl();

        out << "else if (&(hidl_u."
            << field->name()
            << ") != &"
            << parameterName
            << ") ";

        out.block([&] {
            out << "hidl_u."
                << field->name()
                << " = "
                << argumentName
                << ";\n";
        }).endl();
    }).endl().endl();
}

static void emitSafeUnionGetterDefinition(Formatter& out, const std::string& fieldName) {
    out.block([&] {
        out << "if (CC_UNLIKELY(hidl_d != hidl_discriminator::"
            << fieldName
            << ")) ";

        out.block([&] {
            out << "LOG_ALWAYS_FATAL(\"Bad safe_union access: safe_union has discriminator %\" "
                << "PRIu64 \" but discriminator %\" PRIu64 \" was accessed.\",\n";
            out.indent(2, [&] {
                out << "static_cast<uint64_t>(hidl_d), "
                    << "static_cast<uint64_t>(hidl_discriminator::" << fieldName << "));";
            }).endl();
        }).endl().endl();

        out << "return hidl_u."
            << fieldName
            << ";\n";
    }).endl().endl();
}

std::vector<CompoundType::SafeUnionEnumElement> CompoundType::getSafeUnionEnumElements(
    bool useCppTypeName) const {
    std::vector<SafeUnionEnumElement> elements;

    for (const auto& field : *mFields) {
        const std::string fieldTypeName = useCppTypeName
            ? field->type().getCppStackType(true /* specifyNamespaces */)
            : field->type().getJavaType(true /* forInitializer */);

        elements.push_back({field->name(), fieldTypeName});
    }

    return elements;
}

void CompoundType::emitSafeUnionCopyAndAssignDefinition(Formatter& out,
                                                        const std::string& parameterName,
                                                        bool isCopyConstructor,
                                                        bool usesMoveSemantics) const {
    out.block([&] {
        if (!isCopyConstructor) {
            out << "if (this == &"
            << parameterName
            << ") { return *this; }\n\n";
        }

        out << "switch ("
            << parameterName
            << ".hidl_d) ";

        out.block([&] {
            for (const auto& field : *mFields) {
                const std::string parameterFieldName = (parameterName + ".hidl_u." +
                                                        field->name());

                const std::string argumentName = usesMoveSemantics
                                                 ? ("std::move(" + parameterFieldName + ")")
                                                 : parameterFieldName;

                out << "case hidl_discriminator::"
                    << field->name()
                    << ": ";

                if (isCopyConstructor) {
                    out.block([&] {
                        emitSafeUnionFieldConstructor(out, field, argumentName);
                        out << "break;\n";
                    }).endl();
                } else {
                    out.block([&] {
                        out << field->name()
                            << "("
                            << argumentName
                            << ");\n"
                            << "break;\n";
                    }).endl();
                }
            }

            out << "default: ";
            out.block([&] {
                   emitSafeUnionUnknownDiscriminatorError(out, parameterName + ".hidl_d",
                                                          true /*fatal*/);
               })
                .endl();
        }).endl();

        if (isCopyConstructor) {
            out << "\nhidl_d = "
                << parameterName
                << ".hidl_d;\n";
        } else {
            out << "return *this;\n";
        }
    }).endl().endl();
}

void CompoundType::emitSafeUnionTypeConstructors(Formatter& out) const {

    // Default constructor
    out << fullName()
        << "::"
        << localName()
        << "() ";

    out.block([&] {
        out << "static_assert(offsetof("
            << fullName()
            << ", hidl_d) == 0, \"wrong offset\");\n";

        const CompoundLayout layout = getCompoundAlignmentAndSize();

        if (!containsPointer()) {
            out << "static_assert(offsetof(" << fullName()
                << ", hidl_u) == " << layout.innerStruct.offset << ", \"wrong offset\");\n";
        }

        out.endl();

        out << "::std::memset(&hidl_u, 0, sizeof(hidl_u));\n";

        // union itself is zero'd when set
        // padding after descriminator
        size_t dpad = layout.innerStruct.offset - layout.discriminator.size;
        emitPaddingZero(out, layout.discriminator.size /*offset*/, dpad /*size*/);

        size_t innerStructEnd = layout.innerStruct.offset + layout.innerStruct.size;
        // final padding of the struct
        size_t fpad = layout.overall.size - innerStructEnd;
        emitPaddingZero(out, innerStructEnd /*offset*/, fpad /*size*/);

        out.endl();

        CHECK(!mFields->empty());
        out << "hidl_d = hidl_discriminator::" << mFields->at(0)->name() << ";\n";
        emitSafeUnionFieldConstructor(out, mFields->at(0), "");
    }).endl().endl();

    // Destructor
    out << fullName()
        << "::~"
        << localName()
        << "() ";

    out.block([&] {
        out << "hidl_destructUnion();\n";
    }).endl().endl();

    // Move constructor
    out << fullName() << "::" << localName() << "(" << localName() << "&& other) : " << fullName()
        << "() ";

    emitSafeUnionCopyAndAssignDefinition(
            out, "other", true /* isCopyConstructor */, true /* usesMoveSemantics */);

    // Copy constructor
    out << fullName() << "::" << localName() << "(const " << localName()
        << "& other) : " << fullName() << "() ";

    emitSafeUnionCopyAndAssignDefinition(
        out, "other", true /* isCopyConstructor */, false /* usesMoveSemantics */);

    // Move assignment operator
    out << fullName()
        << "& ("
        << fullName()
        << "::operator=)("
        << localName()
        << "&& other) ";

    emitSafeUnionCopyAndAssignDefinition(
            out, "other", false /* isCopyConstructor */, true /* usesMoveSemantics */);

    // Copy assignment operator
    out << fullName()
        << "& ("
        << fullName()
        << "::operator=)(const "
        << localName()
        << "& other) ";

    emitSafeUnionCopyAndAssignDefinition(
            out, "other", false /* isCopyConstructor */, false /* usesMoveSemantics */);
}

void CompoundType::emitSafeUnionTypeDefinitions(Formatter& out) const {
    emitSafeUnionTypeConstructors(out);

    out << "void "
        << fullName()
        << "::hidl_destructUnion() ";

    out.block([&] {
        out << "switch (hidl_d) ";
        out.block([&] {

            for (const auto& field : *mFields) {
                out << "case hidl_discriminator::"
                    << field->name()
                    << ": ";

                out.block([&] {
                    out << "::android::hardware::details::destructElement(&(hidl_u."
                        << field->name()
                        << "));\n"
                        << "break;\n";
                }).endl();
            }

            out << "default: ";
            out.block(
                   [&] { emitSafeUnionUnknownDiscriminatorError(out, "hidl_d", true /*fatal*/); })
                .endl();
        }).endl().endl();
    }).endl().endl();

    for (const NamedReference<Type>* field : *mFields) {
        // Setter (copy)
        out << "void "
            << fullName()
            << "::"
            << field->name()
            << "("
            << field->type().getCppArgumentType()
            << " o) ";

        emitSafeUnionSetterDefinition(out, field, "o", false /* usesMoveSemantics */);

        if (field->type().resolveToScalarType() == nullptr) {
            // Setter (move)
            out << "void "
                << fullName()
                << "::"
                << field->name()
                << "("
                << field->type().getCppStackType()
                << "&& o) ";

            emitSafeUnionSetterDefinition(out, field, "o", true /* usesMoveSemantics */);
        }

        // Getter (mutable)
        out << field->type().getCppStackType()
            << "& ("
            << fullName()
            << "::"
            << field->name()
            << ")() ";

        emitSafeUnionGetterDefinition(out, field->name());

        // Getter (immutable)
        out << field->type().getCppArgumentType()
            << " ("
            << fullName()
            << "::"
            << field->name()
            << ")() const ";

        emitSafeUnionGetterDefinition(out, field->name());
    }

    // Trivial constructor/destructor for internal union
    out << fullName() << "::hidl_union::hidl_union() {}\n\n"
        << fullName() << "::hidl_union::~hidl_union() {}\n\n";

    // Utility method
    out << fullName() << "::hidl_discriminator ("
        << fullName() << "::getDiscriminator)() const ";

    out.block([&] {
        out << "return hidl_d;\n";
    }).endl().endl();
}

void CompoundType::emitTypeDefinitions(Formatter& out, const std::string& prefix) const {
    std::string space = prefix.empty() ? "" : (prefix + "::");
    Scope::emitTypeDefinitions(out, space + localName());

    if (needsEmbeddedReadWrite()) {
        emitStructReaderWriter(out, prefix, true /* isReader */);
        emitStructReaderWriter(out, prefix, false /* isReader */);
    }

    if (needsResolveReferences()) {
        emitResolveReferenceDef(out, prefix, true /* isReader */);
        emitResolveReferenceDef(out, prefix, false /* isReader */);
    }

    if (mStyle == STYLE_SAFE_UNION) {
        emitSafeUnionTypeDefinitions(out);
    }
}

static void emitJavaSafeUnionUnknownDiscriminatorError(Formatter& out, bool fatal) {
    out << "throw new ";

    if (fatal) {
        out << "Error";
    } else {
        out << "IllegalStateException";
    }

    out << "(\"Unknown union discriminator (value: \" + hidl_d + \").\");\n";
}

void CompoundType::emitJavaTypeDeclarations(Formatter& out, bool atTopLevel) const {
    out << "public final ";

    if (!atTopLevel) {
        out << "static ";
    }

    out << "class "
        << localName()
        << " {\n";

    out.indent();

    Scope::emitJavaTypeDeclarations(out, false /* atTopLevel */);

    if (mStyle == STYLE_SAFE_UNION) {
        out << "public " << localName() << "() ";
        out.block([&] {
            CHECK(!mFields->empty());
            mFields->at(0)->type().emitJavaFieldDefaultInitialValue(out, "hidl_o");
        }).endl().endl();

        const std::string discriminatorStorageType = (
                getUnionDiscriminatorType()->getJavaType(false));

        out << "public static final class hidl_discriminator ";
        out.block([&] {
            const auto elements = getSafeUnionEnumElements(false /* useCppTypeName */);
            for (size_t idx = 0; idx < elements.size(); idx++) {
                out << "public static final "
                    << discriminatorStorageType
                    << " "
                    << elements[idx].fieldName
                    << " = "
                    << idx
                    << ";";

                if (!elements[idx].fieldTypeName.empty()) {
                    out << "  // "
                        << elements[idx].fieldTypeName;
                }
                out << "\n";
            }

            out << "\n"
                << "public static final String getName("
                << discriminatorStorageType
                << " value) ";

            out.block([&] {
                out << "switch (value) ";
                out.block([&] {
                    for (size_t idx = 0; idx < elements.size(); idx++) {
                        out << "case "
                            << idx
                            << ": { return \""
                            << elements[idx].fieldName
                            << "\"; }\n";
                    }
                    out << "default: { return \"Unknown\"; }\n";
                }).endl();
            }).endl().endl();

            out << "private hidl_discriminator() {}\n";
        }).endl().endl();

        out << "private " << discriminatorStorageType << " hidl_d = 0;\n";
        out << "private Object hidl_o = null;\n";

        for (const auto& field : *mFields) {
            // Setter
            out << "public void "
                << field->name()
                << "("
                << field->type().getJavaType(false)
                << " "
                << field->name()
                << ") ";

            out.block([&] {
                out << "hidl_d = hidl_discriminator."
                    << field->name()
                    << ";\n";

                out << "hidl_o = "
                    << field->name()
                    << ";\n";
            }).endl().endl();

            // Getter
            out << "public "
                << field->type().getJavaType(false)
                << " "
                << field->name()
                << "() ";

            out.block([&] {
                out << "if (hidl_d != hidl_discriminator."
                    << field->name()
                    << ") ";

                out.block([&] {
                    out << "String className = (hidl_o != null) ? "
                        << "hidl_o.getClass().getName() : \"null\";\n";

                    out << "throw new IllegalStateException(\n";
                    out.indent(2, [&] {
                        out << "\"Read access to inactive union components is disallowed. \" +\n"
                            << "\"Discriminator value is \" + hidl_d + \" (corresponding \" +\n"
                            << "\"to \" + hidl_discriminator.getName(hidl_d) + \"), and \" +\n"
                            << "\"hidl_o is of type \" + className + \".\");\n";
                    });
                }).endl();

                out << "if (hidl_o != null && !"
                    << field->type().getJavaTypeClass()
                    << ".class.isInstance(hidl_o)) ";

                out.block([&] {
                    out << "throw new Error(\"Union is in a corrupted state.\");\n";
                }).endl();

                out << "return ("
                    << field->type().getJavaTypeCast("hidl_o")
                    << ");\n";
            }).endl().endl();
        }

        out << "// Utility method\n"
            << "public "
            << discriminatorStorageType
            << " getDiscriminator() { return hidl_d; }\n\n";

    } else {
        for (const auto& field : *mFields) {
            field->emitDocComment(out);

            out << "public ";
            field->type().emitJavaFieldInitializer(out, field->name());
        }

        out << "\n";
    }

    ////////////////////////////////////////////////////////////////////////////

    if (canCheckEquality()) {
        out << "@Override\npublic final boolean equals(Object otherObject) ";
        out.block([&] {
            out.sIf("this == otherObject", [&] {
                out << "return true;\n";
            }).endl();
            out.sIf("otherObject == null", [&] {
                out << "return false;\n";
            }).endl();
            // Though class is final, we use getClass instead of instanceof to be explicit.
            out.sIf("otherObject.getClass() != " + fullJavaName() + ".class", [&] {
                out << "return false;\n";
            }).endl();
            out << fullJavaName() << " other = (" << fullJavaName() << ")otherObject;\n";

            if (mStyle == STYLE_SAFE_UNION) {
                out.sIf("this.hidl_d != other.hidl_d", [&] {
                    out << "return false;\n";
                }).endl();
                out.sIf("!android.os.HidlSupport.deepEquals(this.hidl_o, other.hidl_o)", [&] {
                    out << "return false;\n";
                }).endl();
            } else {
                for (const auto &field : *mFields) {
                    std::string condition = (field->type().isScalar() || field->type().isEnum())
                        ? "this." + field->name() + " != other." + field->name()
                        : ("!android.os.HidlSupport.deepEquals(this." + field->name()
                                + ", other." + field->name() + ")");
                    out.sIf(condition, [&] {
                        out << "return false;\n";
                    }).endl();
                }
            }
            out << "return true;\n";
        }).endl().endl();

        out << "@Override\npublic final int hashCode() ";
        out.block([&] {
            out << "return java.util.Objects.hash(\n";
            out.indent(2, [&] {
                if (mStyle == STYLE_SAFE_UNION) {
                    out << "android.os.HidlSupport.deepHashCode(this.hidl_o),\n"
                        << "java.util.Objects.hashCode(this.hidl_d)";
                } else {
                    out.join(mFields->begin(), mFields->end(), ", \n", [&] (const auto &field) {
                        out << "android.os.HidlSupport.deepHashCode(this." << field->name() << ")";
                    });
                }
            });
            out << ");\n";
        }).endl().endl();
    } else {
        out << "// equals() is not generated for " << localName() << "\n";
    }

    ////////////////////////////////////////////////////////////////////////////

    out << "@Override\npublic final String toString() ";
    out.block([&] {
        out << "java.lang.StringBuilder builder = new java.lang.StringBuilder();\n"
            << "builder.append(\"{\");\n";

        if (mStyle == STYLE_SAFE_UNION) {
            out << "switch (this.hidl_d) {\n";
            out.indent();
        }

        for (const auto &field : *mFields) {
            if (mStyle == STYLE_SAFE_UNION) {
                out << "case hidl_discriminator."
                    << field->name()
                    << ": ";

                out.block([&] {
                    out << "builder.append(\""
                        << "."
                        << field->name()
                        << " = \");\n";

                    field->type().emitJavaDump(out, "builder", "this." + field->name() + "()");
                    out << "break;\n";
                }).endl();
            }
            else {
                out << "builder.append(\"";
                if (field != *(mFields->begin())) {
                    out << ", ";
                }
                out << "." << field->name() << " = \");\n";
                field->type().emitJavaDump(out, "builder", "this." + field->name());
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] { emitJavaSafeUnionUnknownDiscriminatorError(out, true /*fatal*/); })
                .endl();

            out.unindent();
            out << "}\n";
        }

        out << "builder.append(\"}\");\nreturn builder.toString();\n";
    }).endl().endl();

    CompoundLayout layout = getCompoundAlignmentAndSize();

    ////////////////////////////////////////////////////////////////////////////

    out << "public final void readFromParcel(android.os.HwParcel parcel) {\n";
    out.indent();
    if (containsInterface()) {

        if (mStyle == STYLE_SAFE_UNION) {
            out << "hidl_d = ";
            getUnionDiscriminatorType()->emitJavaReaderWriter(
                    out, "parcel", "hidl_d", true);

            out << "switch (hidl_d) {\n";
            out.indent();
        }

        for (const auto& field : *mFields) {
            if (mStyle == STYLE_SAFE_UNION) {
                out << "case hidl_discriminator."
                    << field->name()
                    << ": ";

                out.block([&] {
                    out << "hidl_o = ";
                    field->type().emitJavaReaderWriter(out, "parcel", "hidl_o", true);

                    out << "break;\n";
                }).endl();
            } else {
                out << field->name() << " = ";
                field->type().emitJavaReaderWriter(out, "parcel", field->name(), true);
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] { emitJavaSafeUnionUnknownDiscriminatorError(out, false /*fatal*/); })
                .endl();

            out.unindent();
            out << "}\n";
        }
    } else {
        out << "android.os.HwBlob blob = parcel.readBuffer(";
        out << layout.overall.size << " /* size */);\n";
        out << "readEmbeddedFromParcel(parcel, blob, 0 /* parentOffset */);\n";
    }
    out.unindent();
    out << "}\n\n";

    ////////////////////////////////////////////////////////////////////////////

    size_t vecAlign, vecSize;
    VectorType::getAlignmentAndSizeStatic(&vecAlign, &vecSize);

    out << "public static final java.util.ArrayList<" << localName()
        << "> readVectorFromParcel(android.os.HwParcel parcel) {\n";
    out.indent();

    out << "java.util.ArrayList<" << localName() << "> _hidl_vec = new java.util.ArrayList();\n";

    if (containsInterface()) {
        out << "int size = parcel.readInt32();\n";
        out << "for(int i = 0 ; i < size; i ++) {\n";
        out.indent();
        out << fullJavaName() << " tmp = ";
        emitJavaReaderWriter(out, "parcel", "tmp", true);
        out << "_hidl_vec.add(tmp);\n";
        out.unindent();
        out << "}\n";
    } else {
        out << "android.os.HwBlob _hidl_blob = parcel.readBuffer(";
        out << vecSize << " /* sizeof hidl_vec<T> */);\n\n";

        VectorType::EmitJavaFieldReaderWriterForElementType(out, 0 /* depth */, this, "parcel",
                                                            "_hidl_blob", "_hidl_vec", "0",
                                                            true /* isReader */);
    }
    out << "\nreturn _hidl_vec;\n";
    out.unindent();
    out << "}\n\n";
    ////////////////////////////////////////////////////////////////////////////
    if (containsInterface()) {
        out << "// readEmbeddedFromParcel is not generated()\n";
    } else {
        out << "public final void readEmbeddedFromParcel(\n";
        out.indent(2);
        out << "android.os.HwParcel parcel, android.os.HwBlob _hidl_blob, long _hidl_offset) {\n";
        out.unindent();

        if (mStyle == STYLE_SAFE_UNION) {
            getUnionDiscriminatorType()->emitJavaFieldReaderWriter(
                out, 0 /* depth */, "parcel", "_hidl_blob", "hidl_d",
                "_hidl_offset + " + std::to_string(layout.discriminator.offset),
                true /* isReader */);

            out << "switch (this.hidl_d) {\n";
            out.indent();
        }

        size_t offset = layout.innerStruct.offset;
        for (const auto& field : *mFields) {

            if (mStyle == STYLE_SAFE_UNION) {
                out << "case hidl_discriminator."
                    << field->name()
                    << ": ";

                out.block([&] {
                    field->type().emitJavaFieldDefaultInitialValue(out, "hidl_o");
                    field->type().emitJavaFieldReaderWriter(
                        out, 0 /* depth */, "parcel", "_hidl_blob", "hidl_o",
                        "_hidl_offset + " + std::to_string(offset), true /* isReader */);

                    out << "break;\n";
                }).endl();
            } else {
                size_t fieldAlign, fieldSize;
                field->type().getAlignmentAndSize(&fieldAlign, &fieldSize);

                offset += Layout::getPad(offset, fieldAlign);
                field->type().emitJavaFieldReaderWriter(
                    out, 0 /* depth */, "parcel", "_hidl_blob", field->name(),
                    "_hidl_offset + " + std::to_string(offset), true /* isReader */);
                offset += fieldSize;
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] { emitJavaSafeUnionUnknownDiscriminatorError(out, false /*fatal*/); })
                .endl();

            out.unindent();
            out << "}\n";
        }
        out.unindent();
        out << "}\n\n";
    }

    ////////////////////////////////////////////////////////////////////////////

    out << "public final void writeToParcel(android.os.HwParcel parcel) {\n";
    out.indent();

    if (containsInterface()) {
        if (mStyle == STYLE_SAFE_UNION) {
            getUnionDiscriminatorType()->emitJavaReaderWriter(
                out, "parcel", "hidl_d", false);

            out << "switch (this.hidl_d) {\n";
            out.indent();
        }

        for (const auto& field : *mFields) {
            if (mStyle == STYLE_SAFE_UNION) {
                out << "case hidl_discriminator."
                    << field->name()
                    << ": ";

                out.block([&] {
                    field->type().emitJavaReaderWriter(out, "parcel", field->name() + "()", false);
                    out << "break;\n";
                }).endl();
            } else {
                field->type().emitJavaReaderWriter(out, "parcel", field->name(), false);
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] { emitJavaSafeUnionUnknownDiscriminatorError(out, true /*fatal*/); })
                .endl();

            out.unindent();
            out << "}\n";
        }
    } else {
        out << "android.os.HwBlob _hidl_blob = new android.os.HwBlob("
            << layout.overall.size
            << " /* size */);\n";

        out << "writeEmbeddedToBlob(_hidl_blob, 0 /* parentOffset */);\n"
            << "parcel.writeBuffer(_hidl_blob);\n";
    }
    out.unindent();
    out << "}\n\n";

    ////////////////////////////////////////////////////////////////////////////

    out << "public static final void writeVectorToParcel(\n";
    out.indent(2);
    out << "android.os.HwParcel parcel, java.util.ArrayList<" << localName() << "> _hidl_vec) {\n";
    out.unindent();

    if (containsInterface()) {
        out << "parcel.writeInt32(_hidl_vec.size());\n";
        out << "for(" << fullJavaName() << " tmp: _hidl_vec) ";
        out.block([&] {
            emitJavaReaderWriter(out, "parcel", "tmp", false);
        }).endl();
    } else {
        out << "android.os.HwBlob _hidl_blob = new android.os.HwBlob(" << vecSize
            << " /* sizeof(hidl_vec<T>) */);\n";

        VectorType::EmitJavaFieldReaderWriterForElementType(out, 0 /* depth */, this, "parcel",
                                                            "_hidl_blob", "_hidl_vec", "0",
                                                            false /* isReader */);

        out << "\nparcel.writeBuffer(_hidl_blob);\n";
    }
    out.unindent();
    out << "}\n\n";
    ////////////////////////////////////////////////////////////////////////////

    if (containsInterface()) {
        out << "// writeEmbeddedToBlob() is not generated\n";
    } else {
        out << "public final void writeEmbeddedToBlob(\n";
        out.indent(2);
        out << "android.os.HwBlob _hidl_blob, long _hidl_offset) {\n";
        out.unindent();

        if (mStyle == STYLE_SAFE_UNION) {
            getUnionDiscriminatorType()->emitJavaFieldReaderWriter(
                out, 0 /* depth */, "parcel", "_hidl_blob", "hidl_d",
                "_hidl_offset + " + std::to_string(layout.discriminator.offset),
                false /* isReader */);

            out << "switch (this.hidl_d) {\n";
            out.indent();
        }

        size_t offset = layout.innerStruct.offset;
        for (const auto& field : *mFields) {
            if (mStyle == STYLE_SAFE_UNION) {
                out << "case hidl_discriminator."
                    << field->name()
                    << ": ";

                out.block([&] {
                    field->type().emitJavaFieldReaderWriter(
                        out, 0 /* depth */, "parcel", "_hidl_blob", field->name() + "()",
                        "_hidl_offset + " + std::to_string(offset), false /* isReader */);

                    out << "break;\n";
                }).endl();
            } else {
                size_t fieldAlign, fieldSize;
                field->type().getAlignmentAndSize(&fieldAlign, &fieldSize);

                offset += Layout::getPad(offset, fieldAlign);
                field->type().emitJavaFieldReaderWriter(
                    out, 0 /* depth */, "parcel", "_hidl_blob", field->name(),
                    "_hidl_offset + " + std::to_string(offset), false /* isReader */);
                offset += fieldSize;
            }
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "default: ";
            out.block([&] { emitJavaSafeUnionUnknownDiscriminatorError(out, true /*fatal*/); })
                .endl();

            out.unindent();
            out << "}\n";
        }
        out.unindent();
        out << "}\n";
    }

    out.unindent();
    out << "};\n\n";
}

void CompoundType::emitStructReaderWriter(
        Formatter &out, const std::string &prefix, bool isReader) const {

    std::string space = prefix.empty() ? "" : (prefix + "::");

    out << "::android::status_t "
        << (isReader ? "readEmbeddedFromParcel"
                     : "writeEmbeddedToParcel")
        << "(\n";

    out.indent(2);

    const std::string name = "obj";
    if (isReader) {
        out << "const " << space << localName() << " &" << name << ",\n";
        out << "const ::android::hardware::Parcel &parcel,\n";
    } else {
        out << "const " << space << localName() << " &" << name << ",\n";
        out << "::android::hardware::Parcel *parcel,\n";
    }

    out << "size_t parentHandle,\n"
        << "size_t parentOffset)";

    out << " {\n";

    out.unindent(2);
    out.indent();

    out << "::android::status_t _hidl_err = ::android::OK;\n\n";

    if (mStyle == STYLE_SAFE_UNION) {
        out << "switch (" << name << ".getDiscriminator()) {\n";
        out.indent();
    }

    for (const auto &field : *mFields) {
        if (!field->type().needsEmbeddedReadWrite()) {
            continue;
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "case " << fullName() << "::hidl_discriminator::"
                << field->name() << ": {\n";
            out.indent();
        }

        const std::string fieldName = (mStyle == STYLE_SAFE_UNION)
                                        ? (name + "." + field->name() + "()")
                                        : (name + "." + field->name());

        const std::string fieldOffset = (mStyle == STYLE_SAFE_UNION)
                                        ? (name + ".hidl_getUnionOffset() " +
                                           "/* safe_union: union offset into struct */")
                                        : ("offsetof(" + fullName() + ", " + field->name() + ")");

        field->type().emitReaderWriterEmbedded(
                out,
                0 /* depth */,
                fieldName,
                field->name() /* sanitizedName */,
                false /* nameIsPointer */,
                "parcel",
                !isReader /* parcelObjIsPointer */,
                isReader,
                ErrorMode_Return,
                "parentHandle",
                "parentOffset + " + fieldOffset);

        if (mStyle == STYLE_SAFE_UNION) {
            out << "break;\n";
            out.unindent();
            out << "}\n";
        }
    }

    if (mStyle == STYLE_SAFE_UNION) {
        out << "default: { break; }\n";
        out.unindent();
        out << "}\n";
    }

    out << "return _hidl_err;\n";

    out.unindent();
    out << "}\n\n";
}

void CompoundType::emitResolveReferenceDef(Formatter& out, const std::string& prefix,
                                           bool isReader) const {
    out << "::android::status_t ";
    const std::string space(prefix.empty() ? "" : (prefix + "::"));

    bool useParent = false;
    for (const auto &field : *mFields) {
        if (field->type().useParentInEmitResolveReferencesEmbedded()) {
            useParent = true;
            break;
        }
    }

    std::string parentHandleName = useParent ? "parentHandle" : "/* parentHandle */";
    std::string parentOffsetName = useParent ? "parentOffset" : "/* parentOffset */";

    if (isReader) {
        out << "readEmbeddedReferenceFromParcel(\n";
        out.indent(2);
        out << space + localName() + " *obj,\n"
            << "const ::android::hardware::Parcel &parcel,\n"
            << "size_t " << parentHandleName << ", "
            << "size_t " << parentOffsetName << ")\n";
        out.unindent(2);
    } else {
        out << "writeEmbeddedReferenceToParcel(\n";
        out.indent(2);
        out << "const " << space + localName() + " &obj,\n"
            << "::android::hardware::Parcel *parcel,\n"
            << "size_t " << parentHandleName << ", "
            << "size_t " << parentOffsetName << ")\n";
        out.unindent(2);
    }

    out << " {\n";

    out.indent();

    out << "::android::status_t _hidl_err = ::android::OK;\n\n";

    const std::string nameDeref(isReader ? "obj->" : "obj.");
    // if not useParent, then parentName and offsetText
    // should not be used at all, then the #error should not be emitted.
    std::string error = useParent ? "" : "\n#error\n";

    if (mStyle == STYLE_SAFE_UNION) {
        out << "switch (" << nameDeref << "getDiscriminator()) {\n";
        out.indent();
    }

    for (const auto &field : *mFields) {
        if (!field->type().needsResolveReferences()) {
            continue;
        }

        if (mStyle == STYLE_SAFE_UNION) {
            out << "case " << fullName() << "::hidl_discriminator::"
                << field->name() << ": {\n";
            out.indent();
        }

        const std::string fieldName = (mStyle == STYLE_SAFE_UNION)
                                        ? (nameDeref + field->name() + "()")
                                        : (nameDeref + field->name());

        const std::string fieldOffset = (mStyle == STYLE_SAFE_UNION)
                                        ? (nameDeref + "hidl_getUnionOffset() " +
                                           "/* safe_union: union offset into struct */")
                                        : ("offsetof(" + fullName() + ", " + field->name() + ")");

        field->type().emitResolveReferencesEmbedded(
            out,
            0 /* depth */,
            fieldName,
            field->name() /* sanitizedName */,
            false,    // nameIsPointer
            "parcel", // const std::string &parcelObj,
            !isReader, // bool parcelObjIsPointer,
            isReader, // bool isReader,
            ErrorMode_Return,
            parentHandleName + error,
            parentOffsetName
                + " + "
                + fieldOffset
                + error);

        if (mStyle == STYLE_SAFE_UNION) {
            out << "break;\n";
            out.unindent();
            out << "}\n";
        }
    }

    if (mStyle == STYLE_SAFE_UNION) {
        out << "default: { _hidl_err = ::android::BAD_VALUE; break; }\n";
        out.unindent();
        out << "}\n";
    }

    out << "return _hidl_err;\n";

    out.unindent();
    out << "}\n\n";
}

bool CompoundType::needsEmbeddedReadWrite() const {
    if (mStyle == STYLE_UNION) {
        return false;
    }

    for (const auto &field : *mFields) {
        if (field->type().needsEmbeddedReadWrite()) {
            return true;
        }
    }

    return false;
}

bool CompoundType::deepNeedsResolveReferences(std::unordered_set<const Type*>* visited) const {
    if (mStyle == STYLE_UNION) {
        return false;
    }

    for (const auto &field : *mFields) {
        if (field->type().needsResolveReferences(visited)) {
            return true;
        }
    }

    return Scope::deepNeedsResolveReferences(visited);
}

bool CompoundType::resultNeedsDeref() const {
    return !containsInterface() ;
}

void CompoundType::emitVtsTypeDeclarations(Formatter& out) const {
    out << "name: \"" << fullName() << "\"\n";
    out << "type: " << getVtsType() << "\n";

    // Emit declaration for each subtype.
    for (const auto &type : getSubTypes()) {
        switch (mStyle) {
            case STYLE_STRUCT:
            {
                out << "sub_struct: {\n";
                break;
            }
            case STYLE_UNION:
            {
                out << "sub_union: {\n";
                break;
            }
            case STYLE_SAFE_UNION:
            {
                out << "sub_safe_union: {\n";
                break;
            }
            default:
            {
                CHECK(!"Should not be here");
            }
        }
        out.indent();
        type->emitVtsTypeDeclarations(out);
        out.unindent();
        out << "}\n";
    }

    // Emit declaration for each field.
    for (const auto &field : *mFields) {
        switch (mStyle) {
            case STYLE_STRUCT:
            {
                out << "struct_value: {\n";
                break;
            }
            case STYLE_UNION:
            {
                out << "union_value: {\n";
                break;
            }
            case STYLE_SAFE_UNION:
            {
                out << "safe_union_value: {\n";
                break;
            }
            default:
            {
                CHECK(!"Should not be here");
            }
        }
        out.indent();
        out << "name: \"" << field->name() << "\"\n";
        field->type().emitVtsAttributeType(out);
        out.unindent();
        out << "}\n";
    }
}

void CompoundType::emitVtsAttributeType(Formatter& out) const {
    out << "type: " << getVtsType() << "\n";
    out << "predefined_type: \"" << fullName() << "\"\n";
}

bool CompoundType::deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const {
    if (mStyle == STYLE_UNION) {
        return false;
    }

    for (const auto* field : *mFields) {
        if (!field->get()->isJavaCompatible(visited)) {
            return false;
        }
    }

    return Scope::deepIsJavaCompatible(visited);
}

bool CompoundType::deepContainsPointer(std::unordered_set<const Type*>* visited) const {
    for (const auto* field : *mFields) {
        if (field->get()->containsPointer(visited)) {
            return true;
        }
    }

    return Scope::deepContainsPointer(visited);
}

void CompoundType::getAlignmentAndSize(size_t *align, size_t *size) const {
    CompoundLayout layout = getCompoundAlignmentAndSize();
    *align = layout.overall.align;
    *size = layout.overall.size;
}

CompoundType::CompoundLayout CompoundType::getCompoundAlignmentAndSize() const {
    CompoundLayout compoundLayout;

    // Local aliases for convenience
    Layout& overall = compoundLayout.overall;
    Layout& innerStruct = compoundLayout.innerStruct;
    Layout& discriminator = compoundLayout.discriminator;

    if (mStyle == STYLE_SAFE_UNION) {
        getUnionDiscriminatorType()->getAlignmentAndSize(
            &(discriminator.align), &(discriminator.size));

        innerStruct.offset = discriminator.size;
    }

    for (const auto &field : *mFields) {

        // Each field is aligned according to its alignment requirement.
        // The surrounding structure's alignment is the maximum of its
        // fields' aligments.
        size_t fieldAlign, fieldSize;
        field->type().getAlignmentAndSize(&fieldAlign, &fieldSize);
        size_t lPad = Layout::getPad(innerStruct.size, fieldAlign);

        innerStruct.size = (mStyle == STYLE_STRUCT)
                            ? (innerStruct.size + lPad + fieldSize)
                            : std::max(innerStruct.size, fieldSize);

        innerStruct.align = std::max(innerStruct.align, fieldAlign);
    }

    // Pad the inner structure's size
    innerStruct.size += Layout::getPad(innerStruct.size,
                                       innerStruct.align);

    // Compute its final offset
    innerStruct.offset += Layout::getPad(innerStruct.offset,
                                         innerStruct.align);

    // An empty struct/union still occupies a byte of space in C++.
    if (innerStruct.size == 0) {
        innerStruct.size = 1;
    }

    overall.size = innerStruct.offset + innerStruct.size;

    // Pad the overall structure's size
    overall.align = std::max(innerStruct.align, discriminator.align);
    overall.size += Layout::getPad(overall.size, overall.align);

    if (mStyle != STYLE_SAFE_UNION) {
        CHECK(overall.offset == innerStruct.offset) << overall.offset << " " << innerStruct.offset;
        CHECK(overall.align == innerStruct.align) << overall.align << " " << innerStruct.align;
        CHECK(overall.size == innerStruct.size) << overall.size << " " << innerStruct.size;
    }

    return compoundLayout;
}

void CompoundType::emitPaddingZero(Formatter& out, size_t offset, size_t size) const {
    if (size > 0) {
        out << "::std::memset(reinterpret_cast<uint8_t*>(this) + " << offset << ", 0, " << size
            << ");\n";
    } else {
        out << "// no padding to zero starting at offset " << offset << "\n";
    }
}

std::unique_ptr<ScalarType> CompoundType::getUnionDiscriminatorType() const {
    static const std::vector<std::pair<int, ScalarType::Kind> > scalars {
        {8, ScalarType::Kind::KIND_UINT8},
        {16, ScalarType::Kind::KIND_UINT16},
        {32, ScalarType::Kind::KIND_UINT32},
    };

    size_t numFields = mFields->size();
    auto kind = ScalarType::Kind::KIND_UINT64;

    for (const auto& scalar : scalars) {
        if (numFields <= (1ULL << scalar.first)) {
            kind = scalar.second; break;
        }
    }

    return std::unique_ptr<ScalarType>(new ScalarType(kind, nullptr));
}

size_t CompoundType::Layout::getPad(size_t offset, size_t align) {
    size_t remainder = offset % align;
    return (remainder > 0) ? (align - remainder) : 0;
}

}  // namespace android

