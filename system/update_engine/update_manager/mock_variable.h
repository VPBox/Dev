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

#ifndef UPDATE_ENGINE_UPDATE_MANAGER_MOCK_VARIABLE_H_
#define UPDATE_ENGINE_UPDATE_MANAGER_MOCK_VARIABLE_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/update_manager/variable.h"

namespace chromeos_update_manager {

// This is a generic mock of the Variable class.
template <typename T>
class MockVariable : public Variable<T> {
 public:
  using Variable<T>::Variable;

  MOCK_METHOD2_T(GetValue, const T*(base::TimeDelta, std::string*));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockVariable);
};

}  // namespace chromeos_update_manager

#endif  // UPDATE_ENGINE_UPDATE_MANAGER_MOCK_VARIABLE_H_
