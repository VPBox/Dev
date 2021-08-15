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

#ifndef UPDATE_ENGINE_PAYLOAD_STATE_INTERFACE_H_
#define UPDATE_ENGINE_PAYLOAD_STATE_INTERFACE_H_

#include <string>

#include "update_engine/common/action_processor.h"
#include "update_engine/common/constants.h"
#include "update_engine/omaha_response.h"

namespace chromeos_update_engine {

// Describes the methods that need to be implemented by the PayloadState class.
// This interface has been carved out to support mocking of the PayloadState
// object.
class PayloadStateInterface {
 public:
  virtual ~PayloadStateInterface() = default;

  // Sets the internal payload state based on the given Omaha response. This
  // response could be the same or different from the one for which we've stored
  // the internal state. If it's different, then this method resets all the
  // internal state corresponding to the old response. Since the Omaha response
  // has a lot of fields that are not related to payload state, it uses only
  // a subset of the fields in the Omaha response to compare equality.
  virtual void SetResponse(const OmahaResponse& response) = 0;

  // This method should be called whenever we have completed downloading all
  // the bytes of a payload and have verified that its size and hash match the
  // expected values. We use this notificaiton to increment the payload attempt
  // number so that the throttle the next attempt to download the same payload
  // (in case there's an error in subsequent steps such as post-install)
  // appropriately.
  virtual void DownloadComplete() = 0;

  // This method should be called whenever we receive new bytes from the
  // network for the current payload. We use this notification to reset the
  // failure count for a given URL since receipt of some bytes means we are
  // able to make forward progress with the current URL.
  virtual void DownloadProgress(size_t count) = 0;

  // This method should be called every time we resume an update attempt.
  virtual void UpdateResumed() = 0;

  // This method should be called every time we begin a new update. This method
  // should not be called when we resume an update from the previously
  // downloaded point. This is used to reset the metrics for each new update.
  virtual void UpdateRestarted() = 0;

  // This method should be called once after an update attempt succeeds. This
  // is when the relevant UMA metrics that are tracked on a per-update-basis
  // are uploaded to the UMA server.
  virtual void UpdateSucceeded() = 0;

  // This method should be called whenever an update attempt fails with the
  // given error code. We use this notification to update the payload state
  // depending on the type of the error that happened.
  virtual void UpdateFailed(ErrorCode error) = 0;

  // This method should be called whenever a succeeded update is canceled, and
  // thus can only be called after UpdateSucceeded(). This is currently used
  // only for manual testing using the update_engine_client.
  virtual void ResetUpdateStatus() = 0;

  // This method should be called every time we initiate a Rollback.
  virtual void Rollback() = 0;

  // Sets the expectations to boot into the new version in the next reboot.
  // This function is called every time a new update is marked as ready by
  // UpdateSuccess(). |target_version_uid| is an unique identifier of the
  // applied payload. It can be any string, as long as the same string is used
  // for the same payload.
  virtual void ExpectRebootInNewVersion(
      const std::string& target_version_uid) = 0;

  // Sets whether P2P is being used to download the update payload. This
  // is used to keep track of download sources being used and should be called
  // before the transfer begins.
  virtual void SetUsingP2PForDownloading(bool value) = 0;

  // Sets whether P2P is being used for sharing the update payloads.
  virtual void SetUsingP2PForSharing(bool value) = 0;

  // Returns true if we should backoff the current download attempt.
  // False otherwise.
  virtual bool ShouldBackoffDownload() = 0;

  // Returns the currently stored response "signature". The signature  is a
  // subset of fields that are of interest to the PayloadState behavior.
  virtual std::string GetResponseSignature() = 0;

  // Returns the payload attempt number.
  virtual int GetPayloadAttemptNumber() = 0;

  // Returns the payload attempt number of the attempted full payload. Returns
  // 0 for delta payloads.
  virtual int GetFullPayloadAttemptNumber() = 0;

