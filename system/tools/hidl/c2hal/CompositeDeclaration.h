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

#ifndef COMPOSITE_DECLARATION_H_
#define COMPOSITE_DECLARATION_H_

#include "Declaration.h"
#include "Type.h"

namespace android {

struct CompositeDeclaration : Declaration {
    CompositeDeclaration(
        const Type::Qualifier::Qualification qualifier,
        const std::string &name,
        std::vector<android::Declaration *> *fieldDeclarations);
    ~CompositeDeclaration();

    void setName(const std::string &name) override;

    const Type::Qualifier::Qualification &getQualifier() const;
    const std::vector<android::Declaration *>* getFieldDeclarations() const;

    static std::string type() { return "composite"; }
    const std::string decType() const override { return type(); }

    void generateSource(Formatter &out) const override;
    void processContents(AST &ast) override;

    void generateInterface(Formatter &out) const;
    std::string getInterfaceName() const;
    bool isInterface() const;

    void setEnumTypeName(const std::string &name);

private:
    const Type::Qualifier::Qualification mQualifier;
    std::vector<android::Declaration *> *mFieldDeclarations;

    std::string mEnumTypeName;

    void generateBody(Formatter &out) const;

    DISALLOW_COPY_AND_ASSIGN(CompositeDeclaration);
};

}  // namespace android

#endif  // COMPOSITE_DECLARATION_H_