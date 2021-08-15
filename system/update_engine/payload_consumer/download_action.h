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

#ifndef UPDATE_ENGINE_PAYLOAD_CONSUMER_DOWNLOAD_ACTION_H_
#define UPDATE_ENGINE_PAYLOAD_CONSUMER_DOWNLOAD_ACTION_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <string>

#include "update_engine/common/action.h"
#include "update_engine/common/boot_control_interface.h"
#include "update_engine/common/http_fetcher.h"
#include "update_engine/common/multi_range_http_fetcher.h"
#include "update_engine/payload_consumer/delta_performer.h"
#include "update_engine/payload_consumer/install_plan.h"
#include "update_engine/system_state.h"

// The Download Action downloads a specified url to disk. The url should point
// to an update in a delta payload format. The payload will be piped into a
// DeltaPerformer that will apply the delta to the disk.

namespace chromeos_update_engine {

class DownloadActionDelegate {
 public:
  virtual ~DownloadActionDelegate() = default;

  // Called periodically after bytes are received. This method will be invoked
  // only if the DownloadAction is running. |bytes_progressed| is the number of
  // bytes downloaded since the last call of this method, |bytes_received|
  // the number of bytes downloaded thus far and |total| is the number of bytes
  // expected.
  virtual void BytesReceived(uint64_t bytes_progressed,
                             uint64_t bytes_received,
                             uint64_t total) = 0;

  // Returns whether the download should be canceled, in which case the
  // |cancel_reason| error should be set to the reason why the download was
  // canceled.
  virtual bool ShouldCancel(ErrorCode* cancel_reason) = 0;

  // Called once the complete payload has been downloaded. Note that any errors
  // while applying or downloading the partial payload will result in this
  // method not being called.
  virtual void DownloadComplete() = 0;
};

class PrefsInterface;

class DownloadAction : public InstallPlanAction, public HttpFetcherDelegate {
 public:
  // Debugging/logging
  static std::string StaticType() { return "DownloadAction"; }

  // Takes ownership of the passed in HttpFetcher. Useful for testing.
  // A good calling pattern is:
  // DownloadAction(prefs, boot_contol, hardware, system_state,
  //                new WhateverHttpFetcher, false);
  DownloadAction(PrefsInterface* prefs,
                 BootControlInterface* boot_control,
                 HardwareInterface* hardware,
                 SystemState* system_state,
                 HttpFetcher* http_fetcher,
                 bool interactive);
  ~DownloadAction() override;

  // InstallPlanAction overrides.
  void PerformAction() override;
  void SuspendAction() override;
  void ResumeAction() override;
  void TerminateProcessing() override;
  std::string Type() const override { return StaticType(); }

  // Testing
  void SetTestFileWriter(FileWriter* writer) { writer_ = writer; }

  int GetHTTPResponseCode() { return http_fetcher_->http_response_code(); }

  // HttpFetcherDelegate methods (see http_fetcher.h)
  bool ReceivedBytes(HttpFetcher* fetcher,
                     const void* bytes,
                     size_t length) override;
  void SeekToOffset(off_t offset) override;
  void TransferComplete(HttpFetcher* fetcher, bool successful) override;
  void TransferTerminated(HttpFetcher* fetcher) override;

  DownloadActionDelegate* delegate() const { return delegate_; }
  void set_delegate(DownloadActionDelegate* delegate) { delegate_ = delegate; }

  void set_base_offset(int64_t base_offset) { base_offset_ = base_offset; }

  HttpFetcher* http_fetcher() { return http_fetcher_.get(); }

  // Returns the p2p file id for the file being written or the empty
  // string if we're not writing to a p2p file.
  std::string p2p_file_id() { return p2p_file_id_; }

 private:
  // Closes the file descriptor for the p2p file being written and
  // clears |p2p_file_id_| to indicate that we're no longer sharing
  // the file. If |delete_p2p_file| is True, also deletes the file.
  // If there is no p2p file descriptor, this method does nothing.
  void CloseP2PSharingFd(bool delete_p2p_file);

  // Starts sharing the p2p file. Must be called before
  // WriteToP2PFile(). Returns True if this worked.
  bool SetupP2PSharingFd();

  // Writes |length| bytes of payload from |data| into |file_offset|
  // of the p2p file. Also does sanity checks; for example ensures we
  // don't end up with a file with holes in it.
  //
  // This method does nothing if SetupP2PSharingFd() hasn't been
  // called or if CloseP2PSharingFd() has been called.
  void WriteToP2PFile(const void* data, size_t length, off_t file_offset);

  // Start downloading the current payload using delta_performer.
  void StartDownloading();

  // The InstallPlan passed in
  InstallPlan install_plan_;

  // Pointer to the current payload in install_plan_.payloads.
  InstallPlan::Payload* payload_{nullptr};

  // SystemState required pointers.
  PrefsInterface* prefs_;
  BootControlInterface* boot_control_;
  HardwareInterface* hardware_;

  // Global context for the system.
  SystemState* system_state_;

  // Pointer to the MultiRangeHttpFetcher that does the http work.
  std::unique_ptr<MultiRangeHttpFetcher> http_fetcher_;

  // If |true|, the update is user initiated (vs. periodic update checks). Hence
  // the |delta_performer_| can decide not to use O_DSYNC flag for faster
  // update.
  bool interactive_;

  // The FileWriter that downloaded data should be written to. It will
  // either point to *decompressing_file_writer_ or *delta_performer_.
  FileWriter* writer_;

  std::unique_ptr<DeltaPerformer> delta_performer_;

  // Used by TransferTerminated to figure if this action terminated itself or
  // was terminated by the action processor.
  ErrorCode code_;

  // For reporting status to outsiders
  DownloadActionDelegate* delegate_;
  uint64_t bytes_received_{0};  // per file/range
  uint64_t bytes_received_previous_payloads_{0};
  uint64_t bytes_total_{0};
  bool download_active_{false};

  // The file-id for the file we're sharing or the empty string
  // if we're not using p2p to share.
  std::string p2p_file_id_;

  // The file descriptor for the p2p file used for caching the payload or -1
  // if we're not using p2p to share.
  int p2p_sharing_fd_;

  // Set to |false| if p2p file is not visible.
  bool p2p_visible_;

  // Loaded from prefs before downloading any payload.
  size_t resume_payload_index_{0};

  // Offset of the payload in the download URL, used by UpdateAttempterAndroid.
  int64_t base_offset_{0};

  DISALLOW_COPY_AND_ASSIGN(DownloadAction);
};

// We want to be sure that we're compiled with large file support on linux,
// just in case we find ourselves downloading large images.
static_assert(8 == sizeof(off_t), "off_t not 64 bit");

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_CONSUMER_DOWNLOAD_ACTION_H_
