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

#ifndef UPDATE_ENGINE_COMMON_PREFS_H_
#define UPDATE_ENGINE_COMMON_PREFS_H_

#include <map>
#include <string>
#include <vector>

#include <base/files/file_path.h>

#include "gtest/gtest_prod.h"  // for FRIEND_TEST
#include "update_engine/common/prefs_interface.h"

namespace chromeos_update_engine {

// Implements a preference store by storing the value associated with a key
// in a given storage passed during construction.
class PrefsBase : public PrefsInterface {
 public:
  // Storage interface used to set and retrieve keys.
  class StorageInterface {
   public:
    StorageInterface() = default;
    virtual ~StorageInterface() = default;

    // Get the key named |key| and store its value in the referenced |value|.
    // Returns whether the operation succeeded.
    virtual bool GetKey(const std::string& key, std::string* value) const = 0;

    // Set the value of the key named |key| to |value| regardless of the
    // previous value. Returns whether the operation succeeded.
    virtual bool SetKey(const std::string& key, const std::string& value) = 0;

    // Returns whether the key named |key| exists.
    virtual bool KeyExists(const std::string& key) const = 0;

    // Deletes the value associated with the key name |key|. Returns whether the
    // key was deleted.
    virtual bool DeleteKey(const std::string& key) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(StorageInterface);
  };

  explicit PrefsBase(StorageInterface* storage) : storage_(storage) {}

  // PrefsInterface methods.
  bool GetString(const std::string& key, std::string* value) const override;
  bool SetString(const std::string& key, const std::string& value) override;
  bool GetInt64(const std::string& key, int64_t* value) const override;
  bool SetInt64(const std::string& key, const int64_t value) override;
  bool GetBoolean(const std::string& key, bool* value) const override;
  bool SetBoolean(const std::string& key, const bool value) override;

  bool Exists(const std::string& key) const override;
  bool Delete(const std::string& key) override;

  void AddObserver(const std::string& key,
                   ObserverInterface* observer) override;
  void RemoveObserver(const std::string& key,
                      ObserverInterface* observer) override;

 private:
  // The registered observers watching for changes.
  std::map<std::string, std::vector<ObserverInterface*>> observers_;

  // The concrete implementation of the storage used for the keys.
  StorageInterface* storage_;

  DISALLOW_COPY_AND_ASSIGN(PrefsBase);
};

// Implements a preference store by storing the value associated with
// a key in a separate file named after the key under a preference
// store directory.

class Prefs : public PrefsBase {
 public:
  Prefs() : PrefsBase(&file_storage_) {}

  // Initializes the store by associating this object with |prefs_dir|
  // as the preference store directory. Returns true on success, false
  // otherwise.
  bool Init(const base::FilePath& prefs_dir);

 private:
  FRIEND_TEST(PrefsTest, GetFileNameForKey);
  FRIEND_TEST(PrefsTest, GetFileNameForKeyBadCharacter);
  FRIEND_TEST(PrefsTest, GetFileNameForKeyEmpty);

  class FileStorage : public PrefsBase::StorageInterface {
   public:
    FileStorage() = default;

    bool Init(const base::FilePath& prefs_dir);

    // PrefsBase::StorageInterface overrides.
    bool GetKey(const std::string& key, std::string* value) const override;
    bool SetKey(const std::string& key, const std::string& value) override;
    bool KeyExists(const std::string& key) const override;
    bool DeleteKey(const std::string& key) override;

   private:
    FRIEND_TEST(PrefsTest, GetFileNameForKey);
    FRIEND_TEST(PrefsTest, GetFileNameForKeyBadCharacter);
    FRIEND_TEST(PrefsTest, GetFileNameForKeyEmpty);

    // Sets |filename| to the full path to the file containing the data
    // associated with |key|. Returns true on success, false otherwise.
    bool GetFileNameForKey(const std::string& key,
                           base::FilePath* filename) const;

    // Preference store directory.
    base::FilePath prefs_dir_;
  };

  // The concrete file storage implementation.
  FileStorage file_storage_;

  DISALLOW_COPY_AND_ASSIGN(Prefs);
};

// Implements a preference store in memory. The stored values are lost when the
// object is destroyed.

class MemoryPrefs : public PrefsBase {
 public:
  MemoryPrefs() : PrefsBase(&mem_storage_) {}

 private:
  class MemoryStorage : public PrefsBase::StorageInterface {
   public:
    MemoryStorage() = default;

    // PrefsBase::StorageInterface overrides.
    bool GetKey(const std::string& key, std::string* value) const override;
    bool SetKey(const std::string& key, const std::string& value) override;
    bool KeyExists(const std::string& key) const override;
    bool DeleteKey(const std::string& key) override;

   private:
    // The std::map holding the values in memory.
    std::map<std::string, std::string> values_;
  };

  // The concrete memory storage implementation.
  MemoryStorage mem_storage_;

  DISALLOW_COPY_AND_ASSIGN(MemoryPrefs);
};
}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_PREFS_H_
