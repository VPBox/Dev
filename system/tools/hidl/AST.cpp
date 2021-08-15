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

#include "Coordinator.h"
#include "EnumType.h"
#include "FmqType.h"
#include "HandleType.h"
#include "Interface.h"
#include "Location.h"
#include "Scope.h"
#include "TypeDef.h"

#include <android-base/logging.h>
#include <hidl-util/FQName.h>
#include <hidl-util/Formatter.h>
#include <hidl-util/StringHelper.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

namespace android {

AST::AST(const Coordinator* coordinator, const Hash* fileHash)
    : mCoordinator(coordinator),
      mFileHash(fileHash),
      mRootScope("(root scope)", FQName(), Location::startOf(fileHash->getPath()),
                 nullptr /* parent */) {}

Scope* AST::getRootScope() {
    return &mRootScope;
}

// used by the parser.
void AST::addSyntaxError() {
    mSyntaxErrors++;
}

size_t AST::syntaxErrors() const {
    return mSyntaxErrors;
}

const std::string& AST::getFilename() const {
    return mFileHash->getPath();
}
const Hash* AST::getFileHash() const {
    return mFileHash;
}

bool AST::setPackage(const char *package) {
    if (!mPackage.setTo(package)) {
        return false;
    }

    if (mPackage.package().empty()
            || mPackage.version().empty()
            || !mPackage.name().empty()) {
        return false;
    }

    return true;
}

FQName AST::package() const {
    return mPackage;
}

bool AST::isInterface() const {
    return mRootScope.getInterface() != nullptr;
}

bool AST::definesInterfaces() const {
    return mRootScope.definesInterfaces();
}

status_t AST::postParse() {
    status_t err;

    // lookupTypes is the first pass for references to be resolved.
    err = lookupTypes();
    if (err != OK) return err;

    // Indicate that all types are now in "postParse" stage.
    err = setParseStage(Type::ParseStage::PARSE, Type::ParseStage::POST_PARSE);
    if (err != OK) return err;

    // validateDefinedTypesUniqueNames is the first call
    // after lookup, as other errors could appear because
    // user meant different type than we assumed.
    err = validateDefinedTypesUniqueNames();
    if (err != OK) return err;
    // topologicalReorder is before resolveInheritance, as we
    // need to have no cycle while getting parent class.
    err = topologicalReorder();
    if (err != OK) return err;
    err = resolveInheritance();
    if (err != OK) return err;
    err = lookupConstantExpressions();
    if (err != OK) return err;
    // checkAcyclicConstantExpressions is after resolveInheritance,
    // as resolveInheritance autofills enum values.
    err = checkAcyclicConstantExpressions();
    if (err != OK) return err;
    err = validateConstantExpressions();
    if (err != OK) return err;
    err = evaluateConstantExpressions();
    if (err != OK) return err;
    err = validate();
    if (err != OK) return err;
    err = checkForwardReferenceRestrictions();
    if (err != OK) return err;
    err = gatherReferencedTypes();
    if (err != OK) return err;

    // Make future packages not to call passes
    // for processed types and expressions
    constantExpressionRecursivePass(
        [](ConstantExpression* ce) {
            ce->setPostParseCompleted();
            return OK;
        },
        true /* processBeforeDependencies */);

    err = setParseStage(Type::ParseStage::POST_PARSE, Type::ParseStage::COMPLETED);
    if (err != OK) return err;

    return OK;
}

status_t AST::constantExpressionRecursivePass(
    const std::function<status_t(ConstantExpression*)>& func, bool processBeforeDependencies) {
    std::unordered_set<const Type*> visitedTypes;
    std::unordered_set<const ConstantExpression*> visitedCE;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE,
                                    [&](Type* type) -> status_t {
                                        for (auto* ce : type->getConstantExpressions()) {
                                            status_t err = ce->recursivePass(
                                                func, &visitedCE, processBeforeDependencies);
                                            if (err != OK) return err;
                                        }
                                        return OK;
                                    },
                                    &visitedTypes);
}

