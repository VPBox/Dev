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

#include "type_cpp.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "logging.h"

using std::string;
using std::vector;

using android::base::Join;
using android::base::StringPrintf;

namespace android {
namespace aidl {
namespace cpp {
namespace {

const char kNoPackage[] = "";
const char kNoHeader[] = "";
const char kNoValidMethod[] = "";
Type* const kNoArrayType = nullptr;
Type* const kNoNullableType = nullptr;

class VoidType : public Type {
 public:
  VoidType() : Type(ValidatableType::KIND_BUILT_IN, kNoPackage, "void",
                    {}, "void", kNoValidMethod, kNoValidMethod) {}
  ~VoidType() override = default;
  bool CanWriteToParcel() const override { return false; }
};  // class VoidType

class CppArrayType : public Type {
 public:
  CppArrayType(int kind,  // from ValidatableType
            const std::string& package,
            const string& underlying_aidl_type,
            const string& cpp_header,
            const string& underlying_cpp_type,
            const string& underlying_cpp_type_nulllable,
            const string& read_method,
            const string& write_method,
            bool is_nullable,
            const string& src_file_name = "")
      : Type(kind, package,
             underlying_aidl_type + "[]",
             GetHeaders(is_nullable, cpp_header),
             GetCppType(is_nullable, underlying_cpp_type),
             read_method, write_method, kNoArrayType,
             (is_nullable)
                 ? kNoNullableType
                 // All arrays are nullable.
                 : new CppArrayType(kind, package, underlying_aidl_type,
                                    cpp_header, underlying_cpp_type_nulllable,
                                    underlying_cpp_type_nulllable,
                                    read_method, write_method, true),
             src_file_name) {}

 private:
  static vector<string> GetHeaders(bool is_nullable, const string& cpp_header) {
    vector<string> result = {"vector"};
    if (is_nullable) {
      result.push_back("memory");
    }
    if (!cpp_header.empty()) {
      result.push_back(cpp_header);
    }
    return result;
  }

  static string GetCppType(bool is_nullable,
                           const string& underlying_cpp_type) {
    if (is_nullable)
      return StringPrintf("::std::unique_ptr<::std::vector<%s>>",
                          underlying_cpp_type.c_str());
    return StringPrintf("::std::vector<%s>",
                        underlying_cpp_type.c_str());
  }

  DISALLOW_COPY_AND_ASSIGN(CppArrayType);
};  // class CppArrayType

class PrimitiveType : public Type {
 public:
  PrimitiveType(const std::string& aidl_type,
                const std::string& header,
                const std::string& cpp_type,
                const std::string& read_method,
                const std::string& write_method,
                const std::string& read_array_method,
                const std::string& write_array_method)
      : Type(ValidatableType::KIND_BUILT_IN, kNoPackage, aidl_type, {header},
             cpp_type, read_method, write_method,
             new CppArrayType(ValidatableType::KIND_BUILT_IN, kNoPackage,
                              aidl_type, header, cpp_type, cpp_type,
                              read_array_method, write_array_method,
                              false)) {}

  ~PrimitiveType() override = default;
  bool IsCppPrimitive() const override { return true; }

 private:
  DISALLOW_COPY_AND_ASSIGN(PrimitiveType);
};  // class PrimitiveType

// Unfortunately, bytes in Java are signed.  However, most C authors would
// say that a byte is not in fact signed.  Compromise: customize this otherwise
// normal primitive to use signed single bytes, but unsigned byte arrays.
class ByteType : public Type {
 public:
  ByteType()
      : Type(ValidatableType::KIND_BUILT_IN, kNoPackage, "byte",
             {"cstdint"}, "int8_t", "readByte", "writeByte",
             new CppArrayType(ValidatableType::KIND_BUILT_IN, kNoPackage,
                              "byte", "cstdint", "uint8_t", "uint8_t",
                              "readByteVector", "writeByteVector",
                              false)) {}

