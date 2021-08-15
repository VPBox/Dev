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

#include <string>
#include <vector>

#include "ast_java.h"
#include "type_namespace.h"

namespace android {
namespace aidl {
namespace java {

class JavaTypeNamespace;

class Type : public ValidatableType {
 public:
  // WriteToParcel flags
  enum { PARCELABLE_WRITE_RETURN_VALUE = 0x0001 };

  // defaultValue is by default set to "null" because that is the default value
  // for most of the types like class and array. default values for built-in
  // types like int, double, boolean, etc. are explicitly set via BasicType
  Type(const JavaTypeNamespace* types, const std::string& name, int kind, bool canWriteToParcel);
  Type(const JavaTypeNamespace* types, const std::string& package, const std::string& name,
       int kind, bool canWriteToParcel, const std::string& declFile = "", int declLine = -1);
  virtual ~Type() = default;

  bool CanWriteToParcel() const override { return m_canWriteToParcel; }

  const ValidatableType* ArrayType() const override { return m_array_type.get(); }
  const ValidatableType* NullableType() const override { return nullptr; }

  virtual std::string JavaType() const { return m_javaType; }
  virtual std::string InstantiableName() const;

  // The namespace where this type is defined in
  const JavaTypeNamespace* GetTypeNamespace() const { return m_types; }

 protected:
  Expression* BuildWriteToParcelFlags(int flags) const;

  const JavaTypeNamespace* m_types;

  std::unique_ptr<Type> m_array_type;

 private:
  Type();
  Type(const Type&);

  std::string m_javaType;
  std::string m_declFile;
  bool m_canWriteToParcel;
};

class BasicArrayType : public Type {
 public:
  BasicArrayType(const JavaTypeNamespace* types, const std::string& name,
                 const std::string& writeArrayParcel,
                 const std::string& createArrayParcel,
                 const std::string& readArrayParcel);

  const ValidatableType* NullableType() const override { return this; }

 private:
  std::string m_writeArrayParcel;
  std::string m_createArrayParcel;
  std::string m_readArrayParcel;
};

class BasicType : public Type {
 public:
  BasicType(const JavaTypeNamespace* types, const std::string& name,
            const std::string& marshallParcel, const std::string& unmarshallParcel,
            const std::string& writeArrayParcel, const std::string& createArrayParcel,
            const std::string& readArrayParcel);

 private:
  std::string m_marshallParcel;
  std::string m_unmarshallParcel;
};

class FileDescriptorArrayType : public Type {
 public:
  explicit FileDescriptorArrayType(const JavaTypeNamespace* types);

  const ValidatableType* NullableType() const override { return this; }
};

class FileDescriptorType : public Type {
 public:
  explicit FileDescriptorType(const JavaTypeNamespace* types);
};

class ParcelFileDescriptorArrayType : public Type {
 public:
  explicit ParcelFileDescriptorArrayType(const JavaTypeNamespace* types);

  const ValidatableType* NullableType() const override { return this; }
};

class ParcelFileDescriptorType : public Type {
 public:
  explicit ParcelFileDescriptorType(const JavaTypeNamespace* types);

  const ValidatableType* NullableType() const override { return this; }
};

class BooleanArrayType : public Type {
 public:
  explicit BooleanArrayType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class BooleanType : public Type {
 public:
  explicit BooleanType(const JavaTypeNamespace* types);
};

class CharArrayType : public Type {
 public:
  explicit CharArrayType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class CharType : public Type {
 public:
  explicit CharType(const JavaTypeNamespace* types);
};

class StringArrayType : public Type {
 public:
  explicit StringArrayType(const JavaTypeNamespace* types);

  const ValidatableType* NullableType() const override { return this; }
};

class StringType : public Type {
 public:
  StringType(const JavaTypeNamespace* types, const std::string& package,
             const std::string& class_name);

