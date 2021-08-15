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
#include <set>
#include <vector>

#include <android-base/macros.h>

#include "type_namespace.h"

namespace android {
namespace aidl {
namespace cpp {

class Type : public ValidatableType {
 public:
  Type(int kind,  // from ValidatableType
       const std::string& package,
       const std::string& aidl_type,
       const std::vector<std::string>& header,
       const std::string& cpp_type,
       const std::string& read_method,
       const std::string& write_method,
       Type* array_type = nullptr,
       Type* nullable_type = nullptr,
       const std::string& src_file_name = "",
       int line = -1);
  virtual ~Type() = default;

  // overrides of ValidatableType
  bool CanWriteToParcel() const override;

  const Type* ArrayType() const override { return array_type_.get(); }
  const Type* NullableType() const override { return nullable_type_.get(); }
  std::string CppType() const { return cpp_type_; }
  const std::string& ReadFromParcelMethod() const {
    return parcel_read_method_;
  }
  const std::string& WriteToParcelMethod() const {
    return parcel_write_method_;
  }

  void GetHeaders(std::set<std::string>* headers) const {
    for (const std::string& header : headers_) {
      if (!header.empty()) {
        headers->insert(header);
      }
    }
  }
  virtual bool IsCppPrimitive() const { return false; }
  virtual std::string WriteCast(const std::string& value) const {
    return value;
  }

 private:
  // |headers| are the headers we must include to use this type
  const std::vector<std::string> headers_;
  // |aidl_type| is what we find in the yacc generated AST (e.g. "int").
  const std::string aidl_type_;
  // |cpp_type| is what we use in the generated C++ code (e.g. "int32_t").
  const std::string cpp_type_;
  const std::string parcel_read_method_;
  const std::string parcel_write_method_;

  const std::unique_ptr<Type> array_type_;
  const std::unique_ptr<Type> nullable_type_;

  DISALLOW_COPY_AND_ASSIGN(Type);
};  // class Type

class TypeNamespace : public ::android::aidl::LanguageTypeNamespace<Type> {
 public:
  TypeNamespace() = default;
  virtual ~TypeNamespace() = default;

  void Init() override;
  bool AddParcelableType(const AidlParcelable& p,
                         const std::string& filename) override;
  bool AddBinderType(const AidlInterface& b,
                     const std::string& filename) override;
  bool AddListType(const std::string& type_name) override;
  bool AddMapType(const std::string& key_type_name,
                  const std::string& value_type_name) override;

  const ValidatableType* GetArgType(const AidlArgument& a, int arg_index,
                                    const AidlDefinedType& context) const override;

  const Type* VoidType() const { return void_type_; }
  const Type* IBinderType() const { return ibinder_type_; }

 private:
  const Type* void_type_ = nullptr;
  const Type* string_type_ = nullptr;
  const Type* ibinder_type_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(TypeNamespace);
};  // class TypeNamespace

}  // namespace cpp
}  // namespace aidl
}  // namespace android
