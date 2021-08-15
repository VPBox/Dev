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
#include "Interface.h"
#include "Method.h"
#include "Reference.h"
#include "Scope.h"

#include <hidl-util/Formatter.h>
#include <android-base/logging.h>

namespace android {

void AST::emitJavaReaderWriter(Formatter& out, const std::string& parcelObj,
                               const NamedReference<Type>* arg, bool isReader,
                               bool addPrefixToName) const {
    if (isReader) {
        out << arg->type().getJavaType()
            << " "
            << (addPrefixToName ? "_hidl_out_" : "")
            << arg->name()
            << " = ";
    }

    arg->type().emitJavaReaderWriter(out, parcelObj,
            (addPrefixToName ? "_hidl_out_" : "") + arg->name(),
            isReader);
}

void AST::generateJavaTypes(Formatter& out, const std::string& limitToType) const {
    // Splits types.hal up into one java file per declared type.
    CHECK(!limitToType.empty()) << getFilename();

    for (const auto& type : mRootScope.getSubTypes()) {
        std::string typeName = type->localName();

        if (type->isTypeDef()) continue;
        if (typeName != limitToType) continue;

        std::vector<std::string> packageComponents;
        getPackageAndVersionComponents(
                &packageComponents, true /* cpp_compatible */);

        out << "package " << mPackage.javaPackage() << ";\n\n\n";

        type->emitJavaTypeDeclarations(out, true /* atTopLevel */);
        return;
    }

    CHECK(false) << "generateJavaTypes could not find limitToType type";
}

void emitGetService(
        Formatter& out,
        const std::string& ifaceName,
        const std::string& fqName,
        bool isRetry) {
    if (isRetry) {
        DocComment(
                "This will invoke the equivalent of the C++ getService(std::string) if retry is\n"
                "true or tryGetService(std::string) if retry is false. If the service is\n"
                "available on the device and retry is true, this will wait for the service to\n"
                "start. Otherwise, it will return immediately even if the service is null.")
                .emit(out);
    } else {
        DocComment(
                "Warning: this will not wait for the interface to come up if it hasn't yet\n"
                "started. See getService(String,boolean) instead.")
                .emit(out);
    }
    out << "public static "
        << ifaceName
        << " getService(String serviceName";
    if (isRetry) {
        out << ", boolean retry";
    }
    out << ") throws android.os.RemoteException ";
    out.block([&] {
        out << "return "
            << ifaceName
            << ".asInterface(android.os.HwBinder.getService(\""
            << fqName
            << "\", serviceName";
        if (isRetry) {
            out << ", retry";
        }
        out << "));\n";
    }).endl().endl();

    if (isRetry) {
        DocComment("Calls getService(\"default\",retry).").emit(out);
    } else {
        DocComment(
                "Warning: this will not wait for the interface to come up if it hasn't yet "
                "started. See getService(String,boolean) instead.")
                .emit(out);
    }
    out << "public static "
        << ifaceName
        << " getService(";
    if (isRetry) {
        out << "boolean retry";
    }
    out << ") throws android.os.RemoteException ";
    out.block([&] {
        out << "return getService(\"default\"";
        if (isRetry) {
            out << ", retry";
        }
        out <<");\n";
    }).endl().endl();
}

void AST::generateJava(Formatter& out, const std::string& limitToType) const {
    CHECK(isJavaCompatible()) << getFilename();

    if (!AST::isInterface()) {
        generateJavaTypes(out, limitToType);
        return;
    }

    const Interface* iface = mRootScope.getInterface();
    const std::string ifaceName = iface->localName();
    const std::string baseName = iface->getBaseName();

    std::vector<std::string> packageComponents;
    getPackageAndVersionComponents(
            &packageComponents, true /* cpp_compatible */);

    out << "package " << mPackage.javaPackage() << ";\n\n";

    out.setNamespace(mPackage.javaPackage() + ".");

    const Interface *superType = iface->superType();

    iface->emitDocComment(out);

    out << "public interface " << ifaceName << " extends ";

    if (superType != nullptr) {
        out << superType->fullJavaName();
    } else {
        out << "android.os.IHwInterface";
    }

    out << " {\n";
    out.indent();

    DocComment("Fully-qualified interface name for this interface.").emit(out);
    out << "public static final String kInterfaceName = \""
        << mPackage.string()
        << "::"
        << ifaceName
        << "\";\n\n";

    DocComment("Does a checked conversion from a binder to this class.").emit(out);
    out << "/* package private */ static "
        << ifaceName
        << " asInterface(android.os.IHwBinder binder) {\n";

    out.indent();

    out << "if (binder == null) {\n";
    out.indent();
    out << "return null;\n";
    out.unindent();
    out << "}\n\n";

    out << "android.os.IHwInterface iface =\n";
    out.indent();
    out.indent();
    out << "binder.queryLocalInterface(kInterfaceName);\n\n";
    out.unindent();
    out.unindent();

    out << "if ((iface != null) && (iface instanceof "
        << ifaceName
        << ")) {\n";

    out.indent();
    out << "return (" << ifaceName << ")iface;\n";
    out.unindent();
    out << "}\n\n";

    out << ifaceName << " proxy = new " << ifaceName << ".Proxy(binder);\n\n";
    out << "try {\n";
    out.indent();
    out << "for (String descriptor : proxy.interfaceChain()) {\n";
    out.indent();
    out << "if (descriptor.equals(kInterfaceName)) {\n";
    out.indent();
    out << "return proxy;\n";
    out.unindent();
    out << "}\n";
    out.unindent();
    out << "}\n";
    out.unindent();
    out << "} catch (android.os.RemoteException e) {\n";
    out.indent();
    out.unindent();
    out << "}\n\n";

    out << "return null;\n";

    out.unindent();
    out << "}\n\n";

    DocComment("Does a checked conversion from any interface to this class.").emit(out);
    out << "public static "
        << ifaceName
        << " castFrom(android.os.IHwInterface iface) {\n";
    out.indent();

    out << "return (iface == null) ? null : "
        << ifaceName
        << ".asInterface(iface.asBinder());\n";

    out.unindent();
    out << "}\n\n";

    out << "@Override\npublic android.os.IHwBinder asBinder();\n\n";

    emitGetService(out, ifaceName, iface->fqName().string(), true /* isRetry */);
    emitGetService(out, ifaceName, iface->fqName().string(), false /* isRetry */);

    iface->emitJavaTypeDeclarations(out, false /* atTopLevel */);

    for (const auto &method : iface->methods()) {
        const bool returnsValue = !method->results().empty();
        const bool needsCallback = method->results().size() > 1;

        if (needsCallback) {
            out << "\n@java.lang.FunctionalInterface\npublic interface " << method->name()
                << "Callback {\n";

            out.indent();

            out << "public void onValues(";
            method->emitJavaResultSignature(out);
            out << ");\n";

            out.unindent();
            out << "}\n\n";
        }

        method->emitDocComment(out);

        if (returnsValue && !needsCallback) {
            out << method->results()[0]->type().getJavaType();
        } else {
            out << "void";
        }

        out << " "
            << method->name()
            << "(";
        method->emitJavaArgSignature(out);

        if (needsCallback) {
            if (!method->args().empty()) {
                out << ", ";
            }

            out << method->name()
                << "Callback _hidl_cb";
        }

        out << ")\n";
        out.indent();
        out << "throws android.os.RemoteException;\n";
        out.unindent();
    }

    out << "\npublic static final class Proxy implements "
        << ifaceName
        << " {\n";

    out.indent();

    out << "private android.os.IHwBinder mRemote;\n\n";
    out << "public Proxy(android.os.IHwBinder remote) {\n";
    out.indent();
    out << "mRemote = java.util.Objects.requireNonNull(remote);\n";
    out.unindent();
    out << "}\n\n";

    out << "@Override\npublic android.os.IHwBinder asBinder() {\n";
    out.indent();
    out << "return mRemote;\n";
    out.unindent();
    out << "}\n\n";


    out << "@Override\npublic String toString() ";
    out.block([&] {
        out.sTry([&] {
            out << "return this.interfaceDescriptor() + \"@Proxy\";\n";
        }).sCatch("android.os.RemoteException ex", [&] {
            out << "/* ignored; handled below. */\n";
        }).endl();
        out << "return \"[class or subclass of \" + "
            << ifaceName << ".kInterfaceName + \"]@Proxy\";\n";
    }).endl().endl();

    // Equals when internal binder object is equal (even if the interface Proxy object
    // itself is different). This is similar to interfacesEqual in C++.
    out << "@Override\npublic final boolean equals(java.lang.Object other) ";
    out.block([&] {
        out << "return android.os.HidlSupport.interfacesEqual(this, other);\n";
    }).endl().endl();

    out << "@Override\npublic final int hashCode() ";
    out.block([&] {
        out << "return this.asBinder().hashCode();\n";
    }).endl().endl();

    const Interface *prevInterface = nullptr;
    for (const auto &tuple : iface->allMethodsFromRoot()) {
        const Method *method = tuple.method();

        const Interface *superInterface = tuple.interface();
        if (prevInterface != superInterface) {
            out << "// Methods from "
                << superInterface->fullName()
                << " follow.\n";
            prevInterface = superInterface;
        }
        const bool returnsValue = !method->results().empty();
        const bool needsCallback = method->results().size() > 1;

        out << "@Override\npublic ";
        if (returnsValue && !needsCallback) {
            out << method->results()[0]->type().getJavaType();
        } else {
            out << "void";
        }

        out << " "
            << method->name()
            << "(";
        method->emitJavaArgSignature(out);

        if (needsCallback) {
            if (!method->args().empty()) {
                out << ", ";
            }

            out << method->name()
                << "Callback _hidl_cb";
        }

        out << ")\n";
        out.indent();
        out.indent();
        out << "throws android.os.RemoteException {\n";
        out.unindent();

        if (method->isHidlReserved() && method->overridesJavaImpl(IMPL_PROXY)) {
            method->javaImpl(IMPL_PROXY, out);
            out.unindent();
            out << "}\n";
            continue;
        }
        out << "android.os.HwParcel _hidl_request = new android.os.HwParcel();\n";
        out << "_hidl_request.writeInterfaceToken("
            << superInterface->fullJavaName()
            << ".kInterfaceName);\n";

        for (const auto &arg : method->args()) {
            emitJavaReaderWriter(
                    out,
                    "_hidl_request",
                    arg,
                    false /* isReader */,
                    false /* addPrefixToName */);
        }

        out << "\nandroid.os.HwParcel _hidl_reply = new android.os.HwParcel();\n";

        out.sTry([&] {
            out << "mRemote.transact("
                << method->getSerialId()
                << " /* "
                << method->name()
                << " */, _hidl_request, _hidl_reply, ";

            if (method->isOneway()) {
                out << Interface::FLAG_ONE_WAY->javaValue();
            } else {
                out << "0 /* flags */";
            }

            out << ");\n";

            if (!method->isOneway()) {
                out << "_hidl_reply.verifySuccess();\n";
            } else {
                CHECK(!returnsValue);
            }

            out << "_hidl_request.releaseTemporaryStorage();\n";

            if (returnsValue) {
                out << "\n";

                for (const auto &arg : method->results()) {
                    emitJavaReaderWriter(
                            out,
                            "_hidl_reply",
                            arg,
                            true /* isReader */,
                            true /* addPrefixToName */);
                }

                if (needsCallback) {
                    out << "_hidl_cb.onValues(";

                    bool firstField = true;
                    for (const auto &arg : method->results()) {
                        if (!firstField) {
                            out << ", ";
                        }

                        out << "_hidl_out_" << arg->name();
                        firstField = false;
                    }

                    out << ");\n";
                } else {
                    const std::string returnName = method->results()[0]->name();
                    out << "return _hidl_out_" << returnName << ";\n";
                }
            }
        }).sFinally([&] {
            out << "_hidl_reply.release();\n";
        }).endl();

        out.unindent();
        out << "}\n\n";
    }

    out.unindent();
    out << "}\n";

    ////////////////////////////////////////////////////////////////////////////

    out << "\npublic static abstract class Stub extends android.os.HwBinder "
        << "implements "
        << ifaceName << " {\n";

    out.indent();

    out << "@Override\npublic android.os.IHwBinder asBinder() {\n";
    out.indent();
    // If we change this behavior in the future and asBinder does not return "this",
    // equals and hashCode should also be overridden.
    out << "return this;\n";
    out.unindent();
    out << "}\n\n";

    for (Method *method : iface->hidlReservedMethods()) {
        // b/32383557 this is a hack. We need to change this if we have more reserved methods.
        CHECK_LE(method->results().size(), 1u);
        std::string resultType = method->results().size() == 0 ? "void" :
                method->results()[0]->type().getJavaType();

        bool canBeOverriden = method->name() == "debug";

        out << "@Override\npublic " << (canBeOverriden ? "" : "final ") << resultType << " "
            << method->name() << "(";
        method->emitJavaArgSignature(out);
        out << ") {\n";

        out.indent();
        method->javaImpl(IMPL_INTERFACE, out);
        out.unindent();
        out << "\n}\n\n";
    }

    out << "@Override\n"
        << "public android.os.IHwInterface queryLocalInterface(String descriptor) {\n";
    out.indent();
    // XXX what about potential superClasses?
    out << "if (kInterfaceName.equals(descriptor)) {\n";
    out.indent();
    out << "return this;\n";
    out.unindent();
    out << "}\n";
    out << "return null;\n";
    out.unindent();
    out << "}\n\n";

    out << "public void registerAsService(String serviceName) throws android.os.RemoteException {\n";
    out.indent();

    out << "registerService(serviceName);\n";

    out.unindent();
    out << "}\n\n";

    out << "@Override\npublic String toString() ";
    out.block([&] {
        out << "return this.interfaceDescriptor() + \"@Stub\";\n";
    }).endl().endl();

    out << "@Override\n"
        << "public void onTransact("
        << "int _hidl_code, "
        << "android.os.HwParcel _hidl_request, "
        << "final android.os.HwParcel _hidl_reply, "
        << "int _hidl_flags)\n";
    out.indent();
    out.indent();
    out << "throws android.os.RemoteException {\n";
    out.unindent();

    out << "switch (_hidl_code) {\n";

    out.indent();

    for (const auto &tuple : iface->allMethodsFromRoot()) {
        const Method *method = tuple.method();

        const Interface *superInterface = tuple.interface();
        const bool returnsValue = !method->results().empty();
        const bool needsCallback = method->results().size() > 1;

        out << "case "
            << method->getSerialId()
            << " /* "
            << method->name()
            << " */:\n{\n";

        out.indent();

        out << "boolean _hidl_is_oneway = (_hidl_flags & " << Interface::FLAG_ONE_WAY->javaValue()
            << ") != 0;\n";
        out << "if (_hidl_is_oneway != " << (method->isOneway() ? "true" : "false") << ") ";
        out.block([&] {
            out << "_hidl_reply.writeStatus(" << UNKNOWN_ERROR << ");\n";
            out << "_hidl_reply.send();\n";
            out << "break;\n";
        });

        if (method->isHidlReserved() && method->overridesJavaImpl(IMPL_STUB)) {
            method->javaImpl(IMPL_STUB, out);
            out.unindent();
            out << "break;\n";
            out << "}\n\n";
            continue;
        }

        out << "_hidl_request.enforceInterface("
            << superInterface->fullJavaName()
            << ".kInterfaceName);\n\n";

        for (const auto &arg : method->args()) {
            emitJavaReaderWriter(
                    out,
                    "_hidl_request",
                    arg,
                    true /* isReader */,
                    false /* addPrefixToName */);
        }

        if (!needsCallback && returnsValue) {
            const NamedReference<Type>* returnArg = method->results()[0];

            out << returnArg->type().getJavaType()
                << " _hidl_out_"
                << returnArg->name()
                << " = ";
        }

        out << method->name()
            << "(";

        bool firstField = true;
        for (const auto &arg : method->args()) {
            if (!firstField) {
                out << ", ";
            }

            out << arg->name();

            firstField = false;
        }

        if (needsCallback) {
            if (!firstField) {
                out << ", ";
            }

            out << "new " << method->name() << "Callback() {\n";
            out.indent();

            out << "@Override\n"
                << "public void onValues(";
            method->emitJavaResultSignature(out);
            out << ") {\n";

            out.indent();
            out << "_hidl_reply.writeStatus(android.os.HwParcel.STATUS_SUCCESS);\n";

            for (const auto &arg : method->results()) {
                emitJavaReaderWriter(
                        out,
                        "_hidl_reply",
                        arg,
                        false /* isReader */,
                        false /* addPrefixToName */);
                // no need to add _hidl_out because out vars are are scoped
            }

            out << "_hidl_reply.send();\n"
                      << "}}";

            out.unindent();
            out.unindent();
        }

        out << ");\n";

        if (!needsCallback && !method->isOneway()) {
            out << "_hidl_reply.writeStatus(android.os.HwParcel.STATUS_SUCCESS);\n";

            if (returnsValue) {
                const NamedReference<Type>* returnArg = method->results()[0];

                emitJavaReaderWriter(
                        out,
                        "_hidl_reply",
                        returnArg,
                        false /* isReader */,
                        true /* addPrefixToName */);
            }

            out << "_hidl_reply.send();\n";
        }

        out << "break;\n";
        out.unindent();
        out << "}\n\n";
    }

    out.unindent();
    out << "}\n";

    out.unindent();
    out << "}\n";

    out.unindent();
    out << "}\n";

    out.unindent();
    out << "}\n";
}

}  // namespace android
