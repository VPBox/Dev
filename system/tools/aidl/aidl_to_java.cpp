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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aidl_to_java.h"
#include "aidl_language.h"
#include "aidl_typenames.h"
#include "logging.h"

#include <android-base/strings.h>

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace android {
namespace aidl {
namespace java {

using android::base::Join;

using std::endl;
using std::function;
using std::map;
using std::string;
using std::vector;

std::string ConstantValueDecorator(const AidlTypeSpecifier& /*type*/,
                                   const std::string& raw_value) {
  // no difference
  return raw_value;
};

const string& JavaNameOf(const AidlTypeSpecifier& aidl) {
  CHECK(aidl.IsResolved()) << aidl.ToString();

  // map from AIDL built-in type name to the corresponding Java type name
  static map<string, string> m = {
      {"void", "void"},
      {"boolean", "boolean"},
      {"byte", "byte"},
      {"char", "char"},
      {"int", "int"},
      {"long", "long"},
      {"float", "float"},
      {"double", "double"},
      {"String", "java.lang.String"},
      {"List", "java.util.List"},
      {"Map", "java.util.Map"},
      {"IBinder", "android.os.IBinder"},
      {"FileDescriptor", "java.io.FileDescriptor"},
      {"CharSequence", "java.lang.CharSequence"},
      {"ParcelFileDescriptor", "android.os.ParcelFileDescriptor"},
  };
  const string& aidl_name = aidl.GetName();
  if (m.find(aidl_name) != m.end()) {
    CHECK(AidlTypenames::IsBuiltinTypename(aidl_name));
    return m[aidl_name];
  } else {
    // 'foo.bar.IFoo' in AIDL maps to 'foo.bar.IFoo' in Java
    return aidl_name;
  }
}

string JavaSignatureOf(const AidlTypeSpecifier& aidl) {
  string ret = JavaNameOf(aidl);
  if (aidl.IsGeneric()) {
    vector<string> arg_names;
    for (const auto& ta : aidl.GetTypeParameters()) {
      arg_names.emplace_back(JavaSignatureOf(*ta));
    }
    ret += "<" + Join(arg_names, ",") + ">";
  }
  if (aidl.IsArray()) {
    ret += "[]";
  }
  return ret;
}

string DefaultJavaValueOf(const AidlTypeSpecifier& aidl) {
  static map<string, string> m = {
      {"boolean", "false"}, {"byte", "0"},     {"char", R"('\u0000')"}, {"int", "0"},
      {"long", "0L"},       {"float", "0.0f"}, {"double", "0.0d"},
  };
  const string& name = aidl.GetName();
  assert(name != "void");

  if (!aidl.IsArray() && m.find(name) != m.end()) {
    CHECK(AidlTypenames::IsBuiltinTypename(name));
    return m[name];
  } else {
    return "null";
  }
}

// These are supported by AIDL syntax, but are unsupported by the AIDL compiler
static bool IsMarshallingUnsupportedFor(const AidlTypeSpecifier& aidl,
                                        const AidlTypenames& typenames) {
  const string name = aidl.GetName();

  // List<T> is support only for String, Binder, ParcelFileDescriptor and Parcelable.
  if (name == "List" && aidl.IsGeneric()) {
    const string& contained_type = aidl.GetTypeParameters().at(0)->GetName();
    if (AidlTypenames::IsBuiltinTypename(contained_type)) {
      if (contained_type != "String" && contained_type != "IBinder" &&
          contained_type != "ParcelFileDescriptor") {
        return true;
      }
    } else {
      const AidlDefinedType* t = typenames.TryGetDefinedType(contained_type);
      if (t != nullptr && t->AsInterface() != nullptr) {
        return true;
      }
    }
  }

  // List[], Map[], CharSequence[] are not supported.
  if (AidlTypenames::IsBuiltinTypename(name) && aidl.IsArray()) {
    if (name == "List" || name == "Map" || name == "CharSequence") {
      return true;
    }
  }

  // T[] is not supported for interfaces
  const AidlDefinedType* t = typenames.TryGetDefinedType(name);
  if (aidl.IsArray() && t != nullptr && t->AsInterface() != nullptr) {
    return true;
  }

  return false;
}

static bool EnsureCodegenIsSupported(const CodeGeneratorContext& c) {
  if (IsMarshallingUnsupportedFor(c.type, c.typenames)) {
    AIDL_ERROR(c.type) << c.type.ToString() << "' is not yet supported.";
    return false;
  }
  return true;
}

static string GetFlagFor(const CodeGeneratorContext& c) {
  if (c.is_return_value) {
    return "android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE";
  } else {
    return "0";
  }
}

bool WriteToParcelFor(const CodeGeneratorContext& c) {
  if (!EnsureCodegenIsSupported(c)) {
    return false;
  }
  static map<string, function<void(const CodeGeneratorContext&)>> method_map{
      {"boolean",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeInt(((" << c.var << ")?(1):(0)));\n";
       }},
      {"boolean[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeBooleanArray(" << c.var << ");\n";
       }},
      {"byte",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeByte(" << c.var << ");\n";
       }},
      {"byte[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeByteArray(" << c.var << ");\n";
       }},
      {"char",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeInt(((int)" << c.var << "));\n";
       }},
      {"char[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeCharArray(" << c.var << ");\n";
       }},
      {"int",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeInt(" << c.var << ");\n";
       }},
      {"int[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeIntArray(" << c.var << ");\n";
       }},
      {"long",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeLong(" << c.var << ");\n";
       }},
      {"long[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeLongArray(" << c.var << ");\n";
       }},
      {"float",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeFloat(" << c.var << ");\n";
       }},
      {"float[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeFloatArray(" << c.var << ");\n";
       }},
      {"double",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeDouble(" << c.var << ");\n";
       }},
      {"double[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeDoubleArray(" << c.var << ");\n";
       }},
      {"String",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeString(" << c.var << ");\n";
       }},
      {"String[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeStringArray(" << c.var << ");\n";
       }},
      {"List",
       [](const CodeGeneratorContext& c) {
         if (c.type.IsGeneric()) {
           const string& contained_type = c.type.GetTypeParameters().at(0)->GetName();
           if (AidlTypenames::IsBuiltinTypename(contained_type)) {
             if (contained_type == "String") {
               c.writer << c.parcel << ".writeStringList(" << c.var << ");\n";
             } else if (contained_type == "IBinder") {
               c.writer << c.parcel << ".writeBinderList(" << c.var << ");\n";
             }
           } else {
             const AidlDefinedType* t = c.typenames.TryGetDefinedType(contained_type);
             CHECK(t != nullptr) << "Unknown type: " << contained_type << endl;
             if (t->AsParcelable() != nullptr) {
               c.writer << c.parcel << ".writeTypedList(" << c.var << ");\n";
             }
           }
         } else {
           c.writer << c.parcel << ".writeList(" << c.var << ");\n";
         }
       }},
      {"Map",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeMap(" << c.var << ");\n";
       }},
      {"IBinder",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeStrongBinder(" << c.var << ");\n";
       }},
      {"IBinder[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeBinderArray(" << c.var << ");\n";
       }},
      {"FileDescriptor",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeRawFileDescriptor(" << c.var << ");\n";
       }},
      {"FileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeRawFileDescriptorArray(" << c.var << ");\n";
       }},
      {"ParcelFileDescriptor",
       [](const CodeGeneratorContext& c) {
         // This is same as writeTypedObject which was introduced with SDK 23.
         // Keeping below code so that the generated code is buildable with older SDK.
         c.writer << "if ((" << c.var << "!=null)) {\n";
         c.writer.Indent();
         c.writer << c.parcel << ".writeInt(1);\n";
         c.writer << c.var << ".writeToParcel(" << c.parcel << ", " << GetFlagFor(c) << ");\n";
         c.writer.Dedent();
         c.writer << "}\n";
         c.writer << "else {\n";
         c.writer.Indent();
         c.writer << c.parcel << ".writeInt(0);\n";
         c.writer.Dedent();
         c.writer << "}\n";
       }},
      {"ParcelFileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".writeTypedArray(" << c.var << ", " << GetFlagFor(c) << ");\n";
       }},
      {"CharSequence",
       [](const CodeGeneratorContext& c) {
         // TextUtils.writeToParcel does not accept null. So, we need to handle
         // the case here.
         c.writer << "if (" << c.var << "!=null) {\n";
         c.writer.Indent();
         c.writer << c.parcel << ".writeInt(1);\n";
         c.writer << "android.text.TextUtils.writeToParcel(" << c.var << ", " << c.parcel << ", "
                  << GetFlagFor(c) << ");\n";
         c.writer.Dedent();
         c.writer << "}\n";
         c.writer << "else {\n";
         c.writer.Indent();
         c.writer << c.parcel << ".writeInt(0);\n";
         c.writer.Dedent();
         c.writer << "}\n";
       }},
  };
  const string type_name = c.type.GetName() + (c.type.IsArray() ? "[]" : "");
  const auto found = method_map.find(type_name);
  if (found != method_map.end()) {
    found->second(c);
  } else {
    const AidlDefinedType* t = c.typenames.TryGetDefinedType(c.type.GetName());
    CHECK(t != nullptr) << "Unknown type: " << c.type.GetName() << endl;
    if (t->AsInterface() != nullptr) {
      if (!c.type.IsArray()) {
        // Why don't we use writeStrongInterface which does the exact same thing?
        // Keeping below code just not to break unit tests.
        c.writer << c.parcel << ".writeStrongBinder((((" << c.var << "!=null))?"
                 << "(" << c.var << ".asBinder()):(null)));\n";
      }
    } else if (t->AsParcelable() != nullptr) {
      if (c.type.IsArray()) {
        c.writer << c.parcel << ".writeTypedArray(" << c.var << ", " << GetFlagFor(c) << ");\n";
      } else {
        // This is same as writeTypedObject.
        // Keeping below code just not to break tests.
        c.writer << "if ((" << c.var << "!=null)) {\n";
        c.writer.Indent();
        c.writer << c.parcel << ".writeInt(1);\n";
        c.writer << c.var << ".writeToParcel(" << c.parcel << ", " << GetFlagFor(c) << ");\n";
        c.writer.Dedent();
        c.writer << "}\n";
        c.writer << "else {\n";
        c.writer.Indent();
        c.writer << c.parcel << ".writeInt(0);\n";
        c.writer.Dedent();
        c.writer << "}\n";
      }
    }
  }
  return true;
}

