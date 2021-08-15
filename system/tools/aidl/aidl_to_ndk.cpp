/*
 * Copyright (C) 2018, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * limitations under the License.
 */

#include "aidl_to_ndk.h"
#include "aidl_language.h"
#include "aidl_to_cpp_common.h"
#include "logging.h"
#include "os.h"

#include <android-base/strings.h>

#include <functional>

using ::android::base::Join;

namespace android {
namespace aidl {
namespace ndk {

std::string NdkHeaderFile(const AidlDefinedType& defined_type, cpp::ClassNames name,
                          bool use_os_sep) {
  char seperator = (use_os_sep) ? OS_PATH_SEPARATOR : '/';
  return std::string("aidl") + seperator + cpp::HeaderFile(defined_type, name, use_os_sep);
}

// This represents a type in AIDL (e.g. 'String' which can be referenced in multiple ways)
struct TypeInfo {
  struct Aspect {
    // name of the type in C++ output
    std::string cpp_name;
    // whether to prefer 'value type' over 'const&'
    bool value_is_cheap;

    std::function<void(const CodeGeneratorContext& c)> read_func;
    std::function<void(const CodeGeneratorContext& c)> write_func;
  };

  // e.g. 'String'
  Aspect raw;

  // e.g. 'String[]'
  std::shared_ptr<Aspect> array;

  // note: Nullable types do not exist in Java. For most Java types, the type is split into a
  // nullable and non-nullable variant. This is because C++ types are more usually non-nullable, but
  // everything in Java is non-nullable. This does mean that some Java interfaces may have to have
  // '@nullable' added to them in order to function as expected w/ the NDK. It also means that some
  // transactions will be allowed in Java which are not allowed in C++. However, in Java, if a null
  // is ignored, it will just result in a NullPointerException and be delivered to the other side.
  // C++ does not have this same capacity (in Android), and so instead, we distinguish nullability
  // in the type system.

  // e.g. '@nullable String'
  std::shared_ptr<Aspect> nullable;

  // e.g. '@nullable String[]'
  std::shared_ptr<Aspect> nullable_array;
};

static std::function<void(const CodeGeneratorContext& c)> StandardRead(const std::string& name) {
  return [name](const CodeGeneratorContext& c) {
    c.writer << name << "(" << c.parcel << ", " << c.var << ")";
  };
}
static std::function<void(const CodeGeneratorContext& c)> StandardWrite(const std::string& name) {
  return [name](const CodeGeneratorContext& c) {
    c.writer << name << "(" << c.parcel << ", " << c.var << ")";
  };
}

TypeInfo PrimitiveType(const std::string& cpp_name, const std::string& pretty_name) {
  return TypeInfo{
      .raw =
          TypeInfo::Aspect{
              .cpp_name = cpp_name,
              .value_is_cheap = true,
              .read_func = StandardRead("AParcel_read" + pretty_name),
              .write_func = StandardWrite("AParcel_write" + pretty_name),
          },
      .array = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
          .cpp_name = "std::vector<" + cpp_name + ">",
          .value_is_cheap = false,
          .read_func = StandardRead("::ndk::AParcel_readVector"),
          .write_func = StandardWrite("::ndk::AParcel_writeVector"),
      }),
      .nullable = nullptr,
      .nullable_array = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
          .cpp_name = "std::optional<std::vector<" + cpp_name + ">>",
          .value_is_cheap = false,
          .read_func = StandardRead("::ndk::AParcel_readVector"),
          .write_func = StandardWrite("::ndk::AParcel_writeVector"),
      }),
  };
}

TypeInfo InterfaceTypeInfo(const AidlInterface& type) {
  const std::string clazz = NdkFullClassName(type, cpp::ClassNames::INTERFACE);

  return TypeInfo{
      .raw =
          TypeInfo::Aspect{
              .cpp_name = "std::shared_ptr<" + clazz + ">",
              .value_is_cheap = false,
              // TODO(b/111445392): these should be non-null
              .read_func = StandardRead(clazz + "::readFromParcel"),
              .write_func = StandardWrite(clazz + "::writeToParcel"),
          },
      .array = nullptr,
      .nullable = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
          .cpp_name = "std::shared_ptr<" + clazz + ">",
          .value_is_cheap = false,
          .read_func = StandardRead(clazz + "::readFromParcel"),
          .write_func = StandardWrite(clazz + "::writeToParcel"),
      }),
      .nullable_array = nullptr,
  };
}