status_t AST::constantExpressionRecursivePass(
    const std::function<status_t(const ConstantExpression*)>& func,
    bool processBeforeDependencies) const {
    std::unordered_set<const Type*> visitedTypes;
    std::unordered_set<const ConstantExpression*> visitedCE;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE,
                                    [&](const Type* type) -> status_t {
                                        for (auto* ce : type->getConstantExpressions()) {
                                            status_t err = ce->recursivePass(
                                                func, &visitedCE, processBeforeDependencies);
                                            if (err != OK) return err;
                                        }
                                        return OK;
                                    },
                                    &visitedTypes);
}

status_t AST::setParseStage(Type::ParseStage oldStage, Type::ParseStage newStage) {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(oldStage,
                                    [oldStage, newStage](Type* type) {
                                        CHECK(type->getParseStage() == oldStage);
                                        type->setParseStage(newStage);
                                        return OK;
                                    },
                                    &visited);
}

status_t AST::lookupTypes() {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(
        Type::ParseStage::PARSE,
        [&](Type* type) -> status_t {
            Scope* scope = type->isScope() ? static_cast<Scope*>(type) : type->parent();

            for (auto* nextRef : type->getReferences()) {
                if (nextRef->isResolved()) {
                    continue;
                }

                Type* nextType = lookupType(nextRef->getLookupFqName(), scope);
                if (nextType == nullptr) {
                    std::cerr << "ERROR: Failed to lookup type '"
                              << nextRef->getLookupFqName().string() << "' at "
                              << nextRef->location() << "\n";
                    return UNKNOWN_ERROR;
                }
                nextRef->set(nextType);
            }

            return OK;
        },
        &visited);
}

status_t AST::gatherReferencedTypes() {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(
        Type::ParseStage::POST_PARSE,
        [&](Type* type) -> status_t {
            for (auto* nextRef : type->getReferences()) {
                const Type *targetType = nextRef->get();
                if (targetType->isNamedType()) {
                    mReferencedTypeNames.insert(
                            static_cast<const NamedType *>(targetType)->fqName());
                }
            }

            return OK;
        },
        &visited);
}

status_t AST::lookupConstantExpressions() {
    std::unordered_set<const Type*> visitedTypes;
    std::unordered_set<const ConstantExpression*> visitedCE;

    return mRootScope.recursivePass(
        Type::ParseStage::POST_PARSE,
        [&](Type* type) -> status_t {
            Scope* scope = type->isScope() ? static_cast<Scope*>(type) : type->parent();

            for (auto* ce : type->getConstantExpressions()) {
                status_t err = ce->recursivePass(
                    [&](ConstantExpression* ce) {
                        for (auto* nextRef : ce->getReferences()) {
                            if (nextRef->isResolved()) continue;

                            LocalIdentifier* iden = lookupLocalIdentifier(*nextRef, scope);
                            if (iden == nullptr) return UNKNOWN_ERROR;
                            nextRef->set(iden);
                        }
                        for (auto* nextRef : ce->getTypeReferences()) {
                            if (nextRef->isResolved()) continue;

                            Type* nextType = lookupType(nextRef->getLookupFqName(), scope);
                            if (nextType == nullptr) {
                                std::cerr << "ERROR: Failed to lookup type '"
                                          << nextRef->getLookupFqName().string() << "' at "
                                          << nextRef->location() << "\n";
                                return UNKNOWN_ERROR;
                            }
                            nextRef->set(nextType);
                        }
                        return OK;
                    },
                    &visitedCE, true /* processBeforeDependencies */);
                if (err != OK) return err;
            }

            return OK;
        },
        &visitedTypes);
}

status_t AST::validateDefinedTypesUniqueNames() const {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(
        Type::ParseStage::POST_PARSE,
        [&](const Type* type) -> status_t {
            // We only want to validate type definition names in this place.
            if (type->isScope()) {
                return static_cast<const Scope*>(type)->validateUniqueNames();
            }
            return OK;
        },
        &visited);
}

status_t AST::resolveInheritance() {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE, &Type::resolveInheritance,
                                    &visited);
}

status_t AST::validateConstantExpressions() const {
    return constantExpressionRecursivePass(
        [](const ConstantExpression* ce) { return ce->validate(); },
        true /* processBeforeDependencies */);
}

status_t AST::evaluateConstantExpressions() {
    return constantExpressionRecursivePass(
        [](ConstantExpression* ce) {
            ce->evaluate();
            return OK;
        },
        false /* processBeforeDependencies */);
}

