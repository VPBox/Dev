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

#include <inttypes.h>
#include <sysexits.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include <base/bind.h>
#include <base/command_line.h>
#include <base/logging.h>
#include <base/macros.h>
#include <base/strings/string_split.h>
#include <base/threading/platform_thread.h>
#include <brillo/daemons/daemon.h>
#include <brillo/flag_helper.h>

#include "update_engine/client.h"
#include "update_engine/common/error_code.h"
#include "update_engine/common/error_code_utils.h"
#include "update_engine/omaha_utils.h"
#include "update_engine/status_update_handler.h"
#include "update_engine/update_status.h"
#include "update_engine/update_status_utils.h"

using chromeos_update_engine::EolStatus;
using chromeos_update_engine::ErrorCode;
using chromeos_update_engine::UpdateStatusToString;
using chromeos_update_engine::utils::ErrorCodeToString;
using std::string;
using std::unique_ptr;
using std::vector;
using update_engine::UpdateStatus;

namespace {

// Constant to signal that we need to continue running the daemon after
// initialization.
const int kContinueRunning = -1;

// The ShowStatus request will be retried `kShowStatusRetryCount` times at
// `kShowStatusRetryInterval` second intervals on failure.
const int kShowStatusRetryCount = 30;
const int kShowStatusRetryIntervalInSeconds = 2;

class UpdateEngineClient : public brillo::Daemon {
 public:
  UpdateEngineClient(int argc, char** argv) : argc_(argc), argv_(argv) {}

  ~UpdateEngineClient() override = default;

 protected:
  int OnInit() override {
    int ret = Daemon::OnInit();
    if (ret != EX_OK)
      return ret;

    client_ = update_engine::UpdateEngineClient::CreateInstance();

    if (!client_) {
      LOG(ERROR) << "UpdateEngineService not available.";
      return 1;
    }

    // We can't call QuitWithExitCode from OnInit(), so we delay the execution
    // of the ProcessFlags method after the Daemon initialization is done.
    base::MessageLoop::current()->task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&UpdateEngineClient::ProcessFlagsAndExit,
                   base::Unretained(this)));
    return EX_OK;
  }

 private:
  // Show the status of the update engine in stdout.
  bool ShowStatus();

  // Return whether we need to reboot. 0 if reboot is needed, 1 if an error
  // occurred, 2 if no reboot is needed.
  int GetNeedReboot();

  // Main method that parses and triggers all the actions based on the passed
  // flags. Returns the exit code of the program of kContinueRunning if it
  // should not exit.
  int ProcessFlags();

  // Processes the flags and exits the program accordingly.
  void ProcessFlagsAndExit();

  // Copy of argc and argv passed to main().
  int argc_;
  char** argv_;

  // Library-based client
  unique_ptr<update_engine::UpdateEngineClient> client_;

  // Pointers to handlers for cleanup
  vector<unique_ptr<update_engine::StatusUpdateHandler>> handlers_;

  DISALLOW_COPY_AND_ASSIGN(UpdateEngineClient);
};

class ExitingStatusUpdateHandler : public update_engine::StatusUpdateHandler {
 public:
  ~ExitingStatusUpdateHandler() override = default;

  void IPCError(const string& error) override;
};

void ExitingStatusUpdateHandler::IPCError(const string& error) {
  LOG(ERROR) << error;
  exit(1);
}

class WatchingStatusUpdateHandler : public ExitingStatusUpdateHandler {
 public:
  ~WatchingStatusUpdateHandler() override = default;

  void HandleStatusUpdate(int64_t last_checked_time,
                          double progress,
                          UpdateStatus current_operation,
                          const string& new_version,
                          int64_t new_size) override;
};

void WatchingStatusUpdateHandler::HandleStatusUpdate(
    int64_t last_checked_time,
    double progress,
    UpdateStatus current_operation,
    const string& new_version,
    int64_t new_size) {
  LOG(INFO) << "Got status update:";
  LOG(INFO) << "  last_checked_time: " << last_checked_time;
  LOG(INFO) << "  progress: " << progress;
  LOG(INFO) << "  current_operation: "
            << UpdateStatusToString(current_operation);
  LOG(INFO) << "  new_version: " << new_version;
  LOG(INFO) << "  new_size: " << new_size;
}