TypeInfo ParcelableTypeInfo(const AidlParcelable& type) {
  const std::string clazz = NdkFullClassName(type, cpp::ClassNames::BASE);

  return TypeInfo{
      .raw =
          TypeInfo::Aspect{
              .cpp_name = clazz,
              .value_is_cheap = false,
              .read_func =
                  [](const CodeGeneratorContext& c) {
                    c.writer << "(" << c.var << ")->readFromParcel(" << c.parcel << ")";
                  },
              .write_func =
                  [](const CodeGeneratorContext& c) {
                    c.writer << "(" << c.var << ").writeToParcel(" << c.parcel << ")";
                  },
          },
      .array = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
          .cpp_name = "std::vector<" + clazz + ">",
          .value_is_cheap = false,
          .read_func = StandardRead("::ndk::AParcel_readVector"),
          .write_func = StandardWrite("::ndk::AParcel_writeVector"),
      }),
      .nullable = nullptr,
      .nullable_array = nullptr,
  };
}

// map from AIDL built-in type name to the corresponding Ndk type name
static map<std::string, TypeInfo> kNdkTypeInfoMap = {
    {"void", TypeInfo{{"void", true, nullptr, nullptr}, nullptr, nullptr, nullptr}},
    {"boolean", PrimitiveType("bool", "Bool")},
    {"byte", PrimitiveType("int8_t", "Byte")},
    {"char", PrimitiveType("char16_t", "Char")},
    {"int", PrimitiveType("int32_t", "Int32")},
    {"long", PrimitiveType("int64_t", "Int64")},
    {"float", PrimitiveType("float", "Float")},
    {"double", PrimitiveType("double", "Double")},
    {"String",
     TypeInfo{
         .raw =
             TypeInfo::Aspect{
                 .cpp_name = "std::string",
                 .value_is_cheap = false,
                 .read_func = StandardRead("::ndk::AParcel_readString"),
                 .write_func = StandardWrite("::ndk::AParcel_writeString"),
             },
         .array = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
             .cpp_name = "std::vector<std::string>",
             .value_is_cheap = false,
             .read_func = StandardRead("::ndk::AParcel_readVector"),
             .write_func = StandardWrite("::ndk::AParcel_writeVector"),
         }),
         .nullable = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
             .cpp_name = "std::optional<std::string>",
             .value_is_cheap = false,
             .read_func = StandardRead("::ndk::AParcel_readString"),
             .write_func = StandardWrite("::ndk::AParcel_writeString"),
         }),
         .nullable_array = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
             .cpp_name = "std::optional<std::vector<std::optional<std::string>>>",
             .value_is_cheap = false,
             .read_func = StandardRead("::ndk::AParcel_readVector"),
             .write_func = StandardWrite("::ndk::AParcel_writeVector"),
         }),
     }},
    // TODO(b/111445392) {"List", ""},
    // TODO(b/111445392) {"Map", ""},
    {"IBinder",
     TypeInfo{
         .raw =
             TypeInfo::Aspect{
                 .cpp_name = "::ndk::SpAIBinder",
                 .value_is_cheap = false,
                 .read_func = StandardRead("::ndk::AParcel_readRequiredStrongBinder"),
                 .write_func = StandardRead("::ndk::AParcel_writeRequiredStrongBinder"),
             },
         .array = nullptr,
         .nullable = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
             .cpp_name = "::ndk::SpAIBinder",
             .value_is_cheap = false,
             .read_func = StandardRead("::ndk::AParcel_readNullableStrongBinder"),
             .write_func = StandardRead("::ndk::AParcel_writeNullableStrongBinder"),
         }),
         .nullable_array = nullptr,
     }},
    // TODO(b/111445392) {"FileDescriptor", ""},
    {"ParcelFileDescriptor",
     TypeInfo{
         .raw =
             TypeInfo::Aspect{
                 .cpp_name = "::ndk::ScopedFileDescriptor",
                 .value_is_cheap = false,
                 .read_func = StandardRead("::ndk::AParcel_readRequiredParcelFileDescriptor"),
                 .write_func = StandardRead("::ndk::AParcel_writeRequiredParcelFileDescriptor"),
             },
         .array = nullptr,
         .nullable = std::shared_ptr<TypeInfo::Aspect>(new TypeInfo::Aspect{
             .cpp_name = "::ndk::ScopedFileDescriptor",
             .value_is_cheap = false,
             .read_func = StandardRead("::ndk::AParcel_readNullableParcelFileDescriptor"),
             .write_func = StandardRead("::ndk::AParcel_writeNullableParcelFileDescriptor"),
         }),
         .nullable_array = nullptr,
     }},
    // TODO(b/111445392) {"CharSequence", ""},
};

