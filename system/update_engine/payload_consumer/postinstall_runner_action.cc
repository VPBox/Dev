//
// Copyright (C) 2011 The Android Open Source Project
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

#include "update_engine/payload_consumer/postinstall_runner_action.h"

#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/logging.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>

#include "update_engine/common/action_processor.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/platform_constants.h"
#include "update_engine/common/subprocess.h"
#include "update_engine/common/utils.h"

namespace {

// The file descriptor number from the postinstall program's perspective where
// it can report status updates. This can be any number greater than 2 (stderr),
// but must be kept in sync with the "bin/postinst_progress" defined in the
// sample_images.sh file.
const int kPostinstallStatusFd = 3;

}  // namespace

namespace chromeos_update_engine {

using brillo::MessageLoop;
using std::string;
using std::vector;

void PostinstallRunnerAction::PerformAction() {
  CHECK(HasInputObject());
  install_plan_ = GetInputObject();

  // Currently we're always powerwashing when rolling back.
  if (install_plan_.powerwash_required || install_plan_.is_rollback) {
    if (hardware_->SchedulePowerwash(install_plan_.is_rollback)) {
      powerwash_scheduled_ = true;
    } else {
      return CompletePostinstall(ErrorCode::kPostinstallPowerwashError);
    }
  }

  // Initialize all the partition weights.
  partition_weight_.resize(install_plan_.partitions.size());
  total_weight_ = 0;
  for (size_t i = 0; i < install_plan_.partitions.size(); ++i) {
    // TODO(deymo): This code sets the weight to all the postinstall commands,
    // but we could remember how long they took in the past and use those
    // values.
    partition_weight_[i] = install_plan_.partitions[i].run_postinstall;
    total_weight_ += partition_weight_[i];
  }
  accumulated_weight_ = 0;
  ReportProgress(0);

  PerformPartitionPostinstall();
}

void PostinstallRunnerAction::PerformPartitionPostinstall() {
  if (!install_plan_.run_post_install) {
    LOG(INFO) << "Skipping post-install according to install plan.";
    return CompletePostinstall(ErrorCode::kSuccess);
  }

  if (install_plan_.download_url.empty()) {
    LOG(INFO) << "Skipping post-install during rollback";
    return CompletePostinstall(ErrorCode::kSuccess);
  }

  // Skip all the partitions that don't have a post-install step.
  while (current_partition_ < install_plan_.partitions.size() &&
         !install_plan_.partitions[current_partition_].run_postinstall) {
    VLOG(1) << "Skipping post-install on partition "
            << install_plan_.partitions[current_partition_].name;
    current_partition_++;
  }
  if (current_partition_ == install_plan_.partitions.size())
    return CompletePostinstall(ErrorCode::kSuccess);

  const InstallPlan::Partition& partition =
      install_plan_.partitions[current_partition_];

  const string mountable_device =
      utils::MakePartitionNameForMount(partition.target_path);
  if (mountable_device.empty()) {
    LOG(ERROR) << "Cannot make mountable device from " << partition.target_path;
    return CompletePostinstall(ErrorCode::kPostinstallRunnerError);
  }

  // Perform post-install for the current_partition_ partition. At this point we
  // need to call CompletePartitionPostinstall to complete the operation and
  // cleanup.
#ifdef __ANDROID__
  fs_mount_dir_ = "/postinstall";
#else   // __ANDROID__
  base::FilePath temp_dir;
  TEST_AND_RETURN(base::CreateNewTempDirectory("au_postint_mount", &temp_dir));
  fs_mount_dir_ = temp_dir.value();
#endif  // __ANDROID__

  // Double check that the fs_mount_dir is not busy with a previous mounted
  // filesystem from a previous crashed postinstall step.
  if (utils::IsMountpoint(fs_mount_dir_)) {
    LOG(INFO) << "Found previously mounted filesystem at " << fs_mount_dir_;
    utils::UnmountFilesystem(fs_mount_dir_);
  }

  base::FilePath postinstall_path(partition.postinstall_path);
  if (postinstall_path.IsAbsolute()) {
    LOG(ERROR) << "Invalid absolute path passed to postinstall, use a relative"
                  "path instead: "
               << partition.postinstall_path;
    return CompletePostinstall(ErrorCode::kPostinstallRunnerError);
  }

  string abs_path =
      base::FilePath(fs_mount_dir_).Append(postinstall_path).value();
  if (!base::StartsWith(
          abs_path, fs_mount_dir_, base::CompareCase::SENSITIVE)) {
    LOG(ERROR) << "Invalid relative postinstall path: "
               << partition.postinstall_path;
    return CompletePostinstall(ErrorCode::kPostinstallRunnerError);
  }

#ifdef __ANDROID__
  // In Chromium OS, the postinstall step is allowed to write to the block
  // device on the target image, so we don't mark it as read-only and should
  // be read-write since we just wrote to it during the update.

  // Mark the block device as read-only before mounting for post-install.
  if (!utils::SetBlockDeviceReadOnly(mountable_device, true)) {
    return CompletePartitionPostinstall(
        1, "Error marking the device " + mountable_device + " read only.");
  }
#endif  // __ANDROID__

  if (!utils::MountFilesystem(mountable_device,
                              fs_mount_dir_,
                              MS_RDONLY,
                              partition.filesystem_type,
                              constants::kPostinstallMountOptions)) {
    return CompletePartitionPostinstall(
        1, "Error mounting the device " + mountable_device);
  }

  LOG(INFO) << "Performing postinst (" << partition.postinstall_path << " at "
            << abs_path << ") installed on device " << partition.target_path
            << " and mountable device " << mountable_device;

  // Logs the file format of the postinstall script we are about to run. This
  // will help debug when the postinstall script doesn't match the architecture
  // of our build.
  LOG(INFO) << "Format file for new " << partition.postinstall_path
            << " is: " << utils::GetFileFormat(abs_path);

  // Runs the postinstall script asynchronously to free up the main loop while
  // it's running.
  vector<string> command = {abs_path};
#ifdef __ANDROID__
  // In Brillo and Android, we pass the slot number and status fd.
  command.push_back(std::to_string(install_plan_.target_slot));
  command.push_back(std::to_string(kPostinstallStatusFd));
#else
  // Chrome OS postinstall expects the target rootfs as the first parameter.
  command.push_back(partition.target_path);
#endif  // __ANDROID__

  current_command_ = Subprocess::Get().ExecFlags(
      command,
      Subprocess::kRedirectStderrToStdout,
      {kPostinstallStatusFd},
      base::Bind(&PostinstallRunnerAction::CompletePartitionPostinstall,
                 base::Unretained(this)));
  // Subprocess::Exec should never return a negative process id.
  CHECK_GE(current_command_, 0);

  if (!current_command_) {
    CompletePartitionPostinstall(1, "Postinstall didn't launch");
    return;
  }

  // Monitor the status file descriptor.
  progress_fd_ =
      Subprocess::Get().GetPipeFd(current_command_, kPostinstallStatusFd);
  int fd_flags = fcntl(progress_fd_, F_GETFL, 0) | O_NONBLOCK;
  if (HANDLE_EINTR(fcntl(progress_fd_, F_SETFL, fd_flags)) < 0) {
    PLOG(ERROR) << "Unable to set non-blocking I/O mode on fd " << progress_fd_;
  }

  progress_task_ = MessageLoop::current()->WatchFileDescriptor(
      FROM_HERE,
      progress_fd_,
      MessageLoop::WatchMode::kWatchRead,
      true,
      base::Bind(&PostinstallRunnerAction::OnProgressFdReady,
                 base::Unretained(this)));
}

void PostinstallRunnerAction::OnProgressFdReady() {
  char buf[1024];
  size_t bytes_read;
  do {
    bytes_read = 0;
    bool eof;
    bool ok =
        utils::ReadAll(progress_fd_, buf, arraysize(buf), &bytes_read, &eof);
    progress_buffer_.append(buf, bytes_read);
    // Process every line.
    vector<string> lines = base::SplitString(
        progress_buffer_, "\n", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);
    if (!lines.empty()) {
      progress_buffer_ = lines.back();
      lines.pop_back();
      for (const auto& line : lines) {
        ProcessProgressLine(line);
      }
    }
    if (!ok || eof) {
      // There was either an error or an EOF condition, so we are done watching
      // the file descriptor.
      MessageLoop::current()->CancelTask(progress_task_);
      progress_task_ = MessageLoop::kTaskIdNull;
      return;
    }
  } while (bytes_read);
}

bool PostinstallRunnerAction::ProcessProgressLine(const string& line) {
  double frac = 0;
  if (sscanf(line.c_str(), "global_progress %lf", &frac) == 1 &&
      !std::isnan(frac)) {
    ReportProgress(frac);
    return true;
  }

  return false;
}

void PostinstallRunnerAction::ReportProgress(double frac) {
  if (!delegate_)
    return;
  if (current_partition_ >= partition_weight_.size() || total_weight_ == 0) {
    delegate_->ProgressUpdate(1.);
    return;
  }
  if (!std::isfinite(frac) || frac < 0)
    frac = 0;
  if (frac > 1)
    frac = 1;
  double postinst_action_progress =
      (accumulated_weight_ + partition_weight_[current_partition_] * frac) /
      total_weight_;
  delegate_->ProgressUpdate(postinst_action_progress);
}

void PostinstallRunnerAction::Cleanup() {
  utils::UnmountFilesystem(fs_mount_dir_);
#ifndef __ANDROID__
  if (!base::DeleteFile(base::FilePath(fs_mount_dir_), false)) {
    PLOG(WARNING) << "Not removing temporary mountpoint " << fs_mount_dir_;
  }
#endif  // !__ANDROID__
  fs_mount_dir_.clear();

  progress_fd_ = -1;
  if (progress_task_ != MessageLoop::kTaskIdNull) {
    MessageLoop::current()->CancelTask(progress_task_);
    progress_task_ = MessageLoop::kTaskIdNull;
  }
  progress_buffer_.clear();
}

void PostinstallRunnerAction::CompletePartitionPostinstall(
    int return_code, const string& output) {
  current_command_ = 0;
  Cleanup();

  if (return_code != 0) {
    LOG(ERROR) << "Postinst command failed with code: " << return_code;
    ErrorCode error_code = ErrorCode::kPostinstallRunnerError;

    if (return_code == 3) {
      // This special return code means that we tried to update firmware,
      // but couldn't because we booted from FW B, and we need to reboot
      // to get back to FW A.
      error_code = ErrorCode::kPostinstallBootedFromFirmwareB;
    }

    if (return_code == 4) {
      // This special return code means that we tried to update firmware,
      // but couldn't because we booted from FW B, and we need to reboot
      // to get back to FW A.
      error_code = ErrorCode::kPostinstallFirmwareRONotUpdatable;
    }

    // If postinstall script for this partition is optional we can ignore the
    // result.
    if (install_plan_.partitions[current_partition_].postinstall_optional) {
      LOG(INFO) << "Ignoring postinstall failure since it is optional";
    } else {
      return CompletePostinstall(error_code);
    }
  }
  accumulated_weight_ += partition_weight_[current_partition_];
  current_partition_++;
  ReportProgress(0);

  PerformPartitionPostinstall();
}

void PostinstallRunnerAction::CompletePostinstall(ErrorCode error_code) {
  // We only attempt to mark the new slot as active if all the postinstall
  // steps succeeded.
  if (error_code == ErrorCode::kSuccess) {
    if (install_plan_.switch_slot_on_reboot) {
      if (!boot_control_->SetActiveBootSlot(install_plan_.target_slot)) {
        error_code = ErrorCode::kPostinstallRunnerError;
      }
    } else {
      error_code = ErrorCode::kUpdatedButNotActive;
    }
  }

  ScopedActionCompleter completer(processor_, this);
  completer.set_code(error_code);

  if (error_code != ErrorCode::kSuccess &&
      error_code != ErrorCode::kUpdatedButNotActive) {
    LOG(ERROR) << "Postinstall action failed.";

    // Undo any changes done to trigger Powerwash.
    if (powerwash_scheduled_)
      hardware_->CancelPowerwash();

    return;
  }

  LOG(INFO) << "All post-install commands succeeded";
  if (HasOutputPipe()) {
    SetOutputObject(install_plan_);
  }
}

void PostinstallRunnerAction::SuspendAction() {
  if (!current_command_)
    return;
  if (kill(current_command_, SIGSTOP) != 0) {
    PLOG(ERROR) << "Couldn't pause child process " << current_command_;
  } else {
    is_current_command_suspended_ = true;
  }
}

void PostinstallRunnerAction::ResumeAction() {
  if (!current_command_)
    return;
  if (kill(current_command_, SIGCONT) != 0) {
    PLOG(ERROR) << "Couldn't resume child process " << current_command_;
  } else {
    is_current_command_suspended_ = false;
  }
}

void PostinstallRunnerAction::TerminateProcessing() {
  if (!current_command_)
    return;
  // Calling KillExec() will discard the callback we registered and therefore
  // the unretained reference to this object.
  Subprocess::Get().KillExec(current_command_);

  // If the command has been suspended, resume it after KillExec() so that the
  // process can process the SIGTERM sent by KillExec().
  if (is_current_command_suspended_) {
    ResumeAction();
  }

  current_command_ = 0;
  Cleanup();
}

}  // namespace chromeos_update_engine
