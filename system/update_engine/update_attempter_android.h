//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef UPDATE_ENGINE_UPDATE_ATTEMPTER_ANDROID_H_
#define UPDATE_ENGINE_UPDATE_ATTEMPTER_ANDROID_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include <base/time/time.h>

#include "update_engine/client_library/include/update_engine/update_status.h"
#include "update_engine/common/action_processor.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/clock.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/common/prefs_interface.h"
#include "update_engine/daemon_state_interface.h"
#include "update_engine/metrics_reporter_interface.h"
#include "update_engine/metrics_utils.h"
#include "update_engine/network_selector_interface.h"
#include "update_engine/payload_consumer/download_action.h"
#include "update_engine/payload_consumer/postinstall_runner_action.h"
#include "update_engine/service_delegate_android_interface.h"
#include "update_engine/service_observer_interface.h"

namespace chromeos_update_engine {

class UpdateAttempterAndroid
    : public ServiceDelegateAndroidInterface,
      public ActionProcessorDelegate,
      public DownloadActionDelegate,
      public PostinstallRunnerAction::DelegateInterface {
 public:
  using UpdateStatus = update_engine::UpdateStatus;

  UpdateAttempterAndroid(DaemonStateInterface* daemon_state,
                         PrefsInterface* prefs,
                         BootControlInterface* boot_control_,
                         HardwareInterface* hardware_);
  ~UpdateAttempterAndroid() override;

  // Further initialization to be done post construction.
  void Init();

  // ServiceDelegateAndroidInterface overrides.
  bool ApplyPayload(const std::string& payload_url,
                    int64_t payload_offset,
                    int64_t payload_size,
                    const std::vector<std::string>& key_value_pair_headers,
                    brillo::ErrorPtr* error) override;
  bool SuspendUpdate(brillo::ErrorPtr* error) override;
  bool ResumeUpdate(brillo::ErrorPtr* error) override;
  bool CancelUpdate(brillo::ErrorPtr* error) override;
  bool ResetStatus(brillo::ErrorPtr* error) override;
  bool VerifyPayloadApplicable(const std::string& metadata_filename,
                               brillo::ErrorPtr* error) override;

  // ActionProcessorDelegate methods:
  void ProcessingDone(const ActionProcessor* processor,
                      ErrorCode code) override;
  void ProcessingStopped(const ActionProcessor* processor) override;
  void ActionCompleted(ActionProcessor* processor,
                       AbstractAction* action,
                       ErrorCode code) override;

  // DownloadActionDelegate overrides.
  void BytesReceived(uint64_t bytes_progressed,
                     uint64_t bytes_received,
                     uint64_t total) override;
  bool ShouldCancel(ErrorCode* cancel_reason) override;
  void DownloadComplete() override;

  // PostinstallRunnerAction::DelegateInterface
  void ProgressUpdate(double progress) override;

 private:
  friend class UpdateAttempterAndroidTest;

  // Schedules an event loop callback to start the action processor. This is
  // scheduled asynchronously to unblock the event loop.
  void ScheduleProcessingStart();

  // Notifies an update request completed with the given error |code| to all
  // observers.
  void TerminateUpdateAndNotify(ErrorCode error_code);

  // Sets the status to the given |status| and notifies a status update to
  // all observers.
  void SetStatusAndNotify(UpdateStatus status);

  // Helper method to construct the sequence of actions to be performed for
  // applying an update using a given HttpFetcher. The ownership of |fetcher| is
  // passed to this function.
  void BuildUpdateActions(HttpFetcher* fetcher);

  // Writes to the processing completed marker. Does nothing if
  // |update_completed_marker_| is empty.
  bool WriteUpdateCompletedMarker();

  // Returns whether an update was completed in the current boot.
  bool UpdateCompletedOnThisBoot();

  // Prefs to use for metrics report
  // |kPrefsPayloadAttemptNumber|: number of update attempts for the current
  // payload_id.
  // |KprefsNumReboots|: number of reboots when applying the current update.
  // |kPrefsSystemUpdatedMarker|: end timestamp of the last successful update.
  // |kPrefsUpdateTimestampStart|: start timestamp in monotonic time of the
  // current update.
  // |kPrefsUpdateBootTimestampStart|: start timestamp in boot time of
  // the current update.
  // |kPrefsCurrentBytesDownloaded|: number of bytes downloaded for the current
  // payload_id.
  // |kPrefsTotalBytesDownloaded|: number of bytes downloaded in total since
  // the last successful update.

  // Metrics report function to call:
  //   |ReportUpdateAttemptMetrics|
  //   |ReportSuccessfulUpdateMetrics|
  // Prefs to update:
  //   |kPrefsSystemUpdatedMarker|
  void CollectAndReportUpdateMetricsOnUpdateFinished(ErrorCode error_code);

  // Metrics report function to call:
  //   |ReportAbnormallyTerminatedUpdateAttemptMetrics|
  //   |ReportTimeToRebootMetrics|
  // Prefs to update:
  //   |kPrefsBootId|, |kPrefsPreviousVersion|
  void UpdatePrefsAndReportUpdateMetricsOnReboot();

  // Prefs to update:
  //   |kPrefsPayloadAttemptNumber|, |kPrefsUpdateTimestampStart|,
  //   |kPrefsUpdateBootTimestampStart|
  void UpdatePrefsOnUpdateStart(bool is_resume);

  // Prefs to delete:
  //   |kPrefsNumReboots|, |kPrefsCurrentBytesDownloaded|
  //   |kPrefsSystemUpdatedMarker|, |kPrefsUpdateTimestampStart|,
  //   |kPrefsUpdateBootTimestampStart|
  void ClearMetricsPrefs();

  DaemonStateInterface* daemon_state_;

  // DaemonStateAndroid pointers.
  PrefsInterface* prefs_;
  BootControlInterface* boot_control_;
  HardwareInterface* hardware_;

  // Last status notification timestamp used for throttling. Use monotonic
  // TimeTicks to ensure that notifications are sent even if the system clock is
  // set back in the middle of an update.
  base::TimeTicks last_notify_time_;

  // Only direct proxy supported.
  DirectProxyResolver proxy_resolver_;

  // The processor for running Actions.
  std::unique_ptr<ActionProcessor> processor_;

  // The InstallPlan used during the ongoing update.
  InstallPlan install_plan_;

  // For status:
  UpdateStatus status_{UpdateStatus::IDLE};
  double download_progress_{0.0};

  // The offset in the payload file where the CrAU part starts.
  int64_t base_offset_{0};

  // Helper class to select the network to use during the update.
  std::unique_ptr<NetworkSelectorInterface> network_selector_;

  std::unique_ptr<ClockInterface> clock_;

  std::unique_ptr<MetricsReporterInterface> metrics_reporter_;

  DISALLOW_COPY_AND_ASSIGN(UpdateAttempterAndroid);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_UPDATE_ATTEMPTER_ANDROID_H_
