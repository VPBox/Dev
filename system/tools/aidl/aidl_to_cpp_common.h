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

#pragma once

#include <string>

#include "aidl_language.h"

// This is used to help generate code targetting C++ (the language) whether using the libbinder or
// libbinder_ndk backend.

namespace android {
namespace aidl {
namespace cpp {

// These roughly correspond to the various class names in the C++ hierarchy:
enum class ClassNames {
  BASE,          // Foo (not a real class, but useful in some circumstances).
  CLIENT,        // BpFoo
  SERVER,        // BnFoo
  INTERFACE,     // IFoo
  DEFAULT_IMPL,  // IFooDefault
  RAW,           // (as shown in the file)
};

string ClassName(const AidlDefinedType& defined_type, ClassNames type);

// Generate the relative path to a header file.  If |use_os_sep| we'll use the
// operating system specific path separator rather than C++'s expected '/' when
// including headers.
std::string HeaderFile(const AidlDefinedType& defined_type, ClassNames class_type,
                       bool use_os_sep = true);

void EnterNamespace(CodeWriter& out, const AidlDefinedType& defined_type);
void LeaveNamespace(CodeWriter& out, const AidlDefinedType& defined_type);

string BuildVarName(const AidlArgument& a);
const string GenLogBeforeExecute(const string className, const AidlMethod& method, bool isServer,
                                 bool isNdk);
const string GenLogAfterExecute(const string className, const AidlInterface& interface,
                                const AidlMethod& method, const string& statusVarName,
                                const string& returnVarName, bool isServer, bool isNdk);
}  // namespace cpp
}  // namespace aidl
}  // namespace android
