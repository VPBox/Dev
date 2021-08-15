/*
 * Copyright (C) 2017 The Android Open Source Project
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
#include "HidlTypeAssertion.h"
#include "Interface.h"
#include "Method.h"
#include "Reference.h"
#include "ScalarType.h"
#include "Scope.h"

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <hidl-util/StringHelper.h>
#include <algorithm>
#include <string>
#include <vector>

namespace android {

void AST::generateCppAdapterHeader(Formatter& out) const {
    const std::string klassName = AST::isInterface() ? getInterface()->getAdapterName() : "Atypes";
    const std::string guard = makeHeaderGuard(klassName, true /* indicateGenerated */);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    if (AST::isInterface()) {
        generateCppPackageInclude(out, mPackage, getInterface()->localName());

        enterLeaveNamespace(out, true /* enter */);
        out.endl();

        const std::string mockName = getInterface()->fqName().cppName();

        out << "class " << klassName << " : public " << mockName << " ";
        out.block([&] {
            out << "public:\n";
            out << "typedef " << mockName << " Pure;\n";

            out << klassName << "(const ::android::sp<" << mockName << ">& impl);\n";

            generateMethods(out, [&](const Method* method, const Interface* /* interface */) {
                if (method->isHidlReserved()) {
                    return;
                }

                out << "virtual ";
                method->generateCppSignature(out);
                out << " override;\n";
            });
            out << "private:\n";
            out << "::android::sp<" << mockName << "> mImpl;\n";

        }) << ";\n\n";

        enterLeaveNamespace(out, false /* enter */);
    } else {
        out << "// no adapters for types.hal\n";
    }

    out << "#endif // " << guard << "\n";
}

void AST::generateCppAdapterSource(Formatter& out) const {
    const std::string klassName = AST::isInterface() ? getInterface()->getAdapterName() : "Atypes";

    generateCppPackageInclude(out, mPackage, klassName);

    if (AST::isInterface()) {
        out << "#include <hidladapter/HidlBinderAdapter.h>\n";
        generateCppPackageInclude(out, mPackage, getInterface()->localName());

        std::set<FQName> allImportedNames;
        getAllImportedNames(&allImportedNames);
        for (const auto& item : allImportedNames) {
            if (item.name() == "types") {
                continue;
            }
            generateCppPackageInclude(out, item, item.getInterfaceAdapterName());
        }

        out.endl();

        enterLeaveNamespace(out, true /* enter */);
        out.endl();

        const std::string mockName = getInterface()->fqName().cppName();

        out << klassName << "::" << klassName << "(const ::android::sp<" << mockName
            << ">& impl) : mImpl(impl) {}";

        generateMethods(out, [&](const Method* method, const Interface* /* interface */) {
            generateAdapterMethod(out, method);
        });

        enterLeaveNamespace(out, false /* enter */);
        out.endl();
    } else {
        out << "// no adapters for types.hal\n";
    }
}

void AST::generateAdapterMethod(Formatter& out, const Method* method) const {
    if (method->isHidlReserved()) {
        return;
    }

    const auto adapt = [](Formatter& out, const std::string& var, const Type* type) {
        if (!type->isInterface()) {
            out << var;
            return;
        }

        const Interface* interface = static_cast<const Interface*>(type);
        out << "static_cast<::android::sp<" << interface->fqName().cppName() << ">>("
            << interface->fqName().cppName() << "::castFrom("
            << "::android::hardware::details::adaptWithDefault("
            << "static_cast<::android::sp<" << interface->fqName().cppName() << ">>(" << var
            << "), [&] { return new " << interface->fqName().getInterfaceAdapterFqName().cppName()
            << "(" << var << "); })))";
    };

    const std::string klassName = getInterface()->getAdapterName();

    method->generateCppSignature(out, klassName);
    out.block([&] {
         bool hasCallback = !method->canElideCallback() && !method->results().empty();

         if (hasCallback) {
             out << method->name() << "_cb _hidl_cb_wrapped = [&](";
             method->emitCppResultSignature(out);
             out << ") ";
             out.block([&] {
                 out << "return _hidl_cb(\n";
                 out.indent([&]() {
                     out.join(method->results().begin(), method->results().end(), ",\n",
                              [&](auto arg) { adapt(out, arg->name(), arg->get()); });
                 });
                 out << ");\n";
             });
             out << ";\n";
         }

         out << "auto _hidl_out = mImpl->" << method->name() << "(\n";
         out.indent([&]() {
             out.join(method->args().begin(), method->args().end(), ",\n",
                      [&](auto arg) { adapt(out, arg->name(), arg->get()); });
             if (hasCallback) {
                 if (!method->args().empty()) {
                     out << ",\n";
                 }
                 out << "_hidl_cb_wrapped";
             }
         });
         out << ");\n";

         const auto elidedCallback = method->canElideCallback();
         if (elidedCallback) {
             out.sIf("!_hidl_out.isOkUnchecked()", [&] { out << "return _hidl_out;\n"; });
             out << "return ";
             adapt(out, "_hidl_out", elidedCallback->get());
             out << ";\n";
         } else {
             out << "return _hidl_out;\n";
         }
     }).endl();
}

}  // namespace android
