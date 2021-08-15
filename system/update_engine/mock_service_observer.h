//
// Copyright (C) 2017 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_MOCK_SERVICE_OBSERVER_H_
#define UPDATE_ENGINE_MOCK_SERVICE_OBSERVER_H_

#include <gmock/gmock.h>
#include "update_engine/service_observer_interface.h"

namespace chromeos_update_engine {

class MockServiceObserver : public ServiceObserverInterface {
 public:
  MOCK_METHOD1(
      SendStatusUpdate,
      void(const update_engine::UpdateEngineStatus& update_engine_status));
  MOCK_METHOD1(SendPayloadApplicationComplete, void(ErrorCode error_code));
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_MOCK_SERVICE_OBSERVER_H_
