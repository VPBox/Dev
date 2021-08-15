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

#include "Interface.h"

#include "Annotation.h"
#include "ArrayType.h"
#include "ConstantExpression.h"
#include "DeathRecipientType.h"
#include "Method.h"
#include "ScalarType.h"
#include "StringType.h"
#include "VectorType.h"

#include <unistd.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include <android-base/logging.h>
#include <hidl-util/Formatter.h>
#include <hidl-util/StringHelper.h>

namespace android {

#define B_PACK_CHARS(c1, c2, c3, c4) \
         ((((c1)<<24)) | (((c2)<<16)) | (((c3)<<8)) | (c4))

/* It is very important that these values NEVER change. These values
 * must remain unchanged over the lifetime of android. This is
 * because the framework on a device will be updated independently of
 * the hals on a device. If the hals are compiled with one set of
 * transaction values, and the framework with another, then the
 * interface between them will be destroyed, and the device will not
 * work.
 */
enum {
    /////////////////// User defined transactions
    FIRST_CALL_TRANSACTION  = 0x00000001,
    LAST_CALL_TRANSACTION   = 0x0effffff,
    /////////////////// HIDL reserved
    FIRST_HIDL_TRANSACTION  = 0x0f000000,
    HIDL_PING_TRANSACTION                     = B_PACK_CHARS(0x0f, 'P', 'N', 'G'),
    HIDL_DESCRIPTOR_CHAIN_TRANSACTION         = B_PACK_CHARS(0x0f, 'C', 'H', 'N'),
    HIDL_GET_DESCRIPTOR_TRANSACTION           = B_PACK_CHARS(0x0f, 'D', 'S', 'C'),
    HIDL_SYSPROPS_CHANGED_TRANSACTION         = B_PACK_CHARS(0x0f, 'S', 'Y', 'S'),
    HIDL_LINK_TO_DEATH_TRANSACTION            = B_PACK_CHARS(0x0f, 'L', 'T', 'D'),
    HIDL_UNLINK_TO_DEATH_TRANSACTION          = B_PACK_CHARS(0x0f, 'U', 'T', 'D'),
    HIDL_SET_HAL_INSTRUMENTATION_TRANSACTION  = B_PACK_CHARS(0x0f, 'I', 'N', 'T'),
    HIDL_GET_REF_INFO_TRANSACTION             = B_PACK_CHARS(0x0f, 'R', 'E', 'F'),
    HIDL_DEBUG_TRANSACTION                    = B_PACK_CHARS(0x0f, 'D', 'B', 'G'),
    HIDL_HASH_CHAIN_TRANSACTION               = B_PACK_CHARS(0x0f, 'H', 'S', 'H'),
    LAST_HIDL_TRANSACTION   = 0x0fffffff,
};

const std::unique_ptr<ConstantExpression> Interface::FLAG_ONE_WAY =
    std::make_unique<LiteralConstantExpression>(ScalarType::KIND_UINT32, 0x01, "oneway");

Interface::Interface(const char* localName, const FQName& fullName, const Location& location,
                     Scope* parent, const Reference<Type>& superType, const Hash* fileHash)
    : Scope(localName, fullName, location, parent), mSuperType(superType), mFileHash(fileHash) {}

std::string Interface::typeName() const {
    return "interface " + localName();
}

const Hash* Interface::getFileHash() const {
    return mFileHash;
}

bool Interface::fillPingMethod(Method *method) const {
    if (method->name() != "ping") {
        return false;
    }

    method->fillImplementation(
        HIDL_PING_TRANSACTION,
        {
            {IMPL_INTERFACE,
                [](auto &out) {
                    out << "return ::android::hardware::Void();\n";
                }
            },
            {IMPL_STUB_IMPL,
                [](auto &out) {
                    out << "return ::android::hardware::Void();\n";
                }
            }
        }, /*cppImpl*/
        {
            {IMPL_INTERFACE,
                [](auto &out) {
                    out << "return;\n";
                }
            },
        } /*javaImpl*/
    );

    return true;
}

bool Interface::fillLinkToDeathMethod(Method *method) const {
    if (method->name() != "linkToDeath") {
        return false;
    }

    method->fillImplementation(
            HIDL_LINK_TO_DEATH_TRANSACTION,
            {
                {IMPL_INTERFACE,
                    [](auto &out) {
                        out << "(void)cookie;\n"
                            << "return (recipient != nullptr);\n";
                    }
                },
                {IMPL_PROXY,
                    [](auto &out) {
                        out << "::android::hardware::ProcessState::self()->startThreadPool();\n";
                        out << "::android::hardware::hidl_binder_death_recipient *binder_recipient"
                            << " = new ::android::hardware::hidl_binder_death_recipient(recipient, cookie, this);\n"
                            << "std::unique_lock<std::mutex> lock(_hidl_mMutex);\n"
                            << "_hidl_mDeathRecipients.push_back(binder_recipient);\n"
                            << "return (remote()->linkToDeath(binder_recipient)"
                            << " == ::android::OK);\n";
                    }
                },
                {IMPL_STUB, nullptr}
            }, /*cppImpl*/
            {
                {IMPL_INTERFACE,
                    [](auto &out) {
                        out << "return true;\n";
                    }
                },
                {IMPL_PROXY,
                    [](auto &out) {
                        out << "return mRemote.linkToDeath(recipient, cookie);\n";
                    }
                },
                {IMPL_STUB, nullptr}
            } /*javaImpl*/
    );
    return true;
}

bool Interface::fillUnlinkToDeathMethod(Method *method) const {
    if (method->name() != "unlinkToDeath") {
        return false;
    }

    method->fillImplementation(
            HIDL_UNLINK_TO_DEATH_TRANSACTION,
            {
                {IMPL_INTERFACE,
                    [](auto &out) {
                        out << "return (recipient != nullptr);\n";
                    }
                },
                {IMPL_PROXY,
                    [](auto &out) {
                        out << "std::unique_lock<std::mutex> lock(_hidl_mMutex);\n"
                            << "for (auto it = _hidl_mDeathRecipients.rbegin();"
                            << "it != _hidl_mDeathRecipients.rend();"
                            << "++it) {\n";
                        out.indent([&] {
                            out.sIf("(*it)->getRecipient() == recipient", [&] {
                                out << "::android::status_t status = remote()->unlinkToDeath(*it);\n"
                                    << "_hidl_mDeathRecipients.erase(it.base()-1);\n"
                                    << "return status == ::android::OK;\n";
                                });
                            }).endl();
                        out << "}\n";
                        out << "return false;\n";
                    }
                },
                {IMPL_STUB, nullptr /* don't generate code */}
            }, /*cppImpl*/
            {
                {IMPL_INTERFACE,
                    [](auto &out) {
                        out << "return true;\n";
                    }
                },
                {IMPL_PROXY,
                    [](auto &out) {
                        out << "return mRemote.unlinkToDeath(recipient);\n";
                    }
                },
                {IMPL_STUB, nullptr /* don't generate code */}
            } /*javaImpl*/
    );
    return true;
}
bool Interface::fillSyspropsChangedMethod(Method *method) const {
    if (method->name() != "notifySyspropsChanged") {
        return false;
    }

    method->fillImplementation(
            HIDL_SYSPROPS_CHANGED_TRANSACTION,
            { { IMPL_INTERFACE, [](auto &out) {
                out << "::android::report_sysprop_change();\n";
                out << "return ::android::hardware::Void();\n";
            } } }, /*cppImpl */
            { { IMPL_INTERFACE, [](auto &out) { /* javaImpl */
                out << "android.os.HwBinder.enableInstrumentation();\n";
            } } } /*javaImpl */
    );
    return true;
}

bool Interface::fillSetHALInstrumentationMethod(Method *method) const {
    if (method->name() != "setHALInstrumentation") {
        return false;
    }

    method->fillImplementation(
            HIDL_SET_HAL_INSTRUMENTATION_TRANSACTION,
            {
                {IMPL_INTERFACE,
                    [](auto &out) {
                        // do nothing for base class.
                        out << "return ::android::hardware::Void();\n";
                    }
                },
                {IMPL_STUB,
                    [](auto &out) {
                        out << "configureInstrumentation();\n";
                    }
                },
                {IMPL_PASSTHROUGH,
                    [](auto &out) {
                        out << "configureInstrumentation();\n";
                        out << "return ::android::hardware::Void();\n";
                    }
                },
            }, /*cppImpl */
            { { IMPL_INTERFACE, [](auto & /*out*/) { /* javaImpl */
                // Not support for Java Impl for now.
            } } } /*javaImpl */
    );
    return true;
}

bool Interface::fillDescriptorChainMethod(Method *method) const {
    if (method->name() != "interfaceChain") {
        return false;
    }

    method->fillImplementation(
        HIDL_DESCRIPTOR_CHAIN_TRANSACTION,
        { { IMPL_INTERFACE, [this](auto &out) {
            std::vector<const Interface *> chain = typeChain();
            out << "_hidl_cb(";
            out.block([&] {
                for (const Interface *iface : chain) {
                    out << iface->fullName() << "::descriptor,\n";
                }
            });
            out << ");\n";
            out << "return ::android::hardware::Void();";
        } } }, /* cppImpl */
        { { IMPL_INTERFACE, [this](auto &out) {
            std::vector<const Interface *> chain = typeChain();
            out << "return new java.util.ArrayList<String>(java.util.Arrays.asList(\n";
            out.indent(); out.indent();
            for (size_t i = 0; i < chain.size(); ++i) {
                if (i != 0)
                    out << ",\n";
                out << chain[i]->fullJavaName() << ".kInterfaceName";
            }
            out << "));\n";
            out.unindent(); out.unindent();
        } } } /* javaImpl */
    );
    return true;
}

void Interface::emitDigestChain(
    Formatter& out, const std::string& prefix, const std::vector<const Interface*>& chain,
    std::function<std::string(std::unique_ptr<ConstantExpression>)> byteToString) const {
    out.join(chain.begin(), chain.end(), ",\n", [&](const auto& iface) {
        out << prefix;
        out << "{";
        out.join(
            iface->getFileHash()->raw().begin(), iface->getFileHash()->raw().end(), ",",
            [&](const auto& e) {
                // Use ConstantExpression::cppValue / javaValue
                // because Java used signed byte for uint8_t.
                out << byteToString(ConstantExpression::ValueOf(ScalarType::Kind::KIND_UINT8, e));
            });
        out << "} /* ";
        out << iface->getFileHash()->hexString();
        out << " */";
    });
}

bool Interface::fillHashChainMethod(Method *method) const {
    if (method->name() != "getHashChain") {
        return false;
    }
    const VectorType *chainType = static_cast<const VectorType *>(&method->results()[0]->type());
    const ArrayType *digestType = static_cast<const ArrayType *>(chainType->getElementType());

    method->fillImplementation(
        HIDL_HASH_CHAIN_TRANSACTION,
        { { IMPL_INTERFACE, [this, digestType](auto &out) {
            std::vector<const Interface *> chain = typeChain();
            out << "_hidl_cb(";
            out.block([&] {
                emitDigestChain(out, "(" + digestType->getInternalDataCppType() + ")", chain,
                                [](const auto& e) { return e->cppValue(); });
            });
            out << ");\n";
            out << "return ::android::hardware::Void();\n";
        } } }, /* cppImpl */
        { { IMPL_INTERFACE, [this, digestType, chainType](auto &out) {
            std::vector<const Interface *> chain = typeChain();
            out << "return new "
                << chainType->getJavaType(false /* forInitializer */)
                << "(java.util.Arrays.asList(\n";
            out.indent(2, [&] {
                // No need for dimensions when elements are explicitly provided.
                emitDigestChain(out, "new " + digestType->getJavaType(false /* forInitializer */),
                                chain, [](const auto& e) { return e->javaValue(); });
            });
            out << "));\n";
        } } } /* javaImpl */
    );
    return true;
}

bool Interface::fillGetDescriptorMethod(Method *method) const {
    if (method->name() != "interfaceDescriptor") {
        return false;
    }

    method->fillImplementation(
        HIDL_GET_DESCRIPTOR_TRANSACTION,
        { { IMPL_INTERFACE, [this](auto &out) {
            out << "_hidl_cb("
                << fullName()
                << "::descriptor);\n"
                << "return ::android::hardware::Void();\n";
        } } }, /* cppImpl */
        { { IMPL_INTERFACE, [this](auto &out) {
            out << "return "
                << fullJavaName()
                << ".kInterfaceName;\n";
        } } } /* javaImpl */
    );
    return true;
}

bool Interface::fillGetDebugInfoMethod(Method *method) const {
    if (method->name() != "getDebugInfo") {
        return false;
    }

    static const std::string sArch =
            "#if defined(__LP64__)\n"
            "::android::hidl::base::V1_0::DebugInfo::Architecture::IS_64BIT\n"
            "#else\n"
            "::android::hidl::base::V1_0::DebugInfo::Architecture::IS_32BIT\n"
            "#endif\n";

    method->fillImplementation(
        HIDL_GET_REF_INFO_TRANSACTION,
        {
            {IMPL_INTERFACE,
                [](auto &out) {
                    // getDebugInfo returns N/A for local objects.
                    out << "::android::hidl::base::V1_0::DebugInfo info = {};\n";
                    out << "info.pid = -1;\n";
                    out << "info.ptr = 0;\n";
                    out << "info.arch = \n" << sArch << ";\n";
                    out << "_hidl_cb(info);\n";
                    out << "return ::android::hardware::Void();\n";
                }
            },
            {IMPL_STUB_IMPL,
                [](auto &out) {
                    out << "::android::hidl::base::V1_0::DebugInfo info = {};\n";
                    out << "info.pid = ::android::hardware::details::getPidIfSharable();\n";
                    out << "info.ptr = ::android::hardware::details::debuggable()"
                        << "? reinterpret_cast<uint64_t>(this) : 0;\n";
                    out << "info.arch = \n" << sArch << ";\n";
                    out << "_hidl_cb(info);\n";
                    out << "return ::android::hardware::Void();\n";
                }
            }
        }, /* cppImpl */
        { { IMPL_INTERFACE, [method](auto &out) {
            const Type &refInfo = method->results().front()->type();
            out << refInfo.getJavaType(false /* forInitializer */) << " info = new "
                << refInfo.getJavaType(true /* forInitializer */) << "();\n"
                << "info.pid = android.os.HidlSupport.getPidIfSharable();\n"
                << "info.ptr = 0;\n"
                << "info.arch = android.hidl.base.V1_0.DebugInfo.Architecture.UNKNOWN;\n"
                << "return info;\n";
        } } } /* javaImpl */
    );

    return true;
}

bool Interface::fillDebugMethod(Method *method) const {
    if (method->name() != "debug") {
        return false;
    }

    method->fillImplementation(HIDL_DEBUG_TRANSACTION,
                               {
                                   {IMPL_INTERFACE,
                                    [](auto& out) {
                                        out << "(void)fd;\n"
                                            << "(void)options;\n"
                                            << "return ::android::hardware::Void();\n";
                                    }},
                               }, /* cppImpl */
                               {
                                   {IMPL_INTERFACE, [](auto& out) { out << "return;\n"; }},
                               } /* javaImpl */
    );

    return true;
}

static std::map<std::string, Method *> gAllReservedMethods;

bool Interface::addMethod(Method *method) {
    if (isIBase()) {
        if (!gAllReservedMethods.emplace(method->name(), method).second) {
            std::cerr << "ERROR: hidl-gen encountered duplicated reserved method " << method->name()
                      << std::endl;
            return false;
        }
        // will add it in addAllReservedMethods
        return true;
    }

    CHECK(!method->isHidlReserved());
    mUserMethods.push_back(method);

    return true;
}

std::vector<const Reference<Type>*> Interface::getReferences() const {
    std::vector<const Reference<Type>*> ret;

    if (!isIBase()) {
        ret.push_back(&mSuperType);
    }

    for (const auto* method : methods()) {
        const auto& references = method->getReferences();
        ret.insert(ret.end(), references.begin(), references.end());
    }

    return ret;
}

std::vector<const ConstantExpression*> Interface::getConstantExpressions() const {
    std::vector<const ConstantExpression*> ret;
    for (const auto* method : methods()) {
        const auto& retMethod = method->getConstantExpressions();
        ret.insert(ret.end(), retMethod.begin(), retMethod.end());
    }
    return ret;
}

std::vector<const Reference<Type>*> Interface::getStrongReferences() const {
    // Interface is a special case as a reference:
    // its definiton must be completed for extension but
    // not necessary for other references.

    std::vector<const Reference<Type>*> ret;
    if (!isIBase()) {
        ret.push_back(&mSuperType);
    }

    for (const auto* method : methods()) {
        const auto& references = method->getStrongReferences();
        ret.insert(ret.end(), references.begin(), references.end());
    }

    return ret;
}

status_t Interface::resolveInheritance() {
    size_t serial = FIRST_CALL_TRANSACTION;
    for (const auto* ancestor : superTypeChain()) {
        serial += ancestor->mUserMethods.size();
    }

    for (Method* method : mUserMethods) {
        if (serial > LAST_CALL_TRANSACTION) {
            std::cerr << "ERROR: More than " << LAST_CALL_TRANSACTION
                      << " methods (including super and reserved) are not allowed at " << location()
                      << std::endl;
            return UNKNOWN_ERROR;
        }

        method->setSerialId(serial);
        serial++;
    }

    return Scope::resolveInheritance();
}

status_t Interface::validate() const {
    CHECK(isIBase() == mSuperType.isEmptyReference());

    if (!isIBase() && !mSuperType->isInterface()) {
        std::cerr << "ERROR: You can only extend interfaces at " << mSuperType.location()
                  << std::endl;
        return UNKNOWN_ERROR;
    }

    status_t err;

    err = validateUniqueNames();
    if (err != OK) return err;

    err = validateAnnotations();
    if (err != OK) return err;

    return Scope::validate();
}

void Interface::getAlignmentAndSize(size_t* align, size_t* size) const {
    *align = 8;
    *size = 8;
}

status_t Interface::validateUniqueNames() const {
    std::unordered_map<std::string, const Interface*> registeredMethodNames;
    for (auto const& tuple : allSuperMethodsFromRoot()) {
        // No need to check super method uniqueness
        registeredMethodNames[tuple.method()->name()] = tuple.interface();
    }

    for (const Method* method : mUserMethods) {
        auto registered = registeredMethodNames.find(method->name());

        if (registered != registeredMethodNames.end()) {
            const Interface* definedInType = registered->second;

            if (definedInType == this) {
                // Defined in this interface
                std::cerr << "ERROR: Redefinition of method '" << method->name() << "'";
            } else if (definedInType->isIBase()) {
                // Defined in IBase
                std::cerr << "ERROR: Redefinition of reserved method '" << method->name() << "'";
            } else {
                // Defined in super not IBase
                std::cerr << "ERROR: Redefinition of method '" << method->name()
                          << "' defined in interface '" << definedInType->fullName() << "'";
            }
            std::cerr << " at " << method->location() << std::endl;
            return UNKNOWN_ERROR;
        }

        registeredMethodNames[method->name()] = this;
    }

    return OK;
}

status_t Interface::validateAnnotations() const {
    for (const Method* method : methods()) {
        for (const Annotation* annotation : method->annotations()) {
            const std::string name = annotation->name();

            if (name == "entry" || name == "exit" || name == "callflow") {
                continue;
            }

            std::cerr << "ERROR: Unrecognized annotation '" << name
                      << "' for method: " << method->name() << ". An annotation should be one of: "
                      << "entry, exit, callflow." << std::endl;
            return UNKNOWN_ERROR;
        }
    }
    return OK;
}

bool Interface::addAllReservedMethods() {
    // use a sorted map to insert them in serial ID order.
    std::map<int32_t, Method *> reservedMethodsById;
    for (const auto &pair : gAllReservedMethods) {
        Method *method = pair.second->copySignature();
        bool fillSuccess = fillPingMethod(method)
            || fillDescriptorChainMethod(method)
            || fillGetDescriptorMethod(method)
            || fillHashChainMethod(method)
            || fillSyspropsChangedMethod(method)
            || fillLinkToDeathMethod(method)
            || fillUnlinkToDeathMethod(method)
            || fillSetHALInstrumentationMethod(method)
            || fillGetDebugInfoMethod(method)
            || fillDebugMethod(method);

        if (!fillSuccess) {
            std::cerr << "ERROR: hidl-gen does not recognize a reserved method " << method->name()
                      << std::endl;
            return false;
        }
        if (!reservedMethodsById.emplace(method->getSerialId(), method).second) {
            std::cerr << "ERROR: hidl-gen uses duplicated serial id for " << method->name()
                      << " and " << reservedMethodsById[method->getSerialId()]->name()
                      << ", serialId = " << method->getSerialId() << std::endl;
            return false;
        }
    }
    for (const auto &pair : reservedMethodsById) {
        this->mReservedMethods.push_back(pair.second);
    }
    return true;
}

const Interface* Interface::superType() const {
    if (isIBase()) return nullptr;
    if (!mSuperType->isInterface()) {
        // This is actually an error
        // that would be caught in validate
        return nullptr;
    }
    return static_cast<const Interface*>(mSuperType.get());
}

std::vector<const Interface *> Interface::typeChain() const {
    std::vector<const Interface *> v;
    const Interface *iface = this;
    while (iface != nullptr) {
        v.push_back(iface);
        iface = iface->superType();
    }
    return v;
}

std::vector<const Interface *> Interface::superTypeChain() const {
    return isIBase() ? std::vector<const Interface*>() : superType()->typeChain();
}

bool Interface::isElidableType() const {
    return true;
}

bool Interface::isInterface() const {
    return true;
}

const std::vector<Method *> &Interface::userDefinedMethods() const {
    return mUserMethods;
}

const std::vector<Method *> &Interface::hidlReservedMethods() const {
    return mReservedMethods;
}

std::vector<Method *> Interface::methods() const {
    std::vector<Method *> v(mUserMethods);
    v.insert(v.end(), mReservedMethods.begin(), mReservedMethods.end());
    return v;
}

std::vector<InterfaceAndMethod> Interface::allMethodsFromRoot() const {
    std::vector<InterfaceAndMethod> v;
    std::vector<const Interface *> chain = typeChain();
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        const Interface *iface = *it;
        for (Method *userMethod : iface->userDefinedMethods()) {
            v.push_back(InterfaceAndMethod(iface, userMethod));
        }
    }
    for (Method *reservedMethod : hidlReservedMethods()) {
        v.push_back(InterfaceAndMethod(
                *chain.rbegin(), // IBase
                reservedMethod));
    }
    return v;
}

