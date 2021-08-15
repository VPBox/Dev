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

#ifndef COORDINATOR_H_

#define COORDINATOR_H_

#include <android-base/macros.h>
#include <hidl-util/FQName.h>
#include <hidl-util/Formatter.h>
#include <utils/Errors.h>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace android {

struct AST;
struct Type;

struct Coordinator {
    Coordinator() {};

    const std::string& getRootPath() const;
    void setRootPath(const std::string &rootPath);
    void setOutputPath(const std::string& outputPath);

    void setVerbose(bool value);
    bool isVerbose() const;

    void setDepFile(const std::string& depFile);

    const std::string& getOwner() const;
    void setOwner(const std::string& owner);

    // adds path only if it doesn't exist
    status_t addPackagePath(const std::string& root, const std::string& path, std::string* error);
    // adds path if it hasn't already been added
    void addDefaultPackagePath(const std::string& root, const std::string& path);

    enum class Location {
        STANDARD_OUT,
        DIRECT,         // mOutputPath + file name
        PACKAGE_ROOT,   // e.x. mRootPath + /nfc/1.0/Android.bp
        GEN_OUTPUT,     // e.x. mOutputPath + /android/hardware/foo/1.0/*.cpp
        GEN_SANITIZED,  // e.x. mOutputPath + /android/hardware/foo/V1_0/*.cpp
    };

    status_t getFilepath(const FQName& fqName, Location location, const std::string& fileName,
                         std::string* path) const;

    Formatter getFormatter(const FQName& fqName, Location location,
                           const std::string& fileName) const;

    // must be called before file access
    void onFileAccess(const std::string& path, const std::string& mode) const;

    status_t writeDepFile(const std::string& forFile) const;

    enum class Enforce {
        FULL,     // default
        NO_HASH,  // only for use with -Lhash
        NONE,     // only for use during enforcement
    };

    // Attempts to parse the interface/types referred to by fqName.
    // Parsing an interface also parses the associated package's types.hal
    // file if it exists.
    // If "parsedASTs" is non-NULL, successfully parsed ASTs are inserted
    // into the set.
    // If !enforce, enforceRestrictionsOnPackage won't be run.
    AST* parse(const FQName& fqName, std::set<AST*>* parsedASTs = nullptr,
               Enforce enforcement = Enforce::FULL) const;

    // Same as parse, but it distinguishes between "missing file" and "could not parse AST"
    // return OK, out *ast:
    //    0xdeadbeef -> successfully parsed
    //    nullptr    -> file not present
    // return !OK
    //    could not parse AST and file exists
    status_t parseOptional(const FQName& fqName, AST** ast, std::set<AST*>* parsedASTs = nullptr,
                           Enforce enforcement = Enforce::FULL) const;

    // Given package-root paths of ["hardware/interfaces",
    // "vendor/<something>/interfaces"], package roots of
    // ["android.hardware", "vendor.<something>.hardware"], and a
    // FQName of "android.hardware.nfc@1.0::INfc, then getPackagePath()
    // will return "hardware/interfaces/nfc/1.0" (if sanitized = false)
    // or "hardware/interfaces/nfc/V1_0" (if sanitized = true).
    status_t getPackagePath(const FQName& fqName, bool relative, bool sanitized,
                            std::string* path) const;

    // Given package roots of ["android.hardware",
    // "vendor.<something>.hardware"] and a FQName of
    // "android.hardware.nfc@1.0::INfc, then getPackageRoot() will
    // return "android.hardware".
    status_t getPackageRoot(const FQName& fqName, std::string* root) const;

    status_t getPackageInterfaceFiles(
            const FQName &package,
            std::vector<std::string> *fileNames) const;

    status_t appendPackageInterfacesToVector(
            const FQName &package,
            std::vector<FQName> *packageInterfaces) const;

    status_t isTypesOnlyPackage(const FQName& package, bool* result) const;

    // Returns types which are imported/defined but not referenced in code
    status_t addUnreferencedTypes(const std::vector<FQName>& packageInterfaces,
                                  std::set<FQName>* unreferencedDefinitions,
                                  std::set<FQName>* unreferencedImports) const;

    // Enforce a set of restrictions on a set of packages. These include:
    //    - minor version upgrades
    // "packages" contains names like "android.hardware.nfc@1.1".
    //    - hashing restrictions
    status_t enforceRestrictionsOnPackage(const FQName& fqName,
                                          Enforce enforcement = Enforce::FULL) const;

private:
    static bool MakeParentHierarchy(const std::string &path);

    enum class HashStatus {
        ERROR,
        UNFROZEN,
        FROZEN,
        CHANGED,  // frozen but changed
    };
    HashStatus checkHash(const FQName& fqName) const;
    status_t getUnfrozenDependencies(const FQName& fqName, std::set<FQName>* result) const;

    // indicates that packages in "android.hardware" will be looked up in hardware/interfaces
    struct PackageRoot {
        std::string path; // e.x. hardware/interfaces
        FQName root; // e.x. android.hardware@0.0
    };

    // nullptr if it doesn't exist
    const PackageRoot* findPackageRoot(const FQName& fqName) const;

    // Given package-root paths of ["hardware/interfaces",
    // "vendor/<something>/interfaces"], package roots of
    // ["android.hardware", "vendor.<something>.hardware"], and a
    // FQName of "android.hardware.nfc@1.0::INfc, then getPackageRootPath()
    // will return "hardware/interfaces".
    status_t getPackageRootPath(const FQName& fqName, std::string* path) const;

    // Given an FQName of "android.hardware.nfc@1.0::INfc", return
    // "android/hardware/".
    status_t convertPackageRootToPath(const FQName& fqName, std::string* path) const;

    std::vector<PackageRoot> mPackageRoots;
    std::string mRootPath;    // root of android source tree (to locate package roots)
    std::string mOutputPath;  // root of output directory
    std::string mDepFile;     // location to write depfile

    // hidl-gen options
    bool mVerbose = false;
    std::string mOwner;

    // cache to parse().
    mutable std::map<FQName, AST *> mCache;

    // cache to enforceRestrictionsOnPackage().
    mutable std::set<FQName> mPackagesEnforced;

    mutable std::set<std::string> mReadFiles;

    // Returns the given path if it is absolute, otherwise it returns
    // the path relative to mRootPath
    std::string makeAbsolute(const std::string& string) const;

    // Rules of enforceRestrictionsOnPackage are listed below.
    status_t enforceMinorVersionUprevs(const FQName& fqName, Enforce enforcement) const;
    status_t enforceHashes(const FQName &fqName) const;

    DISALLOW_COPY_AND_ASSIGN(Coordinator);
};

}  // namespace android

#endif  // COORDINATOR_H_
