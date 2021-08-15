//
// Copyright (C) 2010 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_COMMON_MOCK_ACTION_H_
#define UPDATE_ENGINE_COMMON_MOCK_ACTION_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/common/action.h"

namespace chromeos_update_engine {

class MockAction;

template <>
class ActionTraits<MockAction> {
 public:
  typedef NoneType OutputObjectType;
  typedef NoneType InputObjectType;
};

class MockAction : public Action<MockAction> {
 public:
  MockAction() {
    ON_CALL(*this, Type()).WillByDefault(testing::Return("MockAction"));
  }

  MOCK_METHOD0(PerformAction, void());
  MOCK_METHOD0(TerminateProcessing, void());
  MOCK_METHOD0(SuspendAction, void());
  MOCK_METHOD0(ResumeAction, void());
  MOCK_CONST_METHOD0(Type, std::string());
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MOCK_ACTION_H_