std::vector<InterfaceAndMethod> Interface::allSuperMethodsFromRoot() const {
    return isIBase() ? std::vector<InterfaceAndMethod>() : superType()->allMethodsFromRoot();
}

std::string Interface::getBaseName() const {
    return fqName().getInterfaceBaseName();
}

std::string Interface::getAdapterName() const {
    return fqName().getInterfaceAdapterName();
}

std::string Interface::getProxyName() const {
    return fqName().getInterfaceProxyName();
}

std::string Interface::getStubName() const {
    return fqName().getInterfaceStubName();
}

std::string Interface::getHwName() const {
    return fqName().getInterfaceHwName();
}

std::string Interface::getPassthroughName() const {
    return fqName().getInterfacePassthroughName();
}

FQName Interface::getProxyFqName() const {
    return fqName().getInterfaceProxyFqName();
}

FQName Interface::getStubFqName() const {
    return fqName().getInterfaceStubFqName();
}

FQName Interface::getPassthroughFqName() const {
    return fqName().getInterfacePassthroughFqName();
}

std::string Interface::getCppType(StorageMode mode,
                                  bool specifyNamespaces) const {
    const std::string base =
          std::string(specifyNamespaces ? "::android::" : "")
        + "sp<"
        + fullName()
        + ">";

    switch (mode) {
        case StorageMode_Stack:
        case StorageMode_Result:
            return base;

        case StorageMode_Argument:
            return "const " + base + "&";
    }
}

