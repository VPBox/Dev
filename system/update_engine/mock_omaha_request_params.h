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

#ifndef UPDATE_ENGINE_MOCK_OMAHA_REQUEST_PARAMS_H_
#define UPDATE_ENGINE_MOCK_OMAHA_REQUEST_PARAMS_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/omaha_request_params.h"

namespace chromeos_update_engine {

class MockOmahaRequestParams : public OmahaRequestParams {
 public:
  explicit MockOmahaRequestParams(SystemState* system_state)
      : OmahaRequestParams(system_state) {
    // Delegate all calls to the parent instance by default. This helps the
    // migration from tests using the real RequestParams when they should have
    // use a fake or mock.
    ON_CALL(*this, GetAppId())
        .WillByDefault(
            testing::Invoke(this, &MockOmahaRequestParams::FakeGetAppId));
    ON_CALL(*this, SetTargetChannel(testing::_, testing::_, testing::_))
        .WillByDefault(testing::Invoke(
            this, &MockOmahaRequestParams::FakeSetTargetChannel));
    ON_CALL(*this, UpdateDownloadChannel())
        .WillByDefault(testing::Invoke(
            this, &MockOmahaRequestParams::FakeUpdateDownloadChannel));
    ON_CALL(*this, ShouldPowerwash())
        .WillByDefault(testing::Invoke(
            this, &MockOmahaRequestParams::FakeShouldPowerwash));
  }

  MOCK_CONST_METHOD0(GetAppId, std::string(void));
  MOCK_METHOD3(SetTargetChannel,
               bool(const std::string& channel,
                    bool is_powerwash_allowed,
                    std::string* error));
  MOCK_CONST_METHOD0(target_version_prefix, std::string(void));
  MOCK_METHOD0(UpdateDownloadChannel, void(void));
  MOCK_CONST_METHOD0(IsUpdateUrlOfficial, bool(void));
  MOCK_CONST_METHOD0(ShouldPowerwash, bool(void));

 private:
  // Wrappers to call the parent class and behave like the real object by
  // default. See "Delegating Calls to a Parent Class" in gmock's documentation.
  std::string FakeGetAppId() const { return OmahaRequestParams::GetAppId(); }

  bool FakeSetTargetChannel(const std::string& channel,
                            bool is_powerwash_allowed,
                            std::string* error) {
    return OmahaRequestParams::SetTargetChannel(
        channel, is_powerwash_allowed, error);
  }

  void FakeUpdateDownloadChannel() {
    return OmahaRequestParams::UpdateDownloadChannel();
  }

  bool FakeShouldPowerwash() const {
    return OmahaRequestParams::ShouldPowerwash();
  }
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_MOCK_OMAHA_REQUEST_PARAMS_H_