status_t AST::validate() const {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE, &Type::validate, &visited);
}

status_t AST::topologicalReorder() {
    std::unordered_map<const Type*, size_t> reversedOrder;
    std::unordered_set<const Type*> stack;
    status_t err = mRootScope.topologicalOrder(&reversedOrder, &stack).status;
    if (err != OK) return err;

    std::unordered_set<const Type*> visited;
    mRootScope.recursivePass(Type::ParseStage::POST_PARSE,
                             [&](Type* type) {
                                 if (type->isScope()) {
                                     static_cast<Scope*>(type)->topologicalReorder(reversedOrder);
                                 }
                                 return OK;
                             },
                             &visited);
    return OK;
}

status_t AST::checkAcyclicConstantExpressions() const {
    std::unordered_set<const Type*> visitedTypes;
    std::unordered_set<const ConstantExpression*> visitedCE;
    std::unordered_set<const ConstantExpression*> stack;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE,
                                    [&](const Type* type) -> status_t {
                                        for (auto* ce : type->getConstantExpressions()) {
                                            status_t err =
                                                ce->checkAcyclic(&visitedCE, &stack).status;
                                            CHECK(err != OK || stack.empty());
                                            if (err != OK) return err;
                                        }
                                        return OK;
                                    },
                                    &visitedTypes);
}

status_t AST::checkForwardReferenceRestrictions() const {
    std::unordered_set<const Type*> visited;
    return mRootScope.recursivePass(Type::ParseStage::POST_PARSE,
                                    [](const Type* type) -> status_t {
                                        for (const Reference<Type>* ref : type->getReferences()) {
                                            status_t err =
                                                type->checkForwardReferenceRestrictions(*ref);
                                            if (err != OK) return err;
                                        }
                                        return OK;
                                    },
                                    &visited);
}

bool AST::addImport(const char *import) {
    FQName fqName;
    if (!FQName::parse(import, &fqName)) {
        std::cerr << "ERROR: '" << import << "' is an invalid fully-qualified name." << std::endl;
        return false;
    }

    fqName.applyDefaults(mPackage.package(), mPackage.version());

    if (fqName.name().empty()) {
        // import a package

        std::vector<FQName> packageInterfaces;

        status_t err =
            mCoordinator->appendPackageInterfacesToVector(fqName,
                                                          &packageInterfaces);

        if (err != OK) {
            return false;
        }

        for (const auto &subFQName : packageInterfaces) {
            addToImportedNamesGranular(subFQName);

            // Do not enforce restrictions on imports.
            AST* ast = mCoordinator->parse(subFQName, &mImportedASTs, Coordinator::Enforce::NONE);
            if (ast == nullptr) {
                return false;
            }
            // all previous single type imports are ignored.
            mImportedTypes.erase(ast);
        }

        return true;
    }

    addToImportedNamesGranular(fqName);

    // cases like android.hardware.foo@1.0::IFoo.Internal
    //            android.hardware.foo@1.0::Abc.Internal

    // assume it is an interface, and try to import it.
    const FQName interfaceName = fqName.getTopLevelType();
    // Do not enforce restrictions on imports.
    AST* importAST;
    status_t err = mCoordinator->parseOptional(interfaceName, &importAST, &mImportedASTs,
                                               Coordinator::Enforce::NONE);
    if (err != OK) return false;
    // importAST nullptr == file doesn't exist

    if (importAST != nullptr) {
        // cases like android.hardware.foo@1.0::IFoo.Internal
        //        and android.hardware.foo@1.0::IFoo
        if (fqName == interfaceName) {
            // import a single file.
            // all previous single type imports are ignored.
            // cases like android.hardware.foo@1.0::IFoo
            //        and android.hardware.foo@1.0::types
            mImportedTypes.erase(importAST);
            return true;
        }

        // import a single type from this file
        // cases like android.hardware.foo@1.0::IFoo.Internal
        FQName matchingName;
        Type *match = importAST->findDefinedType(fqName, &matchingName);
        if (match == nullptr) {
            return false;
        }
        // will automatically create a set if it does not exist
        mImportedTypes[importAST].insert(match);
        return true;
    }

    // probably a type in types.hal, like android.hardware.foo@1.0::Abc.Internal
    FQName typesFQName = fqName.getTypesForPackage();

    // Do not enforce restrictions on imports.
    importAST = mCoordinator->parse(typesFQName, &mImportedASTs, Coordinator::Enforce::NONE);

    if (importAST != nullptr) {
        // Attempt to find Abc.Internal in types.
        FQName matchingName;
        Type *match = importAST->findDefinedType(fqName, &matchingName);
        if (match == nullptr) {
            return false;
        }
        // will automatically create a set if not exist
        mImportedTypes[importAST].insert(match);
        return true;
    }

    // can't find an appropriate AST for fqName.
    return false;
}