  ~ByteType() override = default;
  bool IsCppPrimitive() const override { return true; }

 private:
  DISALLOW_COPY_AND_ASSIGN(ByteType);
};  // class PrimitiveType

static string GetCppHeader(const AidlDefinedType& defined_type) {
  vector<string> name = defined_type.GetSplitPackage();
  name.push_back(defined_type.GetName());
  return Join(name, '/') + ".h";
}

class BinderType : public Type {
 public:
  BinderType(const AidlInterface& interface, const std::string& src_file_name)
      : BinderType(interface, src_file_name,
                   new BinderType(interface, src_file_name, kNoNullableType,
                                  "readNullableStrongBinder"),
                   "readStrongBinder") {}
  ~BinderType() override = default;

  string WriteCast(const string& val) const override {
    return write_cast_ + "(" + val + ")";
  }

 private:
  BinderType(const AidlInterface& interface, const std::string& src_file_name, Type* nullable_type,
             const std::string& read)
      : Type(ValidatableType::KIND_GENERATED, interface.GetPackage(), interface.GetName(),
             {GetCppHeader(interface)}, GetCppName(interface), read, "writeStrongBinder",
             kNoArrayType, nullable_type, src_file_name),
        write_cast_(GetRawCppName(interface) + "::asBinder") {}

  static string GetCppName(const AidlInterface& interface) {
    return "::android::sp<" + GetRawCppName(interface) + ">";
  }

  static string GetRawCppName(const AidlInterface& interface) {
    vector<string> name = interface.GetSplitPackage();
    string ret;

    name.push_back(interface.GetName());

    for (const auto& term : name) {
      ret += "::" + term;
    }

    return ret;
  }

  std::string write_cast_;
};

class NullableParcelableType : public Type {
 public:
  NullableParcelableType(const AidlParcelable& parcelable, const std::string& cpp_header,
                         const std::string& src_file_name)
      : Type(ValidatableType::KIND_PARCELABLE, parcelable.GetPackage(), parcelable.GetName(),
             {cpp_header}, GetCppName(parcelable), "readParcelable", "writeNullableParcelable",
             kNoArrayType, kNoNullableType, src_file_name) {}
  ~NullableParcelableType() override = default;

 private:
  static string GetCppName(const AidlParcelable& parcelable) {
    return "::std::unique_ptr<::" + Join(parcelable.GetSplitPackage(), "::") +
        "::" + parcelable.GetCppName() + ">";
  }
};

class ParcelableType : public Type {
 public:
  ParcelableType(const AidlParcelable& parcelable, const std::string& cpp_header,
                 const std::string& src_file_name)
      : Type(ValidatableType::KIND_PARCELABLE, parcelable.GetPackage(), parcelable.GetName(),
             {cpp_header}, GetCppName(parcelable), "readParcelable", "writeParcelable",
             new CppArrayType(ValidatableType::KIND_PARCELABLE, parcelable.GetPackage(),
                              parcelable.GetName(), cpp_header, GetCppName(parcelable),
                              GetCppName(parcelable), "readParcelableVector",
                              "writeParcelableVector", false, src_file_name),
             new NullableParcelableType(parcelable, cpp_header, src_file_name), src_file_name) {}
  ~ParcelableType() override = default;

