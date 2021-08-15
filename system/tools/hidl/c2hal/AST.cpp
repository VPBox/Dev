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

#include "AST.h"

#include "FunctionDeclaration.h"
#include "EnumVarDeclaration.h"
#include "Scope.h"
#include "Declaration.h"
#include "CompositeDeclaration.h"
#include "VarDeclaration.h"
#include "Define.h"
#include "Include.h"
#include "Note.h"

#include <string>
#include <algorithm>
#include <stdlib.h>
#include <sys/stat.h>

namespace android {

AST::AST(const std::string &path,
         const std::string &outputDir,
         const std::string &package,
         bool isOpenGl)
    : mScanner(nullptr),
      mPath(path),
      mOutputDir(outputDir),
      mPackage(package),
      mIsOpenGl(isOpenGl)
    {}

AST::~AST() {
    delete mExpression;

    if(mDeclarations != nullptr) {
        for(auto* decl : *mDeclarations) {
            delete decl;
        }
    }
    delete mDeclarations;

    if(mInterfaces != nullptr) {
        for(auto* inter : *mInterfaces) {
            delete inter;
        }
    }
    delete mInterfaces;

    if(mIncludes != nullptr) {
        for(auto* incl : *mIncludes) {
            delete incl;
        }
    }
    delete mIncludes;
}

void *AST::scanner() {
    return mScanner;
}

void AST::setScanner(void *scanner) {
    mScanner = scanner;
}

bool AST::isOpenGl() const {
    return mIsOpenGl;
}

const std::string& AST::getFilename() const {
    return mPath;
}

void AST::setDeclarations(std::vector<Declaration *> *declarations) {
    // on the top level, no var declarations are allowed.
    for(size_t i = 0; i < declarations->size(); i++) {
        if(declarations->at(i)->decType() == VarDeclaration::type()) {
            declarations->at(i) = new Note(declarations->at(i));
        }
    }

    mDeclarations = declarations;
}

void AST::setIncludes(std::vector<Include *> *includes) {
    mIncludes = includes;
}

Expression *AST::getExpression() const {
    return mExpression;
}
void AST::setExpression(Expression *expression) {
    mExpression = expression;
}

const Scope<Define *> &AST::getDefinesScope() const {
    return mDefinesScope;
}

Scope<Define *> &AST::getDefinesScope() {
    return mDefinesScope;
}

void AST::processContents() {
    CHECK(mDeclarations != nullptr);

    for (auto &declaration : *mDeclarations) {
        CHECK(declaration != nullptr);

        declaration->processContents(*this);
    }

    isolateInterfaces();
    isolateGlobalInterface();
    isolateIncludes();

    isolateConstants(Expression::Type::U64);
    isolateConstants(Expression::Type::S64);
    isolateConstants(Expression::Type::U32);
    isolateConstants(Expression::Type::S32);
}

/* take interface-like structs out of the type file */
void AST::isolateInterfaces() {
    mInterfaces = new std::vector<CompositeDeclaration*>;

    auto it = mDeclarations->begin();
    while (it != mDeclarations->end()) {
        if ((*it)->decType() == CompositeDeclaration::type()
            && ((CompositeDeclaration *) (*it))->isInterface()) {

            mInterfaces->push_back((CompositeDeclaration *) *it);
            it = mDeclarations->erase(it);
        } else {
            it++;
        }
    }
}

/* take global function declarations out of the type file and into a new
 * interface
 */
void AST::isolateGlobalInterface() {
    auto globalFuns = new std::vector<Declaration*>;

    auto it = mDeclarations->begin();
    while (it != mDeclarations->end()) {
        if ((*it)->decType() == FunctionDeclaration::type()) {

            globalFuns->push_back(*it);
            it = mDeclarations->erase(it);
        } else {
            it++;
        }
    }

    if (!globalFuns->empty()) {
        std::string path = mPackage.substr(0, mPackage.find_first_of('@'));
        std::string name = path.substr(path.find_last_of('.') + 1);

        auto interface = new CompositeDeclaration(
            Type::Qualifier::STRUCT,
            name + "_global_t",
            globalFuns);

        mInterfaces->push_back(interface);
    }
}

void AST::isolateIncludes() {
    mIncludes = new std::vector<Include*>;

    auto it = mDeclarations->begin();
    while (it != mDeclarations->end()) {
        if ((*it)->decType() == Include::type()) {

            mIncludes->push_back((Include *) *it);
            it = mDeclarations->erase(it);
        } else {
            it++;
        }
    }
}

void AST::isolateConstants(Expression::Type ofType) {
    auto constants = new std::vector<Declaration*>;

    auto it = mDeclarations->begin();
    while (it != mDeclarations->end()) {
        if ((*it)->decType() == Define::type() &&
            ((Define *)*it)->getExpressionType() == ofType) {

            Define* define = (Define *)*it;

            auto var = new EnumVarDeclaration(define->getName(),
                                              define->getExpression());

            define->setExpression(nullptr);

            constants->push_back(var);
            it = mDeclarations->erase(it);

            delete define;
        } else {
            it++;
        }
    }

    if (!constants->empty()) {
        auto constEnum = new CompositeDeclaration(
            Type::Qualifier::ENUM,
            "Const" + Expression::getTypeDescription(ofType),
            constants);

        constEnum->setEnumTypeName(Expression::getTypeName(ofType));

        mDeclarations->insert(mDeclarations->begin(), constEnum);
    }
}

status_t AST::generateCode() const {
    CHECK(mDeclarations != nullptr);

    status_t err;

    for (auto &interface : *mInterfaces) {
        err = generateFile(interface);

        if (err != OK) {
            return err;
        }
    }

    err = generateTypesFile();

    if (err != OK) {
        return err;
    }

    return OK;
}

status_t AST::generateFile(CompositeDeclaration* declaration) const {
    std::string fileName = declaration->getInterfaceName() + ".hal";

    FILE *file = fopen((getFileDir() + fileName).c_str(), "w");

    if(file == nullptr) {
        return -errno;
    }

    Formatter out(file); // formatter closes out

    generatePackageLine(out);
    generateIncludes(out);

    declaration->generateInterface(out);

    return OK;
}

status_t AST::generateTypesFile() const {
    if (mDeclarations->empty()) {
        return OK;
    }

    FILE *file = fopen((getFileDir() + "types.hal").c_str(), "w");

    if(file == nullptr) {
        return -errno;
    }

    Formatter out(file); // formatter closes out

    generatePackageLine(out);
    generateIncludes(out);

    for (auto &declaration : *mDeclarations) {
        declaration->generateCommentText(out);
        declaration->generateSource(out);
        out << "\n";
    }

    return OK;
}

void AST::generateIncludes(Formatter &out) const {
    for (auto &include : *mIncludes) {
        include->generateSource(out);
        out << "\n";
    }
}

void AST::generatePackageLine(Formatter &out) const {
    out << "package "
        << mPackage
        << ";\n\n";
}

bool MakeParentHierarchy(const std::string &path) {
    static const mode_t kMode = 0755;

    size_t start = 1;  // Ignore leading '/'
    size_t slashPos;
    while ((slashPos = path.find('/', start)) != std::string::npos) {
        std::string partial = path.substr(0, slashPos);

        struct stat st;
        if (stat(partial.c_str(), &st) < 0) {
            if (errno != ENOENT) {
                return false;
            }

            int res = mkdir(partial.c_str(), kMode);
            if (res < 0) {
                return false;
            }
        } else if (!S_ISDIR(st.st_mode)) {
            return false;
        }

        start = slashPos + 1;
    }

    return true;
}

const std::string AST::getFileDir() const {
    CHECK(MakeParentHierarchy(mOutputDir));
    return mOutputDir;
}

} // namespace android
