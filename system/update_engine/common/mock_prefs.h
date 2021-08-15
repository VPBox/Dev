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

#ifndef UPDATE_ENGINE_COMMON_MOCK_PREFS_H_
#define UPDATE_ENGINE_COMMON_MOCK_PREFS_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/prefs_interface.h"

namespace chromeos_update_engine {

class MockPrefs : public PrefsInterface {
 public:
  MOCK_CONST_METHOD2(GetString,
                     bool(const std::string& key, std::string* value));
  MOCK_METHOD2(SetString,
               bool(const std::string& key, const std::string& value));
  MOCK_CONST_METHOD2(GetInt64, bool(const std::string& key, int64_t* value));
  MOCK_METHOD2(SetInt64, bool(const std::string& key, const int64_t value));

  MOCK_CONST_METHOD2(GetBoolean, bool(const std::string& key, bool* value));
  MOCK_METHOD2(SetBoolean, bool(const std::string& key, const bool value));

  MOCK_CONST_METHOD1(Exists, bool(const std::string& key));
  MOCK_METHOD1(Delete, bool(const std::string& key));

  MOCK_METHOD2(AddObserver, void(const std::string& key, ObserverInterface*));
  MOCK_METHOD2(RemoveObserver,
               void(const std::string& key, ObserverInterface*));
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MOCK_PREFS_H_