std::string Interface::getJavaType(bool /* forInitializer */) const {
    return fullJavaName();
}

std::string Interface::getVtsType() const {
    if (StringHelper::EndsWith(localName(), "Callback")) {
        return "TYPE_HIDL_CALLBACK";
    } else {
        return "TYPE_HIDL_INTERFACE";
    }
}

void Interface::emitReaderWriter(
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

        const std::string binderName = "_hidl_binder";
        out << "::android::sp<::android::hardware::IBinder> "
            << binderName << ";\n";

        out << "_hidl_err = ";
        out << parcelObjDeref
            << "readNullableStrongBinder(&"
            << binderName
            << ");\n";

        handleError(out, mode);

        out << name
            << " = "
            << "::android::hardware::fromBinder<"
            << fqName().cppName()
            << ","
            << getProxyFqName().cppName()
            << ","
            << getStubFqName().cppName()
            << ">("
            << binderName
            << ");\n";

        out.unindent();
        out << "}\n\n";
    } else {
        out << "if (" << name << " == nullptr) {\n";
        out.indent();
        out << "_hidl_err = ";
        out << parcelObjDeref
            << "writeStrongBinder(nullptr);\n";
        out.unindent();
        out << "} else {\n";
        out.indent();
        out << "::android::sp<::android::hardware::IBinder> _hidl_binder = "
            << "::android::hardware::getOrCreateCachedBinder(" << name << ".get());\n";
        out << "if (_hidl_binder.get() != nullptr) {\n";
        out.indent([&] {
            out << "_hidl_err = "
                << parcelObjDeref
                << "writeStrongBinder(_hidl_binder);\n";
        });
        out << "} else {\n";
        out.indent([&] {
            out << "_hidl_err = ::android::UNKNOWN_ERROR;\n";
        });
        out << "}\n";
        out.unindent();
        out << "}\n";

        handleError(out, mode);
    }
}

