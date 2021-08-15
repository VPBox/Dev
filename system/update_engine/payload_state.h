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

#ifndef UPDATE_ENGINE_PAYLOAD_STATE_H_
#define UPDATE_ENGINE_PAYLOAD_STATE_H_

#include <algorithm>
#include <string>
#include <vector>

#include <base/time/time.h>
#include <gtest/gtest_prod.h>  // for FRIEND_TEST

#include "update_engine/common/prefs_interface.h"
#include "update_engine/metrics_constants.h"
#include "update_engine/payload_state_interface.h"

namespace chromeos_update_engine {

class SystemState;

// Encapsulates all the payload state required for download. This includes the
// state necessary for handling multiple URLs in Omaha response, the backoff
// state, etc. All state is persisted so that we use the most recently saved
// value when resuming the update_engine process. All state is also cached in
// memory so that we ensure we always make progress based on last known good
// state even when there's any issue in reading/writing from the file system.
class PayloadState : public PayloadStateInterface {
 public:
  PayloadState();
  ~PayloadState() override {}

  // Initializes a payload state object using the given global system state.
  // It performs the initial loading of all persisted state into memory and
  // dumps the initial state for debugging purposes.  Note: the other methods
  // should be called only after calling Initialize on this object.
  bool Initialize(SystemState* system_state);

  // Implementation of PayloadStateInterface methods.
  void SetResponse(const OmahaResponse& response) override;
  void DownloadComplete() override;
  void DownloadProgress(size_t count) override;
  void UpdateResumed() override;
  void UpdateRestarted() override;
  void UpdateSucceeded() override;
  void UpdateFailed(ErrorCode error) override;
  void ResetUpdateStatus() override;
  bool ShouldBackoffDownload() override;
  void Rollback() override;
  void ExpectRebootInNewVersion(const std::string& target_version_uid) override;
  void SetUsingP2PForDownloading(bool value) override;

  void SetUsingP2PForSharing(bool value) override {
    using_p2p_for_sharing_ = value;
  }

  inline std::string GetResponseSignature() override {
    return response_signature_;
  }

  inline int GetFullPayloadAttemptNumber() override {
    return full_payload_attempt_number_;
  }

  inline int GetPayloadAttemptNumber() override {
    return payload_attempt_number_;
  }

  inline std::string GetCurrentUrl() override {
    return (payload_index_ < candidate_urls_.size() &&
            url_index_ < candidate_urls_[payload_index_].size())
               ? candidate_urls_[payload_index_][url_index_]
               : "";
  }

  inline uint32_t GetUrlFailureCount() override { return url_failure_count_; }

  inline uint32_t GetUrlSwitchCount() override { return url_switch_count_; }

  inline int GetNumResponsesSeen() override { return num_responses_seen_; }

  inline base::Time GetBackoffExpiryTime() override {
    return backoff_expiry_time_;
  }

  base::TimeDelta GetUpdateDuration() override;

  base::TimeDelta GetUpdateDurationUptime() override;

  inline uint64_t GetCurrentBytesDownloaded(DownloadSource source) override {
    return source < kNumDownloadSources ? current_bytes_downloaded_[source] : 0;
  }

  inline uint64_t GetTotalBytesDownloaded(DownloadSource source) override {
    return source < kNumDownloadSources ? total_bytes_downloaded_[source] : 0;
  }

  inline uint32_t GetNumReboots() override { return num_reboots_; }

  void UpdateEngineStarted() override;

  inline bool GetRollbackHappened() override { return rollback_happened_; }

  void SetRollbackHappened(bool rollback_happened) override;

  inline std::string GetRollbackVersion() override { return rollback_version_; }

  int GetP2PNumAttempts() override;
  base::Time GetP2PFirstAttemptTimestamp() override;
  void P2PNewAttempt() override;
  bool P2PAttemptAllowed() override;

  bool GetUsingP2PForDownloading() const override {
    return using_p2p_for_downloading_;
  }

  bool GetUsingP2PForSharing() const override { return using_p2p_for_sharing_; }

  base::TimeDelta GetScatteringWaitPeriod() override {
    return scattering_wait_period_;
  }

  void SetScatteringWaitPeriod(base::TimeDelta wait_period) override;

