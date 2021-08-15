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

#ifndef AST_H_
#define AST_H_

#include "Scope.h"
#include "Expression.h"

#include <hidl-util/Formatter.h>
#include <android-base/macros.h>
#include <android-base/logging.h>
#include <string>
#include <vector>
#include <utils/Errors.h>

namespace android {

struct Include;
struct Define;
struct CompositeDeclaration;
struct Declaration;

struct AST {
    AST(const std::string &path,
        const std::string &outputDir,
        const std::string &package,
        bool isOpenGl);
    ~AST();

    void *scanner();
    void setScanner(void *scanner);

    bool isOpenGl() const;

    const std::string &getFilename() const;

    void setDeclarations(std::vector<Declaration *> *declarations);
    void setIncludes(std::vector<Include *> *includes);

    Expression *getExpression() const;
    void setExpression(Expression *expression);

    status_t generateCode() const;

    void processContents();

    const Scope<Define *> &getDefinesScope() const;
    Scope<Define *> &getDefinesScope();

private:
    void * mScanner = nullptr;
    std::string mPath;
    std::string mOutputDir;
    std::string mPackage;

    bool mIsOpenGl;

    Expression* mExpression = nullptr;

    std::vector<Declaration *> *mDeclarations = nullptr;
    std::vector<CompositeDeclaration *> *mInterfaces = nullptr;
    std::vector<Include *> *mIncludes = nullptr;

    Scope<Define *> mDefinesScope;

    const std::string getFileDir() const;

    status_t generateFile(CompositeDeclaration* declaration) const;
    status_t generateTypesFile() const;

    void generateIncludes(Formatter &out) const;
    void generatePackageLine(Formatter &out) const;

    void isolateInterfaces();
    void isolateGlobalInterface();
    void isolateIncludes();
    void isolateConstants(Expression::Type ofType);

    DISALLOW_COPY_AND_ASSIGN(AST);
};

}  // namespace android

#endif  // AST_H_