void AST::addImportedAST(AST *ast) {
    mImportedASTs.insert(ast);
}

FQName AST::makeFullName(const char* localName, Scope* scope) const {
    std::vector<std::string> pathComponents{{localName}};
    for (; scope != &mRootScope; scope = scope->parent()) {
        pathComponents.push_back(scope->localName());
    }

    std::reverse(pathComponents.begin(), pathComponents.end());
    std::string path = StringHelper::JoinStrings(pathComponents, ".");

    return FQName(mPackage.package(), mPackage.version(), path);
}

void AST::addScopedType(NamedType* type, Scope* scope) {
    scope->addType(type);
    mDefinedTypesByFullName[type->fqName()] = type;
}

LocalIdentifier* AST::lookupLocalIdentifier(const Reference<LocalIdentifier>& ref, Scope* scope) {
    const FQName& fqName = ref.getLookupFqName();

    if (fqName.isIdentifier()) {
        LocalIdentifier* iden = scope->lookupIdentifier(fqName.name());
        if (iden == nullptr) {
            std::cerr << "ERROR: identifier " << fqName.string() << " could not be found at "
                      << ref.location() << "\n";
            return nullptr;
        }
        return iden;
    } else {
        std::string errorMsg;
        EnumValue* enumValue = lookupEnumValue(fqName, &errorMsg, scope);
        if (enumValue == nullptr) {
            std::cerr << "ERROR: " << errorMsg << " at " << ref.location() << "\n";
            return nullptr;
        }
        return enumValue;
    }
}

EnumValue* AST::lookupEnumValue(const FQName& fqName, std::string* errorMsg, Scope* scope) {
    FQName enumTypeName = fqName.typeName();
    std::string enumValueName = fqName.valueName();

    CHECK(!enumValueName.empty());

    Type* type = lookupType(enumTypeName, scope);
    if(type == nullptr) {
        *errorMsg = "Cannot find type " + enumTypeName.string();
        return nullptr;
    }
    type = type->resolve();
    if(!type->isEnum()) {
        *errorMsg = "Type " + enumTypeName.string() + " is not an enum type";
        return nullptr;
    }

    EnumType *enumType = static_cast<EnumType *>(type);
    EnumValue *v = static_cast<EnumValue *>(enumType->lookupIdentifier(enumValueName));
    if(v == nullptr) {
        *errorMsg = "Enum type " + enumTypeName.string() + " does not have " + enumValueName;
        return nullptr;
    }

    mReferencedTypeNames.insert(enumType->fqName());

    return v;
}

Type* AST::lookupType(const FQName& fqName, Scope* scope) {
    if (fqName.name().empty()) {
        // Given a package and version???
        return nullptr;
    }

    Type *returnedType = nullptr;

    if (fqName.package().empty() && fqName.version().empty()) {
        // resolve locally first if possible.
        returnedType = lookupTypeLocally(fqName, scope);
        if (returnedType != nullptr) {
            return returnedType;
        }
    }

    status_t status = lookupAutofilledType(fqName, &returnedType);
    if (status != OK) {
        return nullptr;
    }
    if (returnedType != nullptr) {
        return returnedType;
    }

    return lookupTypeFromImports(fqName);
}

// Rule 0: try resolve locally
Type* AST::lookupTypeLocally(const FQName& fqName, Scope* scope) {
    CHECK(fqName.package().empty() && fqName.version().empty()
        && !fqName.name().empty() && fqName.valueName().empty());

    for (; scope != nullptr; scope = scope->parent()) {
        Type* type = scope->lookupType(fqName);
        if (type != nullptr) {
            return type;
        }
    }

    return nullptr;
}

