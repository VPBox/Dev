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

#ifndef INTERFACE_H_

#define INTERFACE_H_

#include <vector>

#include <hidl-hash/Hash.h>

#include "ConstantExpression.h"
#include "Reference.h"
#include "Scope.h"

namespace android {

struct Method;
struct InterfaceAndMethod;

struct Interface : public Scope {
    const static std::unique_ptr<ConstantExpression> FLAG_ONE_WAY;

    Interface(const char* localName, const FQName& fullName, const Location& location,
              Scope* parent, const Reference<Type>& superType, const Hash* fileHash);

    const Hash* getFileHash() const;

    bool addMethod(Method *method);
    bool addAllReservedMethods();

    bool isElidableType() const override;
    bool isInterface() const override;
    bool isIBase() const { return fqName() == gIBaseFqName; }
    std::string typeName() const override;

    const Interface* superType() const;

    // Super type chain to root type.
    // First element is superType().
    std::vector<const Interface *> superTypeChain() const;
    // Super type chain to root type, including myself.
    // First element is this.
    std::vector<const Interface *> typeChain() const;

    // user defined methods (explicit definition in HAL files)
    const std::vector<Method *> &userDefinedMethods() const;
    // HIDL reserved methods (every interface has these implicitly defined)
    const std::vector<Method *> &hidlReservedMethods() const;
    // the sum of userDefinedMethods() and hidlReservedMethods().
    std::vector<Method *> methods() const;

    // userDefinedMethods() for all super type + methods()
    // The order will be as follows (in the transaction code order):
    // great-great-...-great-grand parent->userDefinedMethods()
    // ...
    // parent->userDefinedMethods()
    // this->userDefinedMethods()
    // this->hidlReservedMethods()
    std::vector<InterfaceAndMethod> allMethodsFromRoot() const;

    // allMethodsFromRoot for parent
    std::vector<InterfaceAndMethod> allSuperMethodsFromRoot() const;

    // aliases for corresponding methods in this->fqName()
    std::string getBaseName() const;
    std::string getAdapterName() const;
    std::string getProxyName() const;
    std::string getStubName() const;
    std::string getPassthroughName() const;
    std::string getHwName() const;
    FQName getProxyFqName() const;
    FQName getStubFqName() const;
    FQName getPassthroughFqName() const;

    std::string getCppType(
            StorageMode mode,
            bool specifyNamespaces) const override;

    std::string getJavaType(bool forInitializer) const override;
    std::string getVtsType() const override;

    std::vector<const Reference<Type>*> getReferences() const override;
    std::vector<const Reference<Type>*> getStrongReferences() const override;

    std::vector<const ConstantExpression*> getConstantExpressions() const override;

    status_t resolveInheritance() override;
    status_t validate() const override;
    status_t validateUniqueNames() const;
    status_t validateAnnotations() const;

    void emitReaderWriter(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const override;

    void emitPackageTypeDeclarations(Formatter& out) const override;
    void emitPackageTypeHeaderDefinitions(Formatter& out) const override;
    void emitTypeDefinitions(Formatter& out, const std::string& prefix) const override;

    void getAlignmentAndSize(size_t* align, size_t* size) const override;
    void emitJavaReaderWriter(
            Formatter &out,
            const std::string &parcelObj,
            const std::string &argName,
            bool isReader) const override;

    void emitVtsAttributeType(Formatter& out) const override;

    void emitVtsAttributeDeclaration(Formatter& out) const;
    void emitVtsMethodDeclaration(Formatter& out, bool isInherited) const;

    bool hasOnewayMethods() const;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const override;

    bool isNeverStrongReference() const override;

   private:
    Reference<Type> mSuperType;

    std::vector<Method*> mUserMethods;
    std::vector<Method*> mReservedMethods;

    const Hash* mFileHash;

    bool fillPingMethod(Method* method) const;
    bool fillDescriptorChainMethod(Method* method) const;
    bool fillGetDescriptorMethod(Method* method) const;
    bool fillHashChainMethod(Method* method) const;
    bool fillSyspropsChangedMethod(Method* method) const;
    bool fillLinkToDeathMethod(Method* method) const;
    bool fillUnlinkToDeathMethod(Method* method) const;
    bool fillSetHALInstrumentationMethod(Method* method) const;
    bool fillGetDebugInfoMethod(Method* method) const;
    bool fillDebugMethod(Method* method) const;

    void emitDigestChain(
        Formatter& out, const std::string& prefix, const std::vector<const Interface*>& chain,
        std::function<std::string(std::unique_ptr<ConstantExpression>)> byteToString) const;

    DISALLOW_COPY_AND_ASSIGN(Interface);
};

// An interface / method tuple.
struct InterfaceAndMethod {
    InterfaceAndMethod(const Interface *iface, Method *method)
        : mInterface(iface),
          mMethod(method) {}
    Method *method() const { return mMethod; }
    const Interface *interface() const { return mInterface; }

   private:
    // do not own these objects.
    const Interface *mInterface;
    Method *mMethod;
};

}  // namespace android

#endif  // INTERFACE_H_