 private:
  static string GetCppName(const AidlParcelable& parcelable) {
    return "::" + Join(parcelable.GetSplitPackage(), "::") +
        "::" + parcelable.GetCppName();
  }
};

class NullableMap : public Type {
 public:
  NullableMap()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "Map",
             {"binder/Map.h", "binder/Value.h"},
             "::std::unique_ptr<::android::binder::Map>",
             "readNullableMap", "writeNullableMap") {}
  ~NullableMap() override = default;
};


class MapType : public Type {
 public:
  MapType()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "Map",
             {"binder/Map.h","binder/Value.h"},
             "::android::binder::Map",
             "readMap", "writeMap",
             kNoArrayType,
             new NullableMap() ) {}
  ~MapType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(MapType);
};  // class MapType

class NullableStringListType : public Type {
 public:
  NullableStringListType()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "List<" + string(kStringCanonicalName) + ">",
             {"utils/String16.h", "memory", "vector"},
             "::std::unique_ptr<::std::vector<std::unique_ptr<::android::String16>>>",
             "readString16Vector", "writeString16Vector") {}
  ~NullableStringListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(NullableStringListType);
};  // class NullableStringListType

class StringListType : public Type {
 public:
  StringListType()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "List<" + string(kStringCanonicalName) + ">",
             {"utils/String16.h", "vector"},
             "::std::vector<::android::String16>",
             "readString16Vector", "writeString16Vector",
             kNoArrayType, new NullableStringListType()) {}
  ~StringListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(StringListType);
};  // class StringListType

class NullableUtf8InCppStringListType : public Type {
 public:
  NullableUtf8InCppStringListType()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "List<" + string(kUtf8InCppStringCanonicalName) + ">",
             {"memory", "string", "vector"},
             "::std::unique_ptr<::std::vector<std::unique_ptr<::std::string>>>",
             "readUtf8VectorFromUtf16Vector", "writeUtf8VectorAsUtf16Vector") {}
  ~NullableUtf8InCppStringListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(NullableUtf8InCppStringListType);
};  // class NullableUtf8InCppStringListType

class Utf8InCppStringListType : public Type {
 public:
  Utf8InCppStringListType()
      : Type(ValidatableType::KIND_BUILT_IN,
             "java.util", "List<" + string(kUtf8InCppStringCanonicalName) + ">",
             {"string", "vector"},
             "::std::vector<::std::string>",
             "readUtf8VectorFromUtf16Vector", "writeUtf8VectorAsUtf16Vector",
             kNoArrayType, new NullableUtf8InCppStringListType()) {}
  ~Utf8InCppStringListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(Utf8InCppStringListType);
};  // class Utf8InCppStringListType

class NullableBinderListType : public Type {
 public:
  NullableBinderListType()
      : Type(ValidatableType::KIND_BUILT_IN, "java.util",
             "List<android.os.IBinder>", {"binder/IBinder.h", "vector"},
             "::std::unique_ptr<::std::vector<::android::sp<::android::IBinder>>>",
             "readStrongBinderVector", "writeStrongBinderVector") {}
  ~NullableBinderListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(NullableBinderListType);
};  // class NullableBinderListType

class BinderListType : public Type {
 public:
  BinderListType()
      : Type(ValidatableType::KIND_BUILT_IN, "java.util",
             "List<android.os.IBinder>", {"binder/IBinder.h", "vector"},
             "::std::vector<::android::sp<::android::IBinder>>",
             "readStrongBinderVector", "writeStrongBinderVector",
             kNoArrayType, new NullableBinderListType()) {}
  ~BinderListType() override = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(BinderListType);
};  // class BinderListType

}  // namespace

Type::Type(int kind,
           const std::string& package,
           const std::string& aidl_type,
           const vector<string>& headers,
           const string& cpp_type,
           const string& read_method,
           const string& write_method,
           Type* array_type,
           Type* nullable_type,
           const string& src_file_name,
           int line)
    : ValidatableType(kind, package, aidl_type, src_file_name, line),
      headers_(headers),
      aidl_type_(aidl_type),
      cpp_type_(cpp_type),
      parcel_read_method_(read_method),
      parcel_write_method_(write_method),
      array_type_(array_type),
      nullable_type_(nullable_type) {}

bool Type::CanWriteToParcel() const { return true; }

