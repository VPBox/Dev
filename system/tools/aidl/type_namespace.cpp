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

#include "type_namespace.h"

#include <algorithm>
#include <string>
#include <vector>

#include "aidl_language.h"
#include "logging.h"

using android::base::StringPrintf;
using std::string;
using std::vector;

namespace android {
namespace aidl {

// Since packages cannot contain '-' normally, we cannot be asked
// to create a type that conflicts with these strings.
const char kAidlReservedTypePackage[] = "aidl-internal";
const char kUtf8InCppStringClass[] = "Utf8InCppString";

// These *must* match the package and class names above.
const char kUtf8InCppStringCanonicalName[] = "aidl-internal.Utf8InCppString";

const char kStringCanonicalName[] = "java.lang.String";

const char kUtf8InCppAnnotation[] = "@utfInCpp";

namespace {

bool is_java_keyword(const char* str) {
  static const std::vector<std::string> kJavaKeywords{
      "abstract",   "assert",       "boolean",   "break",      "byte",
      "case",       "catch",        "char",      "class",      "const",
      "continue",   "default",      "do",        "double",     "else",
      "enum",       "extends",      "final",     "finally",    "float",
      "for",        "goto",         "if",        "implements", "import",
      "instanceof", "int",          "interface", "long",       "native",
      "new",        "package",      "private",   "protected",  "public",
      "return",     "short",        "static",    "strictfp",   "super",
      "switch",     "synchronized", "this",      "throw",      "throws",
      "transient",  "try",          "void",      "volatile",   "while",
      "true",       "false",        "null",
  };
  return std::find(kJavaKeywords.begin(), kJavaKeywords.end(), str) !=
      kJavaKeywords.end();
}

} // namespace

ValidatableType::ValidatableType(
    int kind, const string& package, const string& type_name,
    const string& decl_file, int decl_line)
    : kind_(kind),
      type_name_(type_name),
      canonical_name_((package.empty()) ? type_name
                                        : package + "." + type_name),
      origin_file_(decl_file),
      origin_line_(decl_line) {}

string ValidatableType::HumanReadableKind() const {
  switch (Kind()) {
    case ValidatableType::KIND_BUILT_IN:
      return "a built in";
    case ValidatableType::KIND_PARCELABLE:
      return "a parcelable";
    case ValidatableType::KIND_INTERFACE:
      return "an interface";
    case ValidatableType::KIND_GENERATED:
      return "a generated";
  }
  return "unknown";
}

const ValidatableType* TypeNamespace::GetReturnType(const AidlTypeSpecifier& raw_type,
                                                    const AidlDefinedType& context) const {
  string error_msg;
  const ValidatableType* return_type = GetValidatableType(raw_type, &error_msg, context);
  if (return_type == nullptr) {
    AIDL_ERROR(raw_type) << "Return type " << raw_type.ToString() << ": " << error_msg;
    return nullptr;
  }

  return return_type;
}

bool TypeNamespace::AddDefinedTypes(vector<AidlDefinedType*>& types, const string& filename) {
  bool success = true;
  for (const auto type : types) {
    const AidlInterface* interface = type->AsInterface();
    if (interface != nullptr) {
      success &= AddBinderType(*interface, filename);
      continue;
    }

    const AidlParcelable* parcelable = type->AsParcelable();
    if (parcelable != nullptr) {
      success &= AddParcelableType(*parcelable, filename);
      continue;
    }

    CHECK(false) << "aidl internal error: unrecognized type";
  }
  return success;
}

const ValidatableType* TypeNamespace::GetArgType(const AidlArgument& a, int arg_index,
                                                 const AidlDefinedType& context) const {
  string error_prefix =
      StringPrintf("parameter %s (argument %d): ", a.GetName().c_str(), arg_index);

  // check the arg type
  string error_msg;
  const ValidatableType* t = GetValidatableType(a.GetType(), &error_msg, context);
  if (t == nullptr) {
    AIDL_ERROR(a) << error_prefix << error_msg;
    return nullptr;
  }

  const bool can_be_out = typenames_.CanBeOutParameter(a.GetType());
  if (!a.DirectionWasSpecified() && can_be_out) {
    AIDL_ERROR(a) << error_prefix << "'" << a.GetType().ToString()
                  << "' can be an out type, so you must declare it as in, out, or inout.";
    return nullptr;
  }

  if (a.GetDirection() != AidlArgument::IN_DIR && !can_be_out) {
    AIDL_ERROR(a) << error_prefix << "'" << a.ToString() << "' can only be an in parameter.";
    return nullptr;
  }

  // check that the name doesn't match a keyword
  if (is_java_keyword(a.GetName().c_str())) {
    AIDL_ERROR(a) << error_prefix << "Argument name is a Java or aidl keyword";
    return nullptr;
  }

  // Reserve a namespace for internal use
  if (a.GetName().substr(0, 5)  == "_aidl") {
    AIDL_ERROR(a) << error_prefix << "Argument name cannot begin with '_aidl'";
    return nullptr;
  }

  return t;
}

}  // namespace aidl
}  // namespace android