  void SetStagingWaitPeriod(base::TimeDelta wait_period) override;

  void SetP2PUrl(const std::string& url) override { p2p_url_ = url; }

  std::string GetP2PUrl() const override { return p2p_url_; }

  bool NextPayload() override;

 private:
  enum class AttemptType {
    kUpdate,
    kRollback,
  };

  friend class PayloadStateTest;
  FRIEND_TEST(PayloadStateTest, RebootAfterUpdateFailedMetric);
  FRIEND_TEST(PayloadStateTest, RebootAfterUpdateSucceed);
  FRIEND_TEST(PayloadStateTest, RebootAfterCanceledUpdate);
  FRIEND_TEST(PayloadStateTest, RollbackHappened);
  FRIEND_TEST(PayloadStateTest, RollbackVersion);
  FRIEND_TEST(PayloadStateTest, UpdateSuccessWithWipedPrefs);

  // Helper called when an attempt has begun, is called by
  // UpdateResumed(), UpdateRestarted() and Rollback().
  void AttemptStarted(AttemptType attempt_type);

  // Increments the payload attempt number used for metrics.
  void IncrementPayloadAttemptNumber();

  // Increments the payload attempt number which governs the backoff behavior
  // at the time of the next update check.
  void IncrementFullPayloadAttemptNumber();

  // Advances the current URL index to the next available one. If all URLs have
  // been exhausted during the current payload download attempt (as indicated
  // by the payload attempt number), then it will increment the payload attempt
  // number and wrap around again with the first URL in the list. This also
  // updates the URL switch count, if needed.
  void IncrementUrlIndex();

  // Increments the failure count of the current URL. If the configured max
  // failure count is reached for this URL, it advances the current URL index
  // to the next URL and resets the failure count for that URL.
  void IncrementFailureCount();

  // Updates the backoff expiry time exponentially based on the current
  // payload attempt number.
  void UpdateBackoffExpiryTime();

  // Updates the value of current download source based on the current URL
  // index. If the download source is not one of the known sources, it's set
  // to kNumDownloadSources.
  void UpdateCurrentDownloadSource();

  // Updates the various metrics corresponding with the given number of bytes
  // that were downloaded recently.
  void UpdateBytesDownloaded(size_t count);

  // Calculates the PayloadType we're using.
  PayloadType CalculatePayloadType();

  // Collects and reports the various metrics related to an update attempt.
  void CollectAndReportAttemptMetrics(ErrorCode code);

  // Persists values related to the UpdateEngine.Attempt.* metrics so
  // we can identify later if an update attempt ends abnormally.
  void PersistAttemptMetrics();

  // Clears persistent state previously set using AttemptMetricsPersist().
  void ClearPersistedAttemptMetrics();

  // Checks if persistent state previously set using AttemptMetricsPersist()
  // exists and, if so, emits it with |attempt_result| set to
  // metrics::AttemptResult::kAbnormalTermination.
  void ReportAndClearPersistedAttemptMetrics();

  // Collects and reports the various metrics related to a successful update.
  void CollectAndReportSuccessfulUpdateMetrics();

  // Checks if we were expecting to be running in the new version but the
  // boot into the new version failed for some reason. If that's the case, an
  // UMA metric is sent reporting the number of attempts the same applied
  // payload was attempted to reboot. This function is called by UpdateAttempter
  // every time the update engine starts and there's no reboot pending.
  void ReportFailedBootIfNeeded();

  // Resets all the persisted state values which are maintained relative to the
  // current response signature. The response signature itself is not reset.
  void ResetPersistedState();

  // Resets the appropriate state related to download sources that need to be
  // reset on a new update.
  void ResetDownloadSourcesOnNewUpdate();

  // Calculates the response "signature", which is basically a string composed
  // of the subset of the fields in the current response that affect the
  // behavior of the PayloadState.
  std::string CalculateResponseSignature();

  // Initializes the current response signature from the persisted state.
  void LoadResponseSignature();

  // Sets the response signature to the given value. Also persists the value
  // being set so that we resume from the save value in case of a process
  // restart.
  void SetResponseSignature(const std::string& response_signature);