void TypeNamespace::Init() {
  Add(std::make_unique<ByteType>());
  Add(std::make_unique<PrimitiveType>("int", "cstdint", "int32_t", "readInt32", "writeInt32",
                                      "readInt32Vector", "writeInt32Vector"));
  Add(std::make_unique<PrimitiveType>("long", "cstdint", "int64_t", "readInt64", "writeInt64",
                                      "readInt64Vector", "writeInt64Vector"));
  Add(std::make_unique<PrimitiveType>("float", kNoHeader, "float", "readFloat", "writeFloat",
                                      "readFloatVector", "writeFloatVector"));
  Add(std::make_unique<PrimitiveType>("double", kNoHeader, "double", "readDouble", "writeDouble",
                                      "readDoubleVector", "writeDoubleVector"));
  Add(std::make_unique<PrimitiveType>("boolean", kNoHeader, "bool", "readBool", "writeBool",
                                      "readBoolVector", "writeBoolVector"));
  // C++11 defines the char16_t type as a built in for Unicode characters.
  Add(std::make_unique<PrimitiveType>("char", kNoHeader, "char16_t", "readChar", "writeChar",
                                      "readCharVector", "writeCharVector"));

  Type* string_array_type = new CppArrayType(
      ValidatableType::KIND_BUILT_IN, "java.lang", "String",
      "utils/String16.h", "::android::String16",
      "::std::unique_ptr<::android::String16>", "readString16Vector",
      "writeString16Vector", false);

  Type* nullable_string_type =
      new Type(ValidatableType::KIND_BUILT_IN, "java.lang", "String",
               {"memory", "utils/String16.h"}, "::std::unique_ptr<::android::String16>",
               "readString16", "writeString16");

  AddAndSetMember(&string_type_,
                  std::make_unique<Type>(ValidatableType::KIND_BUILT_IN, "java.lang", "String",
                                         std::vector<std::string>{"utils/String16.h"},
                                         "::android::String16", "readString16", "writeString16",
                                         string_array_type, nullable_string_type));

  using ::android::aidl::kAidlReservedTypePackage;
  using ::android::aidl::kUtf8InCppStringClass;

  // This type is a Utf16 string in the parcel, but deserializes to
  // a std::string in Utf8 format when we use it in C++.
  Type* cpp_utf8_string_array = new CppArrayType(
      ValidatableType::KIND_BUILT_IN,
      kAidlReservedTypePackage, kUtf8InCppStringClass,
      "string", "::std::string", "::std::unique_ptr<::std::string>",
      "readUtf8VectorFromUtf16Vector", "writeUtf8VectorAsUtf16Vector",
      false);
  Type* nullable_cpp_utf8_string_type =
      new Type(ValidatableType::KIND_BUILT_IN, kAidlReservedTypePackage, kUtf8InCppStringClass,
               std::vector<std::string>{"string", "memory"}, "::std::unique_ptr<::std::string>",
               "readUtf8FromUtf16", "writeUtf8AsUtf16");
  Add(std::make_unique<Type>(ValidatableType::KIND_BUILT_IN, kAidlReservedTypePackage,
                             kUtf8InCppStringClass, std::vector<std::string>{"string"},
                             "::std::string", "readUtf8FromUtf16", "writeUtf8AsUtf16",
                             cpp_utf8_string_array, nullable_cpp_utf8_string_type));

  Type* nullable_ibinder = new Type(
      ValidatableType::KIND_BUILT_IN, "android.os", "IBinder",
      {"binder/IBinder.h"}, "::android::sp<::android::IBinder>",
      "readNullableStrongBinder", "writeStrongBinder");

  AddAndSetMember(&ibinder_type_,
                  std::make_unique<Type>(ValidatableType::KIND_BUILT_IN, "android.os", "IBinder",
                                         std::vector<std::string>{"binder/IBinder.h"},
                                         "::android::sp<::android::IBinder>", "readStrongBinder",
                                         "writeStrongBinder", kNoArrayType, nullable_ibinder));

  Add(std::make_unique<MapType>());

  Add(std::make_unique<BinderListType>());
  Add(std::make_unique<StringListType>());
  Add(std::make_unique<Utf8InCppStringListType>());

  Type* fd_vector_type = new CppArrayType(
      ValidatableType::KIND_BUILT_IN, kNoPackage, "FileDescriptor",
      "android-base/unique_fd.h",
      "::android::base::unique_fd", "::android::base::unique_fd",
      "readUniqueFileDescriptorVector", "writeUniqueFileDescriptorVector",
      false);

  Add(std::make_unique<Type>(ValidatableType::KIND_BUILT_IN, kNoPackage, "FileDescriptor",
                             std::vector<std::string>{"android-base/unique_fd.h"},
                             "::android::base::unique_fd", "readUniqueFileDescriptor",
                             "writeUniqueFileDescriptor", fd_vector_type));

  Type* pfd_vector_type =
      new CppArrayType(ValidatableType::KIND_BUILT_IN, "android.os", "ParcelFileDescriptor",
                       "binder/ParcelFileDescriptor.h", "::android::os::ParcelFileDescriptor",
                       "::android::os::ParcelFileDescriptor", "readParcelableVector",
                       "writeParcelableVector", false);

  Type* nullable_pfd_type =
      new Type(ValidatableType::KIND_BUILT_IN, "android.os", "ParcelFileDescriptor",
               std::vector<std::string>{"memory", "binder/ParcelFileDescriptor.h"},
               "::std::unique_ptr<::android::os::ParcelFileDescriptor>", "readParcelable",
               "writeNullableParcelable");

  Add(std::make_unique<Type>(ValidatableType::KIND_BUILT_IN, "android.os", "ParcelFileDescriptor",
                             std::vector<std::string>{"binder/ParcelFileDescriptor.h"},
                             "::android::os::ParcelFileDescriptor", "readParcelable",
                             "writeParcelable", pfd_vector_type, nullable_pfd_type));

  // Qualify VoidType so we don't get collisions with the VoidType method
  AddAndSetMember(&void_type_, std::make_unique<class VoidType>());
}

