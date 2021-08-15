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

#include "AST.h"

#include "Coordinator.h"
#include "EnumType.h"
#include "Interface.h"
#include "Method.h"
#include "Reference.h"
#include "ScalarType.h"
#include "Scope.h"

#include <algorithm>
#include <hidl-util/Formatter.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include <set>

namespace android {

void AST::generateFetchSymbol(Formatter &out, const std::string& ifaceName) const {
    out << "HIDL_FETCH_" << ifaceName;
}

void AST::generateStubImplMethod(Formatter& out, const std::string& className,
                                 const Method* method) const {
    // ignore HIDL reserved methods -- implemented in IFoo already.
    if (method->isHidlReserved()) {
        return;
    }

    method->generateCppSignature(out, className, false /* specifyNamespaces */);

    out << " {\n";

    out.indent();
    out << "// TODO implement\n";

    const NamedReference<Type>* elidedReturn = method->canElideCallback();

    if (elidedReturn == nullptr) {
        out << "return Void();\n";
    } else {
        out << "return "
            << elidedReturn->type().getCppResultType()
            << " {};\n";
    }

    out.unindent();

    out << "}\n\n";

    return;
}

void AST::generateCppImplHeader(Formatter& out) const {
    if (!AST::isInterface()) {
        // types.hal does not get a stub header.
        return;
    }

    const Interface* iface = mRootScope.getInterface();
    const std::string baseName = iface->getBaseName();

    out << "// FIXME: your file license if you have one\n\n";
    out << "#pragma once\n\n";

    generateCppPackageInclude(out, mPackage, iface->localName());

    out << "#include <hidl/MQDescriptor.h>\n";
    out << "#include <hidl/Status.h>\n\n";

    enterLeaveNamespace(out, true /* enter */);
    out << "namespace implementation {\n\n";

    out << "using ::android::hardware::hidl_array;\n";
    out << "using ::android::hardware::hidl_memory;\n";
    out << "using ::android::hardware::hidl_string;\n";
    out << "using ::android::hardware::hidl_vec;\n";
    out << "using ::android::hardware::Return;\n";
    out << "using ::android::hardware::Void;\n";
    out << "using ::android::sp;\n";

    out << "\n";

    out << "struct "
        << baseName
        << " : public "
        << iface->localName()
        << " {\n";

    out.indent();

    generateMethods(out, [&](const Method* method, const Interface*) {
        // ignore HIDL reserved methods -- implemented in IFoo already.
        if (method->isHidlReserved()) {
            return;
        }
        method->generateCppSignature(out, "" /* className */,
                false /* specifyNamespaces */);
        out << " override;\n";
    });

    out.unindent();

    out << "};\n\n";

    out << "// FIXME: most likely delete, this is only for passthrough implementations\n"
        << "// extern \"C\" "
        << iface->localName()
        << "* ";
    generateFetchSymbol(out, iface->localName());
    out << "(const char* name);\n\n";

    out << "}  // namespace implementation\n";
    enterLeaveNamespace(out, false /* leave */);
}

void AST::generateCppImplSource(Formatter& out) const {
    if (!AST::isInterface()) {
        // types.hal does not get a stub header.
        return;
    }

    const Interface* iface = mRootScope.getInterface();
    const std::string baseName = iface->getBaseName();

    out << "// FIXME: your file license if you have one\n\n";
    out << "#include \"" << baseName << ".h\"\n\n";

    enterLeaveNamespace(out, true /* enter */);
    out << "namespace implementation {\n\n";

    generateMethods(out, [&](const Method* method, const Interface*) {
        generateStubImplMethod(out, baseName, method);
    });

    out.setLinePrefix("//");
    out << iface->localName()
        << "* ";
    generateFetchSymbol(out, iface->localName());
    out << "(const char* /* name */) {\n";
    out.indent();
    out << "return new " << baseName << "();\n";
    out.unindent();
    out << "}\n\n";
    out.unsetLinePrefix();

    out << "}  // namespace implementation\n";
    enterLeaveNamespace(out, false /* leave */);
}

}  // namespace android