// Rule 1: auto-fill with current package
status_t AST::lookupAutofilledType(const FQName &fqName, Type **returnedType) {
    CHECK(!fqName.name().empty() && fqName.valueName().empty());

    FQName autofilled = fqName;
    autofilled.applyDefaults(mPackage.package(), mPackage.version());
    FQName matchingName;
    // Given this fully-qualified name, the type may be defined in this AST, or other files
    // in import.
    Type *local = findDefinedType(autofilled, &matchingName);
    CHECK(local == nullptr || autofilled == matchingName);
    Type* fromImport = lookupTypeFromImports(autofilled);

    if (local != nullptr && fromImport != nullptr && local != fromImport) {
        // Something bad happen; two types have the same FQName.
        std::cerr << "ERROR: Unable to resolve type name '"
                  << fqName.string()
                  << "' (i.e. '"
                  << autofilled.string()
                  << "'), multiple definitions found.\n";

        return UNKNOWN_ERROR;
    }
    if (local != nullptr) {
        *returnedType = local;
        return OK;
    }
    // If fromImport is nullptr as well, return nullptr to fall through to next rule.
    *returnedType = fromImport;
    return OK;
}

// Rule 2: look at imports
Type *AST::lookupTypeFromImports(const FQName &fqName) {

    Type *resolvedType = nullptr;
    Type *returnedType = nullptr;
    FQName resolvedName;

    for (const auto &importedAST : mImportedASTs) {
        if (mImportedTypes.find(importedAST) != mImportedTypes.end()) {
            // ignore single type imports
            continue;
        }
        FQName matchingName;
        Type *match = importedAST->findDefinedType(fqName, &matchingName);

        if (match != nullptr) {
            if (resolvedType != nullptr) {
                std::cerr << "ERROR: Unable to resolve type name '"
                          << fqName.string()
                          << "', multiple matches found:\n";

                std::cerr << "  " << resolvedName.string() << "\n";
                std::cerr << "  " << matchingName.string() << "\n";

                return nullptr;
            }

            resolvedType = match;
            returnedType = resolvedType;
            resolvedName = matchingName;

            // Keep going even after finding a match.
        }
    }

    for (const auto &pair : mImportedTypes) {
        AST *importedAST = pair.first;
        std::set<Type *> importedTypes = pair.second;

        FQName matchingName;
        Type *match = importedAST->findDefinedType(fqName, &matchingName);
        if (match != nullptr &&
                importedTypes.find(match) != importedTypes.end()) {
            if (resolvedType != nullptr) {
                std::cerr << "ERROR: Unable to resolve type name '"
                          << fqName.string()
                          << "', multiple matches found:\n";

                std::cerr << "  " << resolvedName.string() << "\n";
                std::cerr << "  " << matchingName.string() << "\n";

                return nullptr;
            }

            resolvedType = match;
            returnedType = resolvedType;
            resolvedName = matchingName;

            // Keep going even after finding a match.
        }
    }

    if (resolvedType) {
        returnedType = resolvedType;

        // If the resolved type is not an interface, we need to determine
        // whether it is defined in types.hal, or in some other interface.  In
        // the latter case, we need to emit a dependency for the interface in
        // which the type is defined.
        //
        // Consider the following:
        //    android.hardware.tests.foo@1.0::Record
        //    android.hardware.tests.foo@1.0::IFoo.Folder
        //    android.hardware.tests.foo@1.0::Folder
        //
        // If Record is an interface, then we keep track of it for the purpose
        // of emitting dependencies in the target language (for example #include
        // in C++).  If Record is a UDT, then we assume it is defined in
        // types.hal in android.hardware.tests.foo@1.0.
        //
        // In the case of IFoo.Folder, the same applies.  If IFoo is an
        // interface, we need to track this for the purpose of emitting
        // dependencies.  If not, then it must have been defined in types.hal.
        //
        // In the case of just specifying Folder, the resolved type is
        // android.hardware.tests.foo@1.0::Folder, and the same logic as
        // above applies.

        if (!resolvedType->isInterface()) {
            FQName ifc = resolvedName.getTopLevelType();
            for (const auto &importedAST : mImportedASTs) {
                FQName matchingName;
                Type *match = importedAST->findDefinedType(ifc, &matchingName);
                if (match != nullptr && match->isInterface()) {
                    resolvedType = match;
                }
            }
        }

        if (!resolvedType->isInterface()) {
            // Non-interface types are declared in the associated types header.
            FQName typesName = resolvedName.getTypesForPackage();

            mImportedNames.insert(typesName);
        } else {
            // Do _not_ use fqName, i.e. the name we used to look up the type,
            // but instead use the name of the interface we found.
            // This is necessary because if fqName pointed to a typedef which
            // in turn referenced the found interface we'd mistakenly use the
            // name of the typedef instead of the proper name of the interface.

            const FQName &typeName =
                static_cast<Interface *>(resolvedType)->fqName();

            mImportedNames.insert(typeName);
        }
    }

    return returnedType;
}