bool UpdateEngineClient::ShowStatus() {
  int64_t last_checked_time = 0;
  double progress = 0.0;
  UpdateStatus current_op;
  string new_version;
  int64_t new_size = 0;

  int retry_count = kShowStatusRetryCount;
  while (retry_count > 0) {
    if (client_->GetStatus(&last_checked_time,
                           &progress,
                           &current_op,
                           &new_version,
                           &new_size)) {
      break;
    }
    if (--retry_count == 0) {
      return false;
    }
    LOG(WARNING) << "Will try " << retry_count << " more times!";
    base::PlatformThread::Sleep(
        base::TimeDelta::FromSeconds(kShowStatusRetryIntervalInSeconds));
  }

  printf("LAST_CHECKED_TIME=%" PRIi64
         "\nPROGRESS=%f\nCURRENT_OP=%s\n"
         "NEW_VERSION=%s\nNEW_SIZE=%" PRIi64 "\n",
         last_checked_time,
         progress,
         UpdateStatusToString(current_op),
         new_version.c_str(),
         new_size);

  return true;
}

int UpdateEngineClient::GetNeedReboot() {
  int64_t last_checked_time = 0;
  double progress = 0.0;
  UpdateStatus current_op;
  string new_version;
  int64_t new_size = 0;

  if (!client_->GetStatus(&last_checked_time,
                          &progress,
                          &current_op,
                          &new_version,
                          &new_size)) {
    return 1;
  }

  if (current_op == UpdateStatus::UPDATED_NEED_REBOOT) {
    return 0;
  }

  return 2;
}

class UpdateWaitHandler : public ExitingStatusUpdateHandler {
 public:
  explicit UpdateWaitHandler(bool exit_on_error,
                             update_engine::UpdateEngineClient* client)
      : exit_on_error_(exit_on_error), client_(client) {}

  ~UpdateWaitHandler() override = default;

  void HandleStatusUpdate(int64_t last_checked_time,
                          double progress,
                          UpdateStatus current_operation,
                          const string& new_version,
                          int64_t new_size) override;

 private:
  bool exit_on_error_;
  update_engine::UpdateEngineClient* client_;
};

void UpdateWaitHandler::HandleStatusUpdate(int64_t /* last_checked_time */,
                                           double /* progress */,
                                           UpdateStatus current_operation,
                                           const string& /* new_version */,
                                           int64_t /* new_size */) {
  if (exit_on_error_ && current_operation == UpdateStatus::IDLE) {
    int last_attempt_error;
    ErrorCode code = ErrorCode::kSuccess;
    if (client_ && client_->GetLastAttemptError(&last_attempt_error))
      code = static_cast<ErrorCode>(last_attempt_error);

    LOG(ERROR) << "Update failed, current operation is "
               << UpdateStatusToString(current_operation)
               << ", last error code is " << ErrorCodeToString(code) << "("
               << last_attempt_error << ")";
    exit(1);
  }
  if (current_operation == UpdateStatus::UPDATED_NEED_REBOOT) {
    LOG(INFO) << "Update succeeded -- reboot needed.";
    exit(0);
  }
}

