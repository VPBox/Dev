/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>
#include <string>

#include <android-base/macros.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "aidl_language.h"
#include "logging.h"

namespace android {
namespace aidl {

// Special reserved type names.
extern const char kAidlReservedTypePackage[];
extern const char kUtf8InCppStringClass[];  // UTF16 wire format, UTF8 in C++

// Helpful aliases defined to be <kAidlReservedTypePackage>.<class name>
extern const char kUtf8InCppStringCanonicalName[];

// We sometimes special case this class.
extern const char kStringCanonicalName[];

// Note that these aren't the strings recognized by the parser, we just keep
// here for the sake of logging a common string constant.
extern const char kUtf8InCppAnnotation[];

class ValidatableType {
 public:
  enum {
    KIND_BUILT_IN,
    KIND_PARCELABLE,
    KIND_INTERFACE,
    KIND_GENERATED,
  };

  ValidatableType(int kind,
                  const std::string& package, const std::string& type_name,
                  const std::string& decl_file, int decl_line);
  virtual ~ValidatableType() = default;

  virtual bool CanBeArray() const { return ArrayType() != nullptr; }
  virtual bool CanWriteToParcel() const = 0;

  virtual const ValidatableType* ArrayType() const = 0;
  virtual const ValidatableType* NullableType() const = 0;

  // ShortName() is the class name without a package.
  std::string ShortName() const { return type_name_; }
  // CanonicalName() returns the canonical AIDL type, with packages.
  std::string CanonicalName() const { return canonical_name_; }

  int Kind() const { return kind_; }
  std::string HumanReadableKind() const;
  std::string DeclFile() const { return origin_file_; }
  int DeclLine() const { return origin_line_; }

 private:
  const int kind_;
  const std::string type_name_;
  const std::string canonical_name_;
  const std::string origin_file_;
  const int origin_line_;

  DISALLOW_COPY_AND_ASSIGN(ValidatableType);
};

class TypeNamespace {
 public:
  // Load the TypeNamespace with built in types.  Don't do work in the
  // constructor because many of the useful methods are virtual.
  virtual void Init() = 0;

  bool AddDefinedTypes(vector<AidlDefinedType*>& types, const string& filename);

  // Load this TypeNamespace with user defined types.
  virtual bool AddParcelableType(const AidlParcelable& p,
                                 const std::string& filename) = 0;

  virtual bool AddBinderType(const AidlInterface& b,
                             const std::string& filename) = 0;
  // Add a container type to this namespace.  Returns false only
  // on error. Silently discards requests to add non-container types.
  virtual bool MaybeAddContainerType(const AidlTypeSpecifier& aidl_type) = 0;

  // Returns true iff this has a type for |import|.
  virtual bool HasImportType(const std::string& import) const = 0;

  // Returns a pointer to a type corresponding to |raw_type| or nullptr
  // if this is an invalid return type.
  virtual const ValidatableType* GetReturnType(const AidlTypeSpecifier& raw_type,
                                               const AidlDefinedType& context) const;

  // Returns a pointer to a type corresponding to |a| or nullptr if |a|
  // has an invalid argument type.
  virtual const ValidatableType* GetArgType(const AidlArgument& a, int arg_index,
                                            const AidlDefinedType& context) const;

  // Returns a pointer to a type corresponding to |defined_type|.
  virtual const ValidatableType* GetDefinedType(const AidlDefinedType& defined_type) const = 0;

  AidlTypenames typenames_;

 protected:
  TypeNamespace() = default;
  virtual ~TypeNamespace() = default;

  virtual const ValidatableType* GetValidatableType(const AidlTypeSpecifier& type,
                                                    std::string* error_msg,
                                                    const AidlDefinedType& context) const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(TypeNamespace);
};

template<typename T>
class LanguageTypeNamespace : public TypeNamespace {
 public:
  LanguageTypeNamespace() = default;
  virtual ~LanguageTypeNamespace() = default;

  // Get a pointer to an existing type.  Searches first by fully-qualified
  // name, and then class name (dropping package qualifiers).
  const T* Find(const AidlTypeSpecifier& aidl_type) const;

  // Find a type by its |name|.  If |name| refers to a container type (e.g.
  // List<String>) you must turn it into a canonical name first (e.g.
  // java.util.List<java.lang.String>).
  const T* FindTypeByCanonicalName(const std::string& name) const;
  bool HasTypeByCanonicalName(const std::string& type_name) const {
    return FindTypeByCanonicalName(type_name) != nullptr;
  }
  bool HasImportType(const std::string& import) const override {
    return HasTypeByCanonicalName(import);
  }
  const ValidatableType* GetDefinedType(const AidlDefinedType& defined_type) const override {
    return FindTypeByCanonicalName(defined_type.GetCanonicalName());
  }

