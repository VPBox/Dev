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

#ifndef SCOPE_H_

#define SCOPE_H_

#include "NamedType.h"

#include <map>
#include <unordered_map>
#include <vector>

namespace android {

struct Annotation;
struct ConstantExpression;
struct Formatter;
struct Interface;
struct LocalIdentifier;

struct Scope : public NamedType {
    Scope(const char* localName, const FQName& fullName, const Location& location, Scope* parent);
    virtual ~Scope();

    void addType(NamedType* type);

    status_t validateUniqueNames() const;

    // lookup a type given an FQName.
    // Assume fqName.package(), fqName.version(), fqName.valueName() is empty.
    NamedType *lookupType(const FQName &fqName) const;

    virtual LocalIdentifier *lookupIdentifier(const std::string &name) const;

    bool isScope() const override;

    // Returns the single interface or NULL.
    Interface *getInterface() const;

    bool definesInterfaces() const;

    const std::vector<Annotation*>& annotations() const;

    void setAnnotations(std::vector<Annotation*>* annotations);

    std::vector<const Type*> getDefinedTypes() const override;

    std::vector<const ConstantExpression*> getConstantExpressions() const override;

    void topologicalReorder(const std::unordered_map<const Type*, size_t>& reversedOrder);

    void emitTypeDeclarations(Formatter& out) const override;
    void emitGlobalTypeDeclarations(Formatter& out) const override;
    void emitPackageTypeDeclarations(Formatter& out) const override;
    void emitPackageTypeHeaderDefinitions(Formatter& out) const override;
    void emitPackageHwDeclarations(Formatter& out) const override;

    void emitJavaTypeDeclarations(Formatter& out, bool atTopLevel) const override;

    void emitTypeDefinitions(Formatter& out, const std::string& prefix) const override;

    const std::vector<NamedType *> &getSubTypes() const;

    void emitVtsTypeDeclarations(Formatter& out) const override;

    bool deepIsJavaCompatible(std::unordered_set<const Type*>* visited) const override;

    void appendToExportedTypesVector(
            std::vector<const Type *> *exportedTypes) const override;

   private:
    std::vector<NamedType *> mTypes;
    std::map<std::string, size_t> mTypeIndexByName;
    std::vector<Annotation*> mAnnotations;

    bool mTypeOrderChanged = false;

    DISALLOW_COPY_AND_ASSIGN(Scope);
};

struct RootScope : public Scope {
    RootScope(const char* localName, const FQName& fullName, const Location& location,
              Scope* parent);
    virtual ~RootScope();

    virtual status_t validate() const override;

    std::string typeName() const override;
};

struct LocalIdentifier {
    LocalIdentifier();
    virtual ~LocalIdentifier();
    virtual bool isEnumValue() const;

    const LocalIdentifier* resolve() const;
    LocalIdentifier* resolve();

    virtual ConstantExpression* constExpr() const;
};

}  // namespace android

#endif  // SCOPE_H_

