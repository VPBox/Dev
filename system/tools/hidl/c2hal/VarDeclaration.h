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

#ifndef VARDECLARATION_H_
#define VARDECLARATION_H_

#include "Declaration.h"
#include "Type.h"

namespace android {

struct VarDeclaration : Declaration{
    VarDeclaration(Type *type, const std::string &name);
    ~VarDeclaration();

    void setName(const std::string &name) override;

    Type* getType() const;

    static std::string type() { return "var"; }
    const std::string decType() const override { return type(); }

    void generateSource(Formatter &out) const override;
    void generateParameterSource(Formatter &out) const override;
    void processContents(AST &ast) override;

private:
    Type *mType;

    DISALLOW_COPY_AND_ASSIGN(VarDeclaration);
};

}  // namespace android

#endif  // VARDECLARATION_H_