void AST::addToImportedNamesGranular(const FQName &fqName) {
    if (fqName.package() == package().package()
            && fqName.version() == package().version()) {
        // Our own names are _defined_ here, not imported.
        return;
    }

    mImportedNamesGranular.insert(fqName);
}

Type *AST::findDefinedType(const FQName &fqName, FQName *matchingName) const {
    for (const auto &pair : mDefinedTypesByFullName) {
        const FQName &key = pair.first;
        Type* type = pair.second;

        if (key.endsWith(fqName)) {
            *matchingName = key;
            return type;
        }
    }

    return nullptr;
}

void AST::getImportedPackages(std::set<FQName> *importSet) const {
    for (const auto& fqName : mImportedNamesGranular) {
        FQName packageName = fqName.getPackageAndVersion();

        if (packageName == mPackage) {
            // We only care about external imports, not our own package.
            continue;
        }

        importSet->insert(packageName);
    }
}

void AST::getImportedPackagesHierarchy(std::set<FQName> *importSet) const {
    getImportedPackages(importSet);

    std::set<FQName> newSet;
    for (const auto &ast : mImportedASTs) {
        if (importSet->find(ast->package()) != importSet->end()) {
            ast->getImportedPackagesHierarchy(&newSet);
        }
    }
    importSet->insert(newSet.begin(), newSet.end());
}

void AST::getAllImportedNames(std::set<FQName> *allImportNames) const {
    for (const auto& name : mImportedNames) {
        allImportNames->insert(name);
        AST* ast = mCoordinator->parse(name, nullptr /* imported */, Coordinator::Enforce::NONE);
        ast->getAllImportedNames(allImportNames);
    }
}

void AST::getAllImportedNamesGranular(std::set<FQName> *allImportNames) const {
    for (const auto& fqName : mImportedNamesGranular) {
        if (fqName.name() == "types") {
            // A package will export everything _defined_ but will not
            // re-export anything it itself imported.
            AST* ast = mCoordinator->parse(
                    fqName, nullptr /* imported */, Coordinator::Enforce::NONE);

            ast->addDefinedTypes(allImportNames);
        } else {
            allImportNames->insert(fqName);
        }
    }
}

bool AST::isJavaCompatible() const {
    return mRootScope.isJavaCompatible();
}

void AST::appendToExportedTypesVector(
        std::vector<const Type *> *exportedTypes) const {
    mRootScope.appendToExportedTypesVector(exportedTypes);
}

bool AST::isIBase() const {
    Interface* iface = mRootScope.getInterface();
    return iface != nullptr && iface->isIBase();
}

const Interface *AST::getInterface() const {
    return mRootScope.getInterface();
}

std::string AST::getBaseName() const {
    const Interface* iface = mRootScope.getInterface();

    return iface ? iface->getBaseName() : "types";
}

void AST::addDefinedTypes(std::set<FQName> *definedTypes) const {
    std::for_each(
            mDefinedTypesByFullName.begin(),
            mDefinedTypesByFullName.end(),
            [definedTypes](const auto &elem) {
                if (!elem.second->isTypeDef()) {
                    definedTypes->insert(elem.first);
                }
            });
}

void AST::addReferencedTypes(std::set<FQName> *referencedTypes) const {
    std::for_each(
            mReferencedTypeNames.begin(),
            mReferencedTypeNames.end(),
            [referencedTypes](const auto &fqName) {
                referencedTypes->insert(fqName);
            });
}

}  // namespace android;