  // Initializes the payload attempt number from the persisted state.
  void LoadPayloadAttemptNumber();

  // Initializes the payload attempt number for full payloads from the persisted
  // state.
  void LoadFullPayloadAttemptNumber();

  // Sets the payload attempt number to the given value. Also persists the
  // value being set so that we resume from the same value in case of a process
  // restart.
  void SetPayloadAttemptNumber(int payload_attempt_number);

  // Sets the payload attempt number for full updates to the given value. Also
  // persists the value being set so that we resume from the same value in case
  // of a process restart.
  void SetFullPayloadAttemptNumber(int payload_attempt_number);

  // Sets the current payload index to the given value. Also persists the value
  // being set so that we resume from the same value in case of a process
  // restart.
  void SetPayloadIndex(size_t payload_index);

  // Initializes the current URL index from the persisted state.
  void LoadUrlIndex();

  // Sets the current URL index to the given value. Also persists the value
  // being set so that we resume from the same value in case of a process
  // restart.
  void SetUrlIndex(uint32_t url_index);

  // Initializes the current URL's failure count from the persisted stae.
  void LoadUrlFailureCount();

  // Sets the current URL's failure count to the given value. Also persists the
  // value being set so that we resume from the same value in case of a process
  // restart.
  void SetUrlFailureCount(uint32_t url_failure_count);

  // Sets |url_switch_count_| to the given value and persists the value.
  void SetUrlSwitchCount(uint32_t url_switch_count);

  // Initializes |url_switch_count_| from the persisted stae.
  void LoadUrlSwitchCount();

  // Initializes the backoff expiry time from the persisted state.
  void LoadBackoffExpiryTime();

  // Sets the backoff expiry time to the given value. Also persists the value
  // being set so that we resume from the same value in case of a process
  // restart.
  void SetBackoffExpiryTime(const base::Time& new_time);

  // Initializes |update_timestamp_start_| from the persisted state.
  void LoadUpdateTimestampStart();

  // Sets |update_timestamp_start_| to the given value and persists the value.
  void SetUpdateTimestampStart(const base::Time& value);

  // Sets |update_timestamp_end_| to the given value. This is not persisted
  // as it happens at the end of the update process where state is deleted
  // anyway.
  void SetUpdateTimestampEnd(const base::Time& value);

  // Initializes |update_duration_uptime_| from the persisted state.
  void LoadUpdateDurationUptime();

  // Helper method used in SetUpdateDurationUptime() and
  // CalculateUpdateDurationUptime().
  void SetUpdateDurationUptimeExtended(const base::TimeDelta& value,
                                       const base::Time& timestamp,
                                       bool use_logging);

  // Sets |update_duration_uptime_| to the given value and persists
  // the value and sets |update_duration_uptime_timestamp_| to the
  // current monotonic time.
  void SetUpdateDurationUptime(const base::TimeDelta& value);

  // Adds the difference between current monotonic time and
  // |update_duration_uptime_timestamp_| to |update_duration_uptime_| and
  // sets |update_duration_uptime_timestamp_| to current monotonic time.
  void CalculateUpdateDurationUptime();

  // Returns the full key for a download source given the prefix.
  std::string GetPrefsKey(const std::string& prefix, DownloadSource source);

  // Loads the number of bytes that have been currently downloaded through the
  // previous attempts from the persisted state for the given source. It's
  // reset to 0 every time we begin a full update and is continued from previous
  // attempt if we're resuming the update.
  void LoadCurrentBytesDownloaded(DownloadSource source);

  // Sets the number of bytes that have been currently downloaded for the
  // given source. This value is also persisted.
  void SetCurrentBytesDownloaded(DownloadSource source,
                                 uint64_t current_bytes_downloaded,
                                 bool log);

  // Loads the total number of bytes that have been downloaded (since the last
  // successful update) from the persisted state for the given source. It's
  // reset to 0 every time we successfully apply an update and counts the bytes
  // downloaded for both successful and failed attempts since then.
  void LoadTotalBytesDownloaded(DownloadSource source);

  // Sets the total number of bytes that have been downloaded so far for the
  // given source. This value is also persisted.
  void SetTotalBytesDownloaded(DownloadSource source,
                               uint64_t total_bytes_downloaded,
                               bool log);

