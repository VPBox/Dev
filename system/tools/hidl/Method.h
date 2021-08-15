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

#ifndef METHOD_H_

#define METHOD_H_

#include <android-base/macros.h>
#include <hidl-util/Formatter.h>
#include <utils/Errors.h>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "DocComment.h"
#include "Location.h"
#include "Reference.h"

namespace android {

struct Annotation;
struct ConstantExpression;
struct Formatter;
struct ScalarType;
struct Type;
struct TypedVarVector;

enum MethodImplType {
    IMPL_INTERFACE,
    IMPL_PROXY,
    IMPL_STUB, // overrides the code in onTransact; IMPL_STUB_IMPL will be ignored
    IMPL_STUB_IMPL, // use this->method() instead of mImpl->method()
    IMPL_PASSTHROUGH,
};

using MethodImpl = std::map<MethodImplType, std::function<void(Formatter &)>>;

struct Method : DocCommentable {
    Method(const char* name, std::vector<NamedReference<Type>*>* args,
           std::vector<NamedReference<Type>*>* results, bool oneway,
           std::vector<Annotation*>* annotations, const Location& location);

    std::string name() const;
    const std::vector<NamedReference<Type>*>& args() const;
    const std::vector<NamedReference<Type>*>& results() const;
    bool isOneway() const { return mOneway; }
    bool overridesCppImpl(MethodImplType type) const;
    bool overridesJavaImpl(MethodImplType type) const;
    void cppImpl(MethodImplType type, Formatter &out) const;
    void javaImpl(MethodImplType type, Formatter &out) const;
    bool isHidlReserved() const { return mIsHidlReserved; }
    const std::vector<Annotation *> &annotations() const;

    std::vector<Reference<Type>*> getReferences();
    std::vector<const Reference<Type>*> getReferences() const;

    std::vector<Reference<Type>*> getStrongReferences();
    std::vector<const Reference<Type>*> getStrongReferences() const;

    std::vector<ConstantExpression*> getConstantExpressions();
    std::vector<const ConstantExpression*> getConstantExpressions() const;

    // Make a copy with the same name, args, results, oneway, annotations.
    // Implementations, serial are not copied.
    Method *copySignature() const;

    void setSerialId(size_t serial);
    size_t getSerialId() const;

    // Fill implementation for HIDL reserved methods. mIsHidlReserved will be
    // set to true.
    void fillImplementation(
            size_t serial,
            MethodImpl cppImpl,
            MethodImpl javaImpl);

    void generateCppReturnType(Formatter &out, bool specifyNamespaces = true) const;
    void generateCppSignature(Formatter &out,
                              const std::string &className = "",
                              bool specifyNamespaces = true) const;

    bool hasEmptyCppArgSignature() const;
    void emitCppArgSignature(Formatter &out, bool specifyNamespaces = true) const;
    void emitCppResultSignature(Formatter &out, bool specifyNamespaces = true) const;

    void emitJavaArgSignature(Formatter &out) const;
    void emitJavaResultSignature(Formatter &out) const;

    const NamedReference<Type>* canElideCallback() const;

    void dumpAnnotations(Formatter &out) const;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const;

    const Location& location() const;

   private:
    std::string mName;
    size_t mSerial = 0;
    std::vector<NamedReference<Type>*>* mArgs;
    std::vector<NamedReference<Type>*>* mResults;
    bool mOneway;
    std::vector<Annotation *> *mAnnotations;

    bool mIsHidlReserved = false;
    // The following fields have no meaning if mIsHidlReserved is false.
    // hard-coded implementation for HIDL reserved methods.
    MethodImpl mCppImpl;
    MethodImpl mJavaImpl;

    const Location mLocation;

    DISALLOW_COPY_AND_ASSIGN(Method);
};

struct TypedVarVector : public std::vector<NamedReference<Type>*> {
    TypedVarVector() = default;

    bool add(NamedReference<Type>* v);

   private:
    std::set<std::string> mNames;
};

}  // namespace android

#endif  // METHOD_H_

