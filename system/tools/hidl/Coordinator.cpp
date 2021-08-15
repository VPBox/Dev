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

#include "Coordinator.h"

#include <dirent.h>
#include <sys/stat.h>

#include <algorithm>
#include <iterator>

#include <android-base/logging.h>
#include <hidl-hash/Hash.h>
#include <hidl-util/StringHelper.h>
#include <iostream>

#include "AST.h"
#include "Interface.h"
#include "hidl-gen_l.h"

static bool existdir(const char *name) {
    DIR *dir = opendir(name);
    if (dir == nullptr) {
        return false;
    }
    closedir(dir);
    return true;
}

namespace android {

const std::string &Coordinator::getRootPath() const {
    return mRootPath;
}

void Coordinator::setRootPath(const std::string &rootPath) {
    mRootPath = rootPath;

    if (!mRootPath.empty() && !StringHelper::EndsWith(mRootPath, "/")) {
        mRootPath += "/";
    }
}

void Coordinator::setOutputPath(const std::string& outputPath) {
    mOutputPath = outputPath;
}

void Coordinator::setVerbose(bool verbose) {
    mVerbose = verbose;
}

bool Coordinator::isVerbose() const {
    return mVerbose;
}

void Coordinator::setDepFile(const std::string& depFile) {
    mDepFile = depFile;
}

const std::string& Coordinator::getOwner() const {
    return mOwner;
}
void Coordinator::setOwner(const std::string& owner) {
    mOwner = owner;
}

status_t Coordinator::addPackagePath(const std::string& root, const std::string& path, std::string* error) {
    FQName package = FQName(root, "0.0", "");
    for (const PackageRoot &packageRoot : mPackageRoots) {
        if (packageRoot.root.inPackage(root) || package.inPackage(packageRoot.root.package())) {
            if (error != nullptr) {
                *error = "ERROR: conflicting package roots " +
                         packageRoot.root.package() +
                         " and " +
                         root;
            }

            return UNKNOWN_ERROR;
        }
    }

    mPackageRoots.push_back({path, package});
    return OK;
}
void Coordinator::addDefaultPackagePath(const std::string& root, const std::string& path) {
    addPackagePath(root, path, nullptr /* error */);
}

Formatter Coordinator::getFormatter(const FQName& fqName, Location location,
                                    const std::string& fileName) const {
    if (location == Location::STANDARD_OUT) {
        return Formatter(stdout);
    }

    std::string filepath;
    status_t err = getFilepath(fqName, location, fileName, &filepath);
    if (err != OK) {
        return Formatter::invalid();
    }

    onFileAccess(filepath, "w");

    if (!Coordinator::MakeParentHierarchy(filepath)) {
        fprintf(stderr, "ERROR: could not make directories for %s.\n", filepath.c_str());
        return Formatter::invalid();
    }

    FILE* file = fopen(filepath.c_str(), "w");

    if (file == nullptr) {
        fprintf(stderr, "ERROR: could not open file %s: %d\n", filepath.c_str(), errno);
        return Formatter::invalid();
    }

    return Formatter(file);
}

status_t Coordinator::getFilepath(const FQName& fqName, Location location,
                                  const std::string& fileName, std::string* path) const {
    status_t err;
    std::string packagePath;
    std::string packageRootPath;

    switch (location) {
        case Location::DIRECT: { /* nothing */
            *path = mOutputPath + fileName;
        } break;
        case Location::PACKAGE_ROOT: {
            err = getPackagePath(fqName, false /* relative */, false /* sanitized */, &packagePath);
            if (err != OK) return err;

            *path = mOutputPath + packagePath + fileName;
        } break;
        case Location::GEN_OUTPUT: {
            err = convertPackageRootToPath(fqName, &packageRootPath);
            if (err != OK) return err;
            err = getPackagePath(fqName, true /* relative */, false /* sanitized */, &packagePath);
            if (err != OK) return err;

            *path = mOutputPath + packageRootPath + packagePath + fileName;
        } break;
        case Location::GEN_SANITIZED: {
            err = convertPackageRootToPath(fqName, &packageRootPath);
            if (err != OK) return err;
            err = getPackagePath(fqName, true /* relative */, true /* sanitized */, &packagePath);
            if (err != OK) return err;

            *path = mOutputPath + packageRootPath + packagePath + fileName;
        } break;
        default: { CHECK(false) << "Invalid location: " << static_cast<size_t>(location); }
    }

    return OK;
}

void Coordinator::onFileAccess(const std::string& path, const std::string& mode) const {
    if (mode == "r") {
        // This is a global list. It's not cleared when a second fqname is processed for
        // two reasons:
        // 1). If there is a bug in hidl-gen, the dependencies on the first project from
        //     the second would be required to recover correctly when the bug is fixed.
        // 2). This option is never used in Android builds.
        mReadFiles.insert(StringHelper::LTrim(path, mRootPath));
    }

    if (!mVerbose) {
        return;
    }

    fprintf(stderr,
            "VERBOSE: file access %s %s\n", path.c_str(), mode.c_str());
}

status_t Coordinator::writeDepFile(const std::string& forFile) const {
    // No dep file requested
    if (mDepFile.empty()) return OK;

    onFileAccess(mDepFile, "w");

    FILE* file = fopen(mDepFile.c_str(), "w");
    if (file == nullptr) {
        fprintf(stderr, "ERROR: could not open dep file at %s.\n", mDepFile.c_str());
        return UNKNOWN_ERROR;
    }

    Formatter out(file, 2 /* spacesPerIndent */);
    out << StringHelper::LTrim(forFile, mOutputPath) << ": \\\n";
    out.indent([&] {
        for (const std::string& file : mReadFiles) {
            out << StringHelper::LTrim(file, mRootPath) << " \\\n";
        }
    });
    return OK;
}

AST* Coordinator::parse(const FQName& fqName, std::set<AST*>* parsedASTs,
                        Enforce enforcement) const {
    AST* ret;
    status_t err = parseOptional(fqName, &ret, parsedASTs, enforcement);
    if (err != OK) CHECK(ret == nullptr);  // internal consistency

    // only in a handful of places do we want to distinguish between
    // a missing file and a bad AST. Everywhere else, we just want to
    // throw an error if we expect an AST to be present but it is not.
    return ret;
}

status_t Coordinator::parseOptional(const FQName& fqName, AST** ast, std::set<AST*>* parsedASTs,
                                    Enforce enforcement) const {
    CHECK(fqName.isFullyQualified());

    auto it = mCache.find(fqName);
    if (it != mCache.end()) {
        *ast = (*it).second;

        if (*ast != nullptr && parsedASTs != nullptr) {
            parsedASTs->insert(*ast);
        }

        if (*ast == nullptr) {
            // circular import OR that AST has errors in it
            return UNKNOWN_ERROR;
        }

        return OK;
    }

    // Add this to the cache immediately, so we can discover circular imports.
    mCache[fqName] = nullptr;

    AST *typesAST = nullptr;

    if (fqName.name() != "types") {
        // Any interface file implicitly imports its package's types.hal.
        FQName typesName = fqName.getTypesForPackage();
        // Do not enforce on imports. Do not add imports' imports to this AST.
        status_t err = parseOptional(typesName, &typesAST, nullptr, Enforce::NONE);
        if (err != OK) return err;

        // fall through.
    }

    std::string packagePath;
    status_t err =
        getPackagePath(fqName, false /* relative */, false /* sanitized */, &packagePath);
    if (err != OK) return err;

    const std::string path = makeAbsolute(packagePath + fqName.name() + ".hal");

    *ast = new AST(this, &Hash::getHash(path));

    if (typesAST != nullptr) {
        // If types.hal for this AST's package existed, make it's defined
        // types available to the (about to be parsed) AST right away.
        (*ast)->addImportedAST(typesAST);
    }

    std::unique_ptr<FILE, std::function<void(FILE*)>> file(fopen(path.c_str(), "rb"), fclose);

    if (file == nullptr) {
        mCache.erase(fqName);  // nullptr in cache is used to find circular imports
        delete *ast;
        *ast = nullptr;
        return OK;  // File does not exist, nullptr AST* == file doesn't exist.
    }

    onFileAccess(path, "r");

    // parse file takes ownership of file
    if (parseFile(*ast, std::move(file)) != OK || (*ast)->postParse() != OK) {
        delete *ast;
        *ast = nullptr;
        return UNKNOWN_ERROR;
    }

    if ((*ast)->package().package() != fqName.package() ||
        (*ast)->package().version() != fqName.version()) {
        fprintf(stderr,
                "ERROR: File at '%s' does not match expected package and/or "
                "version.\n",
                path.c_str());

        err = UNKNOWN_ERROR;
    } else {
        if ((*ast)->isInterface()) {
            if (fqName.name() == "types") {
                fprintf(stderr,
                        "ERROR: File at '%s' declares an interface '%s' "
                        "instead of the expected types common to the package.\n",
                        path.c_str(), (*ast)->getInterface()->localName().c_str());

                err = UNKNOWN_ERROR;
            } else if ((*ast)->getInterface()->localName() != fqName.name()) {
                fprintf(stderr,
                        "ERROR: File at '%s' does not declare interface type "
                        "'%s'.\n",
                        path.c_str(),
                        fqName.name().c_str());

                err = UNKNOWN_ERROR;
            }
        } else if (fqName.name() != "types") {
            fprintf(stderr,
                    "ERROR: File at '%s' declares types rather than the "
                    "expected interface type '%s'.\n",
                    path.c_str(),
                    fqName.name().c_str());

            err = UNKNOWN_ERROR;
        } else if ((*ast)->definesInterfaces()) {
            fprintf(stderr,
                    "ERROR: types.hal file at '%s' declares at least one "
                    "interface type.\n",
                    path.c_str());

            err = UNKNOWN_ERROR;
        }
    }

    if (err != OK) {
        delete *ast;
        *ast = nullptr;
        return err;
    }

    if (parsedASTs != nullptr) {
        parsedASTs->insert(*ast);
    }

    // put it into the cache now, so that enforceRestrictionsOnPackage can
    // parse fqName.
    mCache[fqName] = *ast;

    // For each .hal file that hidl-gen parses, the whole package will be checked.
    err = enforceRestrictionsOnPackage(fqName, enforcement);
    if (err != OK) {
        mCache[fqName] = nullptr;
        delete *ast;
        *ast = nullptr;
        return err;
    }

    return OK;
}

const Coordinator::PackageRoot* Coordinator::findPackageRoot(const FQName& fqName) const {
    CHECK(!fqName.package().empty());

    // Find the right package prefix and path for this FQName.  For
    // example, if FQName is "android.hardware.nfc@1.0::INfc", and the
    // prefix:root is set to [ "android.hardware:hardware/interfaces",
    // "vendor.qcom.hardware:vendor/qcom"], then we will identify the
    // prefix "android.hardware" and the package root
    // "hardware/interfaces".

    auto ret = mPackageRoots.end();
    for (auto it = mPackageRoots.begin(); it != mPackageRoots.end(); it++) {
        if (!fqName.inPackage(it->root.package())) {
            continue;
        }

        if (ret != mPackageRoots.end()) {
            std::cerr << "ERROR: Multiple package roots found for " << fqName.string() << " ("
                      << it->root.package() << " and " << ret->root.package() << ")\n";
            return nullptr;
        }

        ret = it;
    }

    if (ret == mPackageRoots.end()) {
        std::cerr << "ERROR: Package root not specified for " << fqName.string() << "\n";
        return nullptr;
    }

    return &(*ret);
}

std::string Coordinator::makeAbsolute(const std::string& path) const {
    if (StringHelper::StartsWith(path, "/") || mRootPath.empty()) {
        return path;
    }

    return mRootPath + path;
}

status_t Coordinator::getPackageRoot(const FQName& fqName, std::string* root) const {
    const PackageRoot* packageRoot = findPackageRoot(fqName);
    if (root == nullptr) {
        return UNKNOWN_ERROR;
    }
    *root = packageRoot->root.package();
    return OK;
}

status_t Coordinator::getPackageRootPath(const FQName& fqName, std::string* path) const {
    const PackageRoot* packageRoot = findPackageRoot(fqName);
    if (packageRoot == nullptr) {
        return UNKNOWN_ERROR;
    }
    *path = packageRoot->path;
    return OK;
}

status_t Coordinator::getPackagePath(const FQName& fqName, bool relative, bool sanitized,
                                     std::string* path) const {
    const PackageRoot* packageRoot = findPackageRoot(fqName);
    if (packageRoot == nullptr) return UNKNOWN_ERROR;

    // Given FQName of "android.hardware.nfc.test@1.0::IFoo" and a prefix
    // "android.hardware", the suffix is "nfc.test".
    std::string suffix = StringHelper::LTrim(fqName.package(), packageRoot->root.package());
    suffix = StringHelper::LTrim(suffix, ".");

    std::vector<std::string> suffixComponents;
    StringHelper::SplitString(suffix, '.', &suffixComponents);

    std::vector<std::string> components;
    if (!relative) {
        components.push_back(StringHelper::RTrimAll(packageRoot->path, "/"));
    }
    components.insert(components.end(), suffixComponents.begin(), suffixComponents.end());
    components.push_back(sanitized ? fqName.sanitizedVersion() : fqName.version());

    *path = StringHelper::JoinStrings(components, "/") + "/";
    return OK;
}

status_t Coordinator::getPackageInterfaceFiles(
        const FQName &package,
        std::vector<std::string> *fileNames) const {
    if (fileNames) fileNames->clear();

    std::string packagePath;
    status_t err =
        getPackagePath(package, false /* relative */, false /* sanitized */, &packagePath);
    if (err != OK) return err;

    const std::string path = makeAbsolute(packagePath);
    std::unique_ptr<DIR, decltype(&closedir)> dir(opendir(path.c_str()), closedir);

    if (dir == nullptr) {
        fprintf(stderr, "ERROR: Could not open package path %s for package %s:\n%s\n",
                packagePath.c_str(), package.string().c_str(), path.c_str());
        return -errno;
    }

    if (fileNames == nullptr) {
        return OK;
    }

    struct dirent *ent;
    while ((ent = readdir(dir.get())) != nullptr) {
        // filesystems may not support d_type and return DT_UNKNOWN
        if (ent->d_type == DT_UNKNOWN) {
            struct stat sb;
            const auto filename = packagePath + std::string(ent->d_name);
            if (stat(filename.c_str(), &sb) == -1) {
                fprintf(stderr, "ERROR: Could not stat %s\n", filename.c_str());
                return -errno;
            }
            if ((sb.st_mode & S_IFMT) != S_IFREG) {
                continue;
            }
        } else if (ent->d_type != DT_REG) {
             continue;
        }

        const auto suffix = ".hal";
        const auto suffix_len = std::strlen(suffix);
        const auto d_namelen = strlen(ent->d_name);

        if (d_namelen < suffix_len
                || strcmp(ent->d_name + d_namelen - suffix_len, suffix)) {
            continue;
        }

        fileNames->push_back(std::string(ent->d_name, d_namelen - suffix_len));
    }

    std::sort(fileNames->begin(), fileNames->end(),
              [](const std::string& lhs, const std::string& rhs) -> bool {
                  if (lhs == "types") {
                      return true;
                  }
                  if (rhs == "types") {
                      return false;
                  }
                  return lhs < rhs;
              });

    return OK;
}

status_t Coordinator::appendPackageInterfacesToVector(
        const FQName &package,
        std::vector<FQName> *packageInterfaces) const {
    packageInterfaces->clear();

    std::vector<std::string> fileNames;
    status_t err = getPackageInterfaceFiles(package, &fileNames);

    if (err != OK) {
        return err;
    }

    for (const auto &fileName : fileNames) {
        FQName subFQName(package.package(), package.version(), fileName);
        packageInterfaces->push_back(subFQName);
    }

    return OK;
}

status_t Coordinator::convertPackageRootToPath(const FQName& fqName, std::string* path) const {
    std::string packageRoot;
    status_t err = getPackageRoot(fqName, &packageRoot);
    if (err != OK) return err;

    if (*(packageRoot.end()--) != '.') {
        packageRoot += '.';
    }

    std::replace(packageRoot.begin(), packageRoot.end(), '.', '/');

    *path = packageRoot;  // now converted to a path
    return OK;
}

status_t Coordinator::isTypesOnlyPackage(const FQName& package, bool* result) const {
    std::vector<FQName> packageInterfaces;

    status_t err = appendPackageInterfacesToVector(package, &packageInterfaces);

    if (err != OK) {
        *result = false;
        return err;
    }

    *result = packageInterfaces.size() == 1 && packageInterfaces[0].name() == "types";
    return OK;
}

status_t Coordinator::addUnreferencedTypes(const std::vector<FQName>& packageInterfaces,
                                           std::set<FQName>* unreferencedDefinitions,
                                           std::set<FQName>* unreferencedImports) const {
    CHECK(unreferencedDefinitions != nullptr);
    CHECK(unreferencedImports != nullptr);

    std::set<FQName> packageDefinedTypes;
    std::set<FQName> packageReferencedTypes;
    std::set<FQName> packageImportedTypes;
    std::set<FQName> typesDefinedTypes;  // only types.hal types

    for (const auto& fqName : packageInterfaces) {
        AST* ast = parse(fqName);
        if (!ast) {
            std::cerr << "ERROR: Could not parse " << fqName.string() << ". Aborting." << std::endl;

            return UNKNOWN_ERROR;
        }

        ast->addDefinedTypes(&packageDefinedTypes);
        ast->addReferencedTypes(&packageReferencedTypes);
        ast->getAllImportedNamesGranular(&packageImportedTypes);

        if (fqName.name() == "types") {
            ast->addDefinedTypes(&typesDefinedTypes);
        }
    }

#if 0
    for (const auto &fqName : packageDefinedTypes) {
        std::cout << "VERBOSE: DEFINED " << fqName.string() << std::endl;
    }

    for (const auto &fqName : packageImportedTypes) {
        std::cout << "VERBOSE: IMPORTED " << fqName.string() << std::endl;
    }

    for (const auto &fqName : packageReferencedTypes) {
        std::cout << "VERBOSE: REFERENCED " << fqName.string() << std::endl;
    }

    for (const auto &fqName : typesDefinedTypes) {
        std::cout << "VERBOSE: DEFINED in types.hal " << fqName.string() << std::endl;
    }
#endif

    for (const auto& fqName : packageReferencedTypes) {
        packageDefinedTypes.erase(fqName);
        packageImportedTypes.erase(fqName);
    }

    // A package implicitly imports its own types.hal, only track them in one set.
    for (const auto& fqName : typesDefinedTypes) {
        packageImportedTypes.erase(fqName);
    }

    // defined but not referenced
    unreferencedDefinitions->insert(packageDefinedTypes.begin(), packageDefinedTypes.end());
    // imported but not referenced
    unreferencedImports->insert(packageImportedTypes.begin(), packageImportedTypes.end());
    return OK;
}

status_t Coordinator::enforceRestrictionsOnPackage(const FQName& fqName,
                                                   Enforce enforcement) const {
    CHECK(enforcement == Enforce::FULL || enforcement == Enforce::NO_HASH ||
          enforcement == Enforce::NONE);

    // need fqName to be something like android.hardware.foo@1.0.
    // name and valueName is ignored.
    if (fqName.package().empty() || fqName.version().empty()) {
        std::cerr << "ERROR: Cannot enforce restrictions on package " << fqName.string()
                  << ": package or version is missing." << std::endl;
        return BAD_VALUE;
    }

    if (enforcement == Enforce::NONE) {
        return OK;
    }

    FQName package = fqName.getPackageAndVersion();
    // look up cache.
    if (mPackagesEnforced.find(package) != mPackagesEnforced.end()) {
        return OK;
    }

    // enforce all rules.
    status_t err;

    err = enforceMinorVersionUprevs(package, enforcement);
    if (err != OK) {
        return err;
    }

    if (enforcement != Enforce::NO_HASH) {
        err = enforceHashes(package);
        if (err != OK) {
            return err;
        }
    }

    // cache it so that it won't need to be enforced again.
    mPackagesEnforced.insert(package);
    return OK;
}

status_t Coordinator::enforceMinorVersionUprevs(const FQName& currentPackage,
                                                Enforce enforcement) const {
    if(!currentPackage.hasVersion()) {
        std::cerr << "ERROR: Cannot enforce minor version uprevs for " << currentPackage.string()
                  << ": missing version." << std::endl;
        return UNKNOWN_ERROR;
    }

    if (currentPackage.getPackageMinorVersion() == 0) {
        return OK; // ignore for @x.0
    }

    bool hasPrevPackage = false;
    FQName prevPackage = currentPackage;
    while (prevPackage.getPackageMinorVersion() > 0) {
        prevPackage = prevPackage.downRev();

        std::string prevPackagePath;
        status_t err = getPackagePath(prevPackage, false /* relative */, false /* sanitized */,
                                      &prevPackagePath);
        if (err != OK) return err;

        if (existdir(makeAbsolute(prevPackagePath).c_str())) {
            hasPrevPackage = true;
            break;
        }
    }
    if (!hasPrevPackage) {
        // no @x.z, where z < y, exist.
        return OK;
    }

    if (prevPackage != currentPackage.downRev()) {
        std::cerr << "ERROR: Cannot enforce minor version uprevs for " << currentPackage.string()
                  << ": Found package " << prevPackage.string() << " but missing "
                  << currentPackage.downRev().string() << "; you cannot skip a minor version."
                  << std::endl;
        return UNKNOWN_ERROR;
    }

    bool prevIsTypesOnly;
    status_t err = isTypesOnlyPackage(prevPackage, &prevIsTypesOnly);
    if (err != OK) return err;

    if (prevIsTypesOnly) {
        // A types only package can be extended in any way.
        return OK;
    }

    std::vector<FQName> packageInterfaces;
    err = appendPackageInterfacesToVector(currentPackage, &packageInterfaces);
    if (err != OK) {
        return err;
    }

    bool extendedInterface = false;
    for (const FQName &currentFQName : packageInterfaces) {
        if (currentFQName.name() == "types") {
            continue; // ignore types.hal
        }

        const Interface *iface = nullptr;
        AST* currentAST = parse(currentFQName, nullptr /* parsedASTs */, enforcement);
        if (currentAST != nullptr) {
            iface = currentAST->getInterface();
        }
        if (iface == nullptr) {
            if (currentAST == nullptr) {
                std::cerr << "WARNING: Skipping " << currentFQName.string()
                          << " because it could not be found or parsed"
                          << " or " << currentPackage.string() << " doesn't pass all requirements."
                          << std::endl;
            } else {
                std::cerr << "WARNING: Skipping " << currentFQName.string()
                          << " because the file might contain more than one interface."
                          << std::endl;
            }
            continue;
        }

        if (iface->superType() == nullptr) {
            CHECK(iface->isIBase());
            continue;
        }

        // Assume that currentFQName == android.hardware.foo@2.2::IFoo.
        FQName lastFQName(prevPackage.package(), prevPackage.version(),
                currentFQName.name());
        AST *lastAST = parse(lastFQName);

        for (; lastFQName.getPackageMinorVersion() > 0 &&
               (lastAST == nullptr || lastAST->getInterface() == nullptr)
             ; lastFQName = lastFQName.downRev(), lastAST = parse(lastFQName)) {
            // nothing
        }

        // Then lastFQName == android.hardware.foo@2.1::IFoo or
        //      lastFQName == android.hardware.foo@2.0::IFoo if 2.1 doesn't exist.

        bool lastFQNameExists = lastAST != nullptr && lastAST->getInterface() != nullptr;

        if (!lastFQNameExists) {
            continue;
        }

        if (iface->superType()->fqName() != lastFQName) {
            std::cerr << "ERROR: Cannot enforce minor version uprevs for "
                      << currentPackage.string() << ": " << iface->fqName().string() << " extends "
                      << iface->superType()->fqName().string()
                      << ", which is not allowed. It must extend " << lastFQName.string()
                      << std::endl;
            return UNKNOWN_ERROR;
        }

        // at least one interface must extend the previous version
        // @2.0::IFoo does not work. It must be @2.1::IFoo for at least one interface.
        if (lastFQName.getPackageAndVersion() == prevPackage.getPackageAndVersion()) {
            extendedInterface = true;
        }

        if (mVerbose) {
            std::cout << "VERBOSE: EnforceMinorVersionUprevs: " << currentFQName.string()
                      << " passes." << std::endl;
        }
    }

    if (!extendedInterface) {
        // No interface extends the interface with the same name in @x.(y-1).
        std::cerr << "ERROR: " << currentPackage.string()
                  << " doesn't pass minor version uprev requirement. "
                  << "Requires at least one interface to extend an interface with the same name "
                  << "from " << prevPackage.string() << "." << std::endl;
        return UNKNOWN_ERROR;
    }

    return OK;
}

Coordinator::HashStatus Coordinator::checkHash(const FQName& fqName) const {
    AST* ast = parse(fqName);
    if (ast == nullptr) return HashStatus::ERROR;

    std::string rootPath;
    status_t err = getPackageRootPath(fqName, &rootPath);
    if (err != OK) return HashStatus::ERROR;

    std::string hashPath = makeAbsolute(rootPath) + "/current.txt";
    std::string error;
    bool fileExists;
    std::vector<std::string> frozen =
        Hash::lookupHash(hashPath, fqName.string(), &error, &fileExists);
    if (fileExists) onFileAccess(hashPath, "r");

    if (error.size() > 0) {
        std::cerr << "ERROR: " << error << std::endl;
        return HashStatus::ERROR;
    }

    // hash not defined, interface not frozen
    if (frozen.size() == 0) {
        // This ensures that it can be detected.
        Hash::clearHash(ast->getFilename());

        return HashStatus::UNFROZEN;
    }

    std::string currentHash = ast->getFileHash()->hexString();

    if (std::find(frozen.begin(), frozen.end(), currentHash) == frozen.end()) {
        std::cerr << "ERROR: " << fqName.string() << " has hash " << currentHash
                  << " which does not match hash on record. This interface has "
                  << "been frozen. Do not change it!" << std::endl;
        return HashStatus::CHANGED;
    }

    return HashStatus::FROZEN;
}

status_t Coordinator::getUnfrozenDependencies(const FQName& fqName,
                                              std::set<FQName>* result) const {
    CHECK(result != nullptr);

    AST* ast = parse(fqName);
    if (ast == nullptr) return UNKNOWN_ERROR;

    std::set<FQName> imported;
    ast->getImportedPackages(&imported);

    // no circular dependency is already guaranteed by parsing
    // indirect dependencies will be checked when the imported interface frozen checks are done
    for (const FQName& importedPackage : imported) {
        std::vector<FQName> packageInterfaces;
        status_t err = appendPackageInterfacesToVector(importedPackage, &packageInterfaces);
        if (err != OK) {
            return err;
        }

        for (const FQName& importedName : packageInterfaces) {
            HashStatus status = checkHash(importedName);
            switch (status) {
                case HashStatus::CHANGED:
                case HashStatus::ERROR:
                    return UNKNOWN_ERROR;
                case HashStatus::FROZEN:
                    continue;
                case HashStatus::UNFROZEN:
                    result->insert(importedName);
                    continue;
                default:
                    LOG(FATAL) << static_cast<uint64_t>(status);
            }
        }
    }

    return OK;
}

status_t Coordinator::enforceHashes(const FQName& currentPackage) const {
    std::vector<FQName> packageInterfaces;
    status_t err = appendPackageInterfacesToVector(currentPackage, &packageInterfaces);
    if (err != OK) {
        return err;
    }

    for (const FQName& currentFQName : packageInterfaces) {
        HashStatus status = checkHash(currentFQName);
        switch (status) {
            case HashStatus::CHANGED:
            case HashStatus::ERROR:
                return UNKNOWN_ERROR;
            case HashStatus::FROZEN: {
                std::set<FQName> unfrozenDependencies;
                err = getUnfrozenDependencies(currentFQName, &unfrozenDependencies);
                if (err != OK) return err;

                if (!unfrozenDependencies.empty()) {
                    std::cerr << "ERROR: Frozen interface " << currentFQName.string()
                              << " cannot depend on unfrozen thing(s):" << std::endl;
                    for (const FQName& name : unfrozenDependencies) {
                        std::cerr << " (unfrozen) " << name.string() << std::endl;
                    }
                    return UNKNOWN_ERROR;
                }
            }
                continue;
            case HashStatus::UNFROZEN:
                continue;
            default:
                LOG(FATAL) << static_cast<uint64_t>(status);
        }
    }

    return err;
}

bool Coordinator::MakeParentHierarchy(const std::string &path) {
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

}  // namespace android

