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

#include "update_engine/payload_consumer/download_action.h"

#include <errno.h>

#include <algorithm>
#include <string>

#include <base/files/file_path.h>
#include <base/metrics/statistics_recorder.h>
#include <base/strings/stringprintf.h>

#include "update_engine/common/action_pipe.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/error_code_utils.h"
#include "update_engine/common/multi_range_http_fetcher.h"
#include "update_engine/common/utils.h"
#include "update_engine/omaha_request_params.h"
#include "update_engine/p2p_manager.h"
#include "update_engine/payload_state_interface.h"

using base::FilePath;
using std::string;

namespace chromeos_update_engine {

DownloadAction::DownloadAction(PrefsInterface* prefs,
                               BootControlInterface* boot_control,
                               HardwareInterface* hardware,
                               SystemState* system_state,
                               HttpFetcher* http_fetcher,
                               bool interactive)
    : prefs_(prefs),
      boot_control_(boot_control),
      hardware_(hardware),
      system_state_(system_state),
      http_fetcher_(new MultiRangeHttpFetcher(http_fetcher)),
      interactive_(interactive),
      writer_(nullptr),
      code_(ErrorCode::kSuccess),
      delegate_(nullptr),
      p2p_sharing_fd_(-1),
      p2p_visible_(true) {
#if BASE_VER < 576279
  base::StatisticsRecorder::Initialize();
#endif
}

DownloadAction::~DownloadAction() {}

void DownloadAction::CloseP2PSharingFd(bool delete_p2p_file) {
  if (p2p_sharing_fd_ != -1) {
    if (close(p2p_sharing_fd_) != 0) {
      PLOG(ERROR) << "Error closing p2p sharing fd";
    }
    p2p_sharing_fd_ = -1;
  }

  if (delete_p2p_file) {
    FilePath path = system_state_->p2p_manager()->FileGetPath(p2p_file_id_);
    if (unlink(path.value().c_str()) != 0) {
      PLOG(ERROR) << "Error deleting p2p file " << path.value();
    } else {
      LOG(INFO) << "Deleted p2p file " << path.value();
    }
  }

  // Don't use p2p from this point onwards.
  p2p_file_id_.clear();
}

bool DownloadAction::SetupP2PSharingFd() {
  P2PManager* p2p_manager = system_state_->p2p_manager();

  if (!p2p_manager->FileShare(p2p_file_id_, payload_->size)) {
    LOG(ERROR) << "Unable to share file via p2p";
    CloseP2PSharingFd(true);  // delete p2p file
    return false;
  }

  // File has already been created (and allocated, xattrs been
  // populated etc.) by FileShare() so just open it for writing.
  FilePath path = p2p_manager->FileGetPath(p2p_file_id_);
  p2p_sharing_fd_ = open(path.value().c_str(), O_WRONLY);
  if (p2p_sharing_fd_ == -1) {
    PLOG(ERROR) << "Error opening file " << path.value();
    CloseP2PSharingFd(true);  // Delete p2p file.
    return false;
  }

  // Ensure file to share is world-readable, otherwise
  // p2p-server and p2p-http-server can't access it.
  //
  // (Q: Why doesn't the file have mode 0644 already? A: Because
  // the process-wide umask is set to 0700 in main.cc.)
  if (fchmod(p2p_sharing_fd_, 0644) != 0) {
    PLOG(ERROR) << "Error setting mode 0644 on " << path.value();
    CloseP2PSharingFd(true);  // Delete p2p file.
    return false;
  }

  // All good.
  LOG(INFO) << "Writing payload contents to " << path.value();
  p2p_manager->FileGetVisible(p2p_file_id_, &p2p_visible_);
  return true;
}

void DownloadAction::WriteToP2PFile(const void* data,
                                    size_t length,
                                    off_t file_offset) {
  if (p2p_sharing_fd_ == -1) {
    if (!SetupP2PSharingFd())
      return;
  }

  // Check that the file is at least |file_offset| bytes long - if
  // it's not something is wrong and we must immediately delete the
  // file to avoid propagating this problem to other peers.
  //
  // How can this happen? It could be that we're resuming an update
  // after a system crash... in this case, it could be that
  //
  //  1. the p2p file didn't get properly synced to stable storage; or
  //  2. the file was deleted at bootup (it's in /var/cache after all); or
  //  3. other reasons
  off_t p2p_size = utils::FileSize(p2p_sharing_fd_);
  if (p2p_size < 0) {
    PLOG(ERROR) << "Error getting file status for p2p file";
    CloseP2PSharingFd(true);  // Delete p2p file.
    return;
  }
  if (p2p_size < file_offset) {
    LOG(ERROR) << "Wanting to write to file offset " << file_offset
               << " but existing p2p file is only " << p2p_size << " bytes.";
    CloseP2PSharingFd(true);  // Delete p2p file.
    return;
  }

  off_t cur_file_offset = lseek(p2p_sharing_fd_, file_offset, SEEK_SET);
  if (cur_file_offset != static_cast<off_t>(file_offset)) {
    PLOG(ERROR) << "Error seeking to position " << file_offset
                << " in p2p file";
    CloseP2PSharingFd(true);  // Delete p2p file.
  } else {
    // OK, seeking worked, now write the data
    ssize_t bytes_written = write(p2p_sharing_fd_, data, length);
    if (bytes_written != static_cast<ssize_t>(length)) {
      PLOG(ERROR) << "Error writing " << length << " bytes at file offset "
                  << file_offset << " in p2p file";
      CloseP2PSharingFd(true);  // Delete p2p file.
    }
  }
}

void DownloadAction::PerformAction() {
  http_fetcher_->set_delegate(this);

  // Get the InstallPlan and read it
  CHECK(HasInputObject());
  install_plan_ = GetInputObject();
  install_plan_.Dump();

  bytes_received_ = 0;
  bytes_received_previous_payloads_ = 0;
  bytes_total_ = 0;
  for (const auto& payload : install_plan_.payloads)
    bytes_total_ += payload.size;

  if (install_plan_.is_resume) {
    int64_t payload_index = 0;
    if (prefs_->GetInt64(kPrefsUpdateStatePayloadIndex, &payload_index) &&
        static_cast<size_t>(payload_index) < install_plan_.payloads.size()) {
      // Save the index for the resume payload before downloading any previous
      // payload, otherwise it will be overwritten.
      resume_payload_index_ = payload_index;
      for (int i = 0; i < payload_index; i++)
        install_plan_.payloads[i].already_applied = true;
    }
  }
  // TODO(senj): check that install plan has at least one payload.
  if (!payload_)
    payload_ = &install_plan_.payloads[0];

  LOG(INFO) << "Marking new slot as unbootable";
  if (!boot_control_->MarkSlotUnbootable(install_plan_.target_slot)) {
    LOG(WARNING) << "Unable to mark new slot "
                 << BootControlInterface::SlotName(install_plan_.target_slot)
                 << ". Proceeding with the update anyway.";
  }

  StartDownloading();
}

void DownloadAction::StartDownloading() {
  download_active_ = true;
  http_fetcher_->ClearRanges();
  if (install_plan_.is_resume &&
      payload_ == &install_plan_.payloads[resume_payload_index_]) {
    // Resuming an update so fetch the update manifest metadata first.
    int64_t manifest_metadata_size = 0;
    int64_t manifest_signature_size = 0;
    prefs_->GetInt64(kPrefsManifestMetadataSize, &manifest_metadata_size);
    prefs_->GetInt64(kPrefsManifestSignatureSize, &manifest_signature_size);
    http_fetcher_->AddRange(base_offset_,
                            manifest_metadata_size + manifest_signature_size);
    // If there're remaining unprocessed data blobs, fetch them. Be careful not
    // to request data beyond the end of the payload to avoid 416 HTTP response
    // error codes.
    int64_t next_data_offset = 0;
    prefs_->GetInt64(kPrefsUpdateStateNextDataOffset, &next_data_offset);
    uint64_t resume_offset =
        manifest_metadata_size + manifest_signature_size + next_data_offset;
    if (!payload_->size) {
      http_fetcher_->AddRange(base_offset_ + resume_offset);
    } else if (resume_offset < payload_->size) {
      http_fetcher_->AddRange(base_offset_ + resume_offset,
                              payload_->size - resume_offset);
    }
  } else {
    if (payload_->size) {
      http_fetcher_->AddRange(base_offset_, payload_->size);
    } else {
      // If no payload size is passed we assume we read until the end of the
      // stream.
      http_fetcher_->AddRange(base_offset_);
    }
  }

  if (writer_ && writer_ != delta_performer_.get()) {
    LOG(INFO) << "Using writer for test.";
  } else {
    delta_performer_.reset(new DeltaPerformer(prefs_,
                                              boot_control_,
                                              hardware_,
                                              delegate_,
                                              &install_plan_,
                                              payload_,
                                              interactive_));
    writer_ = delta_performer_.get();
  }
  if (system_state_ != nullptr) {
    const PayloadStateInterface* payload_state = system_state_->payload_state();
    string file_id = utils::CalculateP2PFileId(payload_->hash, payload_->size);
    if (payload_state->GetUsingP2PForSharing()) {
      // If we're sharing the update, store the file_id to convey
      // that we should write to the file.
      p2p_file_id_ = file_id;
      LOG(INFO) << "p2p file id: " << p2p_file_id_;
    } else {
      // Even if we're not sharing the update, it could be that
      // there's a partial file from a previous attempt with the same
      // hash. If this is the case, we NEED to clean it up otherwise
      // we're essentially timing out other peers downloading from us
      // (since we're never going to complete the file).
      FilePath path = system_state_->p2p_manager()->FileGetPath(file_id);
      if (!path.empty()) {
        if (unlink(path.value().c_str()) != 0) {
          PLOG(ERROR) << "Error deleting p2p file " << path.value();
        } else {
          LOG(INFO) << "Deleting partial p2p file " << path.value()
                    << " since we're not using p2p to share.";
        }
      }
    }

    // Tweak timeouts on the HTTP fetcher if we're downloading from a
    // local peer.
    if (payload_state->GetUsingP2PForDownloading() &&
        payload_state->GetP2PUrl() == install_plan_.download_url) {
      LOG(INFO) << "Tweaking HTTP fetcher since we're downloading via p2p";
      http_fetcher_->set_low_speed_limit(kDownloadP2PLowSpeedLimitBps,
                                         kDownloadP2PLowSpeedTimeSeconds);
      http_fetcher_->set_max_retry_count(kDownloadP2PMaxRetryCount);
      http_fetcher_->set_connect_timeout(kDownloadP2PConnectTimeoutSeconds);
    }
  }

  http_fetcher_->BeginTransfer(install_plan_.download_url);
}

void DownloadAction::SuspendAction() {
  http_fetcher_->Pause();
}

void DownloadAction::ResumeAction() {
  http_fetcher_->Unpause();
}

void DownloadAction::TerminateProcessing() {
  if (writer_) {
    writer_->Close();
    writer_ = nullptr;
  }
  download_active_ = false;
  CloseP2PSharingFd(false);  // Keep p2p file.
  // Terminates the transfer. The action is terminated, if necessary, when the
  // TransferTerminated callback is received.
  http_fetcher_->TerminateTransfer();
}

void DownloadAction::SeekToOffset(off_t offset) {
  bytes_received_ = offset;
}

bool DownloadAction::ReceivedBytes(HttpFetcher* fetcher,
                                   const void* bytes,
                                   size_t length) {
  // Note that bytes_received_ is the current offset.
  if (!p2p_file_id_.empty()) {
    WriteToP2PFile(bytes, length, bytes_received_);
  }

  bytes_received_ += length;
  uint64_t bytes_downloaded_total =
      bytes_received_previous_payloads_ + bytes_received_;
  if (delegate_ && download_active_) {
    delegate_->BytesReceived(length, bytes_downloaded_total, bytes_total_);
  }
  if (writer_ && !writer_->Write(bytes, length, &code_)) {
    if (code_ != ErrorCode::kSuccess) {
      LOG(ERROR) << "Error " << utils::ErrorCodeToString(code_) << " (" << code_
                 << ") in DeltaPerformer's Write method when "
                 << "processing the received payload -- Terminating processing";
    }
    // Delete p2p file, if applicable.
    if (!p2p_file_id_.empty())
      CloseP2PSharingFd(true);
    // Don't tell the action processor that the action is complete until we get
    // the TransferTerminated callback. Otherwise, this and the HTTP fetcher
    // objects may get destroyed before all callbacks are complete.
    TerminateProcessing();
    return false;
  }

  // Call p2p_manager_->FileMakeVisible() when we've successfully
  // verified the manifest!
  if (!p2p_visible_ && system_state_ && delta_performer_.get() &&
      delta_performer_->IsManifestValid()) {
    LOG(INFO) << "Manifest has been validated. Making p2p file visible.";
    system_state_->p2p_manager()->FileMakeVisible(p2p_file_id_);
    p2p_visible_ = true;
  }
  return true;
}

void DownloadAction::TransferComplete(HttpFetcher* fetcher, bool successful) {
  if (writer_) {
    LOG_IF(WARNING, writer_->Close() != 0) << "Error closing the writer.";
    if (delta_performer_.get() == writer_) {
      // no delta_performer_ in tests, so leave the test writer in place
      writer_ = nullptr;
    }
  }
  download_active_ = false;
  ErrorCode code =
      successful ? ErrorCode::kSuccess : ErrorCode::kDownloadTransferError;
  if (code == ErrorCode::kSuccess) {
    if (delta_performer_ && !payload_->already_applied)
      code = delta_performer_->VerifyPayload(payload_->hash, payload_->size);
    if (code == ErrorCode::kSuccess) {
      if (payload_ < &install_plan_.payloads.back() &&
          system_state_->payload_state()->NextPayload()) {
        LOG(INFO) << "Incrementing to next payload";
        // No need to reset if this payload was already applied.
        if (delta_performer_ && !payload_->already_applied)
          DeltaPerformer::ResetUpdateProgress(prefs_, false);
        // Start downloading next payload.
        bytes_received_previous_payloads_ += payload_->size;
        payload_++;
        install_plan_.download_url =
            system_state_->payload_state()->GetCurrentUrl();
        StartDownloading();
        return;
      }

      // All payloads have been applied and verified.
      if (delegate_)
        delegate_->DownloadComplete();

      // Log UpdateEngine.DownloadAction.* histograms to help diagnose
      // long-blocking operations.
      std::string histogram_output;
      base::StatisticsRecorder::WriteGraph("UpdateEngine.DownloadAction.",
                                           &histogram_output);
      LOG(INFO) << histogram_output;
    } else {
      LOG(ERROR) << "Download of " << install_plan_.download_url
                 << " failed due to payload verification error.";
      // Delete p2p file, if applicable.
      if (!p2p_file_id_.empty())
        CloseP2PSharingFd(true);
    }
  }

  // Write the path to the output pipe if we're successful.
  if (code == ErrorCode::kSuccess && HasOutputPipe())
    SetOutputObject(install_plan_);
  processor_->ActionComplete(this, code);
}

void DownloadAction::TransferTerminated(HttpFetcher* fetcher) {
  if (code_ != ErrorCode::kSuccess) {
    processor_->ActionComplete(this, code_);
  } else if (payload_->already_applied) {
    LOG(INFO) << "TransferTerminated with ErrorCode::kSuccess when the current "
                 "payload has already applied, treating as TransferComplete.";
    TransferComplete(fetcher, true);
  }
}

}  // namespace chromeos_update_engine
