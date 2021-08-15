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

#ifndef ENUM_VAR_DECLARATION_H_
#define ENUM_VAR_DECLARATION_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include "Declaration.h"

namespace android {

struct Expression;

struct EnumVarDeclaration : Declaration {
    EnumVarDeclaration(const std::string &name, Expression *expression);
    ~EnumVarDeclaration();

    void setName(const std::string &name) override;
    static std::string type() { return "enum"; }
    const std::string decType() const override { return type(); }

    Expression *getExpression() const;

    void generateSource(Formatter &out) const override;
    void processContents(AST &ast) override;

private:
    Expression *mExpression = nullptr;

    DISALLOW_COPY_AND_ASSIGN(EnumVarDeclaration);
};

}  // namespace android

#endif  // ENUM_VAR_DECLARATION_H_