void Interface::emitPackageTypeDeclarations(Formatter& out) const {
    Scope::emitPackageTypeDeclarations(out);

    out << "static inline std::string toString(" << getCppArgumentType() << " o);\n\n";
}

void Interface::emitPackageTypeHeaderDefinitions(Formatter& out) const {
    Scope::emitPackageTypeHeaderDefinitions(out);

    out << "static inline std::string toString(" << getCppArgumentType() << " o) ";

    out.block([&] {
        out << "std::string os = \"[class or subclass of \";\n"
            << "os += " << fullName() << "::descriptor;\n"
            << "os += \"]\";\n"
            << "os += o->isRemote() ? \"@remote\" : \"@local\";\n"
            << "return os;\n";
    }).endl().endl();
}

void Interface::emitTypeDefinitions(Formatter& out, const std::string& prefix) const {
    std::string space = prefix.empty() ? "" : (prefix + "::");

    Scope::emitTypeDefinitions(out, space + localName());
}

void Interface::emitJavaReaderWriter(
        Formatter &out,
        const std::string &parcelObj,
        const std::string &argName,
        bool isReader) const {
    if (isReader) {
        out << fullJavaName()
            << ".asInterface("
            << parcelObj
            << ".readStrongBinder());\n";
    } else {
        out << parcelObj
            << ".writeStrongBinder("
            << argName
            << " == null ? null : "
            << argName
            << ".asBinder());\n";
    }
}

