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
#include "HidlTypeAssertion.h"
#include "Interface.h"
#include "Method.h"
#include "Reference.h"
#include "ScalarType.h"
#include "Scope.h"

#include <algorithm>
#include <hidl-util/Formatter.h>
#include <hidl-util/StringHelper.h>
#include <android-base/logging.h>
#include <string>
#include <vector>

namespace android {

void AST::getPackageComponents(
        std::vector<std::string> *components) const {
    mPackage.getPackageComponents(components);
}

void AST::getPackageAndVersionComponents(
        std::vector<std::string> *components, bool cpp_compatible) const {
    mPackage.getPackageAndVersionComponents(components, cpp_compatible);
}

std::string AST::makeHeaderGuard(const std::string &baseName,
                                 bool indicateGenerated) const {
    std::string guard;

    if (indicateGenerated) {
        guard += "HIDL_GENERATED_";
    }

    guard += StringHelper::Uppercase(mPackage.tokenName());
    guard += "_";
    guard += StringHelper::Uppercase(baseName);
    guard += "_H";

    return guard;
}

void AST::generateCppPackageInclude(
        Formatter &out,
        const FQName &package,
        const std::string &klass) {

    out << "#include <";

    std::vector<std::string> components;
    package.getPackageAndVersionComponents(&components, false /* cpp_compatible */);

    for (const auto &component : components) {
        out << component << "/";
    }

    out << klass
        << ".h>\n";
}

void AST::enterLeaveNamespace(Formatter &out, bool enter) const {
    std::vector<std::string> packageComponents;
    getPackageAndVersionComponents(
            &packageComponents, true /* cpp_compatible */);

    if (enter) {
        for (const auto &component : packageComponents) {
            out << "namespace " << component << " {\n";
        }

        out.setNamespace(mPackage.cppNamespace() + "::");
    } else {
        out.setNamespace(std::string());

        for (auto it = packageComponents.rbegin();
                it != packageComponents.rend();
                ++it) {
            out << "}  // namespace " << *it << "\n";
        }
    }
}

static void declareGetService(Formatter &out, const std::string &interfaceName, bool isTry) {
    const std::string functionName = isTry ? "tryGetService" : "getService";

    if (isTry) {
        DocComment(
                "This gets the service of this type with the specified instance name. If the\n"
                "service is currently not available or not in the VINTF manifest on a Trebilized\n"
                "device, this will return nullptr. This is useful when you don't want to block\n"
                "during device boot. If getStub is true, this will try to return an unwrapped\n"
                "passthrough implementation in the same process. This is useful when getting an\n"
                "implementation from the same partition/compilation group.\n\n"
                "In general, prefer getService(std::string,bool)")
                .emit(out);
    } else {
        DocComment(
                "This gets the service of this type with the specified instance name. If the\n"
                "service is not in the VINTF manifest on a Trebilized device, this will return\n"
                "nullptr. If the service is not available, this will wait for the service to\n"
                "become available. If the service is a lazy service, this will start the service\n"
                "and return when it becomes available. If getStub is true, this will try to\n"
                "return an unwrapped passthrough implementation in the same process. This is\n"
                "useful when getting an implementation from the same partition/compilation group.")
                .emit(out);
    }
    out << "static ::android::sp<" << interfaceName << "> " << functionName << "("
        << "const std::string &serviceName=\"default\", bool getStub=false);\n";
    DocComment("Deprecated. See " + functionName + "(std::string, bool)").emit(out);
    out << "static ::android::sp<" << interfaceName << "> " << functionName << "("
        << "const char serviceName[], bool getStub=false)"
        << "  { std::string str(serviceName ? serviceName : \"\");"
        << "      return " << functionName << "(str, getStub); }\n";
    DocComment("Deprecated. See " + functionName + "(std::string, bool)").emit(out);
    out << "static ::android::sp<" << interfaceName << "> " << functionName << "("
        << "const ::android::hardware::hidl_string& serviceName, bool getStub=false)"
        // without c_str the std::string constructor is ambiguous
        << "  { std::string str(serviceName.c_str());"
        << "      return " << functionName << "(str, getStub); }\n";
    DocComment("Calls " + functionName +
               "(\"default\", bool). This is the recommended instance name for singleton services.")
            .emit(out);
    out << "static ::android::sp<" << interfaceName << "> " << functionName << "("
        << "bool getStub) { return " << functionName << "(\"default\", getStub); }\n";
}

static void declareServiceManagerInteractions(Formatter &out, const std::string &interfaceName) {
    declareGetService(out, interfaceName, true /* isTry */);
    declareGetService(out, interfaceName, false /* isTry */);

    DocComment(
            "Registers a service with the service manager. For Trebilized devices, the service\n"
            "must also be in the VINTF manifest.")
            .emit(out);
    out << "__attribute__ ((warn_unused_result))"
        << "::android::status_t registerAsService(const std::string &serviceName=\"default\");\n";
    DocComment("Registers for notifications for when a service is registered.").emit(out);
    out << "static bool registerForNotifications(\n";
    out.indent(2, [&] {
        out << "const std::string &serviceName,\n"
            << "const ::android::sp<::android::hidl::manager::V1_0::IServiceNotification> "
            << "&notification);\n";
    });

}

static void implementGetService(Formatter &out,
        const FQName &fqName,
        bool isTry) {

    const std::string interfaceName = fqName.getInterfaceName();
    const std::string functionName = isTry ? "tryGetService" : "getService";

    out << "::android::sp<" << interfaceName << "> " << interfaceName << "::" << functionName << "("
        << "const std::string &serviceName, const bool getStub) ";
    out.block([&] {
        out << "return ::android::hardware::details::getServiceInternal<"
            << fqName.getInterfaceProxyName()
            << ">(serviceName, "
            << (!isTry ? "true" : "false") // retry
            << ", getStub);\n";
    }).endl().endl();
}

static void implementServiceManagerInteractions(Formatter &out,
        const FQName &fqName, const std::string &package) {

    const std::string interfaceName = fqName.getInterfaceName();

    implementGetService(out, fqName, true /* isTry */);
    implementGetService(out, fqName, false /* isTry */);

    out << "::android::status_t " << interfaceName << "::registerAsService("
        << "const std::string &serviceName) ";
    out.block([&] {
        out << "return ::android::hardware::details::registerAsServiceInternal(this, serviceName);\n";
    }).endl().endl();

    out << "bool " << interfaceName << "::registerForNotifications(\n";
    out.indent(2, [&] {
        out << "const std::string &serviceName,\n"
            << "const ::android::sp<::android::hidl::manager::V1_0::IServiceNotification> "
            << "&notification) ";
    });
    out.block([&] {
        out << "const ::android::sp<::android::hidl::manager::V1_0::IServiceManager> sm\n";
        out.indent(2, [&] {
            out << "= ::android::hardware::defaultServiceManager();\n";
        });
        out.sIf("sm == nullptr", [&] {
            out << "return false;\n";
        }).endl();
        out << "::android::hardware::Return<bool> success =\n";
        out.indent(2, [&] {
            out << "sm->registerForNotifications(\"" << package << "::" << interfaceName << "\",\n";
            out.indent(2, [&] {
                out << "serviceName, notification);\n";
            });
        });
        out << "return success.isOk() && success;\n";
    }).endl().endl();
}

void AST::generateInterfaceHeader(Formatter& out) const {
    const Interface *iface = getInterface();
    std::string ifaceName = iface ? iface->localName() : "types";
    const std::string guard = makeHeaderGuard(ifaceName);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    for (const auto &item : mImportedNames) {
        generateCppPackageInclude(out, item, item.name());
    }

    if (!mImportedNames.empty()) {
        out << "\n";
    }

    if (iface) {
        if (isIBase()) {
            out << "// skipped #include IServiceNotification.h\n\n";
        } else {
            out << "#include <android/hidl/manager/1.0/IServiceNotification.h>\n\n";
        }
    }

    out << "#include <hidl/HidlSupport.h>\n";
    out << "#include <hidl/MQDescriptor.h>\n";

    if (iface) {
        out << "#include <hidl/Status.h>\n";
    }

    out << "#include <utils/NativeHandle.h>\n";
    out << "#include <utils/misc.h>\n\n"; /* for report_sysprop_change() */

    enterLeaveNamespace(out, true /* enter */);
    out << "\n";

    if (iface) {
        iface->emitDocComment(out);

        out << "struct "
            << ifaceName;

        const Interface *superType = iface->superType();

        if (superType == nullptr) {
            out << " : virtual public ::android::RefBase";
        } else {
            out << " : public "
                << superType->fullName();
        }

        out << " {\n";

        out.indent();

        DocComment("Type tag for use in template logic that indicates this is a 'pure' class.")
                .emit(out);
        generateCppTag(out, "android::hardware::details::i_tag");

        DocComment("Fully qualified interface name: \"" + iface->fqName().string() + "\"")
                .emit(out);
        out << "static const char* descriptor;\n\n";

        iface->emitTypeDeclarations(out);
    } else {
        mRootScope.emitTypeDeclarations(out);
    }

    if (iface) {
        DocComment(
                "Returns whether this object's implementation is outside of the current process.")
                .emit(out);
        out << "virtual bool isRemote() const ";
        if (!isIBase()) {
            out << "override ";
        }
        out << "{ return false; }\n";

        for (const auto& tuple : iface->allMethodsFromRoot()) {
            const Method* method = tuple.method();

            out << "\n";

            const bool returnsValue = !method->results().empty();
            const NamedReference<Type>* elidedReturn = method->canElideCallback();

            if (elidedReturn == nullptr && returnsValue) {
                DocComment("Return callback for " + method->name()).emit(out);
                out << "using "
                    << method->name()
                    << "_cb = std::function<void(";
                method->emitCppResultSignature(out, true /* specify namespaces */);
                out << ")>;\n";
            }

            method->dumpAnnotations(out);

            method->emitDocComment(out);

            if (elidedReturn) {
                out << "virtual ::android::hardware::Return<";
                out << elidedReturn->type().getCppResultType() << "> ";
            } else {
                out << "virtual ::android::hardware::Return<void> ";
            }

            out << method->name()
                << "(";
            method->emitCppArgSignature(out, true /* specify namespaces */);
            out << ")";
            if (method->isHidlReserved()) {
                if (!isIBase()) {
                    out << " override";
                }
            } else {
                out << " = 0";
            }
            out << ";\n";
        }

        out << "\n// cast static functions\n";
        std::string childTypeResult = iface->getCppResultType();

        for (const Interface *superType : iface->typeChain()) {
            DocComment(
                    "This performs a checked cast based on what the underlying implementation "
                    "actually is.")
                    .emit(out);
            out << "static ::android::hardware::Return<"
                << childTypeResult
                << "> castFrom("
                << superType->getCppArgumentType()
                << " parent"
                << ", bool emitError = false);\n";
        }

        if (isIBase()) {
            out << "\n// skipped getService, registerAsService, registerForNotifications\n\n";
        } else {
            out << "\n// helper methods for interactions with the hwservicemanager\n";
            declareServiceManagerInteractions(out, iface->localName());
        }
    }

    if (iface) {
        out.unindent();

        out << "};\n\n";
    }

    out << "//\n";
    out << "// type declarations for package\n";
    out << "//\n\n";
    mRootScope.emitPackageTypeDeclarations(out);
    out << "//\n";
    out << "// type header definitions for package\n";
    out << "//\n\n";
    mRootScope.emitPackageTypeHeaderDefinitions(out);

    out << "\n";
    enterLeaveNamespace(out, false /* enter */);
    out << "\n";

    out << "//\n";
    out << "// global type declarations for package\n";
    out << "//\n\n";
    mRootScope.emitGlobalTypeDeclarations(out);

    out << "\n#endif  // " << guard << "\n";
}

void AST::generateHwBinderHeader(Formatter& out) const {
    const Interface *iface = getInterface();
    std::string klassName = iface ? iface->getHwName() : "hwtypes";

    const std::string guard = makeHeaderGuard(klassName);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    generateCppPackageInclude(out, mPackage, iface ? iface->localName() : "types");

    out << "\n";

    for (const auto &item : mImportedNames) {
        if (item.name() == "types") {
            generateCppPackageInclude(out, item, "hwtypes");
        } else {
            generateCppPackageInclude(out, item, item.getInterfaceStubName());
            generateCppPackageInclude(out, item, item.getInterfaceProxyName());
        }
    }

    out << "\n";

    out << "#include <hidl/Status.h>\n";
    out << "#include <hwbinder/IBinder.h>\n";
    out << "#include <hwbinder/Parcel.h>\n";

    out << "\n";

    enterLeaveNamespace(out, true /* enter */);

    mRootScope.emitPackageHwDeclarations(out);

    enterLeaveNamespace(out, false /* enter */);

    out << "\n#endif  // " << guard << "\n";
}

static std::string wrapPassthroughArg(Formatter& out, const NamedReference<Type>* arg,
                                      std::string name, std::function<void(void)> handleError) {
    if (!arg->type().isInterface()) {
        return name;
    }
    std::string wrappedName = "_hidl_wrapped_" + name;
    const Interface &iface = static_cast<const Interface &>(arg->type());
    out << iface.getCppStackType() << " " << wrappedName << ";\n";
    // TODO(elsk): b/33754152 Should not wrap this if object is Bs*
    out.sIf(name + " != nullptr && !" + name + "->isRemote()", [&] {
        out << wrappedName
            << " = "
            << "::android::hardware::details::wrapPassthrough("
            << name
            << ");\n";
        out.sIf(wrappedName + " == nullptr", [&] {
            // Fatal error. Happens when the BsFoo class is not found in the binary
            // or any dynamic libraries.
            handleError();
        }).endl();
    }).sElse([&] {
        out << wrappedName << " = " << name << ";\n";
    }).endl().endl();

    return wrappedName;
}

void AST::generatePassthroughMethod(Formatter& out, const Method* method, const Interface* superInterface) const {
    method->generateCppSignature(out);

    out << " override {\n";
    out.indent();

    if (method->isHidlReserved()
        && method->overridesCppImpl(IMPL_PASSTHROUGH)) {
        method->cppImpl(IMPL_PASSTHROUGH, out);
        out.unindent();
        out << "}\n\n";
        return;
    }

    const bool returnsValue = !method->results().empty();
    const NamedReference<Type>* elidedReturn = method->canElideCallback();

    if (returnsValue && elidedReturn == nullptr) {
        generateCheckNonNull(out, "_hidl_cb");
    }

    generateCppInstrumentationCall(
            out,
            InstrumentationEvent::PASSTHROUGH_ENTRY,
            method,
            superInterface);

    std::vector<std::string> wrappedArgNames;
    for (const auto &arg : method->args()) {
        std::string name = wrapPassthroughArg(out, arg, arg->name(), [&] {
            out << "return ::android::hardware::Status::fromExceptionCode(\n";
            out.indent(2, [&] {
                out << "::android::hardware::Status::EX_TRANSACTION_FAILED,\n"
                    << "\"Cannot wrap passthrough interface.\");\n";
            });
        });

        wrappedArgNames.push_back(name);
    }

    out << "::android::hardware::Status _hidl_error = ::android::hardware::Status::ok();\n";
    out << "auto _hidl_return = ";

    if (method->isOneway()) {
        out << "addOnewayTask([mImpl = this->mImpl\n"
            << "#ifdef __ANDROID_DEBUGGABLE__\n"
               ", mEnableInstrumentation = this->mEnableInstrumentation, "
               "mInstrumentationCallbacks = this->mInstrumentationCallbacks\n"
            << "#endif // __ANDROID_DEBUGGABLE__\n";
        for (const std::string& arg : wrappedArgNames) {
            out << ", " << arg;
        }
        out << "] {\n";
        out.indent();
    }

    out << "mImpl->"
        << method->name()
        << "(";

    out.join(method->args().begin(), method->args().end(), ", ", [&](const auto &arg) {
        out << (arg->type().isInterface() ? "_hidl_wrapped_" : "") << arg->name();
    });

    std::function<void(void)> kHandlePassthroughError = [&] {
        out << "_hidl_error = ::android::hardware::Status::fromExceptionCode(\n";
        out.indent(2, [&] {
            out << "::android::hardware::Status::EX_TRANSACTION_FAILED,\n"
                << "\"Cannot wrap passthrough interface.\");\n";
        });
    };

    if (returnsValue && elidedReturn == nullptr) {
        // never true if oneway since oneway methods don't return values

        if (!method->args().empty()) {
            out << ", ";
        }
        out << "[&](";
        out.join(method->results().begin(), method->results().end(), ", ", [&](const auto &arg) {
            out << "const auto &_hidl_out_"
                << arg->name();
        });

        out << ") {\n";
        out.indent();
        generateCppInstrumentationCall(
                out,
                InstrumentationEvent::PASSTHROUGH_EXIT,
                method,
                superInterface);

        std::vector<std::string> wrappedOutNames;
        for (const auto &arg : method->results()) {
            wrappedOutNames.push_back(
                wrapPassthroughArg(out, arg, "_hidl_out_" + arg->name(), kHandlePassthroughError));
        }

        out << "_hidl_cb(";
        out.join(wrappedOutNames.begin(), wrappedOutNames.end(), ", ",
                 [&](const std::string& arg) { out << arg; });
        out << ");\n";
        out.unindent();
        out << "});\n\n";
    } else {
        out << ");\n\n";

        if (elidedReturn != nullptr) {
            const std::string outName = "_hidl_out_" + elidedReturn->name();

            out << elidedReturn->type().getCppResultType() << " " << outName
                << " = _hidl_return;\n";
            out << "(void) " << outName << ";\n";

            const std::string wrappedName =
                wrapPassthroughArg(out, elidedReturn, outName, kHandlePassthroughError);

            if (outName != wrappedName) {
                // update the original value since it is used by generateCppInstrumentationCall
                out << outName << " = " << wrappedName << ";\n\n";

                // update the value to be returned
                out << "_hidl_return = " << outName << "\n;";
            }
        }
        generateCppInstrumentationCall(
                out,
                InstrumentationEvent::PASSTHROUGH_EXIT,
                method,
                superInterface);
    }

    if (method->isOneway()) {
        out.unindent();
        out << "});\n";
    } else {
        out << "if (!_hidl_error.isOk()) return _hidl_error;\n";
    }

    out << "return _hidl_return;\n";

    out.unindent();
    out << "}\n";
}

void AST::generateMethods(Formatter& out, const MethodGenerator& gen, bool includeParent) const {
    const Interface* iface = mRootScope.getInterface();

    const Interface *prevIterface = nullptr;
    for (const auto &tuple : iface->allMethodsFromRoot()) {
        const Method *method = tuple.method();
        const Interface *superInterface = tuple.interface();

        if (!includeParent && superInterface != iface) {
            continue;
        }

        if(prevIterface != superInterface) {
            if (prevIterface != nullptr) {
                out << "\n";
            }
            out << "// Methods from "
                << superInterface->fullName()
                << " follow.\n";
            prevIterface = superInterface;
        }
        gen(method, superInterface);
    }

    out << "\n";
}

void AST::generateTemplatizationLink(Formatter& out) const {
    DocComment("The pure class is what this class wraps.").emit(out);
    out << "typedef " << mRootScope.getInterface()->localName() << " Pure;\n\n";
}

void AST::generateCppTag(Formatter& out, const std::string& tag) const {
    out << "typedef " << tag << " _hidl_tag;\n\n";
}

void AST::generateStubHeader(Formatter& out) const {
    CHECK(AST::isInterface());

    const Interface* iface = mRootScope.getInterface();
    const std::string klassName = iface->getStubName();
    const std::string guard = makeHeaderGuard(klassName);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    generateCppPackageInclude(out, mPackage, iface->getHwName());

    out << "\n";

    enterLeaveNamespace(out, true /* enter */);
    out << "\n";

    out << "struct "
        << klassName;
    if (iface->isIBase()) {
        out << " : public ::android::hardware::BHwBinder";
        out << ", public ::android::hardware::details::HidlInstrumentor {\n";
    } else {
        out << " : public "
            << gIBaseFqName.getInterfaceStubFqName().cppName()
            << " {\n";
    }

    out.indent();
    out << "explicit "
        << klassName
        << "(const ::android::sp<" << iface->localName() << "> &_hidl_impl);"
        << "\n";
    out << "explicit "
        << klassName
        << "(const ::android::sp<" << iface->localName() << "> &_hidl_impl,"
        << " const std::string& HidlInstrumentor_package,"
        << " const std::string& HidlInstrumentor_interface);"
        << "\n\n";
    out << "virtual ~" << klassName << "();\n\n";
    out << "::android::status_t onTransact(\n";
    out.indent();
    out.indent();
    out << "uint32_t _hidl_code,\n";
    out << "const ::android::hardware::Parcel &_hidl_data,\n";
    out << "::android::hardware::Parcel *_hidl_reply,\n";
    out << "uint32_t _hidl_flags = 0,\n";
    out << "TransactCallback _hidl_cb = nullptr) override;\n\n";
    out.unindent();
    out.unindent();

    out.endl();
    generateTemplatizationLink(out);
    DocComment("Type tag for use in template logic that indicates this is a 'native' class.")
            .emit(out);
    generateCppTag(out, "android::hardware::details::bnhw_tag");

    out << "::android::sp<" << iface->localName() << "> getImpl() { return _hidl_mImpl; }\n";

    generateMethods(out,
                    [&](const Method* method, const Interface*) {
                        if (method->isHidlReserved() && method->overridesCppImpl(IMPL_PROXY)) {
                            return;
                        }

                        out << "static ::android::status_t _hidl_" << method->name() << "(\n";

                        out.indent(2,
                                   [&] {
                                       out << "::android::hidl::base::V1_0::BnHwBase* _hidl_this,\n"
                                           << "const ::android::hardware::Parcel &_hidl_data,\n"
                                           << "::android::hardware::Parcel *_hidl_reply,\n"
                                           << "TransactCallback _hidl_cb);\n";
                                   })
                            .endl()
                            .endl();
                    },
                    false /* include parents */);

    out.unindent();
    out << "private:\n";
    out.indent();

    generateMethods(out, [&](const Method* method, const Interface* iface) {
        if (!method->isHidlReserved() || !method->overridesCppImpl(IMPL_STUB_IMPL)) {
            return;
        }
        const bool returnsValue = !method->results().empty();
        const NamedReference<Type>* elidedReturn = method->canElideCallback();

        if (elidedReturn == nullptr && returnsValue) {
            out << "using " << method->name() << "_cb = "
                << iface->fqName().cppName()
                << "::" << method->name() << "_cb;\n";
        }
        method->generateCppSignature(out);
        out << ";\n";
    });

    out << "::android::sp<" << iface->localName() << "> _hidl_mImpl;\n";
    out.unindent();
    out << "};\n\n";

    enterLeaveNamespace(out, false /* enter */);

    out << "\n#endif  // " << guard << "\n";
}

void AST::generateProxyHeader(Formatter& out) const {
    if (!AST::isInterface()) {
        // types.hal does not get a proxy header.
        return;
    }

    const Interface* iface = mRootScope.getInterface();
    const std::string proxyName = iface->getProxyName();
    const std::string guard = makeHeaderGuard(proxyName);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    out << "#include <hidl/HidlTransportSupport.h>\n\n";

    std::vector<std::string> packageComponents;
    getPackageAndVersionComponents(
            &packageComponents, false /* cpp_compatible */);

    generateCppPackageInclude(out, mPackage, iface->getHwName());
    out << "\n";

    enterLeaveNamespace(out, true /* enter */);
    out << "\n";

    out << "struct "
        << proxyName
        << " : public ::android::hardware::BpInterface<"
        << iface->localName()
        << ">, public ::android::hardware::details::HidlInstrumentor {\n";

    out.indent();

    out << "explicit "
        << proxyName
        << "(const ::android::sp<::android::hardware::IBinder> &_hidl_impl);"
        << "\n\n";

    generateTemplatizationLink(out);
    DocComment("Type tag for use in template logic that indicates this is a 'proxy' class.")
            .emit(out);
    generateCppTag(out, "android::hardware::details::bphw_tag");

    out << "virtual bool isRemote() const override { return true; }\n\n";

    generateMethods(
        out,
        [&](const Method* method, const Interface*) {
            if (method->isHidlReserved() && method->overridesCppImpl(IMPL_PROXY)) {
                return;
            }

            out << "static ";
            method->generateCppReturnType(out);
            out << " _hidl_" << method->name() << "("
                << "::android::hardware::IInterface* _hidl_this, "
                << "::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor";

            if (!method->hasEmptyCppArgSignature()) {
                out << ", ";
            }
            method->emitCppArgSignature(out);
            out << ");\n";
        },
        false /* include parents */);

    generateMethods(out, [&](const Method* method, const Interface*) {
        method->generateCppSignature(out);
        out << " override;\n";
    });

    out.unindent();
    out << "private:\n";
    out.indent();
    out << "std::mutex _hidl_mMutex;\n"
        << "std::vector<::android::sp<::android::hardware::hidl_binder_death_recipient>>"
        << " _hidl_mDeathRecipients;\n";
    out.unindent();
    out << "};\n\n";

    enterLeaveNamespace(out, false /* enter */);

    out << "\n#endif  // " << guard << "\n";
}

void AST::generateCppSource(Formatter& out) const {
    std::string baseName = getBaseName();
    const Interface *iface = getInterface();

    const std::string klassName = baseName + (baseName == "types" ? "" : "All");

    out << "#define LOG_TAG \""
        << mPackage.string() << "::" << baseName
        << "\"\n\n";

    out << "#include <log/log.h>\n";
    out << "#include <cutils/trace.h>\n";
    out << "#include <hidl/HidlTransportSupport.h>\n\n";
    out << "#include <hidl/Static.h>\n";
    out << "#include <hwbinder/ProcessState.h>\n";
    out << "#include <utils/Trace.h>\n";
    if (iface) {
        // This is a no-op for IServiceManager itself.
        out << "#include <android/hidl/manager/1.0/IServiceManager.h>\n";

        generateCppPackageInclude(out, mPackage, iface->getProxyName());
        generateCppPackageInclude(out, mPackage, iface->getStubName());
        generateCppPackageInclude(out, mPackage, iface->getPassthroughName());

        for (const Interface *superType : iface->superTypeChain()) {
            generateCppPackageInclude(out,
                                      superType->fqName(),
                                      superType->fqName().getInterfaceProxyName());
        }

        out << "#include <hidl/ServiceManagement.h>\n";
    } else {
        generateCppPackageInclude(out, mPackage, "types");
        generateCppPackageInclude(out, mPackage, "hwtypes");
    }

    out << "\n";

    enterLeaveNamespace(out, true /* enter */);
    out << "\n";

    generateTypeSource(out, iface ? iface->localName() : "");

    if (iface) {
        const Interface* iface = mRootScope.getInterface();

        // need to be put here, generateStubSource is using this.
        out << "const char* "
            << iface->localName()
            << "::descriptor(\""
            << iface->fqName().string()
            << "\");\n\n";
        out << "__attribute__((constructor)) ";
        out << "static void static_constructor() {\n";
        out.indent([&] {
            out << "::android::hardware::details::getBnConstructorMap().set("
                << iface->localName()
                << "::descriptor,\n";
            out.indent(2, [&] {
                out << "[](void *iIntf) -> ::android::sp<::android::hardware::IBinder> {\n";
                out.indent([&] {
                    out << "return new "
                        << iface->getStubName()
                        << "(static_cast<"
                        << iface->localName()
                        << " *>(iIntf));\n";
                });
                out << "});\n";
            });
            out << "::android::hardware::details::getBsConstructorMap().set("
                << iface->localName()
                << "::descriptor,\n";
            out.indent(2, [&] {
                out << "[](void *iIntf) -> ::android::sp<"
                    << gIBaseFqName.cppName()
                    << "> {\n";
                out.indent([&] {
                    out << "return new "
                        << iface->getPassthroughName()
                        << "(static_cast<"
                        << iface->localName()
                        << " *>(iIntf));\n";
                });
                out << "});\n";
            });
        });
        out << "};\n\n";
        out << "__attribute__((destructor))";
        out << "static void static_destructor() {\n";
        out.indent([&] {
            out << "::android::hardware::details::getBnConstructorMap().erase("
                << iface->localName()
                << "::descriptor);\n";
            out << "::android::hardware::details::getBsConstructorMap().erase("
                << iface->localName()
                << "::descriptor);\n";
        });
        out << "};\n\n";

        generateInterfaceSource(out);
        generateProxySource(out, iface->fqName());
        generateStubSource(out, iface);
        generatePassthroughSource(out);

        if (isIBase()) {
            out << "// skipped getService, registerAsService, registerForNotifications\n";
        } else {
            std::string package = iface->fqName().package()
                    + iface->fqName().atVersion();

            implementServiceManagerInteractions(out, iface->fqName(), package);
        }
    }

    HidlTypeAssertion::EmitAll(out);
    out << "\n";

    enterLeaveNamespace(out, false /* enter */);
}

void AST::generateCheckNonNull(Formatter &out, const std::string &nonNull) {
    out.sIf(nonNull + " == nullptr", [&] {
        out << "return ::android::hardware::Status::fromExceptionCode(\n";
        out.indent(2, [&] {
            out << "::android::hardware::Status::EX_ILLEGAL_ARGUMENT,\n"
                << "\"Null synchronous callback passed.\");\n";
        });
    }).endl().endl();
}

void AST::generateTypeSource(Formatter& out, const std::string& ifaceName) const {
    mRootScope.emitTypeDefinitions(out, ifaceName);
}

void AST::declareCppReaderLocals(Formatter& out, const std::vector<NamedReference<Type>*>& args,
                                 bool forResults) const {
    if (args.empty()) {
        return;
    }

    for (const auto &arg : args) {
        const Type &type = arg->type();

        out << type.getCppResultType()
            << " "
            << (forResults ? "_hidl_out_" : "") + arg->name()
            << ";\n";
    }

    out << "\n";
}

void AST::emitCppReaderWriter(Formatter& out, const std::string& parcelObj, bool parcelObjIsPointer,
                              const NamedReference<Type>* arg, bool isReader, Type::ErrorMode mode,
                              bool addPrefixToName) const {
    const Type &type = arg->type();

    type.emitReaderWriter(
            out,
            addPrefixToName ? ("_hidl_out_" + arg->name()) : arg->name(),
            parcelObj,
            parcelObjIsPointer,
            isReader,
            mode);
}

void AST::emitCppResolveReferences(Formatter& out, const std::string& parcelObj,
                                   bool parcelObjIsPointer, const NamedReference<Type>* arg,
                                   bool isReader, Type::ErrorMode mode,
                                   bool addPrefixToName) const {
    const Type &type = arg->type();
    if(type.needsResolveReferences()) {
        type.emitResolveReferences(
                out,
                addPrefixToName ? ("_hidl_out_" + arg->name()) : arg->name(),
                isReader, // nameIsPointer
                parcelObj,
                parcelObjIsPointer,
                isReader,
                mode);
    }
}

void AST::generateProxyMethodSource(Formatter& out, const std::string& klassName,
                                    const Method* method, const Interface* superInterface) const {
    method->generateCppSignature(out,
                                 klassName,
                                 true /* specify namespaces */);

    if (method->isHidlReserved() && method->overridesCppImpl(IMPL_PROXY)) {
        out.block([&] {
            method->cppImpl(IMPL_PROXY, out);
        }).endl().endl();
        return;
    }

    out.block([&] {
        const bool returnsValue = !method->results().empty();
        const NamedReference<Type>* elidedReturn = method->canElideCallback();

        method->generateCppReturnType(out);

        out << " _hidl_out = "
            << superInterface->fqName().cppNamespace()
            << "::"
            << superInterface->getProxyName()
            << "::_hidl_"
            << method->name()
            << "(this, this";

        if (!method->hasEmptyCppArgSignature()) {
            out << ", ";
        }

        out.join(method->args().begin(), method->args().end(), ", ", [&](const auto &arg) {
            out << arg->name();
        });

        if (returnsValue && elidedReturn == nullptr) {
            if (!method->args().empty()) {
                out << ", ";
            }
            out << "_hidl_cb";
        }

        out << ");\n\n";

        out << "return _hidl_out;\n";
    }).endl().endl();
}

void AST::generateStaticProxyMethodSource(Formatter& out, const std::string& klassName,
                                          const Method* method, const Interface* superInterface) const {
    if (method->isHidlReserved() && method->overridesCppImpl(IMPL_PROXY)) {
        return;
    }

    method->generateCppReturnType(out);

    out << klassName
        << "::_hidl_"
        << method->name()
        << "("
        << "::android::hardware::IInterface *_hidl_this, "
        << "::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor";

    if (!method->hasEmptyCppArgSignature()) {
        out << ", ";
    }

    method->emitCppArgSignature(out);
    out << ") {\n";

    out.indent();

    out << "#ifdef __ANDROID_DEBUGGABLE__\n";
    out << "bool mEnableInstrumentation = _hidl_this_instrumentor->isInstrumentationEnabled();\n";
    out << "const auto &mInstrumentationCallbacks = _hidl_this_instrumentor->getInstrumentationCallbacks();\n";
    out << "#else\n";
    out << "(void) _hidl_this_instrumentor;\n";
    out << "#endif // __ANDROID_DEBUGGABLE__\n";

    const bool returnsValue = !method->results().empty();
    const NamedReference<Type>* elidedReturn = method->canElideCallback();
    if (returnsValue && elidedReturn == nullptr) {
        generateCheckNonNull(out, "_hidl_cb");
    }

    generateCppInstrumentationCall(
            out,
            InstrumentationEvent::CLIENT_API_ENTRY,
            method,
            superInterface);

    out << "::android::hardware::Parcel _hidl_data;\n";
    out << "::android::hardware::Parcel _hidl_reply;\n";
    out << "::android::status_t _hidl_err;\n";
    out << "::android::hardware::Status _hidl_status;\n\n";

    declareCppReaderLocals(
            out, method->results(), true /* forResults */);

    out << "_hidl_err = _hidl_data.writeInterfaceToken(";
    out << klassName;
    out << "::descriptor);\n";
    out << "if (_hidl_err != ::android::OK) { goto _hidl_error; }\n\n";

    bool hasInterfaceArgument = false;
    // First DFS: write all buffers and resolve pointers for parent
    for (const auto &arg : method->args()) {
        if (arg->type().isInterface()) {
            hasInterfaceArgument = true;
        }
        emitCppReaderWriter(
                out,
                "_hidl_data",
                false /* parcelObjIsPointer */,
                arg,
                false /* reader */,
                Type::ErrorMode_Goto,
                false /* addPrefixToName */);
    }

    // Second DFS: resolve references.
    for (const auto &arg : method->args()) {
        emitCppResolveReferences(
                out,
                "_hidl_data",
                false /* parcelObjIsPointer */,
                arg,
                false /* reader */,
                Type::ErrorMode_Goto,
                false /* addPrefixToName */);
    }

    if (hasInterfaceArgument) {
        // Start binder threadpool to handle incoming transactions
        out << "::android::hardware::ProcessState::self()->startThreadPool();\n";
    }
    out << "_hidl_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact("
        << method->getSerialId()
        << " /* "
        << method->name()
        << " */, _hidl_data, &_hidl_reply";

    if (method->isOneway()) {
        out << ", " << Interface::FLAG_ONE_WAY->cppValue();
    }
    out << ");\n";

    out << "if (_hidl_err != ::android::OK) { goto _hidl_error; }\n\n";

    if (!method->isOneway()) {
        out << "_hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);\n";
        out << "if (_hidl_err != ::android::OK) { goto _hidl_error; }\n\n";
        out << "if (!_hidl_status.isOk()) { return _hidl_status; }\n\n";


        // First DFS: write all buffers and resolve pointers for parent
        for (const auto &arg : method->results()) {
            emitCppReaderWriter(
                    out,
                    "_hidl_reply",
                    false /* parcelObjIsPointer */,
                    arg,
                    true /* reader */,
                    Type::ErrorMode_Goto,
                    true /* addPrefixToName */);
        }

        // Second DFS: resolve references.
        for (const auto &arg : method->results()) {
            emitCppResolveReferences(
                    out,
                    "_hidl_reply",
                    false /* parcelObjIsPointer */,
                    arg,
                    true /* reader */,
                    Type::ErrorMode_Goto,
                    true /* addPrefixToName */);
        }

        if (returnsValue && elidedReturn == nullptr) {
            out << "_hidl_cb(";

            out.join(method->results().begin(), method->results().end(), ", ", [&] (const auto &arg) {
                if (arg->type().resultNeedsDeref()) {
                    out << "*";
                }
                out << "_hidl_out_" << arg->name();
            });

            out << ");\n\n";
        }
    }

    generateCppInstrumentationCall(
            out,
            InstrumentationEvent::CLIENT_API_EXIT,
            method,
            superInterface);

    if (elidedReturn != nullptr) {
        out << "_hidl_status.setFromStatusT(_hidl_err);\n";
        out << "return ::android::hardware::Return<";
        out << elidedReturn->type().getCppResultType()
            << ">(_hidl_out_" << elidedReturn->name() << ");\n\n";
    } else {
        out << "_hidl_status.setFromStatusT(_hidl_err);\n";
        out << "return ::android::hardware::Return<void>();\n\n";
    }

    out.unindent();
    out << "_hidl_error:\n";
    out.indent();
    out << "_hidl_status.setFromStatusT(_hidl_err);\n";
    out << "return ::android::hardware::Return<";
    if (elidedReturn != nullptr) {
        out << method->results().at(0)->type().getCppResultType();
    } else {
        out << "void";
    }
    out << ">(_hidl_status);\n";

    out.unindent();
    out << "}\n\n";
}

void AST::generateProxySource(Formatter& out, const FQName& fqName) const {
    const std::string klassName = fqName.getInterfaceProxyName();

    out << klassName
        << "::"
        << klassName
        << "(const ::android::sp<::android::hardware::IBinder> &_hidl_impl)\n";

    out.indent();
    out.indent();

    out << ": BpInterface"
        << "<"
        << fqName.getInterfaceName()
        << ">(_hidl_impl),\n"
        << "  ::android::hardware::details::HidlInstrumentor(\""
        << mPackage.string()
        << "\", \""
        << fqName.getInterfaceName()
        << "\") {\n";

    out.unindent();
    out.unindent();
    out << "}\n\n";

    generateMethods(out,
                    [&](const Method* method, const Interface* superInterface) {
                        generateStaticProxyMethodSource(out, klassName, method, superInterface);
                    },
                    false /* include parents */);

    generateMethods(out, [&](const Method* method, const Interface* superInterface) {
        generateProxyMethodSource(out, klassName, method, superInterface);
    });
}

void AST::generateStubSource(Formatter& out, const Interface* iface) const {
    const std::string interfaceName = iface->localName();
    const std::string klassName = iface->getStubName();

    out << klassName
        << "::"
        << klassName
        << "(const ::android::sp<" << interfaceName <<"> &_hidl_impl)\n";

    out.indent();
    out.indent();

    if (iface->isIBase()) {
        out << ": ::android::hardware::details::HidlInstrumentor(\"";
    } else {
        out << ": "
            << gIBaseFqName.getInterfaceStubFqName().cppName()
            << "(_hidl_impl, \"";
    }

    out << mPackage.string()
        << "\", \""
        << interfaceName
        << "\") { \n";
    out.indent();
    out << "_hidl_mImpl = _hidl_impl;\n";
    out << "auto prio = ::android::hardware::details::gServicePrioMap->get("
        << "_hidl_impl, {SCHED_NORMAL, 0});\n";
    out << "mSchedPolicy = prio.sched_policy;\n";
    out << "mSchedPriority = prio.prio;\n";
    out << "setRequestingSid(::android::hardware::details::gServiceSidMap->get(_hidl_impl, "
           "false));\n";
    out.unindent();

    out.unindent();
    out.unindent();
    out << "}\n\n";

    if (iface->isIBase()) {
        // BnHwBase has a constructor to initialize the HidlInstrumentor
        // class properly.
        out << klassName
            << "::"
            << klassName
            << "(const ::android::sp<" << interfaceName << "> &_hidl_impl,"
            << " const std::string &HidlInstrumentor_package,"
            << " const std::string &HidlInstrumentor_interface)\n";

        out.indent();
        out.indent();

        out << ": ::android::hardware::details::HidlInstrumentor("
            << "HidlInstrumentor_package, HidlInstrumentor_interface) {\n";
        out.indent();
        out << "_hidl_mImpl = _hidl_impl;\n";
        out.unindent();

        out.unindent();
        out.unindent();
        out << "}\n\n";
    }

    out << klassName << "::~" << klassName << "() ";
    out.block([&]() {
           out << "::android::hardware::details::gBnMap->eraseIfEqual(_hidl_mImpl.get(), this);\n";
       })
            .endl()
            .endl();

    generateMethods(out,
                    [&](const Method* method, const Interface* superInterface) {
                        return generateStaticStubMethodSource(out, iface->fqName(), method, superInterface);
                    },
                    false /* include parents */);

    generateMethods(out, [&](const Method* method, const Interface*) {
        if (!method->isHidlReserved() || !method->overridesCppImpl(IMPL_STUB_IMPL)) {
            return;
        }
        method->generateCppSignature(out, iface->getStubName());
        out << " ";
        out.block([&] {
            method->cppImpl(IMPL_STUB_IMPL, out);
        }).endl();
    });

    out << "::android::status_t " << klassName << "::onTransact(\n";

    out.indent();
    out.indent();

    out << "uint32_t _hidl_code,\n"
        << "const ::android::hardware::Parcel &_hidl_data,\n"
        << "::android::hardware::Parcel *_hidl_reply,\n"
        << "uint32_t _hidl_flags,\n"
        << "TransactCallback _hidl_cb) {\n";

    out.unindent();

    out << "::android::status_t _hidl_err = ::android::OK;\n\n";
    out << "switch (_hidl_code) {\n";
    out.indent();

    for (const auto &tuple : iface->allMethodsFromRoot()) {
        const Method *method = tuple.method();
        const Interface *superInterface = tuple.interface();

        if (!isIBase() && method->isHidlReserved()) {
            continue;
        }
        out << "case "
            << method->getSerialId()
            << " /* "
            << method->name()
            << " */:\n{\n";

        out.indent();

        out << "bool _hidl_is_oneway = _hidl_flags & " << Interface::FLAG_ONE_WAY->cppValue()
            << ";\n";
        out << "if (_hidl_is_oneway != " << (method->isOneway() ? "true" : "false") << ") ";
        out.block([&] { out << "return ::android::UNKNOWN_ERROR;\n"; }).endl().endl();

        generateStubSourceForMethod(out, method, superInterface);

        out.unindent();
        out << "}\n\n";
    }

    out << "default:\n{\n";
    out.indent();

    if (iface->isIBase()) {
        out << "(void)_hidl_flags;\n";
        out << "return ::android::UNKNOWN_TRANSACTION;\n";
    } else {
        out << "return ";
        out << gIBaseFqName.getInterfaceStubFqName().cppName();
        out << "::onTransact(\n";

        out.indent();
        out.indent();

        out << "_hidl_code, _hidl_data, _hidl_reply, "
            << "_hidl_flags, _hidl_cb);\n";

        out.unindent();
        out.unindent();
    }

    out.unindent();
    out << "}\n";

    out.unindent();
    out << "}\n\n";

    out.sIf("_hidl_err == ::android::UNEXPECTED_NULL", [&] {
        out << "_hidl_err = ::android::hardware::writeToParcel(\n";
        out.indent(2, [&] {
            out << "::android::hardware::Status::fromExceptionCode(::android::hardware::Status::EX_NULL_POINTER),\n";
            out << "_hidl_reply);\n";
        });
    });

    out << "return _hidl_err;\n";

    out.unindent();
    out << "}\n\n";
}

void AST::generateStubSourceForMethod(Formatter& out, const Method* method,
                                      const Interface* superInterface) const {
    if (method->isHidlReserved() && method->overridesCppImpl(IMPL_STUB)) {
        method->cppImpl(IMPL_STUB, out);
        out << "break;\n";
        return;
    }

    out << "_hidl_err = "
        << superInterface->fqName().cppNamespace()
        << "::"
        << superInterface->getStubName()
        << "::_hidl_"
        << method->name()
        << "(this, _hidl_data, _hidl_reply, _hidl_cb);\n";
    out << "break;\n";
}

void AST::generateStaticStubMethodSource(Formatter& out, const FQName& fqName,
                                         const Method* method, const Interface* superInterface) const {
    if (method->isHidlReserved() && method->overridesCppImpl(IMPL_STUB)) {
        return;
    }

    const std::string& klassName = fqName.getInterfaceStubName();

    out << "::android::status_t " << klassName << "::_hidl_" << method->name() << "(\n";

    out.indent();
    out.indent();

    out << "::android::hidl::base::V1_0::BnHwBase* _hidl_this,\n"
        << "const ::android::hardware::Parcel &_hidl_data,\n"
        << "::android::hardware::Parcel *_hidl_reply,\n"
        << "TransactCallback _hidl_cb) {\n";

    out.unindent();

    out << "#ifdef __ANDROID_DEBUGGABLE__\n";
    out << "bool mEnableInstrumentation = _hidl_this->isInstrumentationEnabled();\n";
    out << "const auto &mInstrumentationCallbacks = _hidl_this->getInstrumentationCallbacks();\n";
    out << "#endif // __ANDROID_DEBUGGABLE__\n\n";

    out << "::android::status_t _hidl_err = ::android::OK;\n";

    out << "if (!_hidl_data.enforceInterface("
        << klassName
        << "::Pure::descriptor)) {\n";

    out.indent();
    out << "_hidl_err = ::android::BAD_TYPE;\n";
    out << "return _hidl_err;\n";
    out.unindent();
    out << "}\n\n";

    declareCppReaderLocals(out, method->args(), false /* forResults */);

    // First DFS: write buffers
    for (const auto &arg : method->args()) {
        emitCppReaderWriter(
                out,
                "_hidl_data",
                false /* parcelObjIsPointer */,
                arg,
                true /* reader */,
                Type::ErrorMode_Return,
                false /* addPrefixToName */);
    }

    // Second DFS: resolve references
    for (const auto &arg : method->args()) {
        emitCppResolveReferences(
                out,
                "_hidl_data",
                false /* parcelObjIsPointer */,
                arg,
                true /* reader */,
                Type::ErrorMode_Return,
                false /* addPrefixToName */);
    }

    generateCppInstrumentationCall(
            out,
            InstrumentationEvent::SERVER_API_ENTRY,
            method,
            superInterface);

    const bool returnsValue = !method->results().empty();
    const NamedReference<Type>* elidedReturn = method->canElideCallback();

    std::string callee;

    if (method->isHidlReserved() && method->overridesCppImpl(IMPL_STUB_IMPL)) {
        callee = "_hidl_this";
    } else {
        callee = "static_cast<" + fqName.getInterfaceName() + "*>(_hidl_this->getImpl().get())";
    }

    if (elidedReturn != nullptr) {
        out << elidedReturn->type().getCppResultType()
            << " _hidl_out_"
            << elidedReturn->name()
            << " = "
            << callee << "->" << method->name()
            << "(";

        out.join(method->args().begin(), method->args().end(), ", ", [&] (const auto &arg) {
            if (arg->type().resultNeedsDeref()) {
                out << "*";
            }
            out << arg->name();
        });

        out << ");\n\n";

        out << "::android::hardware::writeToParcel(::android::hardware::Status::ok(), "
            << "_hidl_reply);\n\n";

        elidedReturn->type().emitReaderWriter(
                out,
                "_hidl_out_" + elidedReturn->name(),
                "_hidl_reply",
                true, /* parcelObjIsPointer */
                false, /* isReader */
                Type::ErrorMode_Ignore);

        emitCppResolveReferences(
                out,
                "_hidl_reply",
                true /* parcelObjIsPointer */,
                elidedReturn,
                false /* reader */,
                Type::ErrorMode_Ignore,
                true /* addPrefixToName */);

        generateCppInstrumentationCall(
                out,
                InstrumentationEvent::SERVER_API_EXIT,
                method,
            superInterface);

        out << "_hidl_cb(*_hidl_reply);\n";
    } else {
        if (returnsValue) {
            out << "bool _hidl_callbackCalled = false;\n\n";
        }

        out << "::android::hardware::Return<void> _hidl_ret = " << callee << "->" << method->name()
            << "(";

        out.join(method->args().begin(), method->args().end(), ", ", [&] (const auto &arg) {
            if (arg->type().resultNeedsDeref()) {
                out << "*";
            }

            out << arg->name();
        });

        if (returnsValue) {
            if (!method->args().empty()) {
                out << ", ";
            }

            out << "[&](";

            out.join(method->results().begin(), method->results().end(), ", ", [&](const auto &arg) {
                out << "const auto &_hidl_out_" << arg->name();
            });

            out << ") {\n";
            out.indent();
            out << "if (_hidl_callbackCalled) {\n";
            out.indent();
            out << "LOG_ALWAYS_FATAL(\""
                << method->name()
                << ": _hidl_cb called a second time, but must be called once.\");\n";
            out.unindent();
            out << "}\n";
            out << "_hidl_callbackCalled = true;\n\n";

            out << "::android::hardware::writeToParcel(::android::hardware::Status::ok(), "
                << "_hidl_reply);\n\n";

            // First DFS: buffers
            for (const auto &arg : method->results()) {
                emitCppReaderWriter(
                        out,
                        "_hidl_reply",
                        true /* parcelObjIsPointer */,
                        arg,
                        false /* reader */,
                        Type::ErrorMode_Ignore,
                        true /* addPrefixToName */);
            }

            // Second DFS: resolve references
            for (const auto &arg : method->results()) {
                emitCppResolveReferences(
                        out,
                        "_hidl_reply",
                        true /* parcelObjIsPointer */,
                        arg,
                        false /* reader */,
                        Type::ErrorMode_Ignore,
                        true /* addPrefixToName */);
            }

            generateCppInstrumentationCall(
                    out,
                    InstrumentationEvent::SERVER_API_EXIT,
                    method,
                    superInterface);

            out << "_hidl_cb(*_hidl_reply);\n";

            out.unindent();
            out << "});\n\n";
        } else {
            out << ");\n\n";
            out << "(void) _hidl_cb;\n\n";
            generateCppInstrumentationCall(
                    out,
                    InstrumentationEvent::SERVER_API_EXIT,
                    method,
                    superInterface);
        }

        out << "_hidl_ret.assertOk();\n";

        if (returnsValue) {
            out << "if (!_hidl_callbackCalled) {\n";
            out.indent();
            out << "LOG_ALWAYS_FATAL(\""
                << method->name()
                << ": _hidl_cb not called, but must be called once.\");\n";
            out.unindent();
            out << "}\n\n";
        } else {
            out << "::android::hardware::writeToParcel("
                << "::android::hardware::Status::ok(), "
                << "_hidl_reply);\n\n";
        }
    }

    out << "return _hidl_err;\n";
    out.unindent();
    out << "}\n\n";
}

void AST::generatePassthroughHeader(Formatter& out) const {
    if (!AST::isInterface()) {
        // types.hal does not get a stub header.
        return;
    }

    const Interface* iface = mRootScope.getInterface();
    CHECK(iface != nullptr);

    const std::string klassName = iface->getPassthroughName();

    bool supportOneway = iface->hasOnewayMethods();

    const std::string guard = makeHeaderGuard(klassName);

    out << "#ifndef " << guard << "\n";
    out << "#define " << guard << "\n\n";

    std::vector<std::string> packageComponents;
    getPackageAndVersionComponents(
            &packageComponents, false /* cpp_compatible */);

    out << "#include <android-base/macros.h>\n";
    out << "#include <cutils/trace.h>\n";
    out << "#include <future>\n";

    generateCppPackageInclude(out, mPackage, iface->localName());
    out << "\n";

    out << "#include <hidl/HidlPassthroughSupport.h>\n";
    if (supportOneway) {
        out << "#include <hidl/TaskRunner.h>\n";
    }

    enterLeaveNamespace(out, true /* enter */);
    out << "\n";

    out << "struct "
        << klassName
        << " : " << iface->localName()
        << ", ::android::hardware::details::HidlInstrumentor {\n";

    out.indent();
    out << "explicit "
        << klassName
        << "(const ::android::sp<"
        << iface->localName()
        << "> impl);\n";

    out.endl();
    generateTemplatizationLink(out);
    generateCppTag(out, "android::hardware::details::bs_tag");

    generateMethods(out, [&](const Method* method, const Interface* superInterface) {
        generatePassthroughMethod(out, method, superInterface);
    });

    out.unindent();
    out << "private:\n";
    out.indent();
    out << "const ::android::sp<" << iface->localName() << "> mImpl;\n";

    if (supportOneway) {
        out << "::android::hardware::details::TaskRunner mOnewayQueue;\n";

        out << "\n";

        out << "::android::hardware::Return<void> addOnewayTask("
               "std::function<void(void)>);\n\n";
    }

    out.unindent();

    out << "};\n\n";

    enterLeaveNamespace(out, false /* enter */);

    out << "\n#endif  // " << guard << "\n";
}

void AST::generateInterfaceSource(Formatter& out) const {
    const Interface* iface = mRootScope.getInterface();

    // generate castFrom functions
    std::string childTypeResult = iface->getCppResultType();

    generateMethods(out, [&](const Method* method, const Interface*) {
        bool reserved = method->isHidlReserved();

        if (!reserved) {
            out << "// no default implementation for: ";
        }
        method->generateCppSignature(out, iface->localName());
        if (reserved) {
            out.block([&]() {
                method->cppImpl(IMPL_INTERFACE, out);
            }).endl();
        }

        out << "\n";

        return;
    });

    for (const Interface *superType : iface->typeChain()) {
        out << "::android::hardware::Return<"
            << childTypeResult
            << "> "
            << iface->localName()
            << "::castFrom("
            << superType->getCppArgumentType()
            << " parent, bool "
            << (iface == superType ? "/* emitError */" : "emitError")
            << ") {\n";
        out.indent();
        if (iface == superType) {
            out << "return parent;\n";
        } else {
            out << "return ::android::hardware::details::castInterface<";
            out << iface->localName() << ", "
                << superType->fqName().cppName() << ", "
                << iface->getProxyName()
                << ">(\n";
            out.indent();
            out.indent();
            out << "parent, \""
                << iface->fqName().string()
                << "\", emitError);\n";
            out.unindent();
            out.unindent();
        }
        out.unindent();
        out << "}\n\n";
    }
}

void AST::generatePassthroughSource(Formatter& out) const {
    const Interface* iface = mRootScope.getInterface();

    const std::string klassName = iface->getPassthroughName();

    out << klassName
        << "::"
        << klassName
        << "(const ::android::sp<"
        << iface->fullName()
        << "> impl) : ::android::hardware::details::HidlInstrumentor(\""
        << mPackage.string()
        << "\", \""
        << iface->localName()
        << "\"), mImpl(impl) {";
    if (iface->hasOnewayMethods()) {
        out << "\n";
        out.indent([&] {
            out << "mOnewayQueue.start(3000 /* similar limit to binderized */);\n";
        });
    }
    out << "}\n\n";

    if (iface->hasOnewayMethods()) {
        out << "::android::hardware::Return<void> "
            << klassName
            << "::addOnewayTask(std::function<void(void)> fun) {\n";
        out.indent();
        out << "if (!mOnewayQueue.push(fun)) {\n";
        out.indent();
        out << "return ::android::hardware::Status::fromExceptionCode(\n";
        out.indent();
        out.indent();
        out << "::android::hardware::Status::EX_TRANSACTION_FAILED,\n"
            << "\"Passthrough oneway function queue exceeds maximum size.\");\n";
        out.unindent();
        out.unindent();
        out.unindent();
        out << "}\n";

        out << "return ::android::hardware::Status();\n";

        out.unindent();
        out << "}\n\n";


    }
}

void AST::generateCppAtraceCall(Formatter &out,
                                    InstrumentationEvent event,
                                    const Method *method) const {
    const Interface* iface = mRootScope.getInterface();
    std::string baseString = "HIDL::" + iface->localName() + "::" + method->name();
    switch (event) {
        case SERVER_API_ENTRY:
        {
            out << "atrace_begin(ATRACE_TAG_HAL, \""
                << baseString + "::server\");\n";
            break;
        }
        case PASSTHROUGH_ENTRY:
        {
            out << "atrace_begin(ATRACE_TAG_HAL, \""
                << baseString + "::passthrough\");\n";
            break;
        }
        case SERVER_API_EXIT:
        case PASSTHROUGH_EXIT:
        {
            out << "atrace_end(ATRACE_TAG_HAL);\n";
            break;
        }
        // client uses scope because of gotos
        // this isn't done for server because the profiled code isn't alone in its scope
        // this isn't done for passthrough becuase the profiled boundary isn't even in the same code
        case CLIENT_API_ENTRY: {
            out << "::android::ScopedTrace PASTE(___tracer, __LINE__) (ATRACE_TAG_HAL, \""
                << baseString + "::client\");\n";
            break;
        }
        case CLIENT_API_EXIT:
            break;
        default:
        {
            CHECK(false) << "Unsupported instrumentation event: " << event;
        }
    }
}

void AST::generateCppInstrumentationCall(
        Formatter &out,
        InstrumentationEvent event,
        const Method *method,
        const Interface* superInterface) const {
    generateCppAtraceCall(out, event, method);

    out << "#ifdef __ANDROID_DEBUGGABLE__\n";
    out << "if (UNLIKELY(mEnableInstrumentation)) {\n";
    out.indent();
    out << "std::vector<void *> _hidl_args;\n";
    std::string event_str = "";
    switch (event) {
        case SERVER_API_ENTRY:
        {
            event_str = "InstrumentationEvent::SERVER_API_ENTRY";
            for (const auto &arg : method->args()) {
                out << "_hidl_args.push_back((void *)"
                    << (arg->type().resultNeedsDeref() ? "" : "&")
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        case SERVER_API_EXIT:
        {
            event_str = "InstrumentationEvent::SERVER_API_EXIT";
            for (const auto &arg : method->results()) {
                out << "_hidl_args.push_back((void *)&_hidl_out_"
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        case CLIENT_API_ENTRY:
        {
            event_str = "InstrumentationEvent::CLIENT_API_ENTRY";
            for (const auto &arg : method->args()) {
                out << "_hidl_args.push_back((void *)&"
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        case CLIENT_API_EXIT:
        {
            event_str = "InstrumentationEvent::CLIENT_API_EXIT";
            for (const auto &arg : method->results()) {
                out << "_hidl_args.push_back((void *)"
                    << (arg->type().resultNeedsDeref() ? "" : "&")
                    << "_hidl_out_"
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        case PASSTHROUGH_ENTRY:
        {
            event_str = "InstrumentationEvent::PASSTHROUGH_ENTRY";
            for (const auto &arg : method->args()) {
                out << "_hidl_args.push_back((void *)&"
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        case PASSTHROUGH_EXIT:
        {
            event_str = "InstrumentationEvent::PASSTHROUGH_EXIT";
            for (const auto &arg : method->results()) {
                out << "_hidl_args.push_back((void *)&_hidl_out_"
                    << arg->name()
                    << ");\n";
            }
            break;
        }
        default:
        {
            CHECK(false) << "Unsupported instrumentation event: " << event;
        }
    }

    out << "for (const auto &callback: mInstrumentationCallbacks) {\n";
    out.indent();
    out << "callback("
        << event_str
        << ", \""
        << superInterface->fqName().package()
        << "\", \""
        << superInterface->fqName().version()
        << "\", \""
        << superInterface->localName()
        << "\", \""
        << method->name()
        << "\", &_hidl_args);\n";
    out.unindent();
    out << "}\n";
    out.unindent();
    out << "}\n";
    out << "#endif // __ANDROID_DEBUGGABLE__\n\n";
}

}  // namespace android
