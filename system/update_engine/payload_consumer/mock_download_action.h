//
// Copyright (C) 2015 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_MOCK_DOWNLOAD_ACTION_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_MOCK_DOWNLOAD_ACTION_H_

#include <stdint.h>

#include <gmock/gmock.h>

#include "update_engine/common/error_code.h"
#include "update_engine/payload_consumer/download_action.h"

namespace chromeos_update_engine {

class MockDownloadActionDelegate : public DownloadActionDelegate {
 public:
  MOCK_METHOD3(BytesReceived,
               void(uint64_t bytes_progressed,
                    uint64_t bytes_received,
                    uint64_t total));
  MOCK_METHOD1(ShouldCancel, bool(ErrorCode* cancel_reason));
  MOCK_METHOD0(DownloadComplete, void());
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_MOCK_DOWNLOAD_ACTION_H_