bool TypeNamespace::AddParcelableType(const AidlParcelable& p, const std::string& filename) {
  const std::string cpp_header = p.AsStructuredParcelable() ? GetCppHeader(p) : p.GetCppHeader();

  if (cpp_header.empty()) {
    AIDL_ERROR(p) << "Parcelable " << p.GetCanonicalName() << " has no C++ header defined.";
    return false;
  }

  Add(std::make_unique<ParcelableType>(p, cpp_header, filename));
  return true;
}

bool TypeNamespace::AddBinderType(const AidlInterface& b, const std::string& filename) {
  Add(std::make_unique<BinderType>(b, filename));
  return true;
}

bool TypeNamespace::AddListType(const std::string& type_name) {
  const Type* contained_type = FindTypeByCanonicalName(type_name);
  if (!contained_type) {
    LOG(ERROR) << "Cannot create List<" << type_name << "> because contained "
                  "type cannot be found or is invalid.";
    return false;
  }
  if (contained_type->IsCppPrimitive()) {
    LOG(ERROR) << "Cannot create List<" << type_name << "> because contained "
                  "type is a primitive in Java and Java List cannot hold "
                  "primitives.";
    return false;
  }

  if (contained_type->CanonicalName() == kStringCanonicalName ||
      contained_type->CanonicalName() == kUtf8InCppStringCanonicalName ||
      contained_type == IBinderType()) {
    return true;
  }

  // TODO Support lists of parcelables b/23600712

  LOG(ERROR) << "aidl-cpp does not yet support List<" << type_name << ">";
  return false;
}

bool TypeNamespace::AddMapType(const std::string& /* key_type_name */,
                               const std::string& /* value_type_name */) {
  // TODO Support list types b/25242025
  LOG(ERROR) << "aidl does not implement support for typed maps!";
  return false;
}

const ValidatableType* TypeNamespace::GetArgType(const AidlArgument& a, int arg_index,
                                                 const AidlDefinedType& context) const {
  return ::android::aidl::TypeNamespace::GetArgType(a, arg_index, context);
}

}  // namespace cpp
}  // namespace aidl
}  // namespace android