static TypeInfo::Aspect GetTypeAspect(const AidlTypenames& types, const AidlTypeSpecifier& aidl) {
  CHECK(aidl.IsResolved()) << aidl.ToString();

  const string aidl_name = aidl.GetName();

  // TODO(b/112664205): this is okay for some types
  AIDL_FATAL_IF(aidl.IsGeneric(), aidl) << aidl.ToString();

  TypeInfo info;
  if (AidlTypenames::IsBuiltinTypename(aidl_name)) {
    auto it = kNdkTypeInfoMap.find(aidl_name);
    CHECK(it != kNdkTypeInfoMap.end());
    info = it->second;
  } else {
    const AidlDefinedType* type = types.TryGetDefinedType(aidl_name);
    AIDL_FATAL_IF(type == nullptr, aidl_name) << "Unrecognized type.";

    if (type->AsInterface() != nullptr) {
      info = InterfaceTypeInfo(*type->AsInterface());
    } else if (type->AsParcelable() != nullptr) {
      info = ParcelableTypeInfo(*type->AsParcelable());
    } else {
      AIDL_FATAL(aidl_name) << "Unrecognized type";
    }
  }

  if (aidl.IsArray()) {
    if (aidl.IsNullable()) {
      AIDL_FATAL_IF(info.nullable_array == nullptr, aidl) << "Unsupported type in NDK Backend.";
      return *info.nullable_array;
    }
    AIDL_FATAL_IF(info.array == nullptr, aidl) << "Unsupported type in NDK Backend.";
    return *info.array;
  }

  if (aidl.IsNullable()) {
    AIDL_FATAL_IF(info.nullable == nullptr, aidl) << "Unsupported type in NDK Backend.";
    return *info.nullable;
  }

  return info.raw;
}

std::string NdkFullClassName(const AidlDefinedType& type, cpp::ClassNames name) {
  std::vector<std::string> pieces = {"::aidl"};
  std::vector<std::string> package = type.GetSplitPackage();
  pieces.insert(pieces.end(), package.begin(), package.end());
  pieces.push_back(cpp::ClassName(type, name));

  return Join(pieces, "::");
}

std::string NdkNameOf(const AidlTypenames& types, const AidlTypeSpecifier& aidl, StorageMode mode) {
  TypeInfo::Aspect aspect = GetTypeAspect(types, aidl);

  switch (mode) {
    case StorageMode::STACK:
      return aspect.cpp_name;
    case StorageMode::ARGUMENT:
      if (aspect.value_is_cheap) {
        return aspect.cpp_name;
      } else {
        return "const " + aspect.cpp_name + "&";
      }
    case StorageMode::OUT_ARGUMENT:
      return aspect.cpp_name + "*";
    default:
      AIDL_FATAL(aidl.GetName()) << "Unrecognized mode type: " << static_cast<int>(mode);
  }
}

void WriteToParcelFor(const CodeGeneratorContext& c) {
  TypeInfo::Aspect aspect = GetTypeAspect(c.types, c.type);
  aspect.write_func(c);
}

void ReadFromParcelFor(const CodeGeneratorContext& c) {
  TypeInfo::Aspect aspect = GetTypeAspect(c.types, c.type);
  aspect.read_func(c);
}

std::string NdkArgList(
    const AidlTypenames& types, const AidlMethod& method,
    std::function<std::string(const std::string& type, const std::string& name, bool isOut)>
        formatter) {
  std::vector<std::string> method_arguments;
  for (const auto& a : method.GetArguments()) {
    StorageMode mode = a->IsOut() ? StorageMode::OUT_ARGUMENT : StorageMode::ARGUMENT;
    std::string type = NdkNameOf(types, a->GetType(), mode);
    std::string name = cpp::BuildVarName(*a);
    method_arguments.emplace_back(formatter(type, name, a->IsOut()));
  }

  if (method.GetType().GetName() != "void") {
    std::string type = NdkNameOf(types, method.GetType(), StorageMode::OUT_ARGUMENT);
    std::string name = "_aidl_return";
    method_arguments.emplace_back(formatter(type, name, true));
  }

  return Join(method_arguments, ", ");
}

std::string NdkMethodDecl(const AidlTypenames& types, const AidlMethod& method,
                          const std::string& clazz) {
  std::string class_prefix = clazz.empty() ? "" : (clazz + "::");
  return "::ndk::ScopedAStatus " + class_prefix + method.GetName() + "(" +
         NdkArgList(types, method, FormatArgForDecl) + ")";
}

}  // namespace ndk
}  // namespace aidl
}  // namespace android
