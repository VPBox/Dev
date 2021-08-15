//
// Copyright (C) 2018 The Android Open Source Project
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

#include <brillo/message_loops/fake_message_loop.h>

#include "update_engine/common/mock_http_fetcher.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/fake_system_state.h"
#include "update_engine/omaha_request_action.h"

class Environment {
 public:
  Environment() { logging::SetMinLogLevel(logging::LOG_FATAL); }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  static Environment env;
  brillo::FakeMessageLoop loop(nullptr);
  loop.SetAsCurrent();

  chromeos_update_engine::FakeSystemState fake_system_state;
  auto omaha_request_action =
      std::make_unique<chromeos_update_engine::OmahaRequestAction>(
          &fake_system_state,
          nullptr,
          std::make_unique<chromeos_update_engine::MockHttpFetcher>(
              data, size, nullptr),
          false);
  auto collector_action =
      std::make_unique<chromeos_update_engine::ObjectCollectorAction<
          chromeos_update_engine::OmahaResponse>>();
  BondActions(omaha_request_action.get(), collector_action.get());
  chromeos_update_engine::ActionProcessor action_processor;
  action_processor.EnqueueAction(std::move(omaha_request_action));
  action_processor.EnqueueAction(std::move(collector_action));
  action_processor.StartProcessing();

  loop.Run();
  return 0;
}