  // Loads whether rollback has happened on this device since the last update
  // check where policy was available. This info is preserved over powerwash.
  void LoadRollbackHappened();

  // Loads the blacklisted version from our prefs file.
  void LoadRollbackVersion();

  // Blacklists this version from getting AU'd to until we receive a new update
  // response.
  void SetRollbackVersion(const std::string& rollback_version);

  // Clears any blacklisted version.
  void ResetRollbackVersion();

  inline uint32_t GetUrlIndex() {
    return (url_index_ != 0 && payload_index_ < candidate_urls_.size())
               ? std::min(candidate_urls_[payload_index_].size() - 1,
                          url_index_)
               : 0;
  }

  // Computes the list of candidate URLs from the total list of payload URLs in
  // the Omaha response.
  void ComputeCandidateUrls();

  // Sets |num_responses_seen_| and persist it to disk.
  void SetNumResponsesSeen(int num_responses_seen);

  // Initializes |num_responses_seen_| from persisted state.
  void LoadNumResponsesSeen();

  // Initializes |num_reboots_| from the persisted state.
  void LoadNumReboots();

  // Sets |num_reboots| for the update attempt. Also persists the
  // value being set so that we resume from the same value in case of a process
  // restart.
  void SetNumReboots(uint32_t num_reboots);

  // Checks to see if the device rebooted since the last call and if so
  // increments num_reboots.
  void UpdateNumReboots();

  // Loads the |kPrefsP2PFirstAttemptTimestamp| state variable from disk
  // into |p2p_first_attempt_timestamp_|.
  void LoadP2PFirstAttemptTimestamp();

  // Loads the |kPrefsP2PNumAttempts| state variable into |p2p_num_attempts_|.
  void LoadP2PNumAttempts();

  // Sets the |kPrefsP2PNumAttempts| state variable to |value|.
  void SetP2PNumAttempts(int value);

  // Sets the |kPrefsP2PFirstAttemptTimestamp| state variable to |time|.
  void SetP2PFirstAttemptTimestamp(const base::Time& time);

  // Loads the persisted scattering wallclock-based wait period.
  void LoadScatteringWaitPeriod();

  // Loads the persisted staging wallclock-based wait period.
  void LoadStagingWaitPeriod();

  // Get the total size of all payloads.
  int64_t GetPayloadSize();

  // The global state of the system.
  SystemState* system_state_;

  // Interface object with which we read/write persisted state. This must
  // be set by calling the Initialize method before calling any other method.
  PrefsInterface* prefs_;

  // Interface object with which we read/write persisted state. This must
  // be set by calling the Initialize method before calling any other method.
  // This object persists across powerwashes.
  PrefsInterface* powerwash_safe_prefs_;

  // This is the current response object from Omaha.
  OmahaResponse response_;

  // Whether P2P is being used for downloading and sharing.
  bool using_p2p_for_downloading_;
  bool using_p2p_for_sharing_;

  // Stores the P2P download URL, if one is used.
  std::string p2p_url_;

  // The cached value of |kPrefsP2PFirstAttemptTimestamp|.
  base::Time p2p_first_attempt_timestamp_;

  // The cached value of |kPrefsP2PNumAttempts|.
  int p2p_num_attempts_;

  // This stores a "signature" of the current response. The signature here
  // refers to a subset of the current response from Omaha.  Each update to
  // this value is persisted so we resume from the same value in case of a
  // process restart.
  std::string response_signature_;

  // The number of times we've tried to download the payload. This is
  // incremented each time we download the payload successsfully or when we
  // exhaust all failure limits for all URLs and are about to wrap around back
  // to the first URL.  Each update to this value is persisted so we resume from
  // the same value in case of a process restart.
  int payload_attempt_number_;

  // The number of times we've tried to download the payload in full. This is
  // incremented each time we download the payload in full successsfully or
  // when we exhaust all failure limits for all URLs and are about to wrap
  // around back to the first URL.  Each update to this value is persisted so
  // we resume from the same value in case of a process restart.
  int full_payload_attempt_number_;

  // The index of the current payload.
  size_t payload_index_ = 0;