int UpdateEngineClient::ProcessFlags() {
  DEFINE_string(app_version, "", "Force the current app version.");
  DEFINE_string(channel,
                "",
                "Set the target channel. The device will be powerwashed if the "
                "target channel is more stable than the current channel unless "
                "--nopowerwash is specified.");
  DEFINE_bool(check_for_update, false, "Initiate check for updates.");
  DEFINE_string(
      cohort_hint, "", "Set the current cohort hint to the passed value.");
  DEFINE_bool(follow,
              false,
              "Wait for any update operations to complete."
              "Exit status is 0 if the update succeeded, and 1 otherwise.");
  DEFINE_bool(interactive, true, "Mark the update request as interactive.");
  DEFINE_string(omaha_url, "", "The URL of the Omaha update server.");
  DEFINE_string(p2p_update,
                "",
                "Enables (\"yes\") or disables (\"no\") the peer-to-peer update"
                " sharing.");
  DEFINE_bool(powerwash,
              true,
              "When performing rollback or channel change, "
              "do a powerwash or allow it respectively.");
  DEFINE_bool(reboot, false, "Initiate a reboot if needed.");
  DEFINE_bool(is_reboot_needed,
              false,
              "Exit status 0 if reboot is needed, "
              "2 if reboot is not needed or 1 if an error occurred.");
  DEFINE_bool(block_until_reboot_is_needed,
              false,
              "Blocks until reboot is "
              "needed. Returns non-zero exit status if an error occurred.");
  DEFINE_bool(reset_status, false, "Sets the status in update_engine to idle.");
  DEFINE_bool(rollback,
              false,
              "Perform a rollback to the previous partition. The device will "
              "be powerwashed unless --nopowerwash is specified.");
  DEFINE_bool(can_rollback,
              false,
              "Shows whether rollback partition "
              "is available.");
  DEFINE_bool(show_channel, false, "Show the current and target channels.");
  DEFINE_bool(show_cohort_hint, false, "Show the current cohort hint.");
  DEFINE_bool(show_p2p_update,
              false,
              "Show the current setting for peer-to-peer update sharing.");
  DEFINE_bool(show_update_over_cellular,
              false,
              "Show the current setting for updates over cellular networks.");
  DEFINE_bool(status, false, "Print the status to stdout.");
  DEFINE_bool(update,
              false,
              "Forces an update and waits for it to complete. "
              "Implies --follow.");
  DEFINE_string(update_over_cellular,
                "",
                "Enables (\"yes\") or disables (\"no\") the updates over "
                "cellular networks.");
  DEFINE_bool(watch_for_updates,
              false,
              "Listen for status updates and print them to the screen.");
  DEFINE_bool(prev_version,
              false,
              "Show the previous OS version used before the update reboot.");
  DEFINE_bool(last_attempt_error, false, "Show the last attempt error.");
  DEFINE_bool(eol_status, false, "Show the current end-of-life status.");
  DEFINE_bool(install, false, "Requests an install.");
  DEFINE_string(dlc_module_ids, "", "colon-separated list of DLC IDs.");

  // Boilerplate init commands.
  base::CommandLine::Init(argc_, argv_);
  brillo::FlagHelper::Init(argc_, argv_, "A/B Update Engine Client");

  // Ensure there are no positional arguments.
  const vector<string> positional_args =
      base::CommandLine::ForCurrentProcess()->GetArgs();
  if (!positional_args.empty()) {
    LOG(ERROR) << "Found a positional argument '" << positional_args.front()
               << "'. If you want to pass a value to a flag, pass it as "
                  "--flag=value.";
    return 1;
  }

  // Update the status if requested.
  if (FLAGS_reset_status) {
    LOG(INFO) << "Setting Update Engine status to idle ...";

    if (client_->ResetStatus()) {
      LOG(INFO) << "ResetStatus succeeded; to undo partition table changes "
                   "run:\n"
                   "(D=$(rootdev -d) P=$(rootdev -s); cgpt p -i$(($(echo "
                   "${P#$D} | sed 's/^[^0-9]*//')-1)) $D;)";
    } else {
      LOG(ERROR) << "ResetStatus failed";
      return 1;
    }
  }

  // Changes the current update over cellular network setting.
  if (!FLAGS_update_over_cellular.empty()) {
    bool allowed = FLAGS_update_over_cellular == "yes";
    if (!allowed && FLAGS_update_over_cellular != "no") {
      LOG(ERROR) << "Unknown option: \"" << FLAGS_update_over_cellular
                 << "\". Please specify \"yes\" or \"no\".";
    } else {
      if (!client_->SetUpdateOverCellularPermission(allowed)) {
        LOG(ERROR) << "Error setting the update over cellular setting.";
        return 1;
      }
    }
  }

  // Show the current update over cellular network setting.
  if (FLAGS_show_update_over_cellular) {
    bool allowed;

    if (!client_->GetUpdateOverCellularPermission(&allowed)) {
      LOG(ERROR) << "Error getting the update over cellular setting.";
      return 1;
    }

    LOG(INFO) << "Current update over cellular network setting: "
              << (allowed ? "ENABLED" : "DISABLED");
  }

  // Change/show the cohort hint.
  bool set_cohort_hint =
      base::CommandLine::ForCurrentProcess()->HasSwitch("cohort_hint");
  if (set_cohort_hint) {
    LOG(INFO) << "Setting cohort hint to: \"" << FLAGS_cohort_hint << "\"";
    if (!client_->SetCohortHint(FLAGS_cohort_hint)) {
      LOG(ERROR) << "Error setting the cohort hint.";
      return 1;
    }
  }

  if (FLAGS_show_cohort_hint || set_cohort_hint) {
    string cohort_hint;
    if (!client_->GetCohortHint(&cohort_hint)) {
      LOG(ERROR) << "Error getting the cohort hint.";
      return 1;
    }

    LOG(INFO) << "Current cohort hint: \"" << cohort_hint << "\"";
  }

  if (!FLAGS_powerwash && !FLAGS_rollback && FLAGS_channel.empty()) {
    LOG(ERROR) << "powerwash flag only makes sense rollback or channel change";
    return 1;
  }

  // Change the P2P enabled setting.
  if (!FLAGS_p2p_update.empty()) {
    bool enabled = FLAGS_p2p_update == "yes";
    if (!enabled && FLAGS_p2p_update != "no") {
      LOG(ERROR) << "Unknown option: \"" << FLAGS_p2p_update
                 << "\". Please specify \"yes\" or \"no\".";
    } else {
      if (!client_->SetP2PUpdatePermission(enabled)) {
        LOG(ERROR) << "Error setting the peer-to-peer update setting.";
        return 1;
      }
    }
  }

  // Show the rollback availability.
  if (FLAGS_can_rollback) {
    string rollback_partition;

    if (!client_->GetRollbackPartition(&rollback_partition)) {
      LOG(ERROR) << "Error while querying rollback partition availability.";
      return 1;
    }

    bool can_rollback = true;
    if (rollback_partition.empty()) {
      rollback_partition = "UNAVAILABLE";
      can_rollback = false;
    } else {
      rollback_partition = "AVAILABLE: " + rollback_partition;
    }

    LOG(INFO) << "Rollback partition: " << rollback_partition;
    if (!can_rollback) {
      return 1;
    }
  }

  // Show the current P2P enabled setting.
  if (FLAGS_show_p2p_update) {
    bool enabled;

    if (!client_->GetP2PUpdatePermission(&enabled)) {
      LOG(ERROR) << "Error getting the peer-to-peer update setting.";
      return 1;
    }

    LOG(INFO) << "Current update using P2P setting: "
              << (enabled ? "ENABLED" : "DISABLED");
  }

  // First, update the target channel if requested.
  if (!FLAGS_channel.empty()) {
    if (!client_->SetTargetChannel(FLAGS_channel, FLAGS_powerwash)) {
      LOG(ERROR) << "Error setting the channel.";
      return 1;
    }

    LOG(INFO) << "Channel permanently set to: " << FLAGS_channel;
  }

  // Show the current and target channels if requested.
  if (FLAGS_show_channel) {
    string current_channel;
    string target_channel;

    if (!client_->GetChannel(&current_channel)) {
      LOG(ERROR) << "Error getting the current channel.";
      return 1;
    }

    if (!client_->GetTargetChannel(&target_channel)) {
      LOG(ERROR) << "Error getting the target channel.";
      return 1;
    }

    LOG(INFO) << "Current Channel: " << current_channel;

    if (!target_channel.empty())
      LOG(INFO) << "Target Channel (pending update): " << target_channel;
  }

  bool do_update_request = FLAGS_check_for_update || FLAGS_update ||
                           !FLAGS_app_version.empty() ||
                           !FLAGS_omaha_url.empty();
  if (FLAGS_update)
    FLAGS_follow = true;

  if (do_update_request && FLAGS_rollback) {
    LOG(ERROR) << "Incompatible flags specified with rollback."
               << "Rollback should not include update-related flags.";
    return 1;
  }

  if (FLAGS_rollback) {
    LOG(INFO) << "Requesting rollback.";
    if (!client_->Rollback(FLAGS_powerwash)) {
      LOG(ERROR) << "Rollback request failed.";
      return 1;
    }
  }

  if (FLAGS_install) {
    // Parse DLC module IDs.
    vector<string> dlc_module_ids;
    if (!FLAGS_dlc_module_ids.empty()) {
      dlc_module_ids = base::SplitString(FLAGS_dlc_module_ids,
                                         ":",
                                         base::TRIM_WHITESPACE,
                                         base::SPLIT_WANT_ALL);
    }
    if (dlc_module_ids.empty()) {
      LOG(ERROR) << "dlc_module_ids is empty:" << FLAGS_dlc_module_ids;
      return 1;
    }
    if (!client_->AttemptInstall(FLAGS_omaha_url, dlc_module_ids)) {
      LOG(ERROR) << "AttemptInstall failed.";
      return 1;
    }
    return 0;
  } else if (!FLAGS_dlc_module_ids.empty()) {
    LOG(ERROR) << "dlc_module_ids is not empty while install is not set:"
               << FLAGS_dlc_module_ids;
    return 1;
  }

  // Initiate an update check, if necessary.
  if (do_update_request) {
    LOG_IF(WARNING, FLAGS_reboot) << "-reboot flag ignored.";
    string app_version = FLAGS_app_version;
    if (FLAGS_update && app_version.empty()) {
      app_version = "ForcedUpdate";
      LOG(INFO) << "Forcing an update by setting app_version to ForcedUpdate.";
    }
    LOG(INFO) << "Initiating update check and install.";
    if (!client_->AttemptUpdate(
            app_version, FLAGS_omaha_url, FLAGS_interactive)) {
      LOG(ERROR) << "Error checking for update.";
      return 1;
    }
  }

  // These final options are all mutually exclusive with one another.
  if (FLAGS_follow + FLAGS_watch_for_updates + FLAGS_reboot + FLAGS_status +
          FLAGS_is_reboot_needed + FLAGS_block_until_reboot_is_needed >
      1) {
    LOG(ERROR) << "Multiple exclusive options selected. "
               << "Select only one of --follow, --watch_for_updates, --reboot, "
               << "--is_reboot_needed, --block_until_reboot_is_needed, "
               << "or --status.";
    return 1;
  }

  if (FLAGS_status) {
    LOG(INFO) << "Querying Update Engine status...";
    if (!ShowStatus()) {
      LOG(ERROR) << "Failed to query status";
      return 1;
    }
    return 0;
  }

  if (FLAGS_follow) {
    LOG(INFO) << "Waiting for update to complete.";
    auto handler = new UpdateWaitHandler(true, client_.get());
    handlers_.emplace_back(handler);
    client_->RegisterStatusUpdateHandler(handler);
    return kContinueRunning;
  }

  if (FLAGS_watch_for_updates) {
    LOG(INFO) << "Watching for status updates.";
    auto handler = new WatchingStatusUpdateHandler();
    handlers_.emplace_back(handler);
    client_->RegisterStatusUpdateHandler(handler);
    return kContinueRunning;
  }

  if (FLAGS_reboot) {
    LOG(INFO) << "Requesting a reboot...";
    client_->RebootIfNeeded();
    return 0;
  }

  if (FLAGS_prev_version) {
    string prev_version;

    if (!client_->GetPrevVersion(&prev_version)) {
      LOG(ERROR) << "Error getting previous version.";
    } else {
      LOG(INFO) << "Previous version = " << prev_version;
    }
  }

  if (FLAGS_is_reboot_needed) {
    int ret = GetNeedReboot();

    if (ret == 1) {
      LOG(ERROR) << "Could not query the current operation.";
    }

    return ret;
  }

  if (FLAGS_block_until_reboot_is_needed) {
    auto handler = new UpdateWaitHandler(false, nullptr);
    handlers_.emplace_back(handler);
    client_->RegisterStatusUpdateHandler(handler);
    return kContinueRunning;
  }

  if (FLAGS_last_attempt_error) {
    int last_attempt_error;
    if (!client_->GetLastAttemptError(&last_attempt_error)) {
      LOG(ERROR) << "Error getting last attempt error.";
    } else {
      ErrorCode code = static_cast<ErrorCode>(last_attempt_error);
      printf(
          "ERROR_CODE=%i\n"
          "ERROR_MESSAGE=%s\n",
          last_attempt_error,
          ErrorCodeToString(code).c_str());
    }
  }

  if (FLAGS_eol_status) {
    int eol_status;
    if (!client_->GetEolStatus(&eol_status)) {
      LOG(ERROR) << "Error getting the end-of-life status.";
    } else {
      EolStatus eol_status_code = static_cast<EolStatus>(eol_status);
      printf("EOL_STATUS=%s\n", EolStatusToString(eol_status_code));
    }
  }

  return 0;
}

void UpdateEngineClient::ProcessFlagsAndExit() {
  int ret = ProcessFlags();
  if (ret != kContinueRunning)
    QuitWithExitCode(ret);
}

}  // namespace

int main(int argc, char** argv) {
  UpdateEngineClient client(argc, argv);
  return client.Run();
}