  // Returns the current URL. Returns an empty string if there's no valid URL.
  virtual std::string GetCurrentUrl() = 0;

  // Returns the current URL's failure count.
  virtual uint32_t GetUrlFailureCount() = 0;

  // Returns the total number of times a new URL has been switched to
  // for the current response.
  virtual uint32_t GetUrlSwitchCount() = 0;

  // Returns the total number of different responses seen since the
  // last successful update.
  virtual int GetNumResponsesSeen() = 0;

  // Returns the expiry time for the current backoff period.
  virtual base::Time GetBackoffExpiryTime() = 0;

  // Returns the elapsed time used for this update, including time
  // where the device is powered off and sleeping. If the
  // update has not completed, returns the time spent so far.
  virtual base::TimeDelta GetUpdateDuration() = 0;

  // Returns the time used for this update not including time when
  // the device is powered off or sleeping. If the update has not
  // completed, returns the time spent so far.
  virtual base::TimeDelta GetUpdateDurationUptime() = 0;

  // Returns the number of bytes that have been downloaded for each source for
  // each new update attempt. If we resume an update, we'll continue from the
  // previous value, but if we get a new response or if the previous attempt
  // failed, we'll reset this to 0 to start afresh.
  virtual uint64_t GetCurrentBytesDownloaded(DownloadSource source) = 0;

  // Returns the total number of bytes that have been downloaded for each
  // source since the the last successful update. This is used to compute the
  // overhead we incur.
  virtual uint64_t GetTotalBytesDownloaded(DownloadSource source) = 0;

  // Returns the reboot count for this update attempt.
  virtual uint32_t GetNumReboots() = 0;

  // Called at update_engine startup to do various house-keeping.
  virtual void UpdateEngineStarted() = 0;

  // Returns whether a rollback happened since the last update check with policy
  // present.
  virtual bool GetRollbackHappened() = 0;

  // Sets whether rollback has happened on this device since the last update
  // check where policy was available. This info is preserved over powerwash.
  // This prevents forced updates happening on a rolled back device before
  // device policy is available.
  virtual void SetRollbackHappened(bool rollback_happened) = 0;

  // Returns the version from before a rollback if our last update was a
  // rollback.
  virtual std::string GetRollbackVersion() = 0;

  // Returns the value of number of attempts we've attempted to
  // download the payload via p2p.
  virtual int GetP2PNumAttempts() = 0;

  // Returns the value of timestamp of the first time we've attempted
  // to download the payload via p2p.
  virtual base::Time GetP2PFirstAttemptTimestamp() = 0;

  // Should be called every time we decide to use p2p for an update
  // attempt. This is used to increase the p2p attempt counter and
  // set the timestamp for first attempt.
  virtual void P2PNewAttempt() = 0;

  // Returns |true| if we are allowed to continue using p2p for
  // downloading and |false| otherwise. This is done by recording
  // and examining how many attempts have been done already as well
  // as when the first attempt was.
  virtual bool P2PAttemptAllowed() = 0;

  // Gets the values previously set with SetUsingP2PForDownloading() and
  // SetUsingP2PForSharing().
  virtual bool GetUsingP2PForDownloading() const = 0;
  virtual bool GetUsingP2PForSharing() const = 0;

  // Returns the current (persisted) scattering wallclock-based wait period.
  virtual base::TimeDelta GetScatteringWaitPeriod() = 0;

  // Sets and persists the scattering wallclock-based wait period.
  virtual void SetScatteringWaitPeriod(base::TimeDelta wait_period) = 0;

  // Sets/gets the P2P download URL, if one is to be used.
  virtual void SetP2PUrl(const std::string& url) = 0;
  virtual std::string GetP2PUrl() const = 0;

  // Switch to next payload.
  virtual bool NextPayload() = 0;

  // Sets and persists the staging wallclock-based wait period.
  virtual void SetStagingWaitPeriod(base::TimeDelta wait_period) = 0;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_STATE_INTERFACE_H_