  // The index of the current URL.  This type is different from the one in the
  // accessor methods because PrefsInterface supports only int64_t but we want
  // to provide a stronger abstraction of uint32_t.  Each update to this value
  // is persisted so we resume from the same value in case of a process
  // restart.
  size_t url_index_;

  // The count of failures encountered in the current attempt to download using
  // the current URL (specified by url_index_).  Each update to this value is
  // persisted so we resume from the same value in case of a process restart.
  int64_t url_failure_count_;

  // The number of times we've switched URLs.
  int32_t url_switch_count_;

  // The current download source based on the current URL. This value is
  // not persisted as it can be recomputed every time we update the URL.
  // We're storing this so as not to recompute this on every few bytes of
  // data we read from the socket.
  DownloadSource current_download_source_;

  // The number of different Omaha responses seen. Increases every time
  // a new response is seen. Resets to 0 only when the system has been
  // successfully updated.
  int num_responses_seen_;

  // The number of system reboots during an update attempt. Technically since
  // we don't go out of our way to not update it when not attempting an update,
  // also records the number of reboots before the next update attempt starts.
  uint32_t num_reboots_;

  // The timestamp until which we've to wait before attempting to download the
  // payload again, so as to backoff repeated downloads.
  base::Time backoff_expiry_time_;

  // The most recently calculated value of the update duration.
  base::TimeDelta update_duration_current_;

  // The point in time (wall-clock) that the update was started.
  base::Time update_timestamp_start_;

  // The point in time (wall-clock) that the update ended. If the update
  // is still in progress, this is set to the Epoch (e.g. 0).
  base::Time update_timestamp_end_;

  // The update duration uptime
  base::TimeDelta update_duration_uptime_;

  // The monotonic time when |update_duration_uptime_| was last set
  base::Time update_duration_uptime_timestamp_;

  // The number of bytes that have been downloaded for each source for each new
  // update attempt. If we resume an update, we'll continue from the previous
  // value, but if we get a new response or if the previous attempt failed,
  // we'll reset this to 0 to start afresh. Each update to this value is
  // persisted so we resume from the same value in case of a process restart.
  // The extra index in the array is to no-op accidental access in case the
  // return value from GetCurrentDownloadSource is used without validation.
  uint64_t current_bytes_downloaded_[kNumDownloadSources + 1];

  // The number of bytes that have been downloaded for each source since the
  // the last successful update. This is used to compute the overhead we incur.
  // Each update to this value is persisted so we resume from the same value in
  // case of a process restart.
  // The extra index in the array is to no-op accidental access in case the
  // return value from GetCurrentDownloadSource is used without validation.
  uint64_t total_bytes_downloaded_[kNumDownloadSources + 1];

  // A small timespan used when comparing wall-clock times for coping
  // with the fact that clocks drift and consequently are adjusted
  // (either forwards or backwards) via NTP.
  static const base::TimeDelta kDurationSlack;

  // The ordered list of the subset of payload URL candidates which are
  // allowed as per device policy.
  std::vector<std::vector<std::string>> candidate_urls_;

  // This stores whether rollback has happened since the last time device policy
  // was available during update check. When this is set, we're preventing
  // forced updates to avoid update-rollback loops.
  bool rollback_happened_;

  // This stores a blacklisted version set as part of rollback. When we rollback
  // we store the version of the os from which we are rolling back from in order
  // to guarantee that we do not re-update to it on the next au attempt after
  // reboot.
  std::string rollback_version_;

  // The number of bytes downloaded per attempt.
  int64_t attempt_num_bytes_downloaded_;

  // The boot time when the attempt was started.
  base::Time attempt_start_time_boot_;

  // The monotonic time when the attempt was started.
  base::Time attempt_start_time_monotonic_;

  // The connection type when the attempt started.
  metrics::ConnectionType attempt_connection_type_;

  // Whether we're currently rolling back.
  AttemptType attempt_type_;

  // The current scattering wallclock-based wait period.
  base::TimeDelta scattering_wait_period_;

  // The current staging wallclock-based wait period.
  base::TimeDelta staging_wait_period_;

  DISALLOW_COPY_AND_ASSIGN(PayloadState);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_STATE_H_
