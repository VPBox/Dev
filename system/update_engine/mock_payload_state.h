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

#ifndef UPDATE_ENGINE_MOCK_PAYLOAD_STATE_H_
#define UPDATE_ENGINE_MOCK_PAYLOAD_STATE_H_

#include <string>

#include <gmock/gmock.h>

#include "update_engine/omaha_request_action.h"
#include "update_engine/payload_state_interface.h"

namespace chromeos_update_engine {

class MockPayloadState : public PayloadStateInterface {
 public:
  bool Initialize(SystemState* system_state) { return true; }

  // Significant methods.
  MOCK_METHOD1(SetResponse, void(const OmahaResponse& response));
  MOCK_METHOD0(DownloadComplete, void());
  MOCK_METHOD1(DownloadProgress, void(size_t count));
  MOCK_METHOD0(UpdateResumed, void());
  MOCK_METHOD0(UpdateRestarted, void());
  MOCK_METHOD0(UpdateSucceeded, void());
  MOCK_METHOD1(UpdateFailed, void(ErrorCode error));
  MOCK_METHOD0(ResetUpdateStatus, void());
  MOCK_METHOD0(ShouldBackoffDownload, bool());
  MOCK_METHOD0(UpdateEngineStarted, void());
  MOCK_METHOD0(Rollback, void());
  MOCK_METHOD1(ExpectRebootInNewVersion,
               void(const std::string& target_version_uid));
  MOCK_METHOD0(P2PNewAttempt, void());
  MOCK_METHOD0(P2PAttemptAllowed, bool());
  MOCK_METHOD1(SetUsingP2PForDownloading, void(bool value));
  MOCK_METHOD1(SetUsingP2PForSharing, void(bool value));
  MOCK_METHOD1(SetScatteringWaitPeriod, void(base::TimeDelta));
  MOCK_METHOD1(SetP2PUrl, void(const std::string&));
  MOCK_METHOD0(NextPayload, bool());
  MOCK_METHOD1(SetStagingWaitPeriod, void(base::TimeDelta));

  // Getters.
  MOCK_METHOD0(GetResponseSignature, std::string());
  MOCK_METHOD0(GetPayloadAttemptNumber, int());
  MOCK_METHOD0(GetFullPayloadAttemptNumber, int());
  MOCK_METHOD0(GetCurrentUrl, std::string());
  MOCK_METHOD0(GetUrlFailureCount, uint32_t());
  MOCK_METHOD0(GetUrlSwitchCount, uint32_t());
  MOCK_METHOD0(GetNumResponsesSeen, int());
  MOCK_METHOD0(GetBackoffExpiryTime, base::Time());
  MOCK_METHOD0(GetUpdateDuration, base::TimeDelta());
  MOCK_METHOD0(GetUpdateDurationUptime, base::TimeDelta());
  MOCK_METHOD1(GetCurrentBytesDownloaded, uint64_t(DownloadSource source));
  MOCK_METHOD1(GetTotalBytesDownloaded, uint64_t(DownloadSource source));
  MOCK_METHOD0(GetNumReboots, uint32_t());
  MOCK_METHOD0(GetRollbackHappened, bool());
  MOCK_METHOD1(SetRollbackHappened, void(bool));
  MOCK_METHOD0(GetRollbackVersion, std::string());
  MOCK_METHOD0(GetP2PNumAttempts, int());
  MOCK_METHOD0(GetP2PFirstAttemptTimestamp, base::Time());
  MOCK_CONST_METHOD0(GetUsingP2PForDownloading, bool());
  MOCK_CONST_METHOD0(GetUsingP2PForSharing, bool());
  MOCK_METHOD0(GetScatteringWaitPeriod, base::TimeDelta());
  MOCK_CONST_METHOD0(GetP2PUrl, std::string());
  MOCK_METHOD0(GetStagingWaitPeriod, base::TimeDelta());
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_MOCK_PAYLOAD_STATE_H_