// Ensures that a variable is initialized to refer to the classloader
// of the current object and returns the name of the variable.
static string EnsureAndGetClassloader(CodeGeneratorContext& c) {
  CHECK(c.is_classloader_created != nullptr);
  if (!*(c.is_classloader_created)) {
    c.writer << "java.lang.ClassLoader cl = "
             << "(java.lang.ClassLoader)this.getClass().getClassLoader();\n";
    *(c.is_classloader_created) = true;
  }
  return "cl";
}

bool CreateFromParcelFor(const CodeGeneratorContext& c) {
  if (!EnsureCodegenIsSupported(c)) {
    return false;
  }
  static map<string, function<void(const CodeGeneratorContext&)>> method_map{
      {"boolean",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = (0!=" << c.parcel << ".readInt());\n";
       }},
      {"boolean[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createBooleanArray();\n";
       }},
      {"byte",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readByte();\n";
       }},
      {"byte[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createByteArray();\n";
       }},
      {"char",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = (char)" << c.parcel << ".readInt();\n";
       }},
      {"char[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createCharArray();\n";
       }},
      {"int",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readInt();\n";
       }},
      {"int[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createIntArray();\n";
       }},
      {"long",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readLong();\n";
       }},
      {"long[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createLongArray();\n";
       }},
      {"float",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readFloat();\n";
       }},
      {"float[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createFloatArray();\n";
       }},
      {"double",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readDouble();\n";
       }},
      {"double[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createDoubleArray();\n";
       }},
      {"String",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readString();\n";
       }},
      {"String[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createStringArray();\n";
       }},
      {"List",
       [](const CodeGeneratorContext& c) {
         if (c.type.IsGeneric()) {
           const string& contained_type = c.type.GetTypeParameters().at(0)->GetName();
           if (AidlTypenames::IsBuiltinTypename(contained_type)) {
             if (contained_type == "String") {
               c.writer << c.var << " = " << c.parcel << ".createStringArrayList();\n";
             } else if (contained_type == "IBinder") {
               c.writer << c.var << " = " << c.parcel << ".createBinderArrayList();\n";
             }
           } else {
             const AidlDefinedType* t = c.typenames.TryGetDefinedType(contained_type);
             CHECK(t != nullptr) << "Unknown type: " << contained_type << endl;
             if (t->AsParcelable() != nullptr) {
               c.writer << c.var << " = " << c.parcel << ".createTypedArrayList("
                        << JavaNameOf(*(c.type.GetTypeParameters().at(0))) << ".CREATOR);\n";
             }
           }
         } else {
           const string classloader = EnsureAndGetClassloader(const_cast<CodeGeneratorContext&>(c));
           c.writer << c.var << " = " << c.parcel << ".readArrayList(" << classloader << ");\n";
         }
       }},
      {"Map",
       [](const CodeGeneratorContext& c) {
         const string classloader = EnsureAndGetClassloader(const_cast<CodeGeneratorContext&>(c));
         c.writer << c.var << " = " << c.parcel << ".readHashMap(" << classloader << ");\n";
       }},
      {"IBinder",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readStrongBinder();\n";
       }},
      {"IBinder[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createBinderArray();\n";
       }},
      {"FileDescriptor",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".readRawFileDescriptor();\n";
       }},
      {"FileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createRawFileDescriptorArray();\n";
       }},
      {"ParcelFileDescriptor",
       [](const CodeGeneratorContext& c) {
         // This is same as readTypedObject which was introduced with SDK 23.
         // Keeping below code so that the generated code is buildable with older SDK.
         c.writer << "if ((0!=" << c.parcel << ".readInt())) {\n";
         c.writer.Indent();
         c.writer << c.var << " = " << "android.os.ParcelFileDescriptor.CREATOR.createFromParcel(" << c.parcel
                  << ");\n";
         c.writer.Dedent();
         c.writer << "}\n";
         c.writer << "else {\n";
         c.writer.Indent();
         c.writer << c.var << " = null;\n";
         c.writer.Dedent();
         c.writer << "}\n";
       }},
      {"ParcelFileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel
                  << ".createTypedArray(android.os.ParcelFileDescriptor.CREATOR);\n";
       }},
      {"CharSequence",
       [](const CodeGeneratorContext& c) {
         // We have written 0 for null CharSequence.
         c.writer << "if (0!=" << c.parcel << ".readInt()) {\n";
         c.writer.Indent();
         c.writer << c.var << " = android.text.TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel("
                  << c.parcel << ");\n";
         c.writer.Dedent();
         c.writer << "}\n";
         c.writer << "else {\n";
         c.writer.Indent();
         c.writer << c.var << " = null;\n";
         c.writer.Dedent();
         c.writer << "}\n";
       }},
  };
  const string type_name = c.type.GetName() + (c.type.IsArray() ? "[]" : "");
  const auto found = method_map.find(type_name);
  if (found != method_map.end()) {
    found->second(c);
  } else {
    const AidlDefinedType* t = c.typenames.TryGetDefinedType(c.type.GetName());
    CHECK(t != nullptr) << "Unknown type: " << c.type.GetName() << endl;
    if (t->AsInterface() != nullptr) {
      if (!c.type.IsArray()) {
        c.writer << c.var << " = " << c.type.GetName() << ".Stub.asInterface(" << c.parcel
                 << ".readStrongBinder());\n";
      }
    } else if (t->AsParcelable() != nullptr || t->AsStructuredParcelable() != nullptr) {
      if (c.type.IsArray()) {
        c.writer << c.var << " = " << c.parcel << ".createTypedArray(" << JavaNameOf(c.type)
                 << ".CREATOR);\n";
      } else {
        // This is same as readTypedObject.
        // Keeping below code just not to break unit tests.
        c.writer << "if ((0!=" << c.parcel << ".readInt())) {\n";
        c.writer.Indent();
        c.writer << c.var << " = " << c.type.GetName() << ".CREATOR.createFromParcel(" << c.parcel
                 << ");\n";
        c.writer.Dedent();
        c.writer << "}\n";
        c.writer << "else {\n";
        c.writer.Indent();
        c.writer << c.var << " = null;\n";
        c.writer.Dedent();
        c.writer << "}\n";
      }
    }
  }
  return true;
}

