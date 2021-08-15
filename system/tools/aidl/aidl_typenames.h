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

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

class AidlDefinedType;
class AidlTypeSpecifier;

namespace android {
namespace aidl {

// AidlTypenames is a collection of AIDL types available to a compilation unit.
//
// Basic types (such as int, String, etc.) are added by default, while defined
// types (such as IFoo, MyParcelable, etc.) and types from preprocessed inputs
// are added as they are recognized by the parser.
//
// When AidlTypeSpecifier is encountered during parsing, parser defers the
// resolution of it until the end of the parsing, where it uses AidlTypenames
// to resolve type names in AidlTypeSpecifier.
//
// Note that nothing here is specific to either Java or C++.
class AidlTypenames final {
 public:
  AidlTypenames() = default;
  void Reset();
  bool AddDefinedType(unique_ptr<AidlDefinedType> type);
  bool AddPreprocessedType(unique_ptr<AidlDefinedType> type);
  static bool IsBuiltinTypename(const string& type_name);
  static bool IsPrimitiveTypename(const string& type_name);
  const AidlDefinedType* TryGetDefinedType(const string& type_name) const;
  pair<string, bool> ResolveTypename(const string& type_name) const;
  bool CanBeOutParameter(const AidlTypeSpecifier& type) const;

  // Iterates over all defined and then preprocessed types
  void IterateTypes(const std::function<void(const AidlDefinedType&)>& body) const;

 private:
  map<string, unique_ptr<AidlDefinedType>> defined_types_;
  map<string, unique_ptr<AidlDefinedType>> preprocessed_types_;
};

}  // namespace aidl
}  // namespace android