  std::string JavaType() const override { return "java.lang.String"; }
  const ValidatableType* NullableType() const override { return this; }
};

class CharSequenceType : public Type {
 public:
  explicit CharSequenceType(const JavaTypeNamespace* types);
};

class RemoteExceptionType : public Type {
 public:
  explicit RemoteExceptionType(const JavaTypeNamespace* types);
};

class RuntimeExceptionType : public Type {
 public:
  explicit RuntimeExceptionType(const JavaTypeNamespace* types);
};

class IBinderArrayType : public Type {
 public:
  explicit IBinderArrayType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class IBinderType : public Type {
 public:
  explicit IBinderType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class IInterfaceType : public Type {
 public:
  explicit IInterfaceType(const JavaTypeNamespace* types);
};

class BinderType : public Type {
 public:
  explicit BinderType(const JavaTypeNamespace* types);
};

class BinderProxyType : public Type {
 public:
  explicit BinderProxyType(const JavaTypeNamespace* types);
};

class ParcelType : public Type {
 public:
  explicit ParcelType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class ParcelableInterfaceType : public Type {
 public:
  explicit ParcelableInterfaceType(const JavaTypeNamespace* types);
};

class MapType : public Type {
 public:
  explicit MapType(const JavaTypeNamespace* types);
  const ValidatableType* NullableType() const override { return this; }
};

class ListType : public Type {
 public:
  explicit ListType(const JavaTypeNamespace* types);

  std::string InstantiableName() const override;
  const ValidatableType* NullableType() const override { return this; }
};

class UserDataArrayType : public Type {
 public:
  UserDataArrayType(const JavaTypeNamespace* types, const std::string& package,
                    const std::string& name, bool builtIn,
                    bool canWriteToParcel, const std::string& declFile = "",
                    int declLine = -1);

  const ValidatableType* NullableType() const override { return this; }
};

class UserDataType : public Type {
 public:
  UserDataType(const JavaTypeNamespace* types, const std::string& package,
               const std::string& name, bool builtIn, bool canWriteToParcel,
               const std::string& declFile = "", int declLine = -1);

  const ValidatableType* NullableType() const override { return this; }
};

class InterfaceType : public Type {
 public:
  InterfaceType(const JavaTypeNamespace* types, const std::string& package, const std::string& name,
                bool builtIn, const std::string& declFile, int declLine, const Type* stub,
                const Type* proxy, const Type* defaultImpl);

  const ValidatableType* NullableType() const override { return this; }
  const Type* GetStub() const { return stub_; }
  const Type* GetProxy() const { return proxy_; }
  const Type* GetDefaultImpl() const { return defaultImpl_; }

 private:
  const Type* stub_;
  const Type* proxy_;
  const Type* defaultImpl_;
};

class ClassLoaderType : public Type {
 public:
  explicit ClassLoaderType(const JavaTypeNamespace* types);
};

class GenericListType : public Type {
 public:
  GenericListType(const JavaTypeNamespace* types, const Type* arg);

  std::string InstantiableName() const override;
  std::string JavaType() const override {
    return "java.util.List<" + m_contained_type->JavaType() + ">";
  }

  const ValidatableType* NullableType() const override { return this; }

 private:
  const Type* m_contained_type;
};

class JavaTypeNamespace : public LanguageTypeNamespace<Type> {
 public:
  JavaTypeNamespace() = default;
  virtual ~JavaTypeNamespace() = default;

  void Init() override;
  bool AddParcelableType(const AidlParcelable& p,
                         const std::string& filename) override;
  bool AddBinderType(const AidlInterface& b,
                     const std::string& filename) override;
  bool AddListType(const std::string& contained_type_name) override;
  bool AddMapType(const std::string& key_type_name,
                  const std::string& value_type_name) override;

  const Type* BoolType() const { return m_bool_type; }
  const Type* IntType() const { return m_int_type; }
  const Type* StringType() const { return m_string_type; }
  const Type* TextUtilsType() const { return m_text_utils_type; }
  const Type* RemoteExceptionType() const { return m_remote_exception_type; }
  const Type* RuntimeExceptionType() const { return m_runtime_exception_type; }
  const Type* IBinderType() const { return m_ibinder_type; }
  const Type* IInterfaceType() const { return m_iinterface_type; }
  const Type* BinderNativeType() const { return m_binder_native_type; }
  const Type* BinderProxyType() const { return m_binder_proxy_type; }
  const Type* ParcelType() const { return m_parcel_type; }
  const Type* ParcelableInterfaceType() const {
    return m_parcelable_interface_type;
  }
  const Type* ContextType() const { return m_context_type; }
  const Type* ClassLoaderType() const { return m_classloader_type; }

 private:
  const Type* m_bool_type{nullptr};
  const Type* m_int_type{nullptr};
  const Type* m_string_type{nullptr};
  const Type* m_text_utils_type{nullptr};
  const Type* m_remote_exception_type{nullptr};
  const Type* m_runtime_exception_type{nullptr};
  const Type* m_ibinder_type{nullptr};
  const Type* m_iinterface_type{nullptr};
  const Type* m_binder_native_type{nullptr};
  const Type* m_binder_proxy_type{nullptr};
  const Type* m_parcel_type{nullptr};
  const Type* m_parcelable_interface_type{nullptr};
  const Type* m_context_type{nullptr};
  const Type* m_classloader_type{nullptr};

  DISALLOW_COPY_AND_ASSIGN(JavaTypeNamespace);
};

extern Expression* NULL_VALUE;
extern Expression* THIS_VALUE;
extern Expression* SUPER_VALUE;
extern Expression* TRUE_VALUE;
extern Expression* FALSE_VALUE;

}  // namespace java
}  // namespace aidl
}  // namespace android
