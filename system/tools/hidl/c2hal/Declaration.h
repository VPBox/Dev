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

#ifndef DECLARATION_H_
#define DECLARATION_H_

#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include <hidl-util/Formatter.h>

namespace android {

struct AST;

struct Declaration {
    Declaration(const std::string &name);
    virtual ~Declaration();

    const std::string &getName() const;
    virtual void setName(const std::string &name);

    void forceCamelCase();
    void forcePascalCase();
    void forceUpperSnakeCase();

    const std::string& getComment() const;
    void setComment(const std::string &comment);

    std::string getInterfaceName() const;

    void generateCommentText(Formatter &out) const;

    virtual const std::string decType() const = 0;

    /* for example "int test;" */
    virtual void generateSource(Formatter &out) const = 0;

    /* for example "int test" in fun(int test, float jeff) */
    virtual void generateParameterSource(Formatter &out) const;

    virtual void processContents(AST &ast) = 0;

private:
    std::string mName;
    std::string mComment;

    DISALLOW_COPY_AND_ASSIGN(Declaration);
};

}  // namespace android

#endif  // DECLARATION_H_