  bool MaybeAddContainerType(const AidlTypeSpecifier& aidl_type) override;
  // We dynamically create container types as we discover them in the parse
  // tree.  Returns false if the contained types cannot be canonicalized.
  virtual bool AddListType(const std::string& contained_type_name) = 0;
  virtual bool AddMapType(const std::string& key_type_name,
                          const std::string& value_type_name) = 0;

 protected:
  bool Add(std::unique_ptr<const T> type);
  void AddAndSetMember(const T** member, std::unique_ptr<const T> type) {
    const T* ptr_value = type.get();
    CHECK(Add(std::move(type)));
    *member = ptr_value;
  }

 private:
  // Returns true iff the name can be canonicalized to a container type.
  virtual bool CanonicalizeContainerType(const AidlTypeSpecifier& aidl_type,
                                         std::vector<std::string>* container_class,
                                         std::vector<std::string>* contained_type_names) const;

  // Returns true if this is a container type, rather than a normal type.
  bool IsContainerType(const std::string& type_name) const;

  const ValidatableType* GetValidatableType(const AidlTypeSpecifier& type, std::string* error_msg,
                                            const AidlDefinedType& context) const override;

  std::vector<std::unique_ptr<const T>> types_;

  DISALLOW_COPY_AND_ASSIGN(LanguageTypeNamespace);
};  // class LanguageTypeNamespace

template <typename T>
bool LanguageTypeNamespace<T>::Add(std::unique_ptr<const T> type) {
  const T* existing = FindTypeByCanonicalName(type->CanonicalName());
  if (!existing) {
    types_.push_back(std::move(type));
    return true;
  }

  if (existing->Kind() == ValidatableType::KIND_BUILT_IN) {
    LOG(ERROR) << type->DeclFile() << ":" << type->DeclLine()
               << " attempt to redefine built in class "
               << type->CanonicalName();
    return false;
  }

  if (type->Kind() != existing->Kind()) {
    LOG(ERROR) << type->DeclFile() << ":" << type->DeclLine()
               << " attempt to redefine " << type->CanonicalName()
               << " as " << type->HumanReadableKind();
    LOG(ERROR) << existing->DeclFile() << ":" << existing->DeclLine()
               << " previously defined here as "
               << existing->HumanReadableKind();
    return false;
  }

  return true;
}

template <typename T>
const T* LanguageTypeNamespace<T>::Find(const AidlTypeSpecifier& aidl_type) const {
  using std::string;
  using std::vector;
  using android::base::Join;
  using android::base::Trim;

  string name = Trim(aidl_type.IsArray() ? aidl_type.GetName() : aidl_type.ToString());
  if (IsContainerType(name)) {
    vector<string> container_class;
    vector<string> contained_type_names;
    if (!CanonicalizeContainerType(aidl_type, &container_class,
                                   &contained_type_names)) {
      return nullptr;
    }
    name = Join(container_class, '.') +
           "<" + Join(contained_type_names, ',') + ">";
  }
  // Here, we know that we have the canonical name for this container.
  return FindTypeByCanonicalName(name);
}

template<typename T>
const T* LanguageTypeNamespace<T>::FindTypeByCanonicalName(
    const std::string& raw_name) const {
  using android::base::Trim;

  std::string name = Trim(raw_name);
  const T* ret = nullptr;
  for (const auto& type : types_) {
    // Always prefer a exact match if possible.
    // This works for primitives and class names qualified with a package.
    if (type->CanonicalName() == name) {
      ret = type.get();
      break;
    }
    // We allow authors to drop packages when refering to a class name.
    if (type->ShortName() == name) {
      ret = type.get();
    }
  }

  return ret;
}

template <typename T>
bool LanguageTypeNamespace<T>::MaybeAddContainerType(const AidlTypeSpecifier& aidl_type) {
  using android::base::Join;

  const std::string& type_name = aidl_type.ToString();
  if (!IsContainerType(type_name)) {
    return true;
  }

  std::vector<std::string> container_class;
  std::vector<std::string> contained_type_names;
  if (!CanonicalizeContainerType(aidl_type, &container_class,
                                 &contained_type_names)) {
    return false;
  }

  const std::string canonical_name = Join(container_class, ".") +
      "<" + Join(contained_type_names, ",") + ">";
  if (HasTypeByCanonicalName(canonical_name)) {
    return true;
  }


  // We only support two types right now and this type is one of them.
  switch (contained_type_names.size()) {
    case 1:
      return AddListType(contained_type_names[0]);
    case 2:
      return AddMapType(contained_type_names[0], contained_type_names[1]);
    default:
      break;  // Should never get here, will FATAL below.
  }

  LOG(FATAL) << "aidl internal error";
  return false;
}

template<typename T>
bool LanguageTypeNamespace<T>::IsContainerType(
    const std::string& type_name) const {
  const size_t opening_brace = type_name.find('<');
  const size_t closing_brace = type_name.find('>');
  if (opening_brace != std::string::npos ||
      closing_brace != std::string::npos) {
    return true;  // Neither < nor > appear in normal AIDL types.
  }
  return false;
}

template <typename T>
bool LanguageTypeNamespace<T>::CanonicalizeContainerType(
    const AidlTypeSpecifier& aidl_type, std::vector<std::string>* container_class,
    std::vector<std::string>* contained_type_names) const {
  std::string container = aidl_type.GetName();
  std::vector<std::string> args;
  for (auto& type_arg : aidl_type.GetTypeParameters()) {
    if (type_arg->IsGeneric()) {
      // nesting is not allowed yet.
      LOG(ERROR) << "Nested template type '" << aidl_type.ToString() << "'";
    }

    std::string type_name = type_arg->ToString();
    // Here, we are relying on FindTypeByCanonicalName to do its best when
    // given a non-canonical name for non-compound type (i.e. not another
    // container).
    const T* arg_type = FindTypeByCanonicalName(type_name);
    if (!arg_type) {
      return false;
    }

    // Now get the canonical names for these contained types, remapping them if
    // necessary.
    type_name = arg_type->CanonicalName();
    if (aidl_type.IsUtf8InCpp() && type_name == "java.lang.String") {
      type_name = kUtf8InCppStringCanonicalName;
    }
    args.emplace_back(type_name);
  }

  // Map the container name to its canonical form for supported containers.
  if ((container == "List" || container == "java.util.List") &&
      args.size() == 1) {
    *container_class = {"java", "util", "List"};
    *contained_type_names = args;
    return true;
  }
  if ((container == "Map" || container == "java.util.Map") &&
      args.size() == 2) {
    *container_class = {"java", "util", "Map"};
    *contained_type_names = args;
    return true;
  }

  LOG(ERROR) << "Unknown find container with name " << container << " and " << args.size()
             << " contained types.";
  return false;
}

template <typename T>
const ValidatableType* LanguageTypeNamespace<T>::GetValidatableType(
    const AidlTypeSpecifier& aidl_type, std::string* error_msg,
    const AidlDefinedType& context) const {
  using android::base::StringPrintf;

  const ValidatableType* type = Find(aidl_type);
  if (type == nullptr) {
    *error_msg = "unknown type";
    return nullptr;
  }

  if (aidl_type.GetName() == "void") {
    if (aidl_type.IsArray()) {
      *error_msg = "void type cannot be an array";
      return nullptr;
    }
    if (aidl_type.IsNullable() || aidl_type.IsUtf8InCpp()) {
      *error_msg = "void type cannot be annotated";
      return nullptr;
    }
    // We have no more special handling for void.
    return type;
  }

  bool utf8InCpp = aidl_type.IsUtf8InCpp();

  // Strings inside containers get remapped to appropriate utf8 versions when
  // we convert the container name to its canonical form and the look up the
  // type.  However, for non-compound types (i.e. those not in a container) we
  // must patch them up here.
  if (IsContainerType(type->CanonicalName())) {
    utf8InCpp = false;
  } else if (aidl_type.GetName() == "String" ||
             aidl_type.GetName() == "java.lang.String") {
    utf8InCpp = utf8InCpp || context.IsUtf8InCpp();
  } else if (utf8InCpp) {
    *error_msg = StringPrintf("type '%s' may not be annotated as %s.", aidl_type.GetName().c_str(),
                              kUtf8InCppAnnotation);
    return nullptr;
  }

  if (utf8InCpp) {
    type = FindTypeByCanonicalName(kUtf8InCppStringCanonicalName);
  }

  // One of our UTF8 transforms made type null
  if (type == nullptr) {
    *error_msg = StringPrintf("%s is unsupported when generating code for this language.",
                              kUtf8InCppAnnotation);
    return nullptr;
  }

  if (!type->CanWriteToParcel()) {
    *error_msg = "type cannot be marshalled";
    return nullptr;
  }

  if (aidl_type.IsArray()) {
    type = type->ArrayType();
    if (!type) {
      *error_msg = StringPrintf("type '%s' cannot be an array",
                                aidl_type.GetName().c_str());
      return nullptr;
    }
  }

  if (context.IsNullable()) {
    const ValidatableType* nullableType = type->NullableType();

    if (nullableType) {
      return nullableType;
    }
  }

  if (aidl_type.IsNullable()) {
    type = type->NullableType();
    if (!type) {
      *error_msg = StringPrintf("type '%s%s' cannot be marked as possibly null",
                                aidl_type.GetName().c_str(),
                                (aidl_type.IsArray()) ? "[]" : "");
      return nullptr;
    }
  }

  return type;
}

}  // namespace aidl
}  // namespace android
