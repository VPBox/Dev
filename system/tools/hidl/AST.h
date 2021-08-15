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

#include <android-base/macros.h>
#include <hidl-hash/Hash.h>
#include <hidl-util/FQName.h>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Scope.h"
#include "Type.h"

namespace android {

struct Coordinator;
struct ConstantExpression;
struct EnumValue;
struct Formatter;
struct Interface;
struct Location;
struct Method;
struct NamedType;
template <class T>
struct NamedReference;
struct Type;

struct AST {
    AST(const Coordinator* coordinator, const Hash* fileHash);

    bool setPackage(const char *package);
    bool addImport(const char *import);

    // package and version really.
    FQName package() const;
    bool isInterface() const;
    bool definesInterfaces() const;

    // Adds package, version and scope stack to local name
    FQName makeFullName(const char* localName, Scope* scope) const;

    void addScopedType(NamedType* type, Scope* scope);

    const std::string& getFilename() const;
    const Hash* getFileHash() const;

    // Look up local identifier.
    // It could be plain identifier or enum value as described by lookupEnumValue.
    LocalIdentifier* lookupLocalIdentifier(const Reference<LocalIdentifier>& ref, Scope* scope);

    // Look up an enum value by "FQName:valueName".
    EnumValue* lookupEnumValue(const FQName& fqName, std::string* errorMsg, Scope* scope);

    // Look up a type by FQName, "pure" names, i.e. those without package
    // or version are first looked up in the current scope chain.
    // After that lookup proceeds to imports.
    Type* lookupType(const FQName& fqName, Scope* scope);

    void addImportedAST(AST *ast);

    // Calls all passes after parsing required before
    // being ready to generate output.
    status_t postParse();

    // Recursive pass on constant expression tree
    status_t constantExpressionRecursivePass(
        const std::function<status_t(ConstantExpression*)>& func, bool processBeforeDependencies);
    status_t constantExpressionRecursivePass(
        const std::function<status_t(const ConstantExpression*)>& func,
        bool processBeforeDependencies) const;

    // Recursive tree pass that sets ParseStage of all types to newStage.
    status_t setParseStage(Type::ParseStage oldStage, Type::ParseStage newStage);

    // Recursive tree pass that looks up all referenced types
    status_t lookupTypes();

    // Recursive tree pass that looks up all referenced local identifiers
    // and types referenced by constant expressions
    status_t lookupConstantExpressions();

    // Recursive tree pass that validates that all defined types
    // have unique names in their scopes.
    status_t validateDefinedTypesUniqueNames() const;

    // Recursive tree pass that completes type declarations
    // that depend on super types
    status_t resolveInheritance();

    // Recursive tree pass that validates constant expressions
    status_t validateConstantExpressions() const;

    // Recursive tree pass that evaluates constant expressions
    status_t evaluateConstantExpressions();

    // Recursive tree pass that validates all type-related
    // syntax restrictions
    status_t validate() const;

    // Recursive tree pass that ensures that type definitions and references
    // are acyclic and reorderes type definitions in reversed topological order.
    status_t topologicalReorder();

    // Recursive tree pass that ensures that constant expressions
    // are acyclic.
    status_t checkAcyclicConstantExpressions() const;

    // Recursive tree pass that checks C++ forward declaration restrictions.
    status_t checkForwardReferenceRestrictions() const;

    status_t gatherReferencedTypes();

    void generateCppSource(Formatter& out) const;

    void generateInterfaceHeader(Formatter& out) const;
    void generateHwBinderHeader(Formatter& out) const;
    void generateStubHeader(Formatter& out) const;
    void generateProxyHeader(Formatter& out) const;
    void generatePassthroughHeader(Formatter& out) const;

    void generateCppImplHeader(Formatter& out) const;
    void generateCppImplSource(Formatter& out) const;

    void generateCppAdapterHeader(Formatter& out) const;
    void generateCppAdapterSource(Formatter& out) const;

    void generateJava(Formatter& out, const std::string& limitToType) const;
    void generateJavaTypes(Formatter& out, const std::string& limitToType) const;

    void generateVts(Formatter& out) const;

    void generateDependencies(Formatter& out) const;

    void getImportedPackages(std::set<FQName> *importSet) const;

    // Run getImportedPackages on this, then run getImportedPackages on
    // each AST in each package referenced in importSet.
    void getImportedPackagesHierarchy(std::set<FQName> *importSet) const;

    bool isJavaCompatible() const;

    // Warning: this only includes names explicitly referenced in code.
    //   It does not include all names which are imported.
    //
    // Currently, there is one valid usecase for this: importing exactly
    // the names which need to be imported in generated code. If you import
    // based on getAllImportedNamesGranular instead, you will import things
    // that aren't actually used in the resultant code.
    //
    // Get transitive closure of imported interface/types. This will add
    // everything exported by a package even if only a single type from
    // that package was explicitly imported!
    void getAllImportedNames(std::set<FQName> *allImportSet) const;

    // Get imported types, this includes those explicitly imported as well
    // as all types defined in imported packages.
    void getAllImportedNamesGranular(std::set<FQName> *allImportSet) const;

    void appendToExportedTypesVector(
            std::vector<const Type *> *exportedTypes) const;

    // used by the parser.
    void addSyntaxError();
    size_t syntaxErrors() const;

    bool isIBase() const;

    // or nullptr if not isInterface
    const Interface *getInterface() const;

    // types or Interface base name (e.x. Foo)
    std::string getBaseName() const;

    Scope* getRootScope();

    static void generateCppPackageInclude(Formatter& out, const FQName& package,
                                          const std::string& klass);

