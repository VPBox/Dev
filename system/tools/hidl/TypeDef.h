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

#ifndef TYPE_DEF_H_

#define TYPE_DEF_H_

#include "NamedType.h"

namespace android {

struct TypeDef : public NamedType {
    TypeDef(const char* localName, const FQName& fullName, const Location& location, Scope* parent,
            const Reference<Type>& type);

    const ScalarType *resolveToScalarType() const override;

    std::string typeName() const override;

    Type* referencedType();
    const Type* referencedType() const;

    bool isInterface() const override;
    bool isEnum() const override;
    bool isTypeDef() const override;
    bool needsEmbeddedReadWrite() const override;
    bool resultNeedsDeref() const override;

    const Type* resolve() const override;

    std::vector<const Reference<Type>*> getReferences() const override;

    void emitTypeDeclarations(Formatter& out) const override;

   private:
    Reference<Type> mReferencedType;

    DISALLOW_COPY_AND_ASSIGN(TypeDef);
};

}  // namespace android

#endif  // TYPE_DEF_H_

