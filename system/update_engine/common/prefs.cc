//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common/prefs.h"

#include <algorithm>

#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>

#include "update_engine/common/utils.h"

using std::string;

namespace chromeos_update_engine {

bool PrefsBase::GetString(const string& key, string* value) const {
  return storage_->GetKey(key, value);
}

bool PrefsBase::SetString(const string& key, const string& value) {
  TEST_AND_RETURN_FALSE(storage_->SetKey(key, value));
  const auto observers_for_key = observers_.find(key);
  if (observers_for_key != observers_.end()) {
    std::vector<ObserverInterface*> copy_observers(observers_for_key->second);
    for (ObserverInterface* observer : copy_observers)
      observer->OnPrefSet(key);
  }
  return true;
}

bool PrefsBase::GetInt64(const string& key, int64_t* value) const {
  string str_value;
  if (!GetString(key, &str_value))
    return false;
  base::TrimWhitespaceASCII(str_value, base::TRIM_ALL, &str_value);
  TEST_AND_RETURN_FALSE(base::StringToInt64(str_value, value));
  return true;
}

bool PrefsBase::SetInt64(const string& key, const int64_t value) {
  return SetString(key, base::Int64ToString(value));
}

bool PrefsBase::GetBoolean(const string& key, bool* value) const {
  string str_value;
  if (!GetString(key, &str_value))
    return false;
  base::TrimWhitespaceASCII(str_value, base::TRIM_ALL, &str_value);
  if (str_value == "false") {
    *value = false;
    return true;
  }
  if (str_value == "true") {
    *value = true;
    return true;
  }
  return false;
}

bool PrefsBase::SetBoolean(const string& key, const bool value) {
  return SetString(key, value ? "true" : "false");
}

bool PrefsBase::Exists(const string& key) const {
  return storage_->KeyExists(key);
}

bool PrefsBase::Delete(const string& key) {
  TEST_AND_RETURN_FALSE(storage_->DeleteKey(key));
  const auto observers_for_key = observers_.find(key);
  if (observers_for_key != observers_.end()) {
    std::vector<ObserverInterface*> copy_observers(observers_for_key->second);
    for (ObserverInterface* observer : copy_observers)
      observer->OnPrefDeleted(key);
  }
  return true;
}

void PrefsBase::AddObserver(const string& key, ObserverInterface* observer) {
  observers_[key].push_back(observer);
}

void PrefsBase::RemoveObserver(const string& key, ObserverInterface* observer) {
  std::vector<ObserverInterface*>& observers_for_key = observers_[key];
  auto observer_it =
      std::find(observers_for_key.begin(), observers_for_key.end(), observer);
  if (observer_it != observers_for_key.end())
    observers_for_key.erase(observer_it);
}

// Prefs

bool Prefs::Init(const base::FilePath& prefs_dir) {
  return file_storage_.Init(prefs_dir);
}

bool Prefs::FileStorage::Init(const base::FilePath& prefs_dir) {
  prefs_dir_ = prefs_dir;
  return true;
}

bool Prefs::FileStorage::GetKey(const string& key, string* value) const {
  base::FilePath filename;
  TEST_AND_RETURN_FALSE(GetFileNameForKey(key, &filename));
  if (!base::ReadFileToString(filename, value)) {
    LOG(INFO) << key << " not present in " << prefs_dir_.value();
    return false;
  }
  return true;
}

bool Prefs::FileStorage::SetKey(const string& key, const string& value) {
  base::FilePath filename;
  TEST_AND_RETURN_FALSE(GetFileNameForKey(key, &filename));
  if (!base::DirectoryExists(filename.DirName())) {
    // Only attempt to create the directory if it doesn't exist to avoid calls
    // to parent directories where we might not have permission to write to.
    TEST_AND_RETURN_FALSE(base::CreateDirectory(filename.DirName()));
  }
  TEST_AND_RETURN_FALSE(base::WriteFile(filename, value.data(), value.size()) ==
                        static_cast<int>(value.size()));
  return true;
}

bool Prefs::FileStorage::KeyExists(const string& key) const {
  base::FilePath filename;
  TEST_AND_RETURN_FALSE(GetFileNameForKey(key, &filename));
  return base::PathExists(filename);
}

bool Prefs::FileStorage::DeleteKey(const string& key) {
  base::FilePath filename;
  TEST_AND_RETURN_FALSE(GetFileNameForKey(key, &filename));
  TEST_AND_RETURN_FALSE(base::DeleteFile(filename, false));
  return true;
}

bool Prefs::FileStorage::GetFileNameForKey(const string& key,
                                           base::FilePath* filename) const {
  // Allows only non-empty keys containing [A-Za-z0-9_-].
  TEST_AND_RETURN_FALSE(!key.empty());
  for (size_t i = 0; i < key.size(); ++i) {
    char c = key.at(i);
    TEST_AND_RETURN_FALSE(base::IsAsciiAlpha(c) || base::IsAsciiDigit(c) ||
                          c == '_' || c == '-');
  }
  *filename = prefs_dir_.Append(key);
  return true;
}

// MemoryPrefs

bool MemoryPrefs::MemoryStorage::GetKey(const string& key,
                                        string* value) const {
  auto it = values_.find(key);
  if (it == values_.end())
    return false;
  *value = it->second;
  return true;
}

bool MemoryPrefs::MemoryStorage::SetKey(const string& key,
                                        const string& value) {
  values_[key] = value;
  return true;
}

bool MemoryPrefs::MemoryStorage::KeyExists(const string& key) const {
  return values_.find(key) != values_.end();
}

bool MemoryPrefs::MemoryStorage::DeleteKey(const string& key) {
  auto it = values_.find(key);
  if (it == values_.end())
    return false;
  values_.erase(it);
  return true;
}

}  // namespace chromeos_update_engine