void Interface::emitVtsAttributeDeclaration(Formatter& out) const {
    for (const auto &type : getSubTypes()) {
        // Skip for TypeDef as it is just an alias of a defined type.
        if (type->isTypeDef()) {
            continue;
        }
        out << "attribute: {\n";
        out.indent();
        type->emitVtsTypeDeclarations(out);
        out.unindent();
        out << "}\n\n";
    }
}

void Interface::emitVtsMethodDeclaration(Formatter& out, bool isInherited) const {
    for (const auto &method : methods()) {
        if (method->isHidlReserved()) {
            continue;
        }

        out << "api: {\n";
        out.indent();
        out << "name: \"" << method->name() << "\"\n";
        out << "is_inherited: " << (isInherited ? "true" : "false") << "\n";
        // Generate declaration for each return value.
        for (const auto &result : method->results()) {
            out << "return_type_hidl: {\n";
            out.indent();
            out << "name: \"" << result->name() << "\"\n";
            result->type().emitVtsAttributeType(out);
            out.unindent();
            out << "}\n";
        }
        // Generate declaration for each input argument
        for (const auto &arg : method->args()) {
            out << "arg: {\n";
            out.indent();
            out << "name: \"" << arg->name() << "\"\n";
            arg->type().emitVtsAttributeType(out);
            out.unindent();
            out << "}\n";
        }
        // Generate declaration for each annotation.
        for (const auto &annotation : method->annotations()) {
            out << "callflow: {\n";
            out.indent();
            const std::string name = annotation->name();
            if (name == "entry") {
                out << "entry: true\n";
            } else if (name == "exit") {
                out << "exit: true\n";
            } else if (name == "callflow") {
                const AnnotationParam *param =
                        annotation->getParam("next");
                if (param != nullptr) {
                    for (const auto& value : param->getValues()) {
                        out << "next: " << value << "\n";
                    }
                }
            } else {
                CHECK(false);
            }
            out.unindent();
            out << "}\n";
        }
        out.unindent();
        out << "}\n\n";
    }
}

void Interface::emitVtsAttributeType(Formatter& out) const {
    out << "type: " << getVtsType() << "\n"
        << "predefined_type: \""
        << fullName()
        << "\"\n";
}

bool Interface::hasOnewayMethods() const {
    for (auto const &method : methods()) {
        if (method->isOneway()) {
            return true;
        }
    }

    const Interface* superClass = superType();

    if (superClass != nullptr) {
        return superClass->hasOnewayMethods();
    }

    return false;
}

bool Interface::deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const {
    if (superType() != nullptr && !superType()->isJavaCompatible(visited)) {
        return false;
    }

    for (const auto* method : methods()) {
        if (!method->deepIsJavaCompatible(visited)) {
            return false;
        }
    }

    return Scope::deepIsJavaCompatible(visited);
}

bool Interface::isNeverStrongReference() const {
    return true;
}

}  // namespace android