bool ReadFromParcelFor(const CodeGeneratorContext& c) {
  if (!EnsureCodegenIsSupported(c)) {
    return false;
  }
  static map<string, function<void(const CodeGeneratorContext&)>> method_map{
      {"boolean[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readBooleanArray(" << c.var << ");\n";
       }},
      {"byte[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readByteArray(" << c.var << ");\n";
       }},
      {"char[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readCharArray(" << c.var << ");\n";
       }},
      {"int[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readIntArray(" << c.var << ");\n";
       }},
      {"long[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readLongArray(" << c.var << ");\n";
       }},
      {"float[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readFloatArray(" << c.var << ");\n";
       }},
      {"double[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readDoubleArray(" << c.var << ");\n";
       }},
      {"String[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readStringArray(" << c.var << ");\n";
       }},
      {"List",
       [](const CodeGeneratorContext& c) {
         if (c.type.IsGeneric()) {
           const string& contained_type = c.type.GetTypeParameters().at(0)->GetName();
           if (AidlTypenames::IsBuiltinTypename(contained_type)) {
             if (contained_type == "String") {
               c.writer << c.parcel << ".readStringList(" << c.var << ");\n";
             } else if (contained_type == "IBinder") {
               c.writer << c.parcel << ".readBinderList(" << c.var << ");\n";
             }
           } else {
             const AidlDefinedType* t = c.typenames.TryGetDefinedType(contained_type);
             CHECK(t != nullptr) << "Unknown type: " << contained_type << endl;
             if (t->AsParcelable() != nullptr) {
               c.writer << c.parcel << ".readTypedList(" << c.var << ", "
                        << JavaNameOf(*(c.type.GetTypeParameters().at(0))) << ".CREATOR);\n";
             }
           }
         } else {
           const string classloader = EnsureAndGetClassloader(const_cast<CodeGeneratorContext&>(c));
           c.writer << c.parcel << ".readList(" << c.var << ", " << classloader << ");\n";
         }
       }},
      {"Map",
       [](const CodeGeneratorContext& c) {
         const string classloader = EnsureAndGetClassloader(const_cast<CodeGeneratorContext&>(c));
         c.writer << c.var << " = " << c.parcel << ".readHashMap(" << classloader << ");\n";
       }},
      {"IBinder[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createBinderArray();\n";
       }},
      {"FileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.var << " = " << c.parcel << ".createRawFileDescriptorArray();\n";
       }},
      {"ParcelFileDescriptor",
       [](const CodeGeneratorContext& c) {
         c.writer << "if ((0!=" << c.parcel << ".readInt())) {\n";
         c.writer.Indent();
         c.writer << c.var << " = " << "android.os.ParcelFileDescriptor.CREATOR.createFromParcel(" << c.parcel << ");\n";
         c.writer.Dedent();
         c.writer << "}\n";
       }},
      {"ParcelFileDescriptor[]",
       [](const CodeGeneratorContext& c) {
         c.writer << c.parcel << ".readTypedArray(" << c.var
                  << ", android.os.ParcelFileDescriptor.CREATOR);\n";
       }},
  };
  const string type_name = c.type.GetName() + (c.type.IsArray() ? "[]" : "");
  const auto& found = method_map.find(type_name);
  if (found != method_map.end()) {
    found->second(c);
  } else {
    const AidlDefinedType* t = c.typenames.TryGetDefinedType(c.type.GetName());
    CHECK(t != nullptr) << "Unknown type: " << c.type.GetName() << endl;
    if (t->AsParcelable() != nullptr) {
      if (c.type.IsArray()) {
        c.writer << c.parcel << ".readTypedArray(" << c.var << ", " << c.type.GetName()
                 << ".CREATOR);\n";
      } else {
        c.writer << "if ((0!=" << c.parcel << ".readInt())) {\n";
        c.writer.Indent();
        c.writer << c.var << ".readFromParcel(" << c.parcel << ");\n";
        c.writer.Dedent();
        c.writer << "}\n";
      }
    }
  }
  return true;
}

}  // namespace java
}  // namespace aidl
}  // namespace android
