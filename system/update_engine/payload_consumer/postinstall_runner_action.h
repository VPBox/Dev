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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_POSTINSTALL_RUNNER_ACTION_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_POSTINSTALL_RUNNER_ACTION_H_

#include <string>
#include <vector>

#include <brillo/message_loops/message_loop.h>
#include <gtest/gtest_prod.h>

#include "update_engine/common/action.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/hardware_interface.h"
#include "update_engine/payload_consumer/install_plan.h"

// The Postinstall Runner Action is responsible for running the postinstall
// script of a successfully downloaded update.

namespace chromeos_update_engine {

class BootControlInterface;

class PostinstallRunnerAction : public InstallPlanAction {
 public:
  PostinstallRunnerAction(BootControlInterface* boot_control,
                          HardwareInterface* hardware)
      : boot_control_(boot_control), hardware_(hardware) {}

  // InstallPlanAction overrides.
  void PerformAction() override;
  void SuspendAction() override;
  void ResumeAction() override;
  void TerminateProcessing() override;

  class DelegateInterface {
   public:
    virtual ~DelegateInterface() = default;

    // Called whenever there is an overall progress update from the postinstall
    // programs.
    virtual void ProgressUpdate(double progress) = 0;
  };

  void set_delegate(DelegateInterface* delegate) { delegate_ = delegate; }

  // Debugging/logging
  static std::string StaticType() { return "PostinstallRunnerAction"; }
  std::string Type() const override { return StaticType(); }

 private:
  friend class PostinstallRunnerActionTest;
  FRIEND_TEST(PostinstallRunnerActionTest, ProcessProgressLineTest);

  void PerformPartitionPostinstall();

  // Called whenever the |progress_fd_| has data available to read.
  void OnProgressFdReady();

  // Updates the action progress according to the |line| passed from the
  // postinstall program. Valid lines are:
  //     global_progress <frac>
  //         <frac> should be between 0.0 and 1.0; sets the progress to the
  //         <frac> value.
  bool ProcessProgressLine(const std::string& line);

  // Report the progress to the delegate given that the postinstall operation
  // for |current_partition_| has a current progress of |frac|, a value between
  // 0 and 1 for that step.
  void ReportProgress(double frac);

  // Cleanup the setup made when running postinstall for a given partition.
  // Unmount and remove the mountpoint directory if needed and cleanup the
  // status file descriptor and message loop task watching for it.
  void Cleanup();

  // Subprocess::Exec callback.
  void CompletePartitionPostinstall(int return_code, const std::string& output);

  // Complete the Action with the passed |error_code| and mark the new slot as
  // ready. Called when the post-install script was run for all the partitions.
  void CompletePostinstall(ErrorCode error_code);

  InstallPlan install_plan_;

  // The path where the filesystem will be mounted during post-install.
  std::string fs_mount_dir_;

  // The partition being processed on the list of partitions specified in the
  // InstallPlan.
  size_t current_partition_{0};

  // A non-negative value representing the estimated weight of each partition
  // passed in the install plan. The weight is used to predict the overall
  // progress from the individual progress of each partition and should
  // correspond to the time it takes to run it.
  std::vector<double> partition_weight_;

  // The sum of all the weights in |partition_weight_|.
  double total_weight_{0};

  // The sum of all the weights in |partition_weight_| up to but not including
  // the |current_partition_|.
  double accumulated_weight_{0};

  // The delegate used to notify of progress updates, if any.
  DelegateInterface* delegate_{nullptr};

  // The BootControlInerface used to mark the new slot as ready.
  BootControlInterface* boot_control_;

  // HardwareInterface used to signal powerwash.
  HardwareInterface* hardware_;

  // Whether the Powerwash was scheduled before invoking post-install script.
  // Used for cleaning up if post-install fails.
  bool powerwash_scheduled_{false};

  // Postinstall command currently running, or 0 if no program running.
  pid_t current_command_{0};

  // True if |current_command_| has been suspended by SuspendAction().
  bool is_current_command_suspended_{false};

  // The parent progress file descriptor used to watch for progress reports from
  // the postinstall program and the task watching for them.
  int progress_fd_{-1};
  brillo::MessageLoop::TaskId progress_task_{brillo::MessageLoop::kTaskIdNull};

  // A buffer of a partial read line from the progress file descriptor.
  std::string progress_buffer_;

  DISALLOW_COPY_AND_ASSIGN(PostinstallRunnerAction);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_POSTINSTALL_RUNNER_ACTION_H_
