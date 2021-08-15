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

#ifndef UPDATE_ENGINE_COMMON_MOCK_ACTION_PROCESSOR_H_
#define UPDATE_ENGINE_COMMON_MOCK_ACTION_PROCESSOR_H_

#include <deque>
#include <memory>
#include <utility>

#include <gmock/gmock.h>

#include "update_engine/common/action.h"

namespace chromeos_update_engine {

class MockActionProcessor : public ActionProcessor {
 public:
  MOCK_METHOD0(StartProcessing, void());
  MOCK_METHOD1(EnqueueAction, void(AbstractAction* action));

  // This is a legacy workaround described in:
  // https://github.com/google/googletest/blob/master/googlemock/docs/CookBook.md#legacy-workarounds-for-move-only-types-legacymoveonly
  void EnqueueAction(std::unique_ptr<AbstractAction> action) override {
    EnqueueAction(action.get());
  }
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_COMMON_MOCK_ACTION_PROCESSOR_H_