    void addDefinedTypes(std::set<FQName> *definedTypes) const;
    void addReferencedTypes(std::set<FQName> *referencedTypes) const;

    void addToImportedNamesGranular(const FQName &fqName);

   private:
    const Coordinator* mCoordinator;
    const Hash* mFileHash;

    RootScope mRootScope;

    FQName mPackage;

    // A set of all external interfaces/types that are _actually_ referenced
    // in this AST, this is a subset of those specified in import statements.
    // Note that this set only resolves to the granularity of either an
    // interface type or a whole package.
    std::set<FQName> mImportedNames;

    // This is the set of actually imported types.
    std::set<FQName> mImportedNamesGranular;

    // Warning: this only includes names explicitly referenced in code.
    //   It does not include all names which are imported.
    //
    // A set of all ASTs we explicitly or implicitly (types.hal) import.
    std::set<AST *> mImportedASTs;

    // If a single type (instead of the whole AST) is imported, the AST will be
    // present as a key to this map, with the value being a list of types
    // imported from this AST. If an AST appears in mImportedASTs but not in
    // mImportedTypes, then the whole AST is imported.
    std::map<AST *, std::set<Type *>> mImportedTypes;

    // Types keyed by full names defined in this AST.
    std::map<FQName, Type *> mDefinedTypesByFullName;

    // used by the parser.
    size_t mSyntaxErrors = 0;

    std::set<FQName> mReferencedTypeNames;

    // Helper functions for lookupType.
    Type* lookupTypeLocally(const FQName& fqName, Scope* scope);
    status_t lookupAutofilledType(const FQName &fqName, Type **returnedType);
    Type *lookupTypeFromImports(const FQName &fqName);

    // Find a type matching fqName (which may be partial) and if found
    // return the associated type and fill in the full "matchingName".
    // Only types defined in this very AST are considered.
    Type *findDefinedType(const FQName &fqName, FQName *matchingName) const;

    void getPackageComponents(std::vector<std::string> *components) const;

    void getPackageAndVersionComponents(
            std::vector<std::string> *components, bool cpp_compatible) const;

    std::string makeHeaderGuard(const std::string &baseName,
                                bool indicateGenerated = true) const;
    void enterLeaveNamespace(Formatter &out, bool enter) const;

    static void generateCheckNonNull(Formatter &out, const std::string &nonNull);

    void generateTypeSource(Formatter& out, const std::string& ifaceName) const;

    // a method, and in which interface is it originally defined.
    // be careful of the case where method.isHidlReserved(), where interface
    // is effectively useless.
    using MethodGenerator = std::function<void(const Method*, const Interface*)>;

    void generateTemplatizationLink(Formatter& out) const;
    void generateCppTag(Formatter& out, const std::string& tag) const;

    void generateMethods(Formatter& out, const MethodGenerator& gen,
                         bool includeParents = true) const;
    void generateStubImplMethod(Formatter& out, const std::string& className,
                                const Method* method) const;
    void generatePassthroughMethod(Formatter& out, const Method* method, const Interface* superInterface) const;
    void generateStaticProxyMethodSource(Formatter& out, const std::string& className,
                                         const Method* method, const Interface* superInterface) const;
    void generateProxyMethodSource(Formatter& out, const std::string& className,
                                   const Method* method, const Interface* superInterface) const;
    void generateAdapterMethod(Formatter& out, const Method* method) const;

    void generateFetchSymbol(Formatter &out, const std::string &ifaceName) const;

    void generateProxySource(Formatter& out, const FQName& fqName) const;

    void generateStubSource(Formatter& out, const Interface* iface) const;

    void generateStubSourceForMethod(Formatter& out, const Method* method,
                                     const Interface* superInterface) const;
    void generateStaticStubMethodSource(Formatter& out, const FQName& fqName,
                                        const Method* method, const Interface* superInterface) const;

    void generatePassthroughSource(Formatter& out) const;

    void generateInterfaceSource(Formatter& out) const;

    enum InstrumentationEvent {
        SERVER_API_ENTRY = 0,
        SERVER_API_EXIT,
        CLIENT_API_ENTRY,
        CLIENT_API_EXIT,
        SYNC_CALLBACK_ENTRY,
        SYNC_CALLBACK_EXIT,
        ASYNC_CALLBACK_ENTRY,
        ASYNC_CALLBACK_EXIT,
        PASSTHROUGH_ENTRY,
        PASSTHROUGH_EXIT,
    };

    void generateCppAtraceCall(
            Formatter &out,
            InstrumentationEvent event,
            const Method *method) const;

    void generateCppInstrumentationCall(
            Formatter &out,
            InstrumentationEvent event,
            const Method *method,
            const Interface* superInterface) const;

    void declareCppReaderLocals(Formatter& out, const std::vector<NamedReference<Type>*>& arg,
                                bool forResults) const;

    void emitCppReaderWriter(Formatter& out, const std::string& parcelObj, bool parcelObjIsPointer,
                             const NamedReference<Type>* arg, bool isReader, Type::ErrorMode mode,
                             bool addPrefixToName) const;

    void emitCppResolveReferences(Formatter& out, const std::string& parcelObj,
                                  bool parcelObjIsPointer, const NamedReference<Type>* arg,
                                  bool isReader, Type::ErrorMode mode, bool addPrefixToName) const;

    void emitJavaReaderWriter(Formatter& out, const std::string& parcelObj,
                              const NamedReference<Type>* arg, bool isReader,
                              bool addPrefixToName) const;

    void emitVtsTypeDeclarations(Formatter& out) const;

    DISALLOW_COPY_AND_ASSIGN(AST);
};

}  // namespace android

#endif  // AST_H_
