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

#ifndef DEFINE_H_
#define DEFINE_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include "Declaration.h"
#include "Expression.h"

namespace android {

struct Define : Declaration {
    Define(const std::string &name, const std::string &slurp);
    ~Define();

    static std::string type() { return "define"; }
    const std::string decType() const override { return type(); }

    void generateSource(Formatter &out) const override;
    void processContents(AST &ast) override;

    Expression::Type getExpressionType() const;
    Expression *getExpression() const;
    void setExpression(Expression* expression);

private:
    const std::string mSlurp;

    Expression::Type mExpressionType;
    Expression* mExpression = nullptr;

    DISALLOW_COPY_AND_ASSIGN(Define);
};

}  // namespace android

#endif  // DEFINE_H_