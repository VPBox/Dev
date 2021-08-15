//
// Copyright (C) 2014 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/common/fake_prefs.h"

#include <algorithm>

#include <gtest/gtest.h>

using std::string;

using chromeos_update_engine::FakePrefs;

namespace {

void CheckNotNull(const string& key, void* ptr) {
  EXPECT_NE(nullptr, ptr) << "Called Get*() for key \"" << key
                          << "\" with a null parameter.";
}

}  // namespace

namespace chromeos_update_engine {

FakePrefs::~FakePrefs() {
  EXPECT_TRUE(observers_.empty());
}

// Compile-time type-dependent constants definitions.
template <>
FakePrefs::PrefType const FakePrefs::PrefConsts<string>::type =
    FakePrefs::PrefType::kString;
template <>
string FakePrefs::PrefValue::*const  // NOLINT(runtime/string), not static str.
    FakePrefs::PrefConsts<string>::member = &FakePrefs::PrefValue::as_str;

template <>
FakePrefs::PrefType const FakePrefs::PrefConsts<int64_t>::type =
    FakePrefs::PrefType::kInt64;
template <>
int64_t FakePrefs::PrefValue::*const FakePrefs::PrefConsts<int64_t>::member =
    &FakePrefs::PrefValue::as_int64;

template <>
FakePrefs::PrefType const FakePrefs::PrefConsts<bool>::type =
    FakePrefs::PrefType::kBool;
template <>
bool FakePrefs::PrefValue::*const FakePrefs::PrefConsts<bool>::member =
    &FakePrefs::PrefValue::as_bool;

bool FakePrefs::GetString(const string& key, string* value) const {
  return GetValue(key, value);
}

bool FakePrefs::SetString(const string& key, const string& value) {
  SetValue(key, value);
  return true;
}

bool FakePrefs::GetInt64(const string& key, int64_t* value) const {
  return GetValue(key, value);
}

bool FakePrefs::SetInt64(const string& key, const int64_t value) {
  SetValue(key, value);
  return true;
}

bool FakePrefs::GetBoolean(const string& key, bool* value) const {
  return GetValue(key, value);
}

bool FakePrefs::SetBoolean(const string& key, const bool value) {
  SetValue(key, value);
  return true;
}

bool FakePrefs::Exists(const string& key) const {
  return values_.find(key) != values_.end();
}

bool FakePrefs::Delete(const string& key) {
  if (values_.find(key) == values_.end())
    return false;
  values_.erase(key);
  const auto observers_for_key = observers_.find(key);
  if (observers_for_key != observers_.end()) {
    std::vector<ObserverInterface*> copy_observers(observers_for_key->second);
    for (ObserverInterface* observer : copy_observers)
      observer->OnPrefDeleted(key);
  }
  return true;
}

string FakePrefs::GetTypeName(PrefType type) {
  switch (type) {
    case PrefType::kString:
      return "string";
    case PrefType::kInt64:
      return "int64_t";
    case PrefType::kBool:
      return "bool";
  }
  return "Unknown";
}

void FakePrefs::CheckKeyType(const string& key, PrefType type) const {
  auto it = values_.find(key);
  EXPECT_TRUE(it == values_.end() || it->second.type == type)
      << "Key \"" << key << "\" if defined as " << GetTypeName(it->second.type)
      << " but is accessed as a " << GetTypeName(type);
}

template <typename T>
void FakePrefs::SetValue(const string& key, const T& value) {
  CheckKeyType(key, PrefConsts<T>::type);
  values_[key].type = PrefConsts<T>::type;
  values_[key].value.*(PrefConsts<T>::member) = value;
  const auto observers_for_key = observers_.find(key);
  if (observers_for_key != observers_.end()) {
    std::vector<ObserverInterface*> copy_observers(observers_for_key->second);
    for (ObserverInterface* observer : copy_observers)
      observer->OnPrefSet(key);
  }
}

template <typename T>
bool FakePrefs::GetValue(const string& key, T* value) const {
  CheckKeyType(key, PrefConsts<T>::type);
  auto it = values_.find(key);
  if (it == values_.end())
    return false;
  CheckNotNull(key, value);
  *value = it->second.value.*(PrefConsts<T>::member);
  return true;
}

void FakePrefs::AddObserver(const string& key, ObserverInterface* observer) {
  observers_[key].push_back(observer);
}

void FakePrefs::RemoveObserver(const string& key, ObserverInterface* observer) {
  std::vector<ObserverInterface*>& observers_for_key = observers_[key];
  auto observer_it =
      std::find(observers_for_key.begin(), observers_for_key.end(), observer);
  EXPECT_NE(observer_it, observers_for_key.end())
      << "Trying to remove an observer instance not watching the key " << key;
  if (observer_it != observers_for_key.end())
    observers_for_key.erase(observer_it);
  if (observers_for_key.empty())
    observers_.erase(key);
}

}  // namespace chromeos_update_engine